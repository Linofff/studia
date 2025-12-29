<?php
$title = "Register";
require "partials/htmlhead.php" ?>
    <h2>Register</h2>

    <form class="form_class" action="/register" method="POST" enctype="multipart/form-data">
        <label>Email: <input type="email" name="email" required></label><br>
        <label>Login: <input type="text" name="login" required></label><br>
        <label>Password: <input type="password" name="password" required></label><br>
        <label>Repeat Password: <input type="password" name="repeat_password" required></label><br>

        <label>Profile Picture:
            <input type="file" name="profile_picture" accept="image/jpeg, image/png" required>
        </label>
        <br>

        <input type="submit" value="Register">
    </form>
<?php require "partials/htmlfoot.php" ?>