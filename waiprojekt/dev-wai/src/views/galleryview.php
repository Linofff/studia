<?php
$title = "Gallery";
require "partials/htmlhead.php";
?>
    <h2>Gallery</h2>

    <form action="/gallery/save" method="POST">
        <input type="hidden" name="page" value="<?php echo $currentpage; ?>">
        <div class="photos">
            <?php if (empty($currentimages)): ?>
                <p>No photos to display.</p>
            <?php else: ?>
                <?php foreach ($currentimages as $image): ?>
                    <div class="photo-card">
                        <img src="<?= "images/" . $image["filename"] ?>" alt="image">
                        <p>Author: <?= $image["author"] ?>, Title: <?= $image["title"] ?></p>

                        <label>
                            <?php
                            $isChecked = isset($_SESSION['selected_images'][$image["filename"]]) ? 'checked' : '';
                            ?>
                            <input type="checkbox" name="checks[]" value="<?= $image['filename'] ?>" <?= $isChecked ?>>
                            Remember this photo
                        </label>

                        <?php if (isset($image['privacy']) && $image['privacy'] === 'private'): ?>
                            <p class="private_photo_text"">(Private Photo)</p>
                        <?php endif; ?>

                    </div>
                <?php endforeach; ?>
            <?php endif; ?>
        </div>

        <button class="submit_button" type="submit">Remember Selected</button>
    </form>

    <div class="page_links">
        <a href="/gallery?page=<?= $currentpage-1 > 0 ? $currentpage - 1 : 0 ?>">prev</a>
        <a href="/gallery?page=<?= $currentpage+1 < floor($this->totalimages / $this->imagesperpage) ? $currentpage+1 : floor($this->totalimages / $this->imagesperpage) ?>">next</a>
    </div>
<?php
require "partials/htmlfoot.php"; ?>