const button = document.getElementById("theme-btn");

button.addEventListener("click", () => {
  // Toggle the "dark-theme" class on <body>
  document.body.classList.toggle("light-theme");

  // Update button text depending on current mode
  if (document.body.classList.contains("light-theme")) {
    button.textContent = "Dark Theme";
  } else {
    button.textContent = "Light Theme";
  }
});
