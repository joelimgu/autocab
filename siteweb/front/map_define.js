document.addEventListener("DOMContentLoaded", function () {
    let points = [
        ['A', 43.570593, 1.466513],
        ['B', 43.570853, 1.467021],
        ['C', 43.571061, 1.466837],
        ['D', 43.570342, 1.466029],
        ['E', 43.570567, 1.4658],
        ['F', 43.570671, 1.467218],
        ['G', 43.570706, 1.465661],
        ['H', 43.570942, 1.465445],
        ['I', 43.571185, 1.465951],
        ['J', 43.57144, 1.466431],
        ['K', 43.571233, 1.466667]
    ];

    let pointIcon = L.icon({
        iconUrl: 'point_icone.png',
        iconSize: [20, 20],
        iconAnchor: [0, 20]
    });

    let markers = points.map(point => L.marker([point[1], point[2]], { icon: pointIcon }).bindPopup(point[0]));

    let pointsGrp = L.layerGroup(markers);

    let osm = L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    });

    let map = L.map('map', {
        center: [43.57086, 1.46622],
        zoom: 17,
        layers: [osm, pointsGrp]
    });

    // Any other code related to Leaflet goes here...
});



/*function findMe() {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(localization);
    } else {
        alert("Geolocation is not supported by this browser.");
    }
}

function localization(posicion) {
    var latitude = posicion.coords.latitude;
    var longitude = posicion.coords.longitude;

    // Add the user's location as a marker
    L.marker([latitude, longitude]).addTo(map).bindPopup("Your Location").openPopup();
} */

// Use DOMContentLoaded event to ensure the DOM is fully loaded
//document.addEventListener("DOMContentLoaded", function () {
    // Call the findMe function to start geolocation
    //findMe(); 
