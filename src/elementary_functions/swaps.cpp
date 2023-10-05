#include "../jeu.h"

void Jeu::swap_crea(int i, int j, char type){
    if(type == 'h'){
        herbivore_t temp = herb_tab[i];
        herb_tab[i] = herb_tab[j];
        herb_tab[j] = temp;
    }else{
        carnivore_t temp = carn_tab[i];
        carn_tab[i] = carn_tab[j];
        carn_tab[j] = temp;
    }
}

void Jeu::swap_double(double *nb1, double *nb2){
    double tmp = *nb1;
    *nb1 = *nb2;
    *nb2 = tmp;
}
void Jeu::swap_int(int *nb1, int *nb2){
    int tmp = *nb1;
    *nb1 = *nb2;
    *nb2 = tmp;
}