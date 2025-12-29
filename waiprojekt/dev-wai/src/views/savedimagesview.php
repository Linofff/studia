<?php
$title = "Saved Images";
require "partials/htmlhead.php";
?>
    <h2>Your selected photos</h2>

<?php if (empty($favoriteImages)): ?>
    <p>No photos selected.</p>
    <a href="/gallery">Go back to gallery</a>
<?php else: ?>

    <form method="POST">
        <div class="photos">
            <?php foreach ($favoriteImages as $image): ?>
                <div style="border: 1px solid white; margin: 10px; padding: 10px;">
                    <img src="<?= "images/" . $image["filename"] ?>" alt="image">
                    <p>Title: <?= $image["title"] ?> Author: <?= $image["author"] ?></p>

                    <label style="display:block; margin: 10px 0;">
                        Copies:
                        <input type="number"
                               name="quantities[<?= $image['filename'] ?>]"
                               value="<?= $image['quantity'] ?>"
                               min="1"
                               style="width: 60px; text-align: center; padding: 5px;">
                    </label>

                    <label style="color:red; cursor: pointer;">
                        <input type="checkbox" name="remove_checks[]" value="<?= $image['filename'] ?>">
                        Check to delete
                    </label>
                </div>
            <?php endforeach; ?>
        </div>

        <div style="margin-top: 20px; padding: 10px; border-top: 1px solid #ccc;">
            <button type="submit"
                    formaction="/saved/update"
                    style="background-color: #4CAF50; color: white; padding: 10px 20px; border: none; cursor: pointer;">
                Remember changed values
            </button>

            <button type="submit"
                    formaction="/saved/remove"
                    style="background-color: #f44336; color: white; padding: 10px 20px; border: none; margin-left: 10px; cursor: pointer;">
                Remove selected
            </button>
        </div>
    </form>
<?php endif; ?>

<?php require "partials/htmlfoot.php"; ?>