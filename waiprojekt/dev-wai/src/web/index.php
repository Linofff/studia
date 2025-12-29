<?php
session_start();
require "../utils.php";
require "../Router.php";


$router = new Router();

$router->AddRoute("/", "Upload", "ShowView", "GET");
$router->AddRoute("/", "Upload", "HandleUpload", "POST");

$router->AddRoute("/gallery", "Gallery", "ShowView", "GET");
$router->AddRoute("/gallery/save", "Gallery", "SaveSelection", "POST");
$router->AddRoute("/saved", "Gallery", "ShowSelected", "GET");
$router->AddRoute("/saved/remove", "Gallery", "RemoveFromSelected", "POST");
$router->AddRoute("/saved/update", "Gallery", "UpdateQuantities", "POST");

$router->AddRoute("/register", "Register", "ShowView", "GET");
$router->AddRoute("/register", "Register", "HandleRegister", "POST");

$router->AddRoute("/login", "Login", "ShowView", "GET");
$router->AddRoute("/login", "Login", "HandleLogin", "POST");
$router->AddRoute("/logout", "Login", "Logout", "GET");


$router->Dispatch(parse_url($_SERVER["REQUEST_URI"])["path"], $_SERVER["REQUEST_METHOD"]);