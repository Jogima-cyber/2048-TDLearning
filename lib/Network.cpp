#include "Network.h"
#include <string>
#include <math.h>
#include <vector>

#include <torch/script.h>

#include <iostream>
#include <memory>

using namespace std;

Network::Network()
{
    for(int n=0;n<17;n++){
        for(int i=0;i<16;i++){
            for(int j=0;j<16;j++){
                for(int k=0;k<16;k++){
                    for(int l=0;l<16;l++){
                        weights[n][i][j][k][l] = 0;
                    }
                }
            }
        }
    }
}

float Network::forward(std::vector<std::vector<int>> state){    
    float batched_four_tuple_columns = 
        weights[0][state[0][0]][state[0][1]][state[0][2]][state[0][3]] +
        weights[1][state[1][0]][state[1][1]][state[1][2]][state[1][3]] +
        weights[2][state[2][0]][state[2][1]][state[2][2]][state[2][3]] +
        weights[3][state[3][0]][state[3][1]][state[3][2]][state[3][3]];
    
    float batched_four_tuple_rows =
        weights[4][state[0][0]][state[1][0]][state[2][0]][state[3][0]] +
        weights[5][state[0][1]][state[1][1]][state[2][1]][state[3][1]] +
        weights[6][state[0][2]][state[1][2]][state[2][2]][state[3][2]] +
        weights[7][state[0][3]][state[1][3]][state[2][3]][state[3][3]];
    
    float batched_four_tuple_receipts =
        weights[8][state[0][0]][state[0][1]][state[1][1]][state[1][0]] +
        weights[9][state[0][1]][state[0][2]][state[1][2]][state[1][1]] +
        weights[10][state[0][2]][state[0][3]][state[1][3]][state[1][2]] +
        weights[11][state[1][0]][state[1][1]][state[2][1]][state[2][0]] +
        weights[12][state[1][1]][state[1][2]][state[2][2]][state[2][1]] +
        weights[13][state[1][2]][state[1][3]][state[2][3]][state[2][2]] +
        weights[14][state[2][0]][state[2][1]][state[3][1]][state[3][0]] +
        weights[15][state[2][1]][state[2][2]][state[3][2]][state[3][1]] +
        weights[16][state[2][2]][state[2][3]][state[3][3]][state[3][2]];
    
    return batched_four_tuple_columns + batched_four_tuple_rows + batched_four_tuple_receipts;
}

void Network::update(std::vector<std::vector<int>> state, float expected_value, float learning_rate){
    float val = forward(state);
    float error = expected_value - val;
    float delta = error * learning_rate;
        
    weights[0][state[0][0]][state[0][1]][state[0][2]][state[0][3]] += delta;
    weights[1][state[1][0]][state[1][1]][state[1][2]][state[1][3]] += delta;
    weights[2][state[2][0]][state[2][1]][state[2][2]][state[2][3]] += delta;
    weights[3][state[3][0]][state[3][1]][state[3][2]][state[3][3]] += delta;
    
    weights[4][state[0][0]][state[1][0]][state[2][0]][state[3][0]] += delta;
    weights[5][state[0][1]][state[1][1]][state[2][1]][state[3][1]] += delta;
    weights[6][state[0][2]][state[1][2]][state[2][2]][state[3][2]] += delta;
    weights[7][state[0][3]][state[1][3]][state[2][3]][state[3][3]] += delta;
    
    weights[8][state[0][0]][state[0][1]][state[1][1]][state[1][0]] += delta;
    weights[9][state[0][1]][state[0][2]][state[1][2]][state[1][1]] += delta;
    weights[10][state[0][2]][state[0][3]][state[1][3]][state[1][2]] += delta;
    weights[11][state[1][0]][state[1][1]][state[2][1]][state[2][0]] += delta;
    weights[12][state[1][1]][state[1][2]][state[2][2]][state[2][1]] += delta;
    weights[13][state[1][2]][state[1][3]][state[2][3]][state[2][2]] += delta;
    weights[14][state[2][0]][state[2][1]][state[3][1]][state[3][0]] += delta;
    weights[15][state[2][1]][state[2][2]][state[3][2]][state[3][1]] += delta;
    weights[16][state[2][2]][state[2][3]][state[3][3]][state[3][2]] += delta;
}

void Network::save(){
    auto x = torch::from_blob(weights, {17,16,16,16,16});
    auto bytes = torch::jit::pickle_save(x);
    std::ofstream fout("TDL2048_weights.zip", std::ios::out | std::ios::binary);
    fout.write(bytes.data(), bytes.size());
    fout.close();
}