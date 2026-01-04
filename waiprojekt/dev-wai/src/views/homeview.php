<?php
$title = "Home";
require "partials/htmlhead.php";
?>
<h2>Welcome to the home page
    <?php if (isset($_SESSION['user_login']))
        echo $_SESSION['user_login'];
    ?>
</h2>

<p>On this page, you can share your photos with others, or store it for yourself privately.</p>
<p>Additionally, you can order photos that are stored in the gallery.</p>

<?php
require "partials/htmlfoot.php"; ?>
