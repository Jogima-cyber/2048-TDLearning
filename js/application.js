// Wait till the browser is ready to render the game (avoids glitches)
window.requestAnimationFrame(function () {
  $.ajax({
    url: './weights/TDL2048_weights.json',
    async: false,
    dataType: 'json',
    success: function (weights) {
      // do stuff with response.
      new GameManager(4, KeyboardInputManager, HTMLActuator, LocalStorageManager, Agent, weights);
    }
  });
});
