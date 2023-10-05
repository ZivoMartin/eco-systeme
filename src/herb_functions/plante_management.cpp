#include "../jeu.h"

void Jeu::delete_the_dead_plante(){
    while(delete_plante_tab.size() != 0){
        delete_element(delete_plante_tab[1], 'p', delete_plante_tab[0]);
        if(zone_tab[delete_plante_tab[0]].plante_tab.size() == 0){
            for(unsigned int i = 2; i<delete_plante_tab.size(); i = i + 2){
                if(delete_plante_tab[i] > delete_plante_tab[0]){
                    delete_plante_tab[i] -= 1;
                }
            }
            synchro_zone(zone_tab[delete_plante_tab[0]].id);
            delete_element(delete_plante_tab[0], 'z', -1);
        }
        delete_plante_tab.erase(delete_plante_tab.begin());
        delete_plante_tab.erase(delete_plante_tab.begin());
    }
}

void Jeu::synchro_zone(int id){
    unsigned int size = herb_tab.size();
    for(unsigned int i = 0; i<size; i++){
        if(herb_tab[i].id_zone == id){
            herb_tab[i].eating = false;
        }
    }
}

bool Jeu::recal_id(int indice){
    int size_zone = zone_tab[herb_tab[indice].zone].plante_tab.size();
    if(herb_tab[indice].buisson >= size_zone) herb_tab[indice].buisson = zone_tab[herb_tab[indice].zone].plante_tab.size() - 1;
    while(herb_tab[indice].buisson >= 0 && zone_tab[herb_tab[indice].zone].plante_tab[herb_tab[indice].buisson].id != herb_tab[indice].id_plante){
        herb_tab[indice].buisson -= 1;
    }
    if(herb_tab[indice].buisson == -1){
        herb_tab[indice].eating = false;
        return false;
    }
    return true;
}

int Jeu::sort_plante_tab(int *t, int x, int y, int indice_min){
    int d0 = calcul_distance(x, zone_tab[indice_min].plante_tab[0].x, y, zone_tab[indice_min].plante_tab[0].y);
    int d1 = calcul_distance(x, zone_tab[indice_min].plante_tab[1].x, y, zone_tab[indice_min].plante_tab[1].y);
    int d2 = calcul_distance(x, zone_tab[indice_min].plante_tab[2].x, y, zone_tab[indice_min].plante_tab[2].y);
    if(d0 < d1 && d0 < d2){
        *t = 0;
        if(d1 > d2){
            *(t+1) = 2;
            *(t+2) = 1;
            return d1;
        }else{
            *(t+1) = 1;
            *(t+2) = 2;
            return d2;
        }
    }else if(d1 < d0 && d1 < d2){
        *t = 1;
        if(d0 > d2){
            *(t+1) = 2;
            *(t+2) = 0;
            return d0;
        }else{
            *(t+1) = 0;
            *(t+2) = 2;
            return d2;
        }
    }else{
        *t = 2;
        if(d0 > d1){
            *(t+1) = 1;
            *(t+2) = 0;
            return d0;
        }else{
            *(t+1) = 0;
            *(t+2) = 1;
            return d1;
        }
    }
}
