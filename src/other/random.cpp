#include "../jeu.h"

int Jeu::random(int min, int max){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> random_nbr(min, max);
    return(random_nbr(generator));
}
