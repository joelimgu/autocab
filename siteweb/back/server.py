import asyncio
import websockets

start_status = False  # Variable pour stocker la dernière valeur reçue


async def handle_client(websocket, path):
    global start_status
    
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
            message = await asyncio.wait_for(websocket.recv(), timeout=None)
            if not message:
                break

            print(f"Received message from {path}: {message}")

            # Convertir le message en booléen et mettre à jour start_status
            start_status = message.lower() == "true"

            # Mettre à jour la variable start_status avec la dernière valeur reçue
            start_status = message

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
    except websockets.exceptions.ConnectionClosed:
        print(f"Connection with {path} closed.")
    except Exception as e:
        print(f"Error with {path}: {e}")
    finally:
        if websocket in clients:
            clients.remove(websocket)
            print(f"Client {websocket.remote_address} disconnected. Number of connected clients: {len(clients)}")

async def main():
    server = await websockets.serve(handle_client, "autocab.joel.rs", 5501)
    print("WebSocket server listening on ws://autocab.joel.rs:5501")

    await server.wait_closed()

if __name__ == "__main__":
    clients = set()
    asyncio.run(main())
