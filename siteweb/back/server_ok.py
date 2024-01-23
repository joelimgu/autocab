import asyncio
import websockets

# WebSocket server variables
start_pressed = False
start_status = False
clients = set()

# WebSocket server handling
async def handle_client(websocket, path):
    global start_pressed, start_status

    # Autoriser toutes les origines (à adapter selon vos besoins)
    headers = {
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Origin": "http://autocab.joel.rs:5501",
        "Access-Control-Allow-Methods": "GET, POST, OPTIONS",
        "Access-Control-Allow-Headers": "Origin, Content-Type, Accept",
    }

    if "OPTIONS" in websocket.request_headers.get("Upgrade", ""):
        # Réponse aux requêtes OPTIONS pour les demandes CORS
        return websockets.http.response.HTTPResponse(status=200, headers=headers)

    try:
        clients.add(websocket)  # Ajoutez cette ligne pour ajouter la connexion à la liste
        print(f"Client {websocket.remote_address} connected. Number of connected clients: {len(clients)}")

        while True:
            message = await websocket.recv()

            if not message:
                break

            print(f"Received message from {path}: {message}")

            if message.startswith("GPS-coordinates:"):
                # Extract and print GPS coordinates
                coordinates = message.split(":")[1].split(",")
                lat, lng = map(float, coordinates)
                print(f"Received GPS coordinates: Lat={lat}, Lng={lng}")
            elif message == "start-pressed":
                # Check if the client is the first one to press "Start"
                if not start_pressed:
                    start_pressed = True
                    await websocket.send("allow-redirection")
                else:
                    # Send "Please wait" to clients who are not the first one to press "Start"
                    await websocket.send("Please wait")
            else:
                # Convert the message to a boolean and update start_status
                start_status = message.lower() == "true"

                # Broadcast the message to all connected clients
                for other_client in clients:
                    if other_client != websocket:
                        try:
                            await other_client.send(message)
                            print(f"Sent message to {other_client.remote_address}: {message}")
                        except websockets.exceptions.ConnectionClosedError:
                            continue
    except asyncio.TimeoutError:
        print(f"Connection with {path} closed due to inactivity.")
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
    server = await websockets.serve(handle_client, "autocab.joel.rs", 5501)
    print("WebSocket server listening on ws://autocab.joel.rs:5501")

    # Wait for the WebSocket server to finish
    await server.wait_closed()

# Run the main function
if __name__ == "__main__":
    asyncio.run(main())
