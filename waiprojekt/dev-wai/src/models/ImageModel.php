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

    public function GetAllowedImagesPaged($currentUser, $limit, $skip): array {
        if ($currentUser) {
            $filter = [
                '$or' => [
                    ['privacy' => 'public'],
                    ['privacy' => 'private', 'author' => $currentUser]
                ]
            ];
        } else {
            $filter = ['privacy' => 'public'];
        }

        $options = [
            'limit' => $limit,
            'skip' => $skip,
            'projection' => ['filename' => 1, 'author' => 1, 'title' => 1, 'privacy' => 1],
        ];

        $cursor = $this->collection->find($filter, $options);

        return $cursor->toArray();
    }

    public function CountAllowedImages($currentUser): int {
        $filter = ($currentUser)
            ? ['$or' => [['privacy' => 'public'], ['privacy' => 'private', 'author' => $currentUser]]]
            : ['privacy' => 'public'];

        return $this->collection->countDocuments($filter);
    }

}
