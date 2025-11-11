// const button = document.getElementById("theme-btn");
//
// button.addEventListener("click", () => {
//   document.body.classList.toggle("light-theme");
//
//   if (document.body.classList.contains("light-theme")) {
//     button.textContent = "Dark Theme";
//   } else {
//     button.textContent = "Light Theme";
//   }
// });


const button = document.getElementById("theme-btn");
const body = document.body;

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
