#include "../jeu.h"




int Jeu::pivoter_herb(int start, int end){
    int i = start + 1;
    int j = end;
    int pivot = sqrt(calcul_distance_to_00(start, 'h'));
    while(i != j){
        if(calcul_distance_to_00(i, 'h') <= pivot){
            i = i + 1;
        }else{
            swap_crea(i, j, 'h');
            j = j - 1;
        }
    }
    if(calcul_distance_to_00(i, 'h') > pivot){
        i = i -1;
    }
    swap_crea(start, i, 'h');
    return i;

}

int Jeu::pivoter_carn(int start, int end){
    int i = start + 1;
    int j = end;
    int pivot = calcul_distance_to_00(start, 'c');
    while(i != j){
        if(calcul_distance_to_00(i, 'c') <= pivot){
            i = i + 1;
        }else{
            swap_crea(i, j, 'c');
            j = j - 1;
        }
    }
    if(calcul_distance_to_00(i, 'c') > pivot){
        i = i -1;
    }
    swap_crea(start, i, 'c');
    return i;

}

void Jeu::quick_sort_herb(int start, int end){
    if(start < end){
        int m = pivoter_herb(start, end);
        quick_sort_herb(start, m - 1);
        quick_sort_herb(m + 1, end);
    }
}

void Jeu::quick_sort_carn(int start, int end){
    if(start < end){
        int m = pivoter_carn(start, end);
        quick_sort_carn(start, m - 1);
        quick_sort_carn(m + 1, end);
    }
}



void Jeu::sort_crea(){
    if(herb_tab.size() == 0) return;
    int stack = -1;
    for(int i = herb_tab.size() - 1; i>=0; i--){
        if(herb_tab[i].repro_count <= herb_tab[i].reproductible || herb_tab[i].fuite){
            if(stack == -1){
                stack = i;
            }
        }else if(stack != -1){
            swap_crea(i, stack, 'h');
            stack = i;
        }
    }
    indice_repro_herb = stack;
    stack = -1;
    if(carn_tab.size() == 0) return;
    for(int i = carn_tab.size() - 1; i>=0; i--){
        if(!carn_tab[i].repro){
            if(stack == -1){
                stack = i;
            }
        }else if(stack != -1){
            swap_crea(i, stack, 'c');
            stack = i;
        }
    }
    indice_repro_carn = stack;
}