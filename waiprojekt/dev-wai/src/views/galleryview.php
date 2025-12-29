<?php
$title = "Gallery";
require "partials/htmlhead.php";
?>
    <h2>Gallery</h2>

    <form action="/gallery/save" method="POST">
        <div class="photos">
            <?php foreach ($currentimages as $image): ?>
                <div class="photo-card" style="border: 1px solid #ccc; margin: 10px; padding: 10px;">
                    <img src="<?= "images/" . $image["filename"] ?>" alt="image">
                    <p>Author: <?= $image["author"] ?>, Title: <?= $image["title"] ?></p>

                    <label>
                        <?php
                        $isChecked = isset($_SESSION['selected_images'][$image["filename"]]) ? 'checked' : '';
                        ?>
                        <input type="checkbox" name="checks[]" value="<?= $image['filename'] ?>" <?= $isChecked ?>>
                        Remember this photo
                    </label>
                </div>
            <?php endforeach; ?>
        </div>

        <button type="submit">Zapamiętaj wybrane</button>
    </form>

    <div>
        <a href="/gallery?page=<?= $currentpage-1 > 0 ? $currentpage - 1 : 0 ?>">prev</a>
        <a href="/gallery?page=<?= $currentpage+1 < floor($this->totalimages / $this->imagesperpage) ? $currentpage+1 : floor($this->totalimages / $this->imagesperpage) ?>">next</a>
    </div>
<?php
require "partials/htmlfoot.php"; ?>