<?php
$title = "UploadPhoto";
require "partials/htmlhead.php" ?>

    <h2>Upload Photo</h2>
    <form class="form_class" action="/upload" method="POST" enctype="multipart/form-data">

        <div class="form-group">
            <label for="file_input">Choose photo to upload</label>
            <input type="file" name="file" id="file_input">
        </div>

        <div class="form-group">
            <label for="imagetitle">Input image title</label>
            <input type="text" name="imagetitle" id="imagetitle">
        </div>

        <div class="form-group">
            <label for="imageauthor">Input image author</label>
            <?php if (isset($_SESSION['user_id'])): ?>
                <input type="text" name="imageauthor" id="imageauthor"
                       value="<?= ($_SESSION['user_login']) ?>">
            <?php else: ?>
                <input type="text" name="imageauthor" id="imageauthor">
            <?php endif; ?>
        </div>

        <?php if (isset($_SESSION['user_id'])): ?>
            <div style="margin: 10px 0;">
                <fieldset style="border: none; padding: 0; margin: 0;">
                    <legend style="margin-bottom: 5px;">Photo Visibility</legend>

                    <label style="margin-right: 15px; cursor: pointer;">
                        <input type="radio" name="privacy" value="public" checked>
                        public
                    </label>

                    <label style="cursor: pointer;">
                        <input type="radio" name="privacy" value="private">
                        private
                    </label>
                </fieldset>
            </div>
        <?php endif; ?>

        <input type="submit" value="upload">
    </form>

<?php
require "partials/htmlfoot.php" ?>