import aiomqtt
import asyncio
from datetime import datetime
import json
import sys
import websockets

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

# Main loop to send data to connected dashboards
async def send_data_to_clients():
    while True:        
        print("Server is running")
        
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
        for client in clients:
            try:
                await client.value().send(json.dumps(package, default=str))
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
                  shared_state.dashboard_open_value = update['valve']
    finally:
        del clients[websocket.remote_address]
        
# Handle incoming messages from the MQTT broker
async def handle_mqtt_messages(client):
    await client.subscribe("water_level")
    async for message in client.messages:
        print(message.payload)


async def main():
    # Start the web server and the MQTT client
    server = websockets.serve(handle_client, "localhost", 8765)
    async with aiomqtt.Client("broker.hivemq.com", 1883) as client:
        mqtt_task = asyncio.create_task(handle_mqtt_messages(client))
        await asyncio.gather(server, send_data_to_clients(), mqtt_task)

# Changed loop type to run the server on Windows
if sys.platform.lower() == "win32" or os.name.lower() == "nt":
    from asyncio import set_event_loop_policy, WindowsSelectorEventLoopPolicy
    set_event_loop_policy(WindowsSelectorEventLoopPolicy())

asyncio.run(main())