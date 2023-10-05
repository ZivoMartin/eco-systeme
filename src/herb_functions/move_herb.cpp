#include "../jeu.h"

void Jeu::move_herb(){
    unsigned int i = 0;
    unsigned int size = herb_tab.size();
    while(i<size){
        herb_tab[i].hp -= 1;
        herb_tab[i].faim += 1;
        herb_tab[i].repro_count += 1;
        if(predator_spotting(i)){
            herb_tab[i].fuite = true;
            herb_tab[i].eating = false;
            herb_tab[i].random_count = -1;
        }else if(herb_tab[i].eating){
            eat_plante(i);
        }else if(herb_tab[i].repro_count > herb_tab[i].reproductible && herb_tab[i].faim <= herb_tab[i].start_find_eat && detect_herbivore_partner(i, &size)){
            herb_tab[i].fuite = false;
            herb_tab[i].random_count = -1;

        }else{
            herb_tab[i].fuite = false;
            if(!zone_spotting(i)){
                random_move_herb(i);
            }else{
                herb_tab[i].random_count = -1;
            }
        }
        if(herb_tab[i].hp == 0 || herb_tab[i].faim == herb_tab[i].starve_death){
            delete_element(i, 'h', -1);
            size = size - 1;
        }else{
            i = i + 1;
        }
    }
}


void Jeu::random_move_herb(int indice){
    if(herb_tab[indice].random_count == -1 || herb_tab[indice].random_count == 50){
        herb_tab[indice].actual_random_direction = random(1, 8);
        herb_tab[indice].random_count = 0;
    }
    int value = herb_tab[indice].actual_random_direction;
    int r_v = vitesse_repos_herb/sqrt2;
    if(value == 1){
        herb_tab[indice].x = get_x(herb_tab[indice].x + vitesse_repos_herb);

    }
    else if(value == 2){
        herb_tab[indice].x = get_x(herb_tab[indice].x - vitesse_repos_herb);
    }
    else if(value == 3){
        herb_tab[indice].y = get_y(herb_tab[indice].y + vitesse_repos_herb);
    }
    else if(value == 4){
        herb_tab[indice].y = get_y(herb_tab[indice].y - vitesse_repos_herb);
    }else if(value == 5){
        herb_tab[indice].x = get_x(herb_tab[indice].x + r_v);
        herb_tab[indice].y = get_y(herb_tab[indice].y + r_v);
    }else if(value == 6){
        herb_tab[indice].x = get_x(herb_tab[indice].x + r_v);
        herb_tab[indice].y = get_y(herb_tab[indice].y - r_v);
    }else if(value == 7){
        herb_tab[indice].x = get_x(herb_tab[indice].x - r_v);
        herb_tab[indice].y = get_y(herb_tab[indice].y + r_v);
    }else{
        herb_tab[indice].x = get_x(herb_tab[indice].x - r_v);
        herb_tab[indice].y = get_y(herb_tab[indice].y - r_v);
    }
    move_creature(herb_tab[indice].x, herb_tab[indice].y, herb_tab[indice].point, herb_tab[indice].origineX, herb_tab[indice].origineY);
    herb_tab[indice].random_count += 1;
}



void Jeu::apply_move_herb(int indice, double x1, double y1, int vitesse){
    double x2 = herb_tab[indice].x;
    double y2 = herb_tab[indice].y;
    int x3 = -1;
    int y3 = -1;
    if(x1 == x2 && y1 == y2){
        return;
    }
    determinate_new_pos(x1, y1, x2, y2, &x3, &y3, vitesse, 'h');
    herb_tab[indice].x = x3;
    herb_tab[indice].y = y3;
    move_creature(x3, y3, herb_tab[indice].point, herb_tab[indice].origineX, herb_tab[indice].origineY);
}

void Jeu::move_on_plante(int indice_herb, int indice_zone, int indice_plante, int speed){
    double x1 = zone_tab[indice_zone].plante_tab[indice_plante].x;
    double y1 = zone_tab[indice_zone].plante_tab[indice_plante].y;
    double x2 = herb_tab[indice_herb].x;
    double y2 = herb_tab[indice_herb].y;
    int x3;
    int y3;
    if(x1 >= x2-2*largeur_plante && x1 <= x2+2*largeur_plante  && y1 >= y2-2*largeur_plante && y1 <= y2+2*largeur_plante){
        start_eat(indice_herb, indice_zone, indice_plante);
        return;
    }
    determinate_new_pos(x1, y1, x2, y2, &x3, &y3, speed, 'c');
    herb_tab[indice_herb].x = x3;
    herb_tab[indice_herb].y = y3;
    move_creature(x3, y3, herb_tab[indice_herb].point, herb_tab[indice_herb].origineX, herb_tab[indice_herb].origineY);
    if(x1 >= x2-largeur_plante && x1 <= x2+largeur_plante  && y1 >= y2-largeur_plante && y1 <= y2+largeur_plante){
        start_eat(indice_herb, indice_zone, indice_plante);
    }
}