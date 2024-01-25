import asyncio
import websockets
import subprocess
import json 

# WebSocket server variables
start_pressed = False
clients = set()

# WebSocket server handling
async def handle_client(websocket, path):
    global start_pressed
    try:
        clients.add(websocket)
        print(f"Client {websocket.remote_address} connected. Number of connected clients: {len(clients)}")

        while True:
            message = await websocket.recv()

            if not message:
                break
            message = json.loads(message)

            print(f"Received message from {path}: {message}")
            
            if message["type"] == "start-pressed":
                # Check if the client is the first one to press "Start"
                if not start_pressed:
                    start_pressed = True
                    d = {}
                    d["type"] = "allow-redirection"
                    await websocket.send(json.dumps(d))
                else:
                    # Send "Please wait" to clients who are not the first one to press "Start"
                    await websocket.send("Please wait")
            else:
                # Broadcast the message to all connected clients
                for other_client in clients:
                    if other_client != websocket:
                        try:
                            await other_client.send(json.dumps(message))
                        except websockets.exceptions.ConnectionClosedError:
                            continue
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection with {path} closed. Reason: {e}")
    except Exception as e:
        print(f"Error with {path}: {e}")
    finally:
        if websocket in clients:
            clients.remove(websocket)
            print(f"Client {websocket.remote_address} disconnected. Number of connected clients: {len(clients)}")

async def main():
    # Start the WebSocket server
    websocket_server = await websockets.serve(handle_client, "autocab.joel.rs", 5501)
    print("WebSocket server listening on ws://autocab.joel.rs:5501")

    # Wait for the WebSocket server to finish
    await websocket_server.wait_closed()

# Run the main function
if __name__ == "__main__":
    asyncio.run(main())

