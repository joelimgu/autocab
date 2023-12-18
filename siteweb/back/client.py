import asyncio
import websockets

async def send_message():
    uri = "ws://192.168.1.1:5505"
    async with websockets.connect(uri) as websocket:
        try:
            while True:
                message = input("Enter your message (or type 'exit' to quit): ")
                if message.lower() == 'exit':
                    break
                await websocket.send(message)
                print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

asyncio.run(send_message())
