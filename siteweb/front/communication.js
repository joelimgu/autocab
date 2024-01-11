// Your WebSocket client code
const socket = new WebSocket("ws://161.35.86.239:5501");

// JavaScript code to handle event and toggle images
function handleMessage(event) {
    // Get the message container element
    var messageContainer = document.getElementById('message-container');

    // Parse the message from the event data
    var message = event.data;

    // Create an image element
    var imageElement = document.createElement('img');

    // Set the default image source
    imageElement.src = 'switch_off1.png';

    // Set the image size and other styles
    imageElement.style.width = '40px'; // Set the width of the image
    imageElement.style.height = 'auto'; // Maintain the aspect ratio
    imageElement.style.marginTop = '0px'; // Add spacing below the image

    // Check the message value
    if (message === 'True') {
        // Change the image source to switch_on.png
        imageElement.src = 'switch_on1.png';
    }

    // Clear the message container and append the image
    messageContainer.innerHTML = '';
    messageContainer.appendChild(imageElement);
}

// Set up WebSocket event listener for message handling
socket.onmessage = handleMessage;

socket.onclose = function (event) {
    console.log("WebSocket connection closed.");
};

socket.onerror = function (event) {
    console.error("WebSocket error:", event);
};

// Call handleMessage with a predefined message when the page is ready
document.addEventListener('DOMContentLoaded', function () {
    // Assuming 'false' as the initial state, adjust this if needed
    handleMessage({ data: 'False' });
});
