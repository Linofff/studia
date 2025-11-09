const options = {
  maximumAge: 100,
  enableHighAccuracy: false,
  timeout: 100000,
};

const store = { name: "Strefa kostek Lublin", lat: 51.205773, lon: 22.562113 };

function getDistance(lat1, lon1, lat2, lon2) {
  var R = 6371;
  var dLat = deg2rad(lat2 - lat1);
  var dLon = deg2rad(lon2 - lon1);
  var a =
    Math.sin(dLat / 2) * Math.sin(dLat / 2) +
    Math.cos(deg2rad(lat1)) * Math.cos(deg2rad(lat2)) *
    Math.sin(dLon / 2) * Math.sin(dLon / 2)
    ;
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  var d = R * c;
  return d;
}

function deg2rad(deg) {
  return deg * (Math.PI / 180)
}

const success = (pos) => {
  const userLat = pos.coords.latitude;
  const userLon = pos.coords.longitude;

  const distance = getDistance(userLat, userLon, store.lat, store.lon);

  console.log("Distance: " + distance + " km");

  const storeDiv = document.getElementById("store");

  storeDiv.textContent = "";

  const paragraph = document.createElement("p");
  paragraph.textContent = `Odległość od twojej obecnej lokalizacji w lini prostej to: ${distance.toFixed(2)} km.`;

  storeDiv.appendChild(paragraph);

};

const error = (err) => {
  console.error("Błąd geolokalizacji:", err.message);
};

navigator.geolocation.getCurrentPosition(success, error, options);
