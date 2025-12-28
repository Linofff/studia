<?php

class GalleryController {
    private array $mini_images;
    private $imagesperpage = 3;
    private $totalimages = 0;
    private $currentpage = 0;
    private array $currentimages;

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

    private function GetImagesForPage($page): void {
        $temp = $this->currentpage * $this->imagesperpage;
        $this->currentimages = array();
        for ($i = 0; $i < $this->imagesperpage; $i++) {
            if ($i + $temp < $this->totalimages) {
                $this->currentimages[] = $this->mini_images[$i + $temp];
            }
        }
    }

    private function FetchImages(): void {
        $dir = "../web/images";
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