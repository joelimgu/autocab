document.addEventListener("DOMContentLoaded", function () {
    // Recupera los marcadores seleccionados del LocalStorage
    const storedMarkers = localStorage.getItem('selectedMarkers');


    let osm = L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
        attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
    });

    let personIcon = L.icon({
        
        iconUrl: 'person_icone.png',
        iconSize: [40, 40],
        iconAnchor: [10, 10]

    });
    let arriveIcon = L.icon({
        
        iconUrl: 'arrive_point.png',
        iconSize: [40, 40],
        iconAnchor: [10, 10]

    });

    let pointIcon = L.icon({
        iconUrl: 'point_icone.png',
        iconSize: [40, 40],
        iconAnchor: [10, 10]
    });
    let carIcon = L.icon({ 
        iconUrl: 'carIcon.png',
        iconSize: [100, 100],
        iconAnchor: [50, 50]

    });

    let map = L.map('map', {
        center: [43.57086, 1.46622],
        zoom: 20,
        layers: [osm]
    });

    
    if (storedMarkers) {
        const selectedMarkers = JSON.parse(storedMarkers);

        // Verifica si se seleccionó un destino
        if (selectedMarkers.destination) {
            // Accede a la información del marcador de destino
            const destinationData = selectedMarkers.destination;
            console.log("Destination point:", destinationData);

            // Crea y muestra el marcador de destino en el mapa de "Waiting"
            const destinationMarker = L.marker([destinationData.lat, destinationData.lng], { icon: arriveIcon }).addTo(map);
        }

        // Verifica si se seleccionó un punto de partida
        if (selectedMarkers.startingPoint) {
            // Accede a la información del marcador de punto de partida
            const startingPointData = selectedMarkers.startingPoint;
            console.log("Starting point:", startingPointData);

            // Crea y muestra el marcador de punto de partida en el mapa de "Waiting"
            const startingPointMarker = L.marker([startingPointData.lat, startingPointData.lng], { icon: personIcon }).addTo(map);
        }
    } else {
        console.log("No stored markers found.");
    }
    
    // here we obtain, the point where is the car for print it
    const carMaker = L.marker([43.570582, 1.466470], { icon: carIcon }).addTo(map);






});
