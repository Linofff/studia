<?php
$title = "UploadPhoto";
require "partials/htmlhead.php" ?>
    <h2>Upload Photo</h2>
    <form class="form_class" action="/" method="POST" enctype="multipart/form-data">
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
        <?php if (isset($_SESSION['user_id'])): ?>
        <label>
            Input image author
            <input type="text" name="imageauthor" value="<?= $_SESSION['user_login'] ?>">
            <div style="margin: 10px 0;">
                <span>Photo Visibility</span>

                <label style="margin-right: 15px; cursor: pointer;">
                    <input type="radio" name="privacy" value="public" checked>
                    public
                </label>

                <label style="cursor: pointer;">
                    <input type="radio" name="privacy" value="private">
                    private
                </label>
            </div>
            <?php else: ?>
                <label>
                    Input image author
                    <input type="text" name="imageauthor">
                </label>
            <?php endif; ?>
            <input type="submit" value="upload">
    </form>
<?php
require "partials/htmlfoot.php" ?>