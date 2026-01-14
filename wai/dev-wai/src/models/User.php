<?php
require_once __DIR__."/Mongo.php";
class User {
    private $collection;
    public function __construct() {
        $mongo = new Mongo();
        $this->collection = $mongo->GetDatabase()->selectCollection("users");
    }
    public function Register($email, $login, $password, $photoFilename): bool {
        $existingUser = $this->collection->findOne([
            '$or' => [
                ["email" => $email],
                ["login" => $login]
            ]
        ]);

        if ($existingUser) {
            return false;
        }

        $hashedPassword = password_hash($password, PASSWORD_DEFAULT);

        $result = $this->collection->insertOne([
            "email" => $email,
            "login" => $login,
            "password" => $hashedPassword,
            "profile_picture" => $photoFilename,
            "created_at" => new MongoDB\BSON\UTCDatetime()
        ]);

        return $result->getInsertedCount() > 0;
    }

    public function Login($loginInput, $password): object|bool|array {
        $user = $this->collection->findOne([
            '$or' => [
                ["email" => $loginInput],
                ["login" => $loginInput]
            ]
        ]);

        if ($user && password_verify($password, $user['password'])) {
            return $user;
        }

        return false;
    }

}