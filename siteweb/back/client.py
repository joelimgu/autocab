import socket
import threading

def recevoir_messages(socket_client):
    try:
        while True:
            data = socket_client.recv(1024)
            if not data:
                break
            print("Message reçu:", data.decode('utf-8'))
    except Exception as e:
        print(f"Erreur lors de la réception du message : {e}")
    finally:
        socket_client.close()

# Définir l'adresse du serveur (localhost) et le port
adresse_serveur = ('127.0.0.1', 12345)

# Créer un objet socket
socket_client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Se connecter au serveur
socket_client.connect(adresse_serveur)

# Démarrer un thread pour recevoir les messages
thread_reception = threading.Thread(target=recevoir_messages, args=(socket_client,))
thread_reception.start()

try:
    while True:
        # Envoyer un message au serveur
        message = input("Entrez le message : ")
        socket_client.sendall(message.encode('utf-8'))

except KeyboardInterrupt:
    print("Arrêt du client.")
    socket_client.close()
    