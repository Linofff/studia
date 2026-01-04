<?php

class UploadController {
    public function ShowView(): void {
        require "../views/uploadview.php";
    }

    public function HandleUpload(): void {
        define('width', 200);
        define('height', 125);

        $file = $_FILES['file'];
        $errors = ImageUtils::checkImageForErrors($file);

        if (!empty($errors)) {
            foreach ($errors as $error) {
                echo "<p class='error'>" . $error . "</p>";
            }
        } else {
            define('target_dir', 'images/');

            $file_extension = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));
            $unique_name = uniqid('img_', true);
            $new_file_name = $unique_name . '.' . $file_extension;
            $target_file = target_dir . $new_file_name;

            if (move_uploaded_file($file['tmp_name'], $target_file)) {

                if (isset($_SESSION['user_id'])) {
                    $privacy = isset($_POST['privacy']) && $_POST['privacy'] === 'private' ? 'private' : 'public';
                } else {
                    $privacy = 'public';
                }
                $author = $_POST["imageauthor"];

                echo "<p class='success'>File uploaded successfully.</p>";
                echo "<a href='/gallery'>Go to the gallery</a>";

                $target_mini = target_dir.pathinfo($target_file, PATHINFO_FILENAME)."_mini.".pathinfo($target_file, PATHINFO_EXTENSION);
                ImageUtils::resizeImage($target_file, $target_mini, width, height);

                require_once __DIR__ . "/../models/Image.php";
                $imageModel = new Image();

                $imageModel->saveImage(
                    pathinfo($target_file, PATHINFO_FILENAME)."_mini.".pathinfo($target_file, PATHINFO_EXTENSION),
                    $author,
                    $_POST['imagetitle'],
                    $privacy
                );

            } else {
                echo "<p class='error'>There was an error uploading the file.</p>";
            }
        }
    }

}