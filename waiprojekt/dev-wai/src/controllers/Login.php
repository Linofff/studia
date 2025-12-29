<?php

require_once "../models/User.php";

class LoginController {
    public function ShowView(): void {
        require "../views/loginview.php";
    }

    public function HandleLogin(): void {
        $loginInput = $_POST["login"];
        $password = $_POST["password"];

        $userModel = new User();
        $user = $userModel->Login($loginInput, $password);

        if ($user) {
            if (session_status() === PHP_SESSION_NONE) { session_start(); }

            $_SESSION["user_id"] = (string)$user["_id"];
            $_SESSION["user_login"] = $user["login"];
            $_SESSION["user_img"] = $user["profile_picture"];

            header("Location: /");
            exit();
        } else {
            echo "Invalid login or password <a href='/login'>Try again</a>";
        }
    }

    public function Logout(): void {
        if (session_status() === PHP_SESSION_NONE) { session_start(); }
        session_destroy();
        header("Location: /");
        exit();
    }

}
