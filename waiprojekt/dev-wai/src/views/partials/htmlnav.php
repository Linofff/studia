<nav>
    <div class="nav_gallery_section">
        <a href="/">Upload</a>
        <a href="/gallery">Gallery</a>
    </div>

    <div class="nav_user_section">
        <a href="/saved"><img src="/static/assets/shopping_card.svg"></a>
        <?php echo isset($_SESSION['selected_images']) ? array_sum($_SESSION['selected_images']) : 0 ?>
        <?php if (isset($_SESSION['user_id'])): ?>

            <span style="color: green;">
                Logged in as: <strong><?= htmlspecialchars($_SESSION['user_login']) ?></strong>
            </span>

            <?php if (isset($_SESSION['user_img'])): ?>
                <img src="/ProfilesFoto/<?= htmlspecialchars($_SESSION['user_img']) ?>"
                     alt="Profile Picture"
                     class="profile_picture">
            <?php endif; ?>

            <a href="/logout" style="margin-left: 5px;">Logout</a>

        <?php else: ?>
            <a href="/login">Login</a>
            <a href="/register">Register</a>
        <?php endif; ?>
    </div>

</nav>