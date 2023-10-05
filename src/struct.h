#ifndef STRUCT_H
#define STRUCT_H

#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>

struct carnivore_t{
    int faim;
    int start_hunt;
    int starve_death;
    int vitesse;
    int range;
    int hp;
    int x;
    int y;
    QGraphicsEllipseItem *point;
    int origineX;
    int origineY;
    int random_count;
    int actual_random_direction;
    char etat;
    bool repro;
    int repro_count;
    int height;
    bool corpse_state;
};

struct herbivore_t{
    int id;
    int faim;
    int starve_death;
    int vitesse;
    int range;
    int hp;
    int x;
    int y;
    bool mobile;
    QGraphicsEllipseItem *point;
    int origineX;
    int origineY;
    int repro_count;
    int reproductible;
    bool fuite;
    int random_count;
    int actual_random_direction;
    int count_fuite;
    int previous_predator_x;
    int previous_predator_y;
    bool eating;
    int buisson;
    int zone;
    int id_plante;
    int id_zone;
    int start_find_eat;
    int height;
    bool corpse_state;
};

struct herbe_t{
    int id;
    int x;
    int y;
    int pv;
    int count_apparition;
    QGraphicsRectItem *point;
};

struct zone_herbe_t{
    int id;
    std::vector<herbe_t> plante_tab;
    int size;
    int rayon;
    int x;
    int y;
};

#endif
