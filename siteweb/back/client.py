import asyncio
import websockets

async def send_message():
    uri = "ws://autocab.joel.rs/websocket/"
    async with websockets.connect(uri) as websocket:
        try:
            message = input("Enter your message: ")
            await websocket.send(message)
            print(f"Sent message: {message}")

        except websockets.exceptions.ConnectionClosedError as e:
            print(f"Connexion fermée de manière inattendue. Erreur : {e}")

        except websockets.exceptions.ConnectionClosedOK:
            print("Connexion fermée par le serveur.")

if __name__ == "__main__":
    asyncio.run(send_message())

