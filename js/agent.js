function Agent(weights) {
  this.weights = weights;
}

Agent.prototype.select_move = function (board) {
  board = [...board];
  
  return Math.floor(Math.random()*4);
};
