<?php
$title = "UploadPhoto";
require "partials/htmlhead.php" ?>
<h2>Upload Photo</h2>
<form action="/" method="POST" enctype="multipart/form-data">
    <label>
        Chose photo to upload
        <input type="file" name="file">
    </label>
    <input type="submit" value="upload">
</form>
<?php
require "partials/htmlfoot.php" ?>