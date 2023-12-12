var map = L.map('map').setView([50.84673, 4.35247], 12);//set initial coodinates and zoom view

// Add a title layer (u can use different providers)
L.tileLayer('https://tile.openstreetmap.be/osmbe/{z}/{x}/{y}.png', {
     attribution:
         '&copy; <a href="http://openstreetmap.org">OpenStreetMap</a> contributors' +
         ', Tiles courtesy of <a href="https://geo6.be/">GEO-6</a>',
     maxZoom: 18
 }).addTo(map);
