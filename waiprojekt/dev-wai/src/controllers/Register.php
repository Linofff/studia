<?php
require_once __DIR__ . "/../models/User.php";
require_once "Upload.php";

class RegisterController {

    public function ShowView(): void {
        require "../views/registerview.php";
    }

    public function HandleRegister(): void {
        $email = $_POST['email'];
        $login = $_POST['login'];
        $pass = $_POST['password'];
        $repeat = $_POST['repeat_password'];

        if ($pass !== $repeat) {
            echo "Passwords do not match.";
            return;
        }
        define('allowed_extensions', ['jpg', 'jpeg', 'png']);
        define('max_size', 1024 * 1024);
        define('width', 200);
        define('height', 200);

        $file = $_FILES['profile_picture'];
        $file_extension = strtolower(pathinfo($file['name'], PATHINFO_EXTENSION));
        $errors = [];

        if (!in_array($file_extension, allowed_extensions)) {
            $errors[] = 'File extension is not allowed. Allowed extensions are: ' . implode(', ', allowed_extensions);
        }

        if ($file['size'] > max_size) {
            $errors[] = 'File size is too large. Max size is ' . max_size . ' bytes.';
        }

        if (!empty($errors)) {
            foreach ($errors as $error) {
                echo "<p style='color:red'>" . $error . "</p>";
            }
            $this->ShowView();
            return;
        }

        $target_dir = '../web/ProfilesFoto/';

        $uniqueID = uniqid("user_", true);
        $finalFileName = $uniqueID . ".png";
        $finalFilePath = $target_dir . $finalFileName;

        $tempFilePath = $target_dir . $uniqueID . "_temp." . $file_extension;

        if (move_uploaded_file($file['tmp_name'], $tempFilePath)) {

            try {
                resizeImage($tempFilePath, $finalFilePath, width, height);
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

    // Your exact function from Upload.php

}