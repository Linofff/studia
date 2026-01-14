const options: PositionOptions = {
  maximumAge: 100,
  enableHighAccuracy: false,
  timeout: 20000,
};

interface Store {
  name: string;
  lat: number;
  lon: number;
}

const store: Store = {
  name: "Strefa kostek Lublin",
  lat: 51.205773,
  lon: 22.562113,
};

function deg2rad(deg: number): number {
  return deg * (Math.PI / 180);
}

function getDistance(lat1: number, lon1: number, lat2: number, lon2: number): number {
  const R = 6371;
  const dLat = deg2rad(lat2 - lat1);
  const dLon = deg2rad(lon2 - lon1);

  const a =
    Math.sin(dLat / 2) ** 2 +
    Math.cos(deg2rad(lat1)) *
    Math.cos(deg2rad(lat2)) *
    Math.sin(dLon / 2) ** 2;

  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1 - a));
  return R * c;
}

const success = (pos: GeolocationPosition): void => {
  const userLat = pos.coords.latitude;
  const userLon = pos.coords.longitude;

  const distance = getDistance(userLat, userLon, store.lat, store.lon);
  console.log(`Distance: ${distance.toFixed(2)} km`);

  const storeDiv = document.getElementById("store");
  if (!storeDiv) return;

  storeDiv.textContent = "";

  const paragraph = document.createElement("p");
  paragraph.textContent = `Odległość od sklepu ${store.name} w linii prostej to: ${distance.toFixed(2)} km.`;

  storeDiv.appendChild(paragraph);
};

const error = (err: GeolocationPositionError): void => {
  console.error("Błąd geolokalizacji:", err.message);

  const storeDiv = document.getElementById("store");
  if (!storeDiv) return;

  storeDiv.textContent = "";

  const paragraph = document.createElement("p");
  paragraph.textContent =
    "Nie udało się ustalić lokalizacji. Upewnij się, że zezwoliłeś przeglądarce na dostęp do lokalizacji.";

  storeDiv.appendChild(paragraph);
};

navigator.geolocation.getCurrentPosition(success, error, options);
