function Agent(weights) {
  this.weights = weights;
}

Agent.prototype.select_move = function (board) {
  board = [...board];
  
  return Math.floor(Math.random()*4);
};

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

Agent.prototype.preprocess(state){
    state = [...state]
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){
            state[i][j] = state[i][j] ? Math.log2(state[i][j]) : 0;
        }
    }
    
    return state;
}

Agent.prototype.left_rotation(state){
    var new_state = [ [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0], [0, 0, 0, 0] ];
    
    for(var i=0;i<4;i++){
        for(var j=0;j<4;j++){
            new_state[j][3-i] = state[i][j];
        }
    }
    return new_state;
}

Agent.prototype.rot90(state, k){
    var k = k % 4;
    for(var i=0;i<k;i++){
        state = left_rotation(state);
    }
    
    return state;
}

Agent.prototype._slide_left_and_merge(state){
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

Agent.prototype._move_left_possible(state){
    //Slide tiles on a grid to the left and merge.    
    std::vector< std::vector<int> >::const_iterator row; 
    std::vector<int>::const_iterator col; 
    for (row = state.begin(); row != state.end(); ++row){
        std::vector<int> new_row = {0, 0, 0, 0};
        int counter__ = 0;
        int j = 0;
        for (col = row->begin(); col != row->end(); ++col){ 
            if(*col != 0){
                new_row[counter__] = *col;
                counter__++;
            }
            
            if(new_row[j] != *col){
                return true;
            }
            
            j++;
        }
        
        j = 1;
        for(;j<counter__;j++){
            int prev_el = new_row[j-1];
            int el = new_row[j];
            if(prev_el == el){
                return true;
                j++;
            }
        }
    }
    
    return false;
}

Agent.prototype.move(state, action){
    //Rotate board aligned with left action   
    //Align board action with left action
    state = rot90(state, action);
    
    int reward;
    std::tie(reward, state) = _slide_left_and_merge(state);
        
    state = rot90(state, 4-action);
    
    return std::make_tuple(state, reward);
}

Agent.prototype.possible_moves(state){
    std::vector<int> avalaible_actions;
    
    for(int i=0;i<4;i++){
        if(_move_left_possible(rot90(state,i))){
            avalaible_actions.push_back(i);
        }
    }
        
    return avalaible_actions;
}
