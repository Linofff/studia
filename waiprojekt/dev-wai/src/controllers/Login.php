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
            session_regenerate_id(true);

            $_SESSION["user_id"] = (string)$user["_id"];
            $_SESSION["user_login"] = $user["login"];
            $_SESSION["user_img"] = $user["profile_picture"];

            header("Location: /");
            exit();
        } else {
            echo "<p style='color:red'>" . "Invalid login or password. Try Again" . "</p>";
            $this->ShowView();
        }
    }

    public function Logout(): void {
        if (session_status() === PHP_SESSION_NONE) { session_start(); }
        session_destroy();
        header("Location: /");
        exit();
    }

}
