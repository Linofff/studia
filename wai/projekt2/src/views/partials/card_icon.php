<div style="display: flex; align-items: center; gap: 5px; border: 1px solid white; padding: 4px 4px 2px 2px">
    <a href="/saved"><img style="width: 20px; height: 20px" src="/static/assets/shopping_card.svg"></a>
    <span><?php echo isset($_SESSION['selected_images']) ? array_sum($_SESSION['selected_images']) : 0 ?></span>
</div>