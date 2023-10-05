#include "jeu.h"



Jeu::Jeu(QGraphicsView *parent, QGraphicsScene *scene, QGraphicsView *view)
    : QMainWindow(parent), scene(scene), view(view)
{
    setWindowTitle("Eco-système");
        view->resize(map_width, map_height);
    QTransform transform = view->transform();
    double zoomFactor = 3.0;
    transform.scale(1.0 / zoomFactor, 1.0 / zoomFactor);
    view->setTransform(transform);
    //    KeyPressHandler keyHandler;
    //    QObject::connect(&app, SIGNAL(keyPressed(QKeyEvent*)), &keyHandler, SLOT(handleKeyPress(QKeyEvent*)));
    scene->setSceneRect(0, 0, map_width, map_height);
    QColor backgroundColor(210, 180, 140);
    scene->setBackgroundBrush(backgroundColor);
    init_tab();
    view->show();
    start(0);
    indice_repro_carn = nb_carn_start - 1;
    indice_repro_herb = nb_herb_start - 1;
}

Jeu::~Jeu(){
}

void Jeu::start(int count){
    if(count % 10 == 0){
        get_stats();
        x_stats += 1;   
        if(count == 5000){
            display_stats();
            return;
        }
    }
    QTimer::singleShot(20, this, [=](){
        move_carn();
        move_herb();
        delete_the_dead_plante();
        view->update();
        sort_crea();
        print_max_speed();
        start(count+1);
    });
}


void Jeu::init_tab(){
    // faim, vitesse, champ vision, taille
    QBrush yellow(Qt::yellow);
    QBrush green(Qt::green);
    for(unsigned int i = 0; i<nb_carn_start || i<nb_herb_start || i<nb_plante_start; i++){
        if(i < nb_carn_start){
            float stats[4][3] = {{base_min_vitesse_carn, base_max_vitesse_carn, 0}, {base_min_range_carn, base_max_range_carn, 0}, {base_min_height_carn, base_max_height_carn, 0}, {base_min_starve_carn, base_max_starve_carn, 0}};
            generate_stats(stats);
            int xcarn = random(0, map_width);
            int ycarn = random(0, map_height);
            add_carn(stats[3][2], random(base_min_start_hunt, base_max_start_hunt), stats[0][2], stats[1][2], xcarn, ycarn, i, stats[2][2]);
            carn_tab[i].point->setBrush(yellow);
            scene->addItem(carn_tab[i].point);
        }
        if(i < nb_herb_start){
            float stats[4][3] = {{base_min_vitesse_herb, base_max_vitesse_herb, 0}, {base_min_range_herb, base_max_range_herb, 0}, {base_min_height_herb, base_max_height_herb, 0}, {base_min_starve_herb, base_max_starve_herb, 0}};
            generate_stats(stats);
            int xherb = random(0, map_width);
            int yherb = random(0, map_height);
            add_herb(random(base_min_start_find_eat, base_max_start_find_eat), stats[3][2], stats[0][2], stats[1][2], xherb, yherb, i, stats[2][2]);
            herb_tab[i].point->setBrush(green);
            scene->addItem(herb_tab[i].point);
        }
        if(i < nb_plante_start){
            create_a_plante_grp(random(0, map_width), random(0, map_height), 500, 100);
        }
    }
    herb_count = nb_herb_start;
}

void Jeu::generate_stats(float stats[4][3]){
    float points = 40;
    bool used[] = {false, false, false, false};
    for(int j=0; j<4; j++){
        int k = random(0, 3);
        while(used[k]){
            k = random(0, 3);
        }
        used[k] = true;
        int val = random(stats[k][0], stats[k][1]);
        float conv = (val/stats[k][1])*10;
        if(conv>points){
            stats[k][2] = stats[k][1];
            points -= 10;
        }else{
            stats[k][2] = val;
            points -= conv;
        }
    }
}
