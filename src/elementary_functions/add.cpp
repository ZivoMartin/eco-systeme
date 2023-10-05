#include "../jeu.h"

void Jeu::add_carn(int starve_death, int start_hunt, int vitesse, int range, int x, int y, int i, int height){
    carn_tab.insert(carn_tab.begin() + i, {0, start_hunt, starve_death, vitesse, range, age_max_carn, x, y, new QGraphicsEllipseItem(x-15, y-15, height, height), x, y, -1, 3, 'r', false, 0, height, false});
}
void Jeu::add_herb(int start_find_eat, int starve_death, int vitesse, int range, int x, int y, int i, int height){
    herb_tab.insert(herb_tab.begin()+i, {count_id_herb, 0, starve_death, vitesse, range, age_max_herb, x, y, true, new QGraphicsEllipseItem(x-15, y-15, height, height), x, y, 0, reproduct_herb, false, -1, -1, -1, -1, -1, false, -1, -1, -1, -1, start_find_eat, height, false});
    count_id_herb += 1;
}
void Jeu::add_plante(int x, int y, int indice_zone){
    zone_tab[indice_zone].plante_tab.push_back({count_plante_id, x, y, 50, 0, new QGraphicsRectItem(x, y, 30, 30)});
    zone_tab[indice_zone].plante_tab[zone_tab[indice_zone].plante_tab.size() - 1].point->setPen(Qt::NoPen);
    count_plante_id += 1;
}

void Jeu::add_zone(int x, int y, int rayon, int size){
    zone_tab.push_back({count_zone_id, {}, size, rayon, x, y});
    count_zone_id += 1;
}

void Jeu::create_a_plante_grp(int x, int y, int max_rayon, int nb_plante_max){
    QColor color(0, 154, 23);
    QBrush brush(color);
    int rayon = random(100, max_rayon);
    int nb_plante = random(10, nb_plante_max);
    add_zone(x, y, rayon, nb_plante);
    int indice_zone = zone_tab.size() - 1;
    for(int i = 0; i<nb_plante; i++){
        add_plante(get_x(random(x-rayon, x+rayon)), get_y(random(y-rayon, y+rayon)), indice_zone);
        zone_tab[indice_zone].plante_tab[i].point->setBrush(brush);
        scene->addItem(zone_tab[indice_zone].plante_tab[i].point);
    }
}

void Jeu::delete_element(int indice, char type, int potential_indice_zone){
    if(type == 'h'){
        scene->removeItem(herb_tab[indice].point);
        delete herb_tab[indice].point;
        herb_tab.erase(herb_tab.begin() + indice);
    }else if(type == 'c'){
        if(random(0, 3) == 1){
            create_a_plante_grp(carn_tab[indice].x, carn_tab[indice].y, 100, 20);
        }
        scene->removeItem(carn_tab[indice].point);
        delete carn_tab[indice].point;
        carn_tab.erase(carn_tab.begin() + indice);
    }else if(type == 'p'){
        scene->removeItem(zone_tab[potential_indice_zone].plante_tab[indice].point);
        delete zone_tab[potential_indice_zone].plante_tab[indice].point;
        zone_tab[potential_indice_zone].plante_tab.erase(zone_tab[potential_indice_zone].plante_tab.begin() + indice);
    }else if(type == 'z'){
        zone_tab.erase(zone_tab.begin() + indice);
    }
}



void Jeu::add_an_element(int i1, int i2, char type){

    QBrush yellow(Qt::yellow);
    QBrush green(Qt::green);
    if(type == 'c'){
        int val1 = carn_tab[i1].x + random(-20, 20);
        int val2 = carn_tab[i1].y + random(-20, 20);
        add_carn(get_starve_death(i1, i2, 'c'), get_start_hunt(i1, i2), get_speed(i1, i2, 'c'), get_range(i1, i2, 'c'), val1, val2, i1, get_height(i1, i2, type));
        carn_tab[i1].point->setBrush(yellow);
        scene->addItem(carn_tab[i1].point);
    }else{
        int val1 = herb_tab[i1].x + random(-40, 40);
        int val2 = herb_tab[i2].y + random(-40, 40);
        add_herb(get_start_find_eat(i1, i2), get_starve_death(i1, i2, 'h'), get_speed(i1, i2, 'h'), get_range(i1, i2, 'h'), val1, val2, i1, get_height(i1, i2, type));
        herb_tab[i1].point->setBrush(green);
        scene->addItem(herb_tab[i1].point);
    }
}