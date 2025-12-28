<?php

class UploadController {
    public function ShowView(): void {
        require "../views/uploadview.php";
    }

    public function HandleUpload(): void {
        define('allowed_extensions', ['jpg', 'jpeg', 'png']);
        define('max_size', 1024 * 1024);
        define('width', 200);
        define('height', 125);

        $file = $_FILES['file'];
        $file_extension = pathinfo($file['name'], PATHINFO_EXTENSION);

        $errors = [];

        if (!in_array($file_extension, allowed_extensions)) {
            $errors[] = 'File extension is not allowed. Allowed extensions are: ' . implode(', ', allowed_extensions);
            $errors[] = 'Your file extension: <b> ' . $file_extension . '</b>';
        }

        if ($file['size'] > max_size) {
            $errors[] = 'File size is too large. Max size is ' . max_size . ' bytes.';
            $errors[] = 'File size is ' . $file['size'] . ' bytes.';
        }

        if (!empty($errors)) {
            foreach ($errors as $error) {
                echo "<p class='error'>" . $error . "</p>";
            }
        } else {
            define('target_dir', 'images/');
            $target_file = target_dir . basename($_FILES['file']['name']);

            if (move_uploaded_file($_FILES['file']['tmp_name'], $target_file)) {
                echo "<p class='success'>File uploaded successfully.</p>";
                $target_mini = target_dir.pathinfo($target_file, PATHINFO_FILENAME)."_mini.".pathinfo($target_file, PATHINFO_EXTENSION);
                $this->resizeImage($target_file, $target_mini, width, height);
            } else {
                echo "<p class='error'>There was an error uploading the file.</p>";
            }
        }
    }

    private function resizeImage($sourceImagePath, $targetImagePath, $targetWidth, $targetHeight): void {
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
}