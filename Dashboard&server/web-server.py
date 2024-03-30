import asyncio
import websockets
import time
import json
from datetime import datetime


class SharedState:
    def __init__(self):
        self.dashboard_manual = False
        self.hardware_manual = False
        self.dashboard_open_value = 0
        self.current_valve_opening = 0
        self.current_status = 'Fake Status'
        self.history = [
            {"value": 10, "datetime": datetime(2024, 3, 30, 8, 0, 0)},
            {"value": 15, "datetime": datetime(2024, 3, 30, 8, 15, 0)},
            {"value": 20, "datetime": datetime(2024, 3, 30, 8, 30, 0)},
            {"value": 25, "datetime": datetime(2024, 3, 30, 8, 45, 0)}
        ]

clients = {}

global shared_state
shared_state = SharedState()


async def send_data_to_clients():
    while True:
        print("reset")
        package = {
            'dashboard_manual': shared_state.dashboard_manual,
            'hardware_manual': shared_state.hardware_manual,
            'dashboard_open_value': shared_state.dashboard_open_value,
            'current_valve_opening': shared_state.current_valve_opening,
            'current_status': shared_state.current_status,
            'history': shared_state.history
        }
        for client in clients.values():
            try:
                await client.send(json.dumps(package, default=str))
            except websockets.exceptions.ConnectionClosed:
                pass
        await asyncio.sleep(1)

async def handle_client(websocket, path):  # Pass shared_state as an argument
    clients[websocket.remote_address] = websocket
    try:
        async for message in websocket:
                  print(message)
                  update = json.loads(message)
                  shared_state.dashboard_manual = update['remote_control']  # Access shared_state here
                  shared_state.dashboard_open_value = update['valve']
    finally:
        del clients[websocket.remote_address]


async def main():
    async with websockets.serve(handle_client, "localhost", 8765):
        print("Server started.")
        await send_data_to_clients()


asyncio.run(main())
