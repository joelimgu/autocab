import asyncio
import websockets

#J ai un timeout à 50 secondes peut etre a cause de anaconda

start_status = False

async def send_message():
    global start_status
    uri = "ws://127.0.0.1:5501"
    async with websockets.connect(uri) as websocket:
        try:
            while True:
                message = str(start_status)
                await websocket.send(message)
                print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

asyncio.run(send_message())
