<?php
require "../Router.php";

$router = new Router();

$router->AddRoute("/", "Upload", "ShowView", "GET");
$router->AddRoute("/", "Upload", "HandleUpload", "POST");

$router->AddRoute("/gallery", "Gallery", "ShowView", "GET");

$router->Dispatch(parse_url($_SERVER["REQUEST_URI"])["path"], $_SERVER["REQUEST_METHOD"]);