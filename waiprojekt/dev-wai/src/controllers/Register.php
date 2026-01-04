<?php
require_once __DIR__ . "/../models/User.php";
require_once "Upload.php";

class RegisterController {

    public function ShowView(): void {
        require "../views/registerview.php";
    }

    const int WIDTH = 100;
    const int HEIGHT = 100;
    const string TARGET_DIR = '../web/ProfilesFoto/';

    public function HandleRegister(): void {
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


        $file_extension = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));
        $uniqueID = uniqid("user_", true);
        $finalFileName = $uniqueID . ".png";
        $finalFilePath = self::TARGET_DIR . $finalFileName;

        $tempFilePath = self::TARGET_DIR . $uniqueID . "_temp." . $file_extension;

        if (move_uploaded_file($file['tmp_name'], $tempFilePath)) {
            try {
                ImageUtils::resizeImage($tempFilePath, $finalFilePath, self::WIDTH, self::HEIGHT);
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