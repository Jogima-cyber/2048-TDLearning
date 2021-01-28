#ifndef DEF_NETWORK
#define DEF_NETWORK

#include <string>
#include <vector>

class Network
{
    public:
        float forward(std::vector<std::vector<int>> state);
        void update(std::vector<std::vector<int>> state, float expected_value, float learning_rate);
        void save();
        Network();
        
    private:
        float weights[17][16][16][16][16];
};

#endif