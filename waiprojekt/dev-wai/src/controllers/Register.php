<?php
require_once __DIR__ . "/../models/User.php";
require_once "Upload.php";

class RegisterController {

    public function ShowView(): void {
        require "../views/registerview.php";
    }

    public function HandleRegister(): void {
        define('width', 100);
        define('height', 100);

        $email = $_POST['email'];
        $login = $_POST['login'];
        $pass = $_POST['password'];
        $repeat = $_POST['repeat_password'];

        if ($pass !== $repeat) {
            echo "Passwords do not match.";
            return;
        }

        $file = $_FILES['file'];
        $errors = ImageUtils::checkImageForErrors($file);

        if (!empty($errors)) {
            foreach ($errors as $error) {
                echo "<p style='color:red'>" . $error . "</p>";
            }
            $this->ShowView();
            return;
        }

        $target_dir = '../web/ProfilesFoto/';

        $file_extension = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));
        $uniqueID = uniqid("user_", true);
        $finalFileName = $uniqueID . ".png";
        $finalFilePath = $target_dir . $finalFileName;

        $tempFilePath = $target_dir . $uniqueID . "_temp." . $file_extension;

        if (move_uploaded_file($file['tmp_name'], $tempFilePath)) {
            try {
                ImageUtils::resizeImage($tempFilePath, $finalFilePath, width, height);
                if (file_exists($tempFilePath)) {
                    unlink($tempFilePath);
                }

                $userModel = new User();
                if ($userModel->Register($email, $login, $pass, $finalFileName)) {
                    header("Location: /login");
                    exit();

                } else {
                    echo "<p style='color:red'>Registration failed. Login or Email already taken.</p>";
                }
            } catch (Exception $e) {
                echo "<p style='color:red'>Error processing image.</p>";
            }
        } else {
            echo "<p style='color:red'>There was an error uploading the file.</p>";
        }
    }

}