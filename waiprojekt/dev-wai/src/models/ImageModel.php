<?php
require_once "Mongo.php";

class ImageModel {
    private $mongo;
    private $collection;
    public function saveImage($file, $author, $title): void {
        $this->collection->insertOne([
            "author" => $author,
            "title" => $title,
            "filename" => $file
        ]);
    }

    public function __construct() {
        $this->mongo = new Mongo();
        $this->collection = $this->mongo->GetDatabase()->images;
    }

    public function getImage($filename) {
        $image = $this->collection->findOne(["filename" => $filename]);
        return $image;
    }
}
