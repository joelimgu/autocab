
// Your WebSocket client code
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
    
    // Check the value of the message and apply styles accordingly
    if (event.data.includes("true")) {
        newMessage.textContent = " " + event.data;
        newMessage.classList.add("true-message");
    } else if (event.data.includes("false")) {
        newMessage.textContent = " " + event.data;
        newMessage.classList.add("false-message");
    } else {
        newMessage.textContent = " " + event.data;
    }

    //messageContainer.appendChild(newMessage);
    
    // Append an image based on the value of "newMessage"
    const imageContainer = document.createElement("div");
    imageContainer.classList.add("image-container");

    const image = document.createElement("img");
    if (event.data.includes("true")) {
        image.src = "switch_on1.png"; // Replace with the actual path to your image
    } else {
        image.src = "switch_off1.png"; // Replace with the actual path to your image
    }
    image.alt = "Image Alt Text";
    imageContainer.appendChild(image);

    messageContainer.appendChild(imageContainer);
};

socket.onclose = function (event) {
    console.log("WebSocket connection closed.");
};

socket.onerror = function (event) {
    console.error("WebSocket error:", event);
};
