<?php

class GalleryController {
    private array $mini_images;
    private $imagesperpage = 3;
    private $totalimages = 0;
    private $currentpage = 0;
    private array $currentimages;

    public function __construct() {
        if (session_status() === PHP_SESSION_NONE) {
            session_start();
        }
        if (!isset($_SESSION['selected_images'])) {
            $_SESSION['selected_images'] = [];
        }
    }

    public function ShowView(): void {
        $this->FetchImages();
        if (!isset($_GET["page"])) {
            $this->currentpage = 0;
        } else {
            $this->currentpage = intval($_GET["page"]);
            if ($this->currentpage < 0) {
                $this->currentpage = 0;
            }
            if ($this->currentpage > floor($this->totalimages / $this->imagesperpage)) {
                $this->currentpage = floor($this->totalimages / $this->imagesperpage);
            }
        }
        $this->GetImagesForPage($this->currentpage);
        $currentimages = $this->currentimages;
        $currentpage = $this->currentpage;
        require "../views/galleryview.php";
    }

    public function SaveSelection(): void {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['checks'])) {
            foreach ($_POST['checks'] as $filename) {
                if (!isset($_SESSION['selected_images'][$filename])) {
                    $_SESSION['selected_images'][$filename] = 1;
                }
            }
        }
        header("Location: /gallery");
        exit;
    }

    public function ShowSelected(): void {
        $favorites = $_SESSION['selected_images'];
        $favoriteImages = [];

        require_once __DIR__ . "/../models/ImageModel.php";
        $imageModel = new ImageModel();

        foreach ($favorites as $filename => $qty) {
            $image = $imageModel->getImage($filename);
            if ($image) {
                $favoriteImages[] = [
                    "filename" => $filename,
                    "author" => $image['author'],
                    "title" => $image['title'],
                    "quantity" => $qty,
                ];
            }
        }
        require "../views/savedimagesview.php";
    }

    public function RemoveFromSelected(): void {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['remove_checks'])) {
            $toRemove = $_POST['remove_checks'];

            foreach ($toRemove as $filename) {
                if (isset($_SESSION['selected_images'][$filename])) {
                    unset($_SESSION['selected_images'][$filename]);
                }
            }
        }

        header("Location: /saved");
        exit;
    }

    public function UpdateQuantities(): void {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['quantities'])) {
            foreach ($_POST['quantities'] as $filename => $qty) {
                $qty = intval($qty);
                if ($qty < 1) {
                    $qty = 1;
                }

                if (isset($_SESSION['selected_images'][$filename])) {
                    $_SESSION['selected_images'][$filename] = $qty;
                }
            }
        }
        header("Location: /saved");
        exit;
    }

    private function GetImagesForPage($page): void {
        $temp = $this->currentpage * $this->imagesperpage;
        $this->currentimages = array();

        require_once __DIR__."/../models/ImageModel.php";
        $imageModel = new ImageModel();

        for ($i = 0; $i < $this->imagesperpage; $i++) {
            if ($i + $temp < $this->totalimages) {
                if (isset($this->mini_images[$i + $temp])) {
                    $filename = $this->mini_images[$i + $temp];
                    $image = $imageModel->getImage($filename);
                    if ($image) {
                        $this->currentimages[] = [
                            "filename" => $filename,
                            "author" => $image["author"],
                            "title" => $image["title"],
                            "privacy" => isset($image["privacy"]) ? $image["privacy"] : 'public',
                        ];
                    }
                }
            }
        }
    }

    private function FetchImages(): void {
       require_once __DIR__ . "/../models/ImageModel.php";
       $imageModel = new ImageModel();

       $user = isset($_SESSION['user_login']) ? $_SESSION['user_login'] : null;

       $this->mini_images = $imageModel->getAllowedImagesNames($user);

       $this->totalimages = count($this->mini_images);
    }
}