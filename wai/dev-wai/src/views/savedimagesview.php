<?php
$title = "Saved Images";
require "partials/htmlhead.php";
?>
<h2>Your selected photos</h2>

<?php if (empty($selectedImages)): ?>
    <p>No photos selected.</p>
    <a href="/gallery">Go back to gallery</a>
<?php else: ?>

    <form method="POST">
        <div class="photos">
            <?php foreach ($selectedImages as $image): ?>
                <div class="saved_gallery">
                    <img src="<?= "images/" . $image["filename"] ?>" alt="image">
                    <p>Title: <?= $image["title"] ?> Author: <?= $image["author"] ?></p>

                    <label class="saved_gallery_label">
                        Copies:
                        <input type="number"
                               name="quantities[<?= $image['filename'] ?>]"
                               value="<?= $image['quantity'] ?>"
                               min="1"
                               class="quantity">
                    </label>

                    <label style="color:red; cursor: pointer;">
                        <input type="checkbox" name="remove_checks[]" value="<?= $image['filename'] ?>">
                        Check to delete
                    </label>
                </div>
            <?php endforeach; ?>
        </div>

        <div class="saved_gallery_buttons_div">
            <button type="submit"
                    formaction="/saved/update"
                    class="update_values_button">
                Remember changed values
            </button>

            <button type="submit"
                    formaction="/saved/remove"
                    class="remove_selected_button">
                Remove selected
            </button>
        </div>
    </form>
<?php endif; ?>

<?php require "partials/htmlfoot.php"; ?>