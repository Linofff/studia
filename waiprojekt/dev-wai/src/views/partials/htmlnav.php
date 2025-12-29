<nav style="display: flex; justify-content: space-between; align-items: center; padding: 10px; border-bottom: 1px solid #ccc;">

    <div style="display: flex; gap: 15px;">
        <a href="/">Upload</a>
        <a href="/gallery">Gallery</a>
    </div>

    <div style="display: flex; align-items: center; gap: 15px;">
        <a href="/saved"><img src="/static/assets/shopping_card.svg"></a>
        <?php echo isset($_SESSION['selected_images']) ? array_sum($_SESSION['selected_images']) : 0 ?>
        <?php if (isset($_SESSION['user_id'])): ?>

            <span style="color: green;">
                Logged in as: <strong><?= htmlspecialchars($_SESSION['user_login']) ?></strong>
            </span>

            <?php if (isset($_SESSION['user_img'])): ?>
                <img src="/ProfilesFoto/<?= htmlspecialchars($_SESSION['user_img']) ?>"
                     alt="ProfilePicture"
                     style="width: 20px; height: 20px; border: solid 1px">
            <?php endif; ?>

            <a href="/logout" style="margin-left: 5px;">Logout</a>

        <?php else: ?>
            <a href="/login">Login</a>
            <a href="/register">Register</a>
        <?php endif; ?>
    </div>

</nav>