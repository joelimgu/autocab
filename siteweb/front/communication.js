

let socket = new WebSocket("ws://autocab.joel.rs/websocket/");

function handleMessage(event) {
    // Get the message container element
    let messageContainer = document.getElementById('message-container');
    // Parse the message from the event data
    console.log("Received message:", event.data);

    let message
    try {
        message = JSON.parse(event.data);
    } catch (e) {
        console.error('could\'t parse message as JSON: ' + event.data + e)
    }

    // Create an image element
    var imageElement = document.createElement('img');

    // Set the default image source
    imageElement.src = 'switch_off1.png';

    // Set the image size and other styles
    imageElement.style.width = '40px'; // Set the width of the image
    imageElement.style.height = 'auto'; // Maintain the aspect ratio
    imageElement.style.marginTop = '0px'; // Add spacing below the image

    // Check the message value
    if (message.type === 'status' && message.data.status) {
        // Change the image source to switch_on.png
        imageElement.src = 'switch_on1.png';
    } else if (message.type === 'toServeur') {
        console.log("serveur_feedback recieved")
    }


    // Clear the message container and append the image
    messageContainer.innerHTML = '';
    messageContainer.appendChild(imageElement);
}

function startWS() {

// Your WebSocket client code
    socket = new WebSocket("ws://autocab.joel.rs/websocket/");

// JavaScript code to handle event and toggle images


// Set up WebSocket event listener for message handling
    socket.onmessage = handleMessage;

    socket.onopen = (_e) => {
        console.log("ws connection opened")
    }

    socket.onclose = function (event) {
        console.log("WebSocket connection closed.");
        setTimeout(() => {
            console.log("reconnecting")
            startWS()
        }, 200)
    };

    socket.onerror = function (event) {
        console.error("WebSocket error:", event);
    };
}

// Call handleMessage with a predefined message when the page is ready
document.addEventListener('DOMContentLoaded', function () {
    // Assuming 'false' as the initial state, adjust this if needed
    handleMessage({
        data: JSON.stringify({
            type: "status",
            data: {
                "status": false
            }
        })
    });
});

startWS()