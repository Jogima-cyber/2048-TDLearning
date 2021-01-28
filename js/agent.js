function Agent(weights) {
  this.weights = weights;
}

Agent.prototype.select_move = function (board) {
  board = [...board];
  console.log(board)
  var action_selected = this.chooseBestTransitionAfterstate(board)
  console.log(action_selected)
  return action_selected;
};

Agent.prototype.chooseBestTransitionAfterstate = function(state){
    var possible_actions = this.possible_moves(state);
    
    var best_action;
    var best_value = Number.NEGATIVE_INFINITY;
    
    for(var i = 0;i<possible_actions.length;i++){
        var action = possible_actions[i];
        
        var [after_state, reward] = this.move(state, action);
        
        var value = reward + this.vfunction(this.preprocess(after_state));
        
        if(value > best_value){
            best_value = value;
            best_action = action;
        }
    }
    
    return best_action;
}


Agent.prototype.vfunction = function(state){
  var batched_four_tuple_columns = 
      this.weights[0][state[0][0]][state[0][1]][state[0][2]][state[0][3]] +
      this.weights[1][state[1][0]][state[1][1]][state[1][2]][state[1][3]] +
      this.weights[2][state[2][0]][state[2][1]][state[2][2]][state[2][3]] +
      this.weights[3][state[3][0]][state[3][1]][state[3][2]][state[3][3]];
    
  var batched_four_tuple_rows =
      this.weights[4][state[0][0]][state[1][0]][state[2][0]][state[3][0]] +
      this.weights[5][state[0][1]][state[1][1]][state[2][1]][state[3][1]] +
      this.weights[6][state[0][2]][state[1][2]][state[2][2]][state[3][2]] +
      this.weights[7][state[0][3]][state[1][3]][state[2][3]][state[3][3]];
    
  var batched_four_tuple_receipts =
      this.weights[8][state[0][0]][state[0][1]][state[1][1]][state[1][0]] +
      this.weights[9][state[0][1]][state[0][2]][state[1][2]][state[1][1]] +
      this.weights[10][state[0][2]][state[0][3]][state[1][3]][state[1][2]] +
      this.weights[11][state[1][0]][state[1][1]][state[2][1]][state[2][0]] +
      this.weights[12][state[1][1]][state[1][2]][state[2][2]][state[2][1]] +
      this.weights[13][state[1][2]][state[1][3]][state[2][3]][state[2][2]] +
      this.weights[14][state[2][0]][state[2][1]][state[3][1]][state[3][0]] +
      this.weights[15][state[2][1]][state[2][2]][state[3][2]][state[3][1]] +
      this.weights[16][state[2][2]][state[2][3]][state[3][3]][state[3][2]];
    
    return batched_four_tuple_columns + batched_four_tuple_rows + batched_four_tuple_receipts;
}

Agent.prototype.all = function(state){
  for (var i=0;i<4;i++){ 
    for (var j=0;j<4;j++){ 
      if(state[i][j] == 0){
        return false;
      }
    }
  }
  return true;
}

Agent.prototype.preprocess = function(state){
    state = [...state]
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){
            state[i][j] = state[i][j] ? Math.log2(state[i][j]) : 0;
        }
    }
    
    return state;
}

Agent.prototype.left_rotation = function(state){
    var new_state = [ [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0] ];
    
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){
            new_state[j][3-i] = state[i][j];
        }
    }
    return new_state;
}

Agent.prototype.rot90 = function(state, k){
    var k = k % 4;
    for(var i=0;i<k;i++){
        state = this.left_rotation(state);
    }
    
    return state;
}

Agent.prototype._slide_left_and_merge = function(state){
    //Slide tiles on a grid to the left and merge.
    var score = 0;
    
    var result = [ [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0] ];
    
    for (var i=0;i<4;i++){
        var new_row = [0, 0, 0, 0];
        var counter__ = 0;
        for (var j=0;j<4;j++){ 
            if(state[i][j] != 0){
                new_row[counter__] = state[i][j];
                counter__ += 1;
            }
        }
        
        var counter_ = 0;
        var counter = 0;
        var j = 1;
        for(;j<counter__;j++){
            var prev_el = new_row[j-1];
            var el = new_row[j];
            if(prev_el == el){
                score += prev_el + el;
                result[i][counter] = prev_el + el;
                j += 1;
                counter_ += 1;
            } else {
                result[i][counter] = prev_el;
                counter_ += 1;
            }
            counter += 1;
        }
        
        if(j == counter__){
            result[i][counter_] = new_row[j-1];
        }
    }
    
    return [score, result];
}

Agent.prototype._move_left_possible = function(state){
    //Slide tiles on a grid to the left and merge.    
    for (var i=0;i<4;i++){
        var new_row = [0, 0, 0, 0];
        var counter__ = 0;
        for (var j=0;j<4;j++){
            if(state[i][j] != 0){
                new_row[counter__] = state[i][j];
                counter__ += 1;
            }
            
            if(new_row[j] != state[i][j]){
                return true;
            }
        }
        
        j = 1;
        for(;j<counter__;j++){
            var prev_el = new_row[j-1];
            var el = new_row[j];
            if(prev_el == el){
                return true;
            }
        }
    }
    
    return false;
}

Agent.prototype.move = function(state, action){
    //Rotate board aligned with left action   
    //Align board action with left action
    state = this.rot90(state, action);
    
    var [reward, state] = this._slide_left_and_merge(state);
        
    state = this.rot90(state, 4-action);
    
    return [state, reward];
}

Agent.prototype.possible_moves = function(state){
    var avalaible_actions = [];
    
    for(var i=0;i<4;i++){
        if(this._move_left_possible(this.rot90(state,i))){
            avalaible_actions.push(i);
        }
    }
        
    return avalaible_actions;
}
