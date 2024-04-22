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
        self.history = []

# Store the connected clients in a dictionary
clients = {}

# Shared instance of the system
global shared_state
shared_state = SharedState()
ports = list(port_list.comports())
for p in ports:
    print (p)

#ser = serial.Serial(ports[2].device, 115200)
#ser.reset_input_buffer()
#ser.reset_output_buffer()

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
            print(message)
            update = json.loads(message)
            
            # IMPROVE SYSTEM MANAGEMENT
            
            shared_state.dashboard_manual = update['remote_control']
            shared_state.current_valve_opening = update['valve']
            shared_state.dashboard_open_value = update['valve']
            shared_state.history.append(update['valve'])
    finally:
        del clients[websocket.remote_address]
        
# Handle incoming messages from the MQTT broker
async def handle_mqtt_messages(client):
    await client.subscribe("water_level_iot_24h")
    async for message in client.messages:
        print(message.payload)

'''
async def arduino():
    while True:
        if ser.in_waiting > 0:
            data = ser.readline().decode('utf-8').strip()
            if data:
                try:
                    jsonData = json.loads(data)
                    if jsonData['manual_control'] == 'false':
                        if water_level < W1:
                            valve = 0
                        elif water_level > W1 and water_level < W2:
                            valve = 25
                        elif water_level > W3 and water_level < W4:
                            valve = 50
                        elif water_level > W5:
                            valve = 100
                            
                        myJson = {"valve": valve}
                        string = json.dumps(myJson).encode()
                        ser.write(string)
                        ser.flush()
                    else:
                        print(data)
                except json.JSONDecodeError as e:
                    print(f"Errore nel caricamento del JSON: {e}")
            else:
                print('Data not received from Arduino')
        else:
            print('No data available')
            
        await asyncio.sleep(2)
'''

async def main():
    # Start the web server and the MQTT client
    server = websockets.serve(handle_client, "localhost", 8765)
    async with aiomqtt.Client("broker.mqtt-dashboard.com", 1883) as client:
        mqtt_task = asyncio.create_task(handle_mqtt_messages(client))
        await asyncio.gather(server, send_data_to_clients(), mqtt_task)


# Changed loop type to run the server on Windows
# if sys.platform.lower() == "win32" or os.name.lower() == "nt":
#     from asyncio import set_event_loop_policy, WindowsSelectorEventLoopPolicy
#     set_event_loop_policy(WindowsSelectorEventLoopPolicy())


asyncio.run(main())