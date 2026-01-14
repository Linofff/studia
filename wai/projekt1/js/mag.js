$(".magnified").hover(function(e) {
  var imgPosition = $(".magnify").position(),
    imgHeight = $(".magnified").height(),
    imgWidth = $(".magnified").width();

  $(".magnifier").show();

  $(this).mousemove(function(e) {
    var posX = e.pageX - imgPosition.left,
      posY = e.pageY - imgPosition.top,
      percX = (posX / imgWidth) * 100,
      percY = (posY / imgHeight) * 100,
      perc = percX + "% " + percY + "%";

    $(".magnifier").css({
      top: posY,
      left: posX,
      backgroundPosition: perc
    });
  });
}, function() {
  $(".magnifier").hide();
});

