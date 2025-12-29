<?php
$title = "Gallery";
require "partials/htmlhead.php";
?>
    <h2>Gallery</h2>

    <form action="/gallery/save" method="POST">
        <div class="photos">
            <?php if (empty($currentimages)): ?>
                <p>No photos to display.</p>
            <?php else: ?>
                <?php foreach ($currentimages as $image): ?>
                    <div style="border: solid 1px white; margin: 10px; padding: 10px;">
                        <img src="<?= "images/" . $image["filename"] ?>" alt="image">
                        <p>Author: <?= $image["author"] ?>, Title: <?= $image["title"] ?></p>

                        <?php if (isset($image['privacy']) && $image['privacy'] === 'private'): ?>
                            <p style="color: red; font-weight: bold;">(Private Photo)</p>
                        <?php endif; ?>

                        <label>
                            <?php
                            $isChecked = isset($_SESSION['selected_images'][$image["filename"]]) ? 'checked' : '';
                            ?>
                            <input type="checkbox" name="checks[]" value="<?= $image['filename'] ?>" <?= $isChecked ?>>
                            Remember this photo
                        </label>
                    </div>
                <?php endforeach; ?>
            <?php endif; ?>
        </div>

        <button type="submit">Remember Selected</button>
    </form>

    <div>
        <?php
        // Safety check for next button logic
        $maxPage = ($this->totalimages > 0) ? floor($this->totalimages / $this->imagesperpage) : 0;
        ?>
        <a href="/gallery?page=<?= $currentpage-1 > 0 ? $currentpage - 1 : 0 ?>">prev</a>
        <a href="/gallery?page=<?= $currentpage+1 < $maxPage ? $currentpage+1 : $maxPage ?>">next</a>
    </div>
<?php
require "partials/htmlfoot.php"; ?>