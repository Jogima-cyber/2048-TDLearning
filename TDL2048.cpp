#include <iostream>
#include <tuple>
#include <chrono>
#include <random>
#include <vector>
#include <math.h>

#include "lib/Network.h"

using namespace std::chrono; 

bool all(std::vector<std::vector<int>> state){
    std::vector< std::vector<int> >::const_iterator row; 
    std::vector<int>::const_iterator col; 

    for (row = state.begin(); row != state.end(); ++row){ 
        for (col = row->begin(); col != row->end(); ++col){ 
            if(*col == 0){
                return false;
            }
        }
    }
    return true;
}

std::vector<std::vector<int>> preprocess(std::vector<std::vector<int>> state){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            state[i][j] = state[i][j] ? int(log2(state[i][j])) : 0;
        }
    }
    
    return state;
}

std::vector<std::vector<int>> _place_random_tiles(std::vector<std::vector<int>> state, std::mt19937_64& rng, int count=1){
    if(!all(state)){
        int choices[2] = {2, 4};
        float probs[2] = {0.9, 0.1};

        std::vector<int> tiles(count);

        // initialize a uniform distribution between 0 and 1
        std::uniform_real_distribution<double> unif(0, 1);

        for(int i=0;i<count;i++){
            double r = unif(rng);

            if(r < probs[0]){
                tiles[i] = choices[0];
            } else {
                tiles[i] = choices[1];
            }
        }
        
        std::vector<int> empty_locations;
        std::vector<int> probs_lc;

        std::vector< std::vector<int> >::const_iterator row; 
        std::vector<int>::const_iterator col; 

        int i = 0;
        for (row = state.begin(); row != state.end(); ++row){
            int j = 0;
            for (col = row->begin(); col != row->end(); ++col){ 
                if(*col == 0){
                    empty_locations.push_back(4*i+j);
                    probs_lc.push_back(1);
                }
                j++;
            }
            i++;
        }

        //Pick uniformly as many as count
        std::discrete_distribution<> dist(probs_lc.begin(), probs_lc.end());
        for(int i=0;i<count;i++){
            int el = empty_locations[dist(rng)];
            int h = el % 4;
            int w = (el - h) / 4;
            state[w][h] = tiles[i];
        }
    }
    
    return state;
}

std::vector<std::vector<int>> initial_state(std::mt19937_64& rng){
    std::vector<std::vector<int>> state = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    
    return _place_random_tiles(state, rng, 2);
}

void show_state(std::vector<std::vector<int>> state){
    std::vector< std::vector<int> >::const_iterator row; 
    std::vector<int>::const_iterator col; 
    std::cout << "[\n";
    for (row = state.begin(); row != state.end(); ++row){ 
        for (col = row->begin(); col != row->end(); ++col){ 
            std::cout << *col << ','; 
        }
        std::cout << '\n'; 
    }
    std::cout << "]";
}

std::vector<std::vector<int>> left_rotation(std::vector<std::vector<int>> state){
    std::vector<std::vector<int>> new_state = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    std::vector< std::vector<int> >::const_iterator row; 
    std::vector<int>::const_iterator col; 
    
    int i = 0;
    for (row = state.begin(); row != state.end(); ++row){
        int j = 0;
        for (col = row->begin(); col != row->end(); ++col){ 
            new_state[j][3-i] = *col;
            j++;
        }
        i++;
    }
    return new_state;
}

std::vector<std::vector<int>> rot90(std::vector<std::vector<int>> state, int k){
    k = k % 4;
    for(int i=0;i<k;i++){
        state = left_rotation(state);
    }
    
    return state;
}

std::tuple<float, std::vector<std::vector<int>>> _slide_left_and_merge(std::vector<std::vector<int>> state){
    //Slide tiles on a grid to the left and merge.
    int score = 0;
    
    std::vector<std::vector<int>> result = { {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0} };
    int i = 0;
    
    std::vector< std::vector<int> >::const_iterator row; 
    std::vector<int>::const_iterator col; 
    for (row = state.begin(); row != state.end(); ++row){
        std::vector<int> new_row = {0, 0, 0, 0};
        int counter__ = 0;
        for (col = row->begin(); col != row->end(); ++col){ 
            if(*col != 0){
                new_row[counter__] = *col;
                counter__++;
            }
        }
        
        int counter_ = 0;
        int counter = 0;
        int j = 1;
        for(;j<counter__;j++){
            int prev_el = new_row[j-1];
            int el = new_row[j];
            if(prev_el == el){
                score += prev_el + el;
                result[i][counter] = prev_el + el;
                j++;
                counter_++;
            } else {
                result[i][counter] = prev_el;
                counter_++;
            }
            counter++;
        }
        
        if(j == counter__){
            result[i][counter_] = new_row[j-1];
        }
        
        i++;
    }
    
    return std::make_tuple(score, result);
}

bool _move_left_possible(std::vector<std::vector<int>> state){
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

std::tuple<std::vector<std::vector<int>>, float> move(std::vector<std::vector<int>> state, int action){
    //Rotate board aligned with left action   
    //Align board action with left action
    state = rot90(state, action);
    
    int reward;
    std::tie(reward, state) = _slide_left_and_merge(state);
        
    state = rot90(state, 4-action);
    
    return std::make_tuple(state, reward);
}

std::vector<int> possible_moves(std::vector<std::vector<int>> state){
    std::vector<int> avalaible_actions;
    
    for(int i=0;i<4;i++){
        if(_move_left_possible(rot90(state,i))){
            avalaible_actions.push_back(i);
        }
    }
        
    return avalaible_actions;
}

bool is_done(std::vector<std::vector<int>> state){
    if(!all(state)){
        return false;
    }
    
    for(int i=0;i<4;i++){
        if(_move_left_possible(rot90(state,i))){
            return false;
        }
    }
    
    return true;
}

void show_avalaible_actions(std::vector<int> avalaible_actions){
    std::cout << " -- Avalaible actions : ";
    for(int i=0;i<avalaible_actions.size();i++){
        std::cout << avalaible_actions[i] << ", ";
    }
}

std::tuple<std::vector<std::vector<int>>, float> chooseBestTransitionAfterstate(Network& network, std::vector<std::vector<int>> state){
    std::vector<int> possible_actions = possible_moves(state);
    
    std::vector<std::vector<int>> best_transition;
    float reward_best = 0;
    float best_value = -std::numeric_limits<float>::infinity();
    
    for(int i = 0;i<possible_actions.size();i++){
        int action = possible_actions[i];
        
        int reward;
        std::vector<std::vector<int>> after_state;
        std::tie(after_state, reward) = move(state, action);
        
        float value = reward + network.forward(preprocess(after_state));
        
        if(value > best_value){
            best_transition = after_state;
            best_value = value;
            reward_best = reward;
        }
    }
    
    if(best_transition.size() == 0){
        std::cout << "Error -- best value : " << best_value;
        show_avalaible_actions(possible_actions);
        std::cout << ", net forward of state : " << network.forward(preprocess(state)) << "\n";
    }
    
    return std::make_tuple(best_transition, reward_best);
}

float getBestValueAction(Network& network, std::vector<std::vector<int>> state){
    std::vector<int> possible_actions = possible_moves(state);
    
    float best_value = -std::numeric_limits<float>::infinity();
    
    for(int i = 0;i<possible_actions.size();i++){
        int action = possible_actions[i];
        
        int reward;
        std::vector<std::vector<int>> after_state;
        std::tie(after_state, reward) = move(state, action);
        
        float value = reward + network.forward(preprocess(after_state));
        
        if(value > best_value){
            best_value = value;
        }
    }
    
    return best_value;
}

void play_game(Network& network, float epsilon, float learning_rate, std::mt19937_64& rng){
    std::vector<std::vector<int>> state = initial_state(rng);
    
    //show_state(state);
    
    bool done = false;
    
    while(not done){
        std::uniform_real_distribution<double> explo_d(0, 1);
        float rd = explo_d(rng);
        
        int reward;
        std::vector<std::vector<int>> after_state;
        
        if(rd < epsilon){
            std::vector<int> possible_actions = possible_moves(state);
            std::uniform_real_distribution<double> unif(0, possible_actions.size());
            int rd = floor(unif(rng));
            int action_selected = possible_actions[rd];
            
            std::tie(after_state, reward) = move(state, action_selected);
        } else {
            std::tie(after_state, reward) = chooseBestTransitionAfterstate(network, state);
        }
        state = _place_random_tiles(after_state, rng);
        float correctActionValue = 0;
        
        //show_state(state);
        
        done = is_done(state);
        if(!done){
            correctActionValue += getBestValueAction(network, state);
        }
        
        network.update(preprocess(after_state), correctActionValue, learning_rate);
    }
}

int matrix_max(std::vector<std::vector<int>> state){
    int m = 0;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            m = std::max(m, state[i][j]);
        }
    }
    
    return m;
}

std::tuple<float, int> playByAfterstates(Network& network, std::mt19937_64& rng){
    float sumRewards = 0;
    std::vector<std::vector<int>> state = initial_state(rng);
    bool done = false;
    
    while(not done){
        int reward;
        std::vector<std::vector<int>> after_state;
        
        std::tie(after_state, reward) = chooseBestTransitionAfterstate(network, state);
        sumRewards += reward;
        state = _place_random_tiles(after_state, rng);
        
        done = is_done(state);
    }
    
    return std::make_tuple(sumRewards, matrix_max(state));
}

void evaluatePerformance(int numEpisodes, int e, Network& network, std::mt19937_64& rng){
    float performance = 0;
    float ratio = 0;
    int maxTile = 0;
    
    for(int i=0;i<numEpisodes;i++){
        int sumRewards;
        int maxTile_;
        std::tie(sumRewards, maxTile_) = playByAfterstates(network, rng);

        performance += sumRewards;
        ratio += (maxTile_ >= 2048) ? 1 : 0;
        maxTile = std::max(maxTile, maxTile_);
    }
    
    std::cout << "After " << e << " games: avg score = " << (performance / numEpisodes) << ", avg ratio = " << (ratio / numEpisodes) << ", maxTile = " << maxTile << "\n";
}

int main() {
    /* initialize random seed: */
    std::mt19937_64 rng;
    rng.seed(123);
    
    Network net;
        
    auto start = high_resolution_clock::now();
    
    for(int i=0;i<100000;i++){
        play_game(net, 0.001, 0.01, rng);
        
        if((i+1) % 1000 == 0){
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start); 
            std::cout << (i+1) << " iterations in " << duration.count()/(i+1) << " microseconds per iterations -- forecasted time for 100000 it. : "<< duration.count()/(10*(i+1)) << " seconds\n";
        }
        if((i+1) % 5000 == 0){
            evaluatePerformance(100, i+1, net, rng);
        }
    }
    
    net.save();
}