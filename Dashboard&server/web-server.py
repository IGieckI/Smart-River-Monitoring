import aiomqtt
import asyncio
from datetime import datetime
import json
import sys
import websockets
import serial
import serial.tools.list_ports as port_list
import random
import time

class SharedState:
    """A class to store shared state between the server and the clients."""
    def __init__(self):
        self.dashboard_manual = False
        self.hardware_manual = False
        self.dashboard_open_value = -1
        self.current_valve_opening = -1
        self.current_status = 'Waiting for data...'
        self.history = [
            {"value": 10, "datetime": datetime(2024, 3, 30, 8, 0, 0)},
            {"value": 15, "datetime": datetime(2024, 3, 30, 8, 15, 0)},
            {"value": 20, "datetime": datetime(2024, 3, 30, 8, 30, 0)},
            {"value": 25, "datetime": datetime(2024, 3, 30, 8, 45, 0)}
        ]

# Store the connected clients in a dictionary
clients = {}

# Water level
water_level = 0

# Water level thresholds
W1 = 10 #cm
W2 = 25 #cm
W3 = 50 #cm
W4 = 75 #cm
W5 = 90 #cm

# Shared instance of the system
global shared_state
shared_state = SharedState()
ports = list(port_list.comports())
for p in ports:
    print (p)

ser = serial.Serial(ports[2].device, 115200)
ser.reset_input_buffer()
ser.reset_output_buffer()

time.sleep(2)

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
            # print(message)
            update = json.loads(message)
            
            # IMPROVE SYSTEM MANAGEMENT
            
            shared_state.dashboard_manual = update['remote_control']
            shared_state.current_valve_opening = update['valve']
            shared_state.dashboard_open_value = update['valve']
            # shared_state.history.append(update['valve'])
    finally:
        del clients[websocket.remote_address]
        
# Handle incoming messages from the MQTT broker
async def handle_mqtt_messages(client):
    global water_level
    await client.subscribe("water_level_iot_24h")
    async for message in client.messages:
        messageDecoded = message.payload.decode()
        jsonDataMessage = json.loads(messageDecoded)
        water_level = jsonDataMessage['water_level']
        # await asyncio.sleep(0.5)

async def arduino():
    print("Arduino task started")
    global water_level
    while True:
        print("Arduino task running")
        # arduino send data to server any time
        if ser.in_waiting > 0:
            data = ser.readline().decode()
            if data:
                print(data)
                try:
                    jsonData = json.loads(data)
                    # print(jsonData)
                    shared_state.hardware_manual = jsonData['manual_control']
                    # if the hardware is in AUTO MODE
                    if jsonData['manual_control'] == 'false':
                        shared_state.current_status = 'Automatic'
                        # if the dashboard is in MANUAL MODE
                        if shared_state.dashboard_manual == True:
                            valve = shared_state.dashboard_open_value
                        else:
                            if water_level < W1:
                                valve = 0
                            elif water_level > W1 and water_level <= W2:
                                valve = 25
                            elif water_level > W3 and water_level <= W4:
                                valve = 50
                            elif water_level > W5:
                                valve = 100
                            else:
                                valve = 0
                        shared_state.current_valve_opening = valve
                        myJson = {"valve": valve}
                        string = json.dumps(myJson).encode()
                        ser.write(string)
                        ser.flush()
                    else:
                        shared_state.current_status = 'Manual'
                        # print(data)
                except json.JSONDecodeError as e:
                    print(f"Errore nel caricamento del JSON: {e}")
            else:
                print('Data not received from Arduino')
        else:
            print('No data available')
            
        await asyncio.sleep(2)


async def main():
    # Start the web server and the MQTT client
    server = websockets.serve(handle_client, "localhost", 8765)
    async with aiomqtt.Client("broker.mqtt-dashboard.com", 1883) as client:
        arduino_task = asyncio.create_task(arduino())
        mqtt_task = asyncio.create_task(handle_mqtt_messages(client))
        await asyncio.gather(server, send_data_to_clients(), mqtt_task, arduino_task)


# Changed loop type to run the server on Windows
# if sys.platform.lower() == "win32" or os.name.lower() == "nt":
#     from asyncio import set_event_loop_policy, WindowsSelectorEventLoopPolicy
#     set_event_loop_policy(WindowsSelectorEventLoopPolicy())


asyncio.run(main())