// Check if a user has already started the process
const hasStarted = localStorage.getItem('hasStarted');

if (hasStarted) {
    // Redirect to Waiting.html if the process has already started
    window.location.href = 'Waiting.html';
}

// Function to be called when the user presses the Start button
function startProcess() {
    // Set a flag in localStorage to indicate that the process has started
    localStorage.setItem('hasStarted', true);

    // Redirect to Waiting.html
    window.location.href = 'Waiting.html';
}
