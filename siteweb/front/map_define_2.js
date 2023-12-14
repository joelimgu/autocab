let pointIcon = L.icon({
    iconUrl: 'point_icone.png',
    iconSize: [20, 20],
    iconAnchor: [0, 20]
});


let points = [
    { name: 'A', lat: 43.570593, lng: 1.466513, message1: 'Your position', message2: 'Your destination' },
    { name: 'B', lat: 43.570853, lng: 1.467021, message1: 'Your position',  message2: 'Your destination'},
    {name: 'C', lat: 43.571061, lng: 1.466837, message1: 'Your position',  message2: 'Your destination'},
    {name: 'D', lat: 43.570342, lng: 1.466029, message1: 'Your position',  message2: 'Your destination'},
    {name: 'E', lat: 43.570567, lng: 1.4658, message1: 'Your position',  message2: 'Your destination'},
    {name: 'F', lat: 43.570671, lng: 1.467218, message1: 'Your position',  message2: 'Your destination'},
    {name: 'G', lat: 43.570706, lng: 1.465661, message1: 'Your position',  message2: 'Your destination'},
    {name: 'H', lat: 43.570942, lng: 1.465445, message1: 'Your position',  message2: 'Your destination'},
    {name: 'I', lat: 43.571185, lng: 1.465951, message1: 'Your position',  message2: 'Your destination'},
    {name: 'J', lat: 43.57144, lng: 1.466431, message1: 'Your position',  message2: 'Your destination'},
    {name: 'K', lat: 43.571233, lng: 1.466667, message1: 'Your position',  message2: 'Your destination'}

];

let markers = points.map(point => {
    const marker = L.marker([point.lat, point.lng], { icon: pointIcon });

    // Customize the popup message based on the point
    if (point.message1) {
        marker.bindPopup(point.message1);
    } else {
        marker.bindPopup(point.name);
    }

    marker.pointData = point; // Attach additional data to the marker
    return marker;
});

// Function to handle marker click
function onMarkerClick(e) {
    // Access the additional data from the marker
    const markerData = e.target.pointData;
    // Now you can access all the data associated with the clicked marker
    console.log("Clicked on marker:", markerData);
    // You can also store the data or perform any other actions here
}

// Add click event to each marker
markers.forEach(marker => {
    marker.on('click', onMarkerClick);
});