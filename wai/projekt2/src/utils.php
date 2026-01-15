<?php

class ImageUtils {
    public const ALLOWED_EXTS = ['jpg', 'jpeg', 'png'];
    public const MAX_SIZE = 1024 * 1024; // 1MB

    public static function resizeImage($sourceImagePath, $targetImagePath, $targetWidth, $targetHeight): void {
        $ext = strtolower(pathinfo($sourceImagePath, PATHINFO_EXTENSION));

        $src = match ($ext) {
            'jpg', 'jpeg' => imagecreatefromjpeg($sourceImagePath),
            'png' => imagecreatefrompng($sourceImagePath),
            default => null,
        };

        $resize = imagescale($src, $targetWidth, $targetHeight);

        imagepng($resize, $targetImagePath);

        imagedestroy($src);
        imagedestroy($resize);
    }

    public static function checkImageForErrors($file): array {
        $file_extension = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));

        $errors = [];

        if (!in_array($file_extension, self::ALLOWED_EXTS)) {
            $errors[] = 'File extension is not allowed. Allowed extensions are: ' . implode(', ', self::ALLOWED_EXTS);
            $errors[] = 'Your file extension: <b> ' . $file_extension . '</b>';
        }

        if ($file['size'] > self::MAX_SIZE) {
            $errors[] = 'File size is too large. Max size is ' . self::MAX_SIZE . ' bytes.';
            $errors[] = 'File size is ' . $file['size'] . ' bytes.';
        }

        return $errors;
    }
}