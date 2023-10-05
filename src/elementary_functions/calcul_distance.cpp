#include "../jeu.h"


int Jeu::calcul_distance(int x1, int x2, int y1, int y2){
    return(pow(x1-x2, 2) + pow(y1-y2, 2));
}

int Jeu::calculate_distance(int i_herb, int i_carn, char type){
    int xc = carn_tab[i_carn].x;
    int yc = carn_tab[i_carn].y;
    int xh = herb_tab[i_herb].x;
    int yh = herb_tab[i_herb].y;
    int d = calcul_distance(xh, xc, yh, yc);
    int range;
    if(type == 'h'){
        range = herb_tab[i_herb].range;
    }else{
        range = carn_tab[i_carn].range;
    }
    if(xh<range && map_width-xc < range){
        int d2 = calcul_distance(xh + map_width, xc, yh, yc);
        if(d2<d){
            d = d2;
        }
    }else if(xc<range && map_width-xh < range){
        int d2 = calcul_distance(xh - map_width, xc, yh, yc);
        if(d2<d){
            d = d2;
        }
    }
    if(yh<range && map_height-yc < range){
        int d2 = calcul_distance(xh, xc, yh + map_height, yc);
        if(d2<d){
            d = d2;
        }
    }else if(yc<range && map_width-yh < range){
        int d2 = calcul_distance(xh, xc, yh - map_height, yc);
        if(d2<d){
            d = d2;
        }
    }
    return d;
}

int Jeu::calcul_distance_to_00(int i, char type){
    if(type == 'h'){
        return pow(herb_tab[i].x, 2) + pow(herb_tab[i].y, 2);
    }else{
        return pow(carn_tab[i].x, 2) + pow(carn_tab[i].y, 2);
    }
}