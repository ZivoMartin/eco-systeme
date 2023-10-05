#include "../jeu.h"

int Jeu::get_speed(int i1, int i2, char type){
    int val;
    if(type == 'h'){
        int vitesse = herb_tab[i1].vitesse;
        if(random(0, 1) == 0){
            vitesse = herb_tab[i2].vitesse;
        }
        val = random(vitesse - 1, vitesse + 1);
        if(val < vitesse_repos_herb) val = vitesse_repos_herb;
    }else{
        int vitesse = carn_tab[i1].vitesse;
        if(random(0, 1) == 0){
            vitesse = carn_tab[i2].vitesse;
        }
        val = random(vitesse - 1, vitesse + 1);
        if(val == vitesse_repos_carn) val = vitesse_repos_carn;
    }
    return val;
}

int Jeu::get_range(int i1, int i2, char type){
    int val;
    if(type == 'h'){
        int range = herb_tab[i1].range;
        if(random(0, 1) == 0){
            range = herb_tab[i2].range;
        }
        val = random(range - 2, range + 2);
        if(val == 0) val = 1;
        else if(val > range_herb_max) val = range_herb_max;
    }else{
        int range = carn_tab[i1].range;
        if(random(0, 1) == 0){
            range = carn_tab[i2].range;
        }
        val = random(range - 2, range + 2);
        if(val == 0) val = 1;
        else if(val > range_carn_max) val = range_carn_max;
    }
    return val;
}

int Jeu::get_starve_death(int i1, int i2, char type){
    int starve_death;
    if(type == 'c'){
        starve_death = carn_tab[i1].starve_death;
        if(random(0, 1) == 0){
            starve_death = carn_tab[i2].starve_death;
        }
    }else{
        starve_death = herb_tab[i1].starve_death;
        if(random(0, 1) == 0){
            starve_death = herb_tab[i2].starve_death;
        }
    }
    return random(starve_death - 1, starve_death + 1);
}

int Jeu::get_start_hunt(int i1, int i2){
    int start_hunt = carn_tab[i1].start_hunt;
    if(random(0, 1) == 0){
        start_hunt = carn_tab[i2].start_hunt;
    }
    return random(start_hunt - 10, start_hunt + 10);
}

int Jeu::get_start_find_eat(int i1, int i2){
    int start_find_eat = herb_tab[i1].start_find_eat;
    if(random(0, 1) == 0){
        start_find_eat = herb_tab[i2].start_find_eat;
    }
    return random(start_find_eat - 10, start_find_eat + 10);
}

int Jeu::get_height(int i1, int i2, char type){
    int height;
    if(type == 'c'){
        height = carn_tab[i1].height;
        if(random(0, 1) == 0){
            height = carn_tab[i2].height;
        }
    }else{
        height = herb_tab[i1].height;
        if(random(0, 1) == 0){
            height = herb_tab[i2].height;
        }
    }
    return random(height - 2, height + 2);
}


int Jeu::get_x(int value){
    while(value>=map_width){
        value = value - map_width;
    }
    while(value<0){
        value = value + map_width;
    }
    return value;
}

int Jeu::get_y(int value){
    while(value>=map_height){
        value = value - map_height;
    }
    while(value<0){
        value = value + map_height;
    }
    return value;
}