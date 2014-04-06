$(function() {
  var sound, player, randTime, loop;

  randTime = function() {
    var randTime = Math.round(
      (Math.random() * this.duration) * 100000
    ) / 100000;

    this.currentTime = randTime;
    console.log(randTime, this.currentTime, this.duration);
    //console.log(this, randTime, this.currentTime);
  };

  loop = function() {
    setInterval(randTime.bind(this), 2000);
  };

  $("#player").on("canplay", function() {
    this.play();

    loop.call(this);
  });
});
