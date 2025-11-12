const button = document.getElementById("theme-btn") as HTMLButtonElement;
const body: HTMLElement = document.body;

if (button) {
  const savedTheme = localStorage.getItem("theme");

  if (savedTheme === "light") {
    body.classList.add("light-theme");
    button.textContent = "Dark Theme";
  } else {
    button.textContent = "Light Theme";
  }

  button.addEventListener("click", () => {
    body.classList.toggle("light-theme");

    if (body.classList.contains("light-theme")) {
      localStorage.setItem("theme", "light");
      button.textContent = "Dark Theme";
    } else {
      localStorage.setItem("theme", "dark");
      button.textContent = "Light Theme";
    }
  });
} else {
  console.warn("Theme toggle button not found in the DOM.");
}

