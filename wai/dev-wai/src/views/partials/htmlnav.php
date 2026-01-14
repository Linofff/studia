<link href="/static/assets/style.css" rel="stylesheet">
<nav style="display: flex; justify-content: space-between; align-items: center; padding: 10px; border-bottom: 1px solid white">
    <div class="nav_gallery_section">
        <a href="/"><img style="width: 20px; height: 20px" src="/static/assets/hause.svg" alt="Home"></a>
        <a href="/upload">Upload</a>
        <a href="/gallery">Gallery</a>
    </div>

    <div style="display: flex; align-items: center; gap: 15px">
        <?php require "card_icon.php"; ?>
        <?php if (isset($_SESSION['user_id'])): ?>

            <span style="color: green;">
                Logged in as: <strong><?= htmlspecialchars($_SESSION['user_login']) ?></strong>
            </span>

            <?php if (isset($_SESSION['user_img'])): ?>
                <img src="/ProfilesFoto/<?= htmlspecialchars($_SESSION['user_img']) ?>"
                     alt="Profile Picture"
                     style="height: 20px; width: 20px">
            <?php endif; ?>

            <a href="/logout" style="margin-left: 5px;">Logout</a>

        <?php else: ?>
            <a href="/login">Login</a>
            <a href="/register">Register</a>
        <?php endif; ?>
    </div>

</nav>
<br>
