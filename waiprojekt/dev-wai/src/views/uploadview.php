<?php
$title = "UploadPhoto";
require "partials/htmlhead.php" ?>
    <h2>Upload Photo</h2>
    <form action="/" method="POST" enctype="multipart/form-data">
        <label>
            Choose photo to upload
            <input type="file" name="file">
        </label>
        <br>
        <label>
            Input image title
            <input type="text" name="imagetitle">
        </label>
        <br>
        <label>
            Input image author
            <input type="text" name="imageauthor">
        </label>
        <br>
        <input type="submit" value="upload">
    </form>
<?php
require "partials/htmlfoot.php" ?>