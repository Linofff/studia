<?php
$title = "Gallery";
require "partials/htmlhead.php";
?>
    <div class="photos">
        <?php foreach ($currentimages as $image): ?>
            <img src="<?= "images/" . $image ?>">
        <?php endforeach; ?>
    </div>
    <div>
        <a href="/gallery?page=<?= $currentpage-1 > 0 ? $currentpage - 1 : 0 ?>">prev</a>
        <a href="/gallery?page=<?= $currentpage+1 < floor($this->totalimages / $this->imagesperpage) ? $currentpage+1 : floor($this->totalimages / $this->imagesperpage) ?>">next</a>
    </div>
<?php
require "partials/htmlfoot.php"; ?>