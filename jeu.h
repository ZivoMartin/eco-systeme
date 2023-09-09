#ifndef
#include <iostream>
#include <QApplication>
#include <QMainWindow>
#include "struct.h"
#include <math.h>
#include <vector>
#include <random>
#include <QTimer>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QBrush>
#include <QPen>
#include <QKeyEvent>
#include <QObject>
#include <QPen>


class Jeu : public QMainWindow
{
    Q_OBJECT

public:
    Jeu(QGraphicsView *parent, QGraphicsScene *scene, QGraphicsView *view);
    ~Jeu();
    void init_tab();
    void start();
    void print_creatures();

    void move_carn();
    void move_herb();
    bool predator_spotting(int indice);
    bool reperer_partenaire_carnivore(int indice, int *size);
    bool reperer_partenaire_herbivore(int indice, int *size);
    void prey_spotting(int indice);
    bool zone_spotting(int indice);
    void determinate_new_pos(double x1, double y1, double x2, double y2, int *x3, int *y3, int vitesse, char type);
    void apply_carn_move(int indice, int indice_proie);
    bool marcher_vers_partenaire_carnivore(int indice, int indice_part);
    bool marcher_vers_partenaire_herbivore(int indice, int indice_part);
    void move_on_plante(int indice_herb, int indice_zone, int indice_plante, int speed);
    void eat_herb(int indice_carn, int indice_herb);
    void accoupler_carnivore(int indice_carn, int indice_part);
    void accoupler_herbivore(int indice_herb, int indice_part);
    void apply_move_herb(int indice, double x1, double y1, int vitesse);
    void find_border_point(double m, double b, int *bx1, int *by1, int *bx2, int *by2);
    void add_an_element(int i1, int i2, char type);
    void move_creature(int x, int y, QGraphicsEllipseItem *point, int origX, int origY);
    void random_move_carn(int indice, int speed, int end_rando_count);
    void random_move_herb(int indice);
    int get_speed(int i1, int i2, char type);
    int get_range(int i1, int i2, char type);
    int get_starve_death(int i1, int i2, char type);
    int get_start_hunt(int i1, int i2);
    int get_start_find_eat(int i1, int i2);
    int get_height(int i1, int i2, char type);
    void add_carn(int starve_death, int start_hunt, int vitesse, int range, int x, int y, int i, int height);
    void add_herb(int start_find_eat, int starve_death, int vitesse, int range, int x, int y, int i, int height);
    void add_plante(int x, int y, int indice_zone);
    void add_zone(int x, int y, int rayon, int size);
    void create_a_plante_grp(int x, int y, int max_rayon, int nb_plante_max);
    void delete_element(int indice, char type, int potential_indice_zone);
    void change_the_color(char color, int indice);
    int calculate_distance(int i_herb, int i_carn, char type);
    void eat_plante(int indice);
    void delete_the_dead_plante();
    void start_eat(int indice_herb, int indice_zone, int indice_plante);
    bool recal_id(int indice);
    void synchro_zone(int id);
    int sort_plante_tab(int *t, int x, int y, int indice_min);

    int pivoter_carn(int start, int end);
    int pivoter_herb(int start, int end);
    void quick_sort_herb(int start, int end);
    void quick_sort_carn(int start, int end);
    int calcul_distance_to_00(int i, char type);
    void swap_crea(int i, int j, char type);
    void sort_crea();
    void print_max_speed();

    int random(int min, int max);
    int get_x(int value);
    int get_y(int value);
    int find_index_predator(int x, int y);
    int find_index_herbivor(int x, int y);

    static const int herb_max = 500;
    static const int vitesse_herb_max = 200;
    static const int vitesse_carn_max = 200;
    static const int vitesse_repos_carn = 3;
    static const int vitesse_repos_herb = 3;
    static const int base_min_vitesse_carn = 5;
    static const int base_max_vitesse_carn = 8;
    static const int base_min_vitesse_herb = 5;
    static const int base_max_vitesse_herb = 8;

    static const int base_min_height_carn = 30;
    static const int base_max_height_carn = 50;
    static const int base_min_height_herb = 20;
    static const int base_max_height_herb = 50;

    static const int range_herb_max = 4000;
    static const int range_carn_max = 4000;
    static const int base_min_range_carn = 500;
    static const int base_max_range_carn = 600;
    static const int base_min_range_herb = 200;
    static const int herb_range_repro = 1000;
    static const int carn_range_repro = 1000;
    static const int base_max_range_herb = 400;
    static const int base_min_starve_carn = 1500;
    static const int base_max_starve_carn = 2000;
    static const int base_min_starve_herb = 1000;
    static const int base_max_starve_herb = 1500;
    static const int base_max_start_hunt = 1000;
    static const int base_min_start_hunt = 750;
    static const int base_min_start_find_eat = 750;
    static const int base_max_start_find_eat = 1000;
    static const int base_min_vitesse_repos = 3;
    static const int base_max_vitesse_repos = 5;
    static const int carn_max = 1000;
    static const int nb_carn_start = 300;
    static const int nb_herb_start = 300;
    static const int nb_plante_start = 50;
    static const int age_max_herb = 10000;
    static const int age_max_carn = 10000;
    static const int reproduct_herb = 500;
    static const int sqrt2 = sqrt(2);
    static const int range_herb_plante = 2000;
    static const int largeur_plante = 30;





private:
    std::vector<carnivore_t> carn_tab;
    std::vector<herbivore_t> herb_tab;
    std::vector<zone_herbe_t> zone_tab;
    std::vector<int> delete_plante_tab;

    int map_height = 20000;
    int map_width = 20000;

    int count_id_herb = 0;
    int indice_repro_carn;
    int indice_repro_herb;
    int count_plante_id = 0;
    int count_zone_id = 0;
    int carn_count = 0;
    int herb_count = 0;

    QGraphicsScene *scene;
    QGraphicsView *view;
};



#endif // JEU_H
