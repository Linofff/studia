<?php
session_start();
require "../utils.php";
require "../Router.php";

$router = new Router();

$router->AddRoute("/", "Home", "ShowView", "GET");

$router->AddRoute("/upload", "Upload", "ShowView", "GET");
$router->AddRoute("/upload", "Upload", "HandleUpload", "POST");

$router->AddRoute("/gallery", "Gallery", "ShowView", "GET");
$router->AddRoute("/gallery/save", "Gallery", "SaveSelection", "POST");

$router->AddRoute("/saved", "Saved", "ShowView", "GET");
$router->AddRoute("/saved/remove", "Saved", "RemoveFromSelected", "POST");
$router->AddRoute("/saved/update", "Saved", "UpdateQuantities", "POST");

$router->AddRoute("/register", "Register", "ShowView", "GET");
$router->AddRoute("/register", "Register", "HandleRegister", "POST");

$router->AddRoute("/login", "Login", "ShowView", "GET");
$router->AddRoute("/login", "Login", "HandleLogin", "POST");
$router->AddRoute("/logout", "Login", "Logout", "GET");


$router->Dispatch(parse_url($_SERVER["REQUEST_URI"])["path"], $_SERVER["REQUEST_METHOD"]);