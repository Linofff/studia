<?php
require_once __DIR__ . "/../../vendor/autoload.php";
const MONGO_URI = "mongodb://wai_web:w%40i_w3b@192.168.56.10:27017/?authSource=wai";

class Mongo {
    private $client;

    public function __construct() {
        try {
            $this->client = new MongoDB\Client(MONGO_URI);
        } catch (Exception $e) {
            die($e->getMessage());
        }
    }

    public function GetDatabase(): \MongoDB\Database {
        return $this->client->selectDatabase("wai");
    }
}