<?php
require_once "Mongo.php";

class ImageModel {
    private $mongo;
    private $collection;

    public function saveImage($file, $author, $title, $privacy): void {
        $this->collection->insertOne([
            "author" => $author,
            "title" => $title,
            "filename" => $file,
            "privacy" => $privacy
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

    public function GetAllowedImagesNames($currentUser): array {
        if ($currentUser){
            $filter = [
                '$or' => [
                    ['privacy' => 'public'],
                    ['privacy' => 'private', 'author' => $currentUser]
                ]
            ];
        } else {
            $filter = ['privacy' => 'public'];
        }

        $cursor = $this->collection->find($filter, ['projection' => ['filename' => 1]]);

        $resoults = [];
        foreach ($cursor as $document) {
            $resoults[] = $document["filename"];
        }
        return $resoults;
    }

}
