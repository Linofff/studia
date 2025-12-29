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
        for ($i = 0; $i < $this->imagesperpage; $i++) {
            if ($i + $temp < $this->totalimages) {
                require_once __DIR__ . "/../models/ImageModel.php";
                $imageModel = new ImageModel();
                // Check if index exists to avoid errors
                if (isset($this->mini_images[$i + $temp])) {
                    $image = $imageModel->getImage($this->mini_images[$i + $temp]);
                    $this->currentimages[] = [
                        "filename" => $this->mini_images[$i + $temp],
                        "author" => $image["author"],
                        "title" => $image["title"],
                    ];
                }
            }
        }
    }

    private function FetchImages(): void {
        $dir = "../web/images";
        if (is_dir($dir)) {
            $files = scandir($dir);
            foreach ($files as $file) {
                if ($file != "." && $file != "..") {
                    if (str_contains($file, "_mini")) {
                        $this->mini_images[] = $file;
                        $this->totalimages++;
                    }
                }
            }
        }
    }
}