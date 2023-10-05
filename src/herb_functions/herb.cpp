#include "../jeu.h"

bool Jeu::predator_spotting(int indice){
    if(carn_tab.size() == 0) return false;
    int min = calculate_distance(indice, 0, 'h');
    int indice_min = 0;
    int speed = pow(herb_tab[indice].vitesse, 2);
    int size = carn_tab.size();
    for(int i = 1; i<size; i++){
        if(carn_tab[i].height > herb_tab[indice].height - 30){
            int d = calculate_distance(indice, i, 'h');
            if(d < 2*speed){
                apply_move_herb(indice, carn_tab[i].x, carn_tab[i].y, herb_tab[indice].vitesse);
                herb_tab[indice].count_fuite = 100;
                herb_tab[indice].previous_predator_x = carn_tab[i].x;
                herb_tab[indice].previous_predator_y = carn_tab[i].y;
                return true;
            }
            if(d < min && carn_tab[i].etat != 'r'){
                min = d;
                indice_min = i;
            }
        }

    }
    if(min < pow(herb_tab[indice].range, 2)){
        apply_move_herb(indice, carn_tab[indice_min].x, carn_tab[indice_min].y, herb_tab[indice].vitesse);
        herb_tab[indice].count_fuite = 100;
        herb_tab[indice].previous_predator_x = carn_tab[indice_min].x;
        herb_tab[indice].previous_predator_y = carn_tab[indice_min].y;
        return true;
    }
    else if(herb_tab[indice].count_fuite != -1){
        apply_move_herb(indice, herb_tab[indice].previous_predator_x, herb_tab[indice].previous_predator_y, herb_tab[indice].vitesse);
        herb_tab[indice].count_fuite -= 1;
        return true;
    }
    return false;
}

bool Jeu::zone_spotting(int indice){
    if(zone_tab.size() == 0) return false;
    int indice_min = 0;
    int x = herb_tab[indice].x;
    int y = herb_tab[indice].y;
    int min = calcul_distance(x, zone_tab[0].x, y, zone_tab[0].y);
    unsigned int i = 0;
    while(i<zone_tab.size()){
        int d = calcul_distance(x, zone_tab[i].x, y, zone_tab[i].y);
        if(d<min){
            min = d;
            indice_min = i;
            if(d<herb_tab[indice].range){
                i = zone_tab.size();
            }
        }
        i += 1;
    }
    int size_zone = zone_tab[indice_min].plante_tab.size();
    if(size_zone <= 1){
        move_on_plante(indice, indice_min, random(0, size_zone), vitesse_repos_herb);
        return true;
    }
    int tab_pl[] = {-1, -1, -1};
    min = sort_plante_tab(tab_pl, x, y, indice_min);
    for(int i = 0; i<size_zone; i++){
        int d = calcul_distance(x, zone_tab[indice_min].plante_tab[i].x, y, zone_tab[indice_min].plante_tab[i].y);
        if(d<min){
            if(d < calcul_distance(x, zone_tab[indice_min].plante_tab[tab_pl[1]].x, y, zone_tab[indice_min].plante_tab[tab_pl[1]].y)){
                if(d < calcul_distance(x, zone_tab[indice_min].plante_tab[tab_pl[0]].x, y, zone_tab[indice_min].plante_tab[tab_pl[0]].y)){
                    tab_pl[2] = tab_pl[1];
                    tab_pl[1] = tab_pl[0];
                    tab_pl[0] = i;
                }else{
                    tab_pl[2] = tab_pl[1];
                    tab_pl[1] = i;
                }
                min = calcul_distance(x, zone_tab[indice_min].plante_tab[tab_pl[2]].x, y, zone_tab[indice_min].plante_tab[tab_pl[2]].y);
            }else{
                min = d;
                tab_pl[2] = i;
            }
        }
    }
    move_on_plante(indice, indice_min, tab_pl[0], vitesse_repos_herb);
    return true;
}

bool Jeu::detect_herbivore_partner(unsigned int indice, unsigned int *size){
    if(indice_repro_herb == *size){
        return false;
    }
    int min = calcul_distance(herb_tab[indice].x, herb_tab[indice_repro_herb+1].x, herb_tab[indice].y, herb_tab[indice_repro_herb+1].y);
    int indice_min = indice_repro_herb + 1;
    if(indice == indice_repro_herb+1){
        min = calcul_distance(herb_tab[indice].x, herb_tab[indice_repro_herb+2].x, herb_tab[indice].y, herb_tab[indice_repro_herb+2].y);
        indice_min = indice_repro_herb + 2;
    }
    for(unsigned int i = indice_min + 1; i<*size; i++){
        if(indice != i && abs(herb_tab[indice].height - herb_tab[i].height) < 15){
            int d = calcul_distance(herb_tab[indice].x, herb_tab[i].x, herb_tab[indice].y, herb_tab[i].y);
            if(d < min){
                min = d;
                indice_min = i;
            }
        }
    }
    if(min < pow(herb_tab[indice].range, 2)){
        if( marcher_vers_partenaire_herbivore(indice, indice_min)){
            *size += 1;
        }
        return true;
    }
    return false;
}



int Jeu::find_index_herbivor(int x, int y){
    int size = herb_tab.size();
    for(int i = 0; i<size; i++){
        if(herb_tab[i].x == x && herb_tab[i].y == y){
            return i;
        }
    }
    return -1;
}


bool Jeu::marcher_vers_partenaire_herbivore(int indice, int indice_part){
    double x1 = herb_tab[indice_part].x;
    double y1 = herb_tab[indice_part].y;
    double x2 = herb_tab[indice].x;
    double y2 = herb_tab[indice].y;
    int x3;
    int y3;
    if(x1 == x2 && y1 == y2){
        accoupler_herbivore(indice, indice_part);
        return true;
    }
    determinate_new_pos(x1, y1, x2, y2, &x3, &y3, vitesse_repos_herb, 'c');
    herb_tab[indice].x = x3;
    herb_tab[indice].y = y3;
    move_creature(x3, y3, herb_tab[indice].point, herb_tab[indice].origineX, herb_tab[indice].origineY);
    if(herb_tab[indice_part].x == x3 && herb_tab[indice_part].y == y3){
        accoupler_herbivore(indice, indice_part);
        return true;
    }
    return false;
}

void Jeu::accoupler_herbivore(int indice_herb, int indice_part){
    herb_tab[indice_herb].repro_count = 0;
    herb_tab[indice_part].repro_count = 0;
    add_an_element(indice_herb, indice_part, 'h');
}

void Jeu::eat_plante(int indice){
    if(!recal_id(indice)){
        return;
    }
    int indice_plante = herb_tab[indice].buisson;
    int indice_zone = herb_tab[indice].zone;
    if(zone_tab[indice_zone].plante_tab[herb_tab[indice].buisson].pv == 0){
        herb_tab[indice].eating = false;
        return;
    }
    herb_tab[indice].faim -= 5;
    if(herb_tab[indice].faim < 0) herb_tab[indice].faim = 0;
    zone_tab[indice_zone].plante_tab[indice_plante].pv -= 1;
    if(zone_tab[indice_zone].plante_tab[indice_plante].pv <= 0){
        unsigned int i = 1;
        while(i<delete_plante_tab.size() && delete_plante_tab[i] > indice_plante){
            i += 2;
        }
        i = i-1;
        delete_plante_tab.insert(delete_plante_tab.begin() + i, indice_plante);
        delete_plante_tab.insert(delete_plante_tab.begin() + i, indice_zone);
        herb_tab[indice].eating = false;
    }
}





void Jeu::start_eat(int indice_herb, int indice_zone, int indice_plante){
    herb_tab[indice_herb].eating = true;
    herb_tab[indice_herb].buisson = indice_plante;
    herb_tab[indice_herb].zone = indice_zone;
    herb_tab[indice_herb].id_plante = zone_tab[indice_zone].plante_tab[indice_plante].id;
    herb_tab[indice_herb].id_zone = zone_tab[indice_zone].id;
}
