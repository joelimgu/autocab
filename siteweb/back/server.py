import socket
import threading

def traiter_client(socket_client, adresse):
    try:
        print(f"Connexion acceptée depuis {adresse}")

        while True:
            data = socket_client.recv(1024)
            if not data:
                break

            print(f"Reçu depuis {adresse}: {data.decode('utf-8')}")

            # Diffuser le message à tous les clients connectés
            for autre_client in clients:
                if autre_client != socket_client:
                    try:
                        autre_client.sendall(data)
                    except:
                        continue

    except Exception as e:
        print(f"Erreur lors du traitement du client {adresse}: {e}")
    finally:
        print(f"Connexion depuis {adresse} fermée.")
        clients.remove(socket_client)
        socket_client.close()

# Définir l'adresse du serveur (localhost) et le port
adresse_serveur = ('127.0.0.1', 12345)

# Créer un objet socket
socket_serveur = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Lier le socket à l'adresse du serveur
socket_serveur.bind(adresse_serveur)

# Écouter les connexions entrantes
socket_serveur.listen(5)

print("Le serveur écoute sur", adresse_serveur)

# Liste pour stocker les clients connectés
clients = []

try:
    while True:
        # Accepter une connexion
        socket_client, adresse_client = socket_serveur.accept()
        clients.append(socket_client)

        # Créer un thread pour traiter le client
        thread_client = threading.Thread(target=traiter_client, args=(socket_client, adresse_client))
        thread_client.start()

except KeyboardInterrupt:
    print("Arrêt du serveur.")
    for client in clients:
        client.close()
    socket_serveur.close()
    