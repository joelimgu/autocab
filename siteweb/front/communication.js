const socket = new WebSocket("ws://127.0.0.1:5501");

socket.onopen = function (event) {
    console.log("WebSocket connection opened.");

    // Send a message to the server
    const message = "Hello from HTML/JavaScript client!";
    socket.send(message);
};

socket.onmessage = function (event) {
    console.log("Received response:", event.data);

    // Display the received message on the webpage
    const messageContainer = document.getElementById("message-container");
    const newMessage = document.createElement("p");
    newMessage.textContent = " " + event.data;
    messageContainer.appendChild(newMessage);
};

socket.onclose = function (event) {
    console.log("WebSocket connection closed.");
};

socket.onerror = function (event) {
    console.error("WebSocket error:", event);
};