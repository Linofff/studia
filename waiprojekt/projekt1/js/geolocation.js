var options = {
  maximumAge: 100,
  enableHighAccuracy: false,
  timeout: 20000,
};
var store = {
  name: "Strefa kostek Lublin",
  lat: 51.205773,
  lon: 22.562113,
};
function deg2rad(deg) {
  return deg * (Math.PI / 180);
}
function getDistance(lat1, lon1, lat2, lon2) {
  var R = 6371; // Earth's radius in km
  var dLat = deg2rad(lat2 - lat1);
  var dLon = deg2rad(lon2 - lon1);
  var a = Math.pow(Math.sin(dLat / 2), 2) +
    Math.cos(deg2rad(lat1)) *
    Math.cos(deg2rad(lat2)) *
    Math.pow(Math.sin(dLon / 2), 2);
  var c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c;
}
var success = function(pos) {
  var userLat = pos.coords.latitude;
  var userLon = pos.coords.longitude;
  var distance = getDistance(userLat, userLon, store.lat, store.lon);
  console.log("Distance: ".concat(distance.toFixed(2), " km"));
  var storeDiv = document.getElementById("store");
  if (!storeDiv)
    return;
  storeDiv.textContent = "";
  var paragraph = document.createElement("p");
  paragraph.textContent = "Odleg\u0142o\u015B\u0107 od sklepu ".concat(store.name, " w linii prostej to: ").concat(distance.toFixed(2), " km.");
  storeDiv.appendChild(paragraph);
};
var error = function(err) {
  console.error("Błąd geolokalizacji:", err.message);
  var storeDiv = document.getElementById("store");
  if (!storeDiv)
    return;
  storeDiv.textContent = "";
  var paragraph = document.createElement("p");
  paragraph.textContent =
    "Nie udało się ustalić lokalizacji. Upewnij się, że zezwoliłeś przeglądarce na dostęp do lokalizacji.";
  storeDiv.appendChild(paragraph);
};
navigator.geolocation.getCurrentPosition(success, error, options);
