import asyncio
import websockets

start_status = False  # Variable partagée
start_status_lock = asyncio.Lock()  # Utilisation de asyncio.Lock

async def handle_client(websocket, path):
    global start_status
    
    # Autoriser toutes les origines (à adapter selon vos besoins)
    headers = {
        "Access-Control-Allow-Origin": "*",
        "Access-Control-Allow-Methods": "GET, POST, OPTIONS",
        "Access-Control-Allow-Headers": "Origin, Content-Type, Accept",
    }

    if "OPTIONS" in websocket.request_headers.get("Upgrade", ""):
        # Réponse aux requêtes OPTIONS pour les demandes CORS
        return websockets.http.response.HTTPResponse(status=200, headers=headers)

    try:
        async with start_status_lock:
            clients.add(websocket)  # Ajoutez cette ligne pour ajouter la connexion à la liste
            print(f"Client {websocket.remote_address} connected. Number of connected clients: {len(clients)}")

        while True:
            # Attendez que la variable start_status soit mise à jour
            await asyncio.sleep(0.1)
            
            async with start_status_lock:
                message = str(start_status)
                try:
                    await websocket.send(message)
                    print(f"Sent message to {websocket.remote_address}: {message}")
                except websockets.exceptions.ConnectionClosedError:
                    break
    except Exception as e:
        print(f"Error with {path}: {e}")
    finally:
        async with start_status_lock:
            if websocket in clients:
                clients.remove(websocket)
                print(f"Client {websocket.remote_address} disconnected. Number of connected clients: {len(clients)}")

async def main():
    server = await websockets.serve(handle_client, "127.0.0.1", 5501)
    print("WebSocket server listening on ws://127.0.0.1:5501")

    await server.wait_closed()

if __name__ == "__main__":
    clients = set()
    asyncio.run(main())
