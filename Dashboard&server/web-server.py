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
            shared_state.dashboard_open_value = update['valve']
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
        shared_state.history.append({"value": water_level, "datetime": datetime.now()})
        # remove the first element of the list
        if len(shared_state.history) > 20:
            shared_state.history.pop(0) 
        print(shared_state.history)
        await asyncio.sleep(0.5)

async def arduino():
    global water_level
    global W1
    global W2
    global W3
    global W4
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
                            elif water_level >= W1 and water_level < W2:
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

async def system_status():
    global water_level
    global W1
    global W2
    global W3
    global W4
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
    # Start the web server and the MQTT client
    server = websockets.serve(handle_client, "localhost", 8765)
    async with aiomqtt.Client("broker.mqtt-dashboard.com", 1883) as client:
        arduino_task = asyncio.create_task(arduino())
        mqtt_task = asyncio.create_task(handle_mqtt_messages(client))
        system_status_task = asyncio.create_task(system_status())
        await asyncio.gather(server, send_data_to_clients(), mqtt_task, arduino_task, system_status_task)


# Changed loop type to run the server on Windows
# if sys.platform.lower() == "win32" or os.name.lower() == "nt":
#     from asyncio import set_event_loop_policy, WindowsSelectorEventLoopPolicy
#     set_event_loop_policy(WindowsSelectorEventLoopPolicy())


asyncio.run(main())