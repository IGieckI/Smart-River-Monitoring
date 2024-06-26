import aiomqtt
import asyncio
from datetime import datetime
import json
import sys
import websockets
import serial
import serial.tools.list_ports as port_list
import time
import configparser
import pathlib

class SharedState:
    """A class to store shared state between the server and the clients"""
    def __init__(self):
        self.dashboard_manual = False
        self.hardware_manual = False
        self.dashboard_open_value = -1
        self.current_valve_opening = -1
        self.current_status = 'Waiting for data...'
        self.history = []

# Store the connected clients in a dictionary
clients = {}

# Define variables to initialize the connected ESP
CFG_FILE = pathlib.Path(__file__).parent.absolute() / "config.cfg"
INITIALIZATION_MQTT_TOPIC = None
W1 = W2 = W3 = W4 = None
ARDUINO_PORT = 2

# Shared instance of the system
shared_state = SharedState()
water_level = 0

# Main loop to send data to connected dashboards
async def send_data_to_clients():
    while True:        
        # Map over HTML elements of the dashboard
        package = {
            'dashboard_manual': shared_state.dashboard_manual,
            'hardware_manual': shared_state.hardware_manual,
            'dashboard_open_value': shared_state.dashboard_open_value,
            'current_valve_opening': shared_state.current_valve_opening,
            'current_status': shared_state.current_status,
            'history': shared_state.history
        }
        
        # Send the data to all connected clients
        for client in clients.values(): 
            try:
                await client.send(json.dumps(package, default=str))  
            except websockets.exceptions.ConnectionClosed:
                clients.pop(client)
        await asyncio.sleep(1)

# Handle incoming messages from the clients
async def handle_client(websocket):
    clients[websocket.remote_address] = websocket
    try:
        async for message in websocket:
            update = json.loads(message)            
            shared_state.dashboard_manual = update['remote_control']
            shared_state.dashboard_open_value = update['valve']
    finally:
        del clients[websocket.remote_address]
        
# Handle incoming messages from the MQTT broker
async def handle_mqtt_data(client):
    global water_level
    await client.subscribe("water_level_iot_24h")
    async for message in client.messages:
        messageDecoded = message.payload.decode()
        jsonDataMessage = json.loads(messageDecoded)
        water_level = jsonDataMessage['water_level']
        shared_state.history.append({"value": water_level, "datetime": datetime.now()})
        
        if len(shared_state.history) > 20:
            shared_state.history.pop(0)
        await asyncio.sleep(0.5)

# Send initialization data to the ESP
async def mqtt_initialization_data_task(client):    
    json_data = json.dumps(read_config_file(section="ESP"))
    client.publish(INITIALIZATION_MQTT_TOPIC, json_data)

# Read the configuration file
def read_config_file(cfg_file_name = CFG_FILE, section = None):
    config = configparser.ConfigParser()
    config.read(cfg_file_name)
    data = {}
    
    if section is None:
        for section in config.sections():
            data[section] = dict(config.items(section))
    else:
        data = dict(config.items(section))

    return data

# Communicate with the Arduino
async def arduino(ser : serial.Serial):
    global water_level, W1, W2, W3, W4
    
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode().strip()
            if data:
                try:
                    jsonData = json.loads(data)
                    manual_control = jsonData['manual_control']
                    shared_state.hardware_manual = manual_control
                    if manual_control == True:
                        vMan = jsonData['valve']
                        shared_state.current_valve_opening = vMan
                    else:
                        if shared_state.dashboard_manual == True:
                            vAuto = shared_state.dashboard_open_value
                        else:
                            if water_level < W1:
                                vAuto = 0
                            elif water_level >= W1 and water_level < W3:
                                vAuto = 25
                            elif water_level >= W3 and water_level < W4:
                                vAuto = 50
                            elif water_level >= W4:
                                vAuto = 100
                            else:
                                vAuto = 0
                        data4Arduino = {
                            "valve": vAuto
                        }
                        shared_state.current_valve_opening = vAuto
                        string = json.dumps(data4Arduino).encode()
                        ser.write(string)
                        ser.flush()
                except json.JSONDecodeError as e:
                    print(f"Errore nel caricamento del JSON: {e}")
            print(data)
        await asyncio.sleep(0.05)

# Check the system status
async def system_status():
    global water_level, W1, W2, W3, W4
    
    while True:
        if water_level < W1:
            shared_state.current_status = 'ALARM TOO LOW'
        elif water_level >= W1 and water_level < W2:
            shared_state.current_status = 'NORMAL'
        elif water_level >= W2 and water_level < W3:
            shared_state.current_status = 'PRE ALARM TOO HIGH'
        elif water_level >= W3 and water_level < W4:
            shared_state.current_status = 'ALARM TOO HIGH'
        elif water_level >= W4:
            shared_state.current_status = 'ALARM TOO HIGH CRITIC'
        await asyncio.sleep(0.5)

async def main():
    
    try:
        # Start the web server and the MQTT client
        server = websockets.serve(handle_client, "localhost", 8765)
        
        # Set global variables from the config file
        config = read_config_file(section="SYSTEM")
        INITIALIZATION_MQTT_TOPIC = config["initialization_mqtt_topic"]
        W1 = config["w1"]
        W2 = config["w2"]
        W3 = config["w3"]
        W4 = config["w4"]
        ARDUINO_PORT = int(config["arduino_port"])

        # Looking for the arduino port
        ports = list(port_list.comports())
        ser = serial.Serial(ports[ARDUINO_PORT].device, 115200)
        ser.reset_input_buffer()
        ser.reset_output_buffer()
    except FileNotFoundError:
        print("Config file not found. Please check the file path.")
        return
    except KeyError:
        print("One or more fields cannot be found in the config file. Please check the file.")
        return
    except IndexError:
        print("Invalid ARDUINO_PORT. Please check the port number and make sure the arduino is connected correctly.")
        return
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        return

    time.sleep(2)

    async with aiomqtt.Client("broker.mqtt-dashboard.com", 1883) as client:
        arduino_task = asyncio.create_task(arduino(ser))
        mqtt_initialization_task = asyncio.create_task(mqtt_initialization_data_task(client))
        mqtt_data_task = asyncio.create_task(handle_mqtt_data(client))

        system_status_task = asyncio.create_task(system_status())
        await asyncio.gather(server, send_data_to_clients(), mqtt_initialization_data_task, arduino_task, system_status_task)


# Changed loop type to run the server on Windows, IF RUNNING ON DIFFERENCE OS, COMMENT THE FOLLOWING LINES
if sys.platform.lower() == "win32" or os.name.lower() == "nt":
    from asyncio import set_event_loop_policy, WindowsSelectorEventLoopPolicy
    set_event_loop_policy(WindowsSelectorEventLoopPolicy())

asyncio.run(main())