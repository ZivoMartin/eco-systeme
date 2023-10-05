#include "../jeu.h"

void Jeu::prey_spotting(int indice){
    if(herb_tab.size() == 0){
        random_move_carn(indice, carn_tab[indice].vitesse, 200);
        return;
    }
    int min = calculate_distance(0, indice, 'c');
    int indice_min = 0;
    int size = herb_tab.size();
    for(int i = 1; i<size; i++){
        if(herb_tab[i].height - 20 < carn_tab[indice].height || herb_tab[i].corpse_state){
            int d = calculate_distance(i, indice, 'c');
            if(d < pow(herb_tab[i].vitesse, 2)){
                if(carn_tab[indice].etat == 't'){
                    carn_tab[indice].etat = 'c';
                    change_the_color('r', indice, 'c');
                }
                apply_carn_move(indice, i);
                return;
            }
            if(d < min){
                min = d;
                indice_min = i;
            }
        }
        size = herb_tab.size();
    }
    if(min < pow(carn_tab[indice].range, 2)){
        if(carn_tab[indice].etat == 't'){
            carn_tab[indice].etat = 'c';
            change_the_color('r', indice, 'c');
        }
        apply_carn_move(indice, indice_min);
        return;
    }
    if(carn_tab[indice].etat == 'c'){
        carn_tab[indice].etat = 't';
        change_the_color('o', indice, 'c');
    }
    random_move_carn(indice, carn_tab[indice].vitesse, 200);
}

bool Jeu::detect_carnivore_partner(unsigned int indice, unsigned int *size){
    if(indice_repro_carn == *size){
        return false;
    }
    int min = calcul_distance(carn_tab[indice].x, carn_tab[indice_repro_carn+1].x, carn_tab[indice].y, carn_tab[indice_repro_carn+1].y);
    int indice_min = indice_repro_carn + 1;
    if(indice == indice_repro_carn+1){
        min = calcul_distance(carn_tab[indice].x, carn_tab[indice_repro_carn+2].x, carn_tab[indice].y, carn_tab[indice_repro_carn+2].y);
        indice_min = indice_repro_carn + 2;
    }
    for(unsigned int i = indice_min + 1; i<*size; i++){
        if(indice != i && abs(carn_tab[indice].height - carn_tab[i].height) < 15){
            int d = calcul_distance(carn_tab[indice].x, carn_tab[i].x, carn_tab[indice].y, carn_tab[i].y);
            if(d < min){
                min = d;
                indice_min = i;
            }
        }
    }
    if(min < pow(carn_tab[indice].range, 2)){
        if( marcher_vers_partenaire_carnivore(indice, indice_min)){
            *size += 1;
        }
        return true;
    }
    return false;
}


int Jeu::find_index_predator(int x, int y){
    int size = carn_tab.size();
    for(int i = 0; i<size; i++){
        if(carn_tab[i].x == x && carn_tab[i].y == y){
            return i;
        }
    }
    return -1;
}


void Jeu::accoupler_carnivore(int indice_carn, int indice_part){
    carn_tab[indice_carn].repro = false;
    carn_tab[indice_part].repro = false;
    add_an_element(indice_carn, indice_part, 'c');
}


void Jeu::eat_herb(int indice_carn, int indice_herb){
    delete_element(indice_herb, 'h', -1);
    carn_tab[indice_carn].faim = 0;
    herb_count -= 1;
    carn_tab[indice_carn].etat = 'r';
    change_the_color('y', indice_carn, 'c');
    carn_tab[indice_carn].repro = true;
}