var map = L.map('map').setView([51.505,-0.09],13); //set initial coodinates and zoom view

// Add a title layer (u can use different providers)
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { attribution: '@ OpenStreetMap contributors'}).addTo(map);