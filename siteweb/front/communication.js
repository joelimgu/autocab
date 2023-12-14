const socket = new WebSocket("ws://localhost:5501");

socket.onopen = function (event) {
    console.log("Connexion WebSocket ouverte.");

    // Send a message to the server
    const message = "Hello from HTML/JavaScript client!";
    socket.send(message);
};

socket.onmessage = function (event) {
    console.log("Received response:", event.data);
};

socket.onclose = function (event) {
    console.log("Connexion WebSocket fermée.");
};

socket.onerror = function (event) {
    console.error("WebSocket error:", event);
};