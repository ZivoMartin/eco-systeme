#include "../jeu.h"

void Jeu::move_carn(){
    unsigned int i = 0;
    unsigned int size = carn_tab.size();
    while(i<size){
        carn_tab[i].faim += 1;
        carn_tab[i].hp -= 1;
        if(carn_tab[i].faim == carn_tab[i].start_hunt){
            carn_tab[i].etat = 't';
            change_the_color('o', i, 'c');
            carn_tab[i].random_count = -1;
            carn_tab[i].repro = false;
            carn_tab[i].repro_count = 0;
        }
        if(carn_tab[i].etat == 'r'){
            if(!carn_tab[i].repro){
                carn_tab[i].repro_count += 1;
            }
            if(carn_tab[i].repro_count == carn_tab[i].starve_death/4){
                carn_tab[i].repro = true;
            }
            if(carn_tab[i].repro){
                if(detect_carnivore_partner(i, &size)){
                    carn_tab[i].random_count = -1;
                }else{
                    random_move_carn(i, vitesse_repos_carn, 50);
                }
            }else{
                random_move_carn(i, vitesse_repos_carn, 50);
            }
        }else{
            prey_spotting(i);
        }
        if(carn_tab[i].faim == carn_tab[i].starve_death || carn_tab[i].hp == 0){
            delete_element(i, 'c', -1);
            size = size - 1;

        }else{
            i++;
        }
    }
}


void Jeu::random_move_carn(int indice, int speed, int end_random_count){
    if(carn_tab[indice].random_count == -1 || carn_tab[indice].random_count == end_random_count){
        carn_tab[indice].actual_random_direction = random(1, 8);
        carn_tab[indice].random_count = 0;
    }
    int value = carn_tab[indice].actual_random_direction;
    int r_v = speed/sqrt2;
    if(value == 1){
        carn_tab[indice].x = get_x(carn_tab[indice].x + speed);

    }
    else if(value == 2){
        carn_tab[indice].x = get_x(carn_tab[indice].x - speed);
    }
    else if(value == 3){
        carn_tab[indice].y = get_y(carn_tab[indice].y + speed);
    }
    else if(value == 4){
        carn_tab[indice].y = get_y(carn_tab[indice].y - speed);
    }else if(value == 5){
        carn_tab[indice].x = get_x(carn_tab[indice].x + r_v);
        carn_tab[indice].y = get_y(carn_tab[indice].y + r_v);
    }else if(value == 6){
        carn_tab[indice].x = get_x(carn_tab[indice].x + r_v);
        carn_tab[indice].y = get_y(carn_tab[indice].y - r_v);
    }else if(value == 7){
        carn_tab[indice].x = get_x(carn_tab[indice].x - r_v);
        carn_tab[indice].y = get_y(carn_tab[indice].y + r_v);
    }else{
        carn_tab[indice].x = get_x(carn_tab[indice].x - r_v);
        carn_tab[indice].y = get_y(carn_tab[indice].y - r_v);
    }
    move_creature(carn_tab[indice].x, carn_tab[indice].y, carn_tab[indice].point, carn_tab[indice].origineX, carn_tab[indice].origineY);
    carn_tab[indice].random_count += 1;
}

void Jeu::apply_carn_move(int indice, int indice_proie){
    if(carn_tab[indice].random_count != -1){
        change_the_color('r', indice, 'c');
        carn_tab[indice].random_count = -1;
    }
    double x1 = herb_tab[indice_proie].x;
    double y1 = herb_tab[indice_proie].y;
    double x2 = carn_tab[indice].x;
    double y2 = carn_tab[indice].y;
    int x3;
    int y3;
    int height = carn_tab[indice].height/2;
    if((x2 >= x1-height && x2 <= x1+height) && (y2 >= y1-height && y2 <= y1+height)){
        eat_herb(indice, indice_proie);
        return;
    }
    determinate_new_pos(x1, y1, x2, y2, &x3, &y3, carn_tab[indice].vitesse, 'c');
    carn_tab[indice].x = x3;
    carn_tab[indice].y = y3;
    move_creature(x3, y3, carn_tab[indice].point, carn_tab[indice].origineX, carn_tab[indice].origineY);
    if((x3 >= x1-height && x3 <= x1+height) && (y3 >= y1-height && y3 <= y1+height)){
        eat_herb(indice, indice_proie);
    }
}


bool Jeu::marcher_vers_partenaire_carnivore(int indice, int indice_part){
    double x1 = carn_tab[indice_part].x;
    double y1 = carn_tab[indice_part].y;
    double x2 = carn_tab[indice].x;
    double y2 = carn_tab[indice].y;
    int x3;
    int y3;
    if(x1 == x2 && y1 == y2){
        accoupler_carnivore(indice, indice_part);
        return true;
    }
    determinate_new_pos(x1, y1, x2, y2, &x3, &y3, vitesse_repos_carn, 'c');
    carn_tab[indice].x = x3;
    carn_tab[indice].y = y3;
    move_creature(x3, y3, carn_tab[indice].point, carn_tab[indice].origineX, carn_tab[indice].origineY);
    if(carn_tab[indice_part].x == x3 && carn_tab[indice_part].y == y3){
        accoupler_carnivore(indice, indice_part);
        return true;
    }
    return false;
}