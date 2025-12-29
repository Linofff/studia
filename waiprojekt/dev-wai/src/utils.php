<?php
function resizeImage($sourceImagePath, $targetImagePath, $targetWidth, $targetHeight): void {
    $ext = strtolower(pathinfo($sourceImagePath, PATHINFO_EXTENSION));

    $src = match ($ext) {
        'jpg', 'jpeg' => imagecreatefromjpeg($sourceImagePath),
        'png' => imagecreatefrompng($sourceImagePath),
    };

    $resize = imagescale($src, $targetWidth, $targetHeight);

    imagepng($resize, $targetImagePath);

    imagedestroy($src);
    imagedestroy($resize);
}
