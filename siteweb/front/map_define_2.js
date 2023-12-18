let chooseDestination= true;
let personMarker = null;
let arriveMarker = null; 
let car = null;

document.addEventListener("DOMContentLoaded", function () {
    


    let selectedMarkers = {
        destination: null,
        startingPoint: null
    };

    document.getElementById('instruction-message').innerHTML = '<p>Please select your destination point </p>';


    let personIcon = L.icon({
        
        iconUrl: 'person_icone.png',
        iconSize: [20, 20],
        iconAnchor: [0, 20]

    });
    let arriveIcon = L.icon({
        
        iconUrl: 'arrive_point.png',
        iconSize: [20, 20],
        iconAnchor: [0, 20]

    });

    let pointIcon = L.icon({
        iconUrl: 'point_icone.png',
        iconSize: [30, 30],
        iconAnchor: [15, 30]
    });
    let carIcon = L.icon({ 
        iconUrl: 'carIcone.jpg',
        iconSize: [40, 40],
        iconAnchor: [10, 10]

    });

    let points = [
        {name: 'A', lat: 43.570593, lng: 1.466513 },
        {name: 'B', lat: 43.570853, lng: 1.467021 },
        {name: 'C', lat: 43.571061, lng: 1.466837 },
        {name: 'D', lat: 43.570342, lng: 1.466029 },
        {name: 'E', lat: 43.570567, lng: 1.4658   },
        {name: 'F', lat: 43.570671, lng: 1.467218 },
        {name: 'G', lat: 43.570706, lng: 1.465661 },
        {name: 'H', lat: 43.570942, lng: 1.465445 },
        {name: 'I', lat: 43.571185, lng: 1.465951 },
        {name: 'J', lat: 43.57144,  lng: 1.466431 },
        {name: 'K', lat: 43.571233, lng: 1.466667 }
    ];

    let markers = points.map(point => {
        const marker = L.marker([point.lat, point.lng], { icon: pointIcon });
        // Attach additional data to the marker
        marker.pointData = point;
        // Add click event to each marker
        marker.on('click', function (e) {
            const markerData = e.target.pointData;


            // Cerrar la ventana emergente anterior si existe
            if (window.lastPopup) {
                window.lastPopup.close();
            }

            if (window.lastPopup) {
                window.lastPopup.close();
            }


            // Mostrar la nueva ventana emergente con el mensaje correspondiente
            const confirmed = confirm(`Your ${chooseDestination ? 'destination' : 'starting point'} is: ${markerData.name}\nDo you want to proceed?`);

            // Realizar acciones basadas en la respuesta
            if (confirmed) {
                // Acciones si se hace clic en "Sí"
                if (chooseDestination) {
                    document.getElementById('instruction-message').innerHTML = '<p>Please select your starting point <br> or <br> Press cancel for make another request </p>';

                    console.log("User destination point :", markerData);
                    chooseDestination = false;
                    selectedMarkers.destination = markerData;
                    map.removeLayer(marker);
                    arriveMarker = L.marker([point.lat, point.lng], { icon: arriveIcon }).addTo(map);

                } else {
                    console.log("User starting point :", markerData);
                    selectedMarkers.startingPoint = markerData;
                    map.removeLayer(marker);
                    personMarker = L.marker([point.lat, point.lng], { icon: personIcon }).addTo(map);
                    document.getElementById('instruction-message').innerHTML = '<p> Press start to continue <br> Press cancel for make another call</p>';
                }

            } else {
                // Acciones si se hace clic en "No"
            }
        });

        return marker;
    });

    let pointsGrp = L.layerGroup(markers);

    let osm = L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    });

    let map = L.map('map', {
        center: [43.57086, 1.46622],
        zoom: 20,
        layers: [osm, pointsGrp]
    });

      // Agrega el evento de clic al botón "Start"
    const startButton = document.getElementById('startButton');
    startButton.addEventListener('click', function () {
        // Guarda los marcadores en el LocalStorage
        localStorage.setItem('selectedMarkers', JSON.stringify(selectedMarkers));
    });


});