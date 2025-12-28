<?php
require "../Router.php";

$router = new Router();

$router->AddRoute("/", "Upload", "ShowView", "GET");
$router->AddRoute("/", "Upload", "HandleUpload", "POST");

$router->Dispatch($_SERVER["REQUEST_URI"], $_SERVER["REQUEST_METHOD"]);
