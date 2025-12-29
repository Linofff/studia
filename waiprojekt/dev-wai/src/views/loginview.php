<?php
$title = "Login";
require "partials/htmlhead.php" ?>
<h2>Log in</h2>

<form class="form_class" action="/login" method="POST">
    <label>
        Login:
        <input type="text" name="login" required>
    </label>
    <label>

    </label>
    <br>
    <label>
        Password:
        <input type="password" name="password" required>
    </label>
    <br>
    <input type="submit" value="Log In">
</form>
<?php
require "partials/htmlfoot.php" ?>
