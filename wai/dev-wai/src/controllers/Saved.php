<?php

class SavedController {

    public function __construct() {
        if (session_status() === PHP_SESSION_NONE) {
            session_start();
        }
        if (!isset($_SESSION['selected_images'])) {
            $_SESSION['selected_images'] = [];
        }
    }

    public function ShowView(): void {
        $selected = $_SESSION['selected_images'];
        $selectedImages = [];

        require_once __DIR__ . "/../models/Image.php";
        $imageModel = new Image();

        foreach ($selected as $filename => $qty) {
            $image = $imageModel->getImage($filename);
            if ($image) {
                $selectedImages[] = [
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
}
