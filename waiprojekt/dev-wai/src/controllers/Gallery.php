<?php

class GalleryController {
    private array $mini_images;
    private $imagesperpage = 5;
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

        $currentimages = $this->currentimages;
        $currentpage = $this->currentpage;

        require "../views/galleryview.php";
    }

    private function FetchImages(): void {
        require_once __DIR__ . "/../models/ImageModel.php";
        $imageModel = new ImageModel();
        $user = $_SESSION['user_login'] ?? null;

        $this->totalimages = $imageModel->CountAllowedImages($user);

        $this->currentpage = isset($_GET["page"]) ? max(0, intval($_GET["page"])) : 0;
        $skip = $this->currentpage * $this->imagesperpage;
        $this->currentimages = $imageModel->GetAllowedImagesPaged($user, $this->imagesperpage, $skip);
    }

    public function SaveSelection(): void {
        if ($_SERVER['REQUEST_METHOD'] === 'POST' && isset($_POST['checks'])) {
            foreach ($_POST['checks'] as $filename) {
                if (!isset($_SESSION['selected_images'][$filename])) {
                    $_SESSION['selected_images'][$filename] = 1;
                }
            }
        }
        $page = isset($_POST['page']) ? intval($_POST['page']) : 0;
        header("Location: /gallery?page=$page");
        exit;
    }


}