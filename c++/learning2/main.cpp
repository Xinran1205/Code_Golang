#include <iostream>
#include <vector>

std::vector<float> interpolateSingleFloats(float from,float to, int numberOfValues){
    int gap = numberOfValues - 1;
    float step = (to - from) / gap;
    std::vector<float> result;
    for (int i = 0; i < numberOfValues; i++){
        result.push_back(from + (step * i));
    }
    return result;
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    std::vector<float> result;
    result = interpolateSingleFloats(2.2, 8.5, 7);
    for(size_t i=0; i<result.size(); i++) std::cout << result[i] << " ";
    std::cout << std::endl;
    return 0;
}
