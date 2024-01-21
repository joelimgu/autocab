  // Global counter variable
  let startCounter = 0;

  // Function to be called when the user presses the Start button
  function startProcess() {
      // Increment the counter
      startCounter++;

      // Check if more than one person has started the process
      if (startCounter > 1) {
          // Display a message to the user
          alert('Sorry, someone is already using the car. Please wait.');
      } else {
          // Redirect to Waiting.html
          window.location.href = 'Waiting.html';
      }
  }