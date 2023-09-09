#ifndef JEU_C
#define JEU_C
#include "jeu.h"

int calcul_distance(int x1, int x2, int y1, int y2){
    return(pow(x1-x2, 2) + pow(y1-y2, 2));
}

void swap_double(double *nb1, double *nb2){
    double tmp = *nb1;
    *nb1 = *nb2;
    *nb2 = tmp;
}
void swap_int(int *nb1, int *nb2){
    int tmp = *nb1;
    *nb1 = *nb2;
    *nb2 = tmp;
}

Jeu::Jeu(QGraphicsView *parent, QGraphicsScene *scene, QGraphicsView *view)
    : QMainWindow(parent), scene(scene), view(view)
{
    setWindowTitle("Eco-système");
        view->resize(map_width, map_height);
    QTransform transform = view->transform();
    double zoomFactor = 4.0;
    transform.scale(1.0 / zoomFactor, 1.0 / zoomFactor);
    view->setTransform(transform);
    //    KeyPressHandler keyHandler;
    //    QObject::connect(&app, SIGNAL(keyPressed(QKeyEvent*)), &keyHandler, SLOT(handleKeyPress(QKeyEvent*)));
    scene->setSceneRect(0, 0, map_width, map_height);
    QColor backgroundColor(210, 180, 140);
    scene->setBackgroundBrush(backgroundColor);
    init_tab();
    view->show();
    start();
    indice_repro_carn = nb_carn_start - 1;
    indice_repro_herb = nb_herb_start - 1;
}


Jeu::~Jeu()
{
}

void Jeu::start(){
    QTimer::singleShot(20, this, [=](){
        move_carn();
        move_herb();
        view->update();
        sort_crea();
        delete_the_dead_plante();
        print_max_speed();
        start();
    });
}


void Jeu::print_max_speed(){
    int t_vc[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i = 1; i<carn_tab.size(); i++){
        t_vc[carn_tab[i].vitesse - 3] += 1;
    }
    for(int i = 0; i<10; i++){
        std::cout << "carn a la vitesse " << i + 3 << " : " << t_vc[i] << std::endl;
    }
    std::cout << "SIZE CARN: " << carn_tab.size() << std::endl;
    int t_vh[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for(int i = 1; i<herb_tab.size(); i++){
        t_vh[herb_tab[i].vitesse - 3] += 1;
    }
    for(int i = 0; i<10; i++){
        std::cout << "herb a la vitesse" << i + 3 << " : " << t_vh[i] << std::endl;
    }
    std::cout << "SIZE HERB : " << herb_tab.size()<<std::endl << std::endl;
}

void Jeu::init_tab(){
    QBrush yellow(Qt::yellow);
    QBrush green(Qt::green);
    for(int i = 0; i<nb_carn_start || i<nb_herb_start || i<nb_plante_start; i++){
        if(i < nb_carn_start){
            int xcarn = random(0, map_width);
            int ycarn = random(0, map_height);
            int height = random(base_min_height_herb, base_max_height_herb);
            if(random(1, 20) == 3) height = 80;
            add_carn(random(base_min_starve_carn, base_max_starve_carn), random(base_min_start_hunt, base_max_start_hunt), random(base_min_vitesse_carn, base_max_vitesse_carn), random(base_min_range_carn, base_max_range_carn), xcarn, ycarn, i, height);
            carn_tab[i].point->setBrush(yellow);
            scene->addItem(carn_tab[i].point);
        }
        if(i < nb_herb_start){
            int xherb = random(0, map_width);
            int yherb = random(0, map_height);
            int height = random(base_min_height_herb, base_max_height_herb);
            if(random(1, 10) == 3) height = 80;
            add_herb(random(base_min_start_find_eat, base_max_start_find_eat), random(base_min_starve_herb, base_max_starve_herb), random(base_min_vitesse_herb, base_max_vitesse_herb), random(base_min_range_herb, base_max_range_herb), xherb, yherb, i, height);
            herb_tab[i].point->setBrush(green);
            scene->addItem(herb_tab[i].point);
        }
        if(i < nb_plante_start){
            create_a_plante_grp(random(0, map_width), random(0, map_height), 500, 100);
        }
    }
    herb_count = nb_herb_start;
}

void Jeu::add_carn(int starve_death, int start_hunt, int vitesse, int range, int x, int y, int i, int height){
    carn_tab.insert(carn_tab.begin() + i, {0, start_hunt, starve_death, vitesse, range, 0, x, y, new QGraphicsEllipseItem(x-15, y-15, height, height), x, y, -1, 3, 'r', false, 0, height});
}
void Jeu::add_herb(int start_find_eat, int starve_death, int vitesse, int range, int x, int y, int i, int height){
    herb_tab.insert(herb_tab.begin()+i, {count_id_herb, 0, starve_death, vitesse, range, 0, x, y, true, new QGraphicsEllipseItem(x-15, y-15, height, height), x, y, 0, reproduct_herb, false, -1, -1, -1, -1, -1, false, -1, -1, -1, -1, start_find_eat, height});
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

void Jeu::move_carn(){
    int i = 0;
    int size = carn_tab.size();
    while(i<size){
        carn_tab[i].faim += 1;
        carn_tab[i].age += 1;
        if(carn_tab[i].faim == carn_tab[i].start_hunt){
            carn_tab[i].etat = 't';
            change_the_color('o', i);
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
                if(reperer_partenaire_carnivore(i, &size)){
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
        if(carn_tab[i].faim == carn_tab[i].starve_death || carn_tab[i].age == age_max_carn){
            delete_element(i, 'c', -1);
            size = size - 1;
        }else{
            i++;
        }

    }
}

void Jeu::move_herb(){
    int i = 0;
    int size = herb_tab.size();
    while(i<size){
        herb_tab[i].age += 1;
        herb_tab[i].faim += 1;
        herb_tab[i].repro_count += 1;
        if(predator_spotting(i)){
            herb_tab[i].fuite = true;
            herb_tab[i].eating = false;
            herb_tab[i].random_count = -1;
        }else if(herb_tab[i].eating){
            eat_plante(i);
        }else if(herb_tab[i].repro_count > herb_tab[i].reproductible && herb_tab[i].faim <= herb_tab[i].start_find_eat && reperer_partenaire_herbivore(i, &size)){
            herb_tab[i].fuite = false;
            herb_tab[i].random_count = -1;
        //}else if(herb_tab.etat == 'w'){

        }else{
            herb_tab[i].fuite = false;
            if(!zone_spotting(i)){
                random_move_herb(i);
            }else{
                herb_tab[i].random_count = -1;
            }
        }
        if(herb_tab[i].age == age_max_herb || herb_tab[i].faim == herb_tab[i].starve_death){
            delete_element(i, 'h', -1);
            size = size - 1;
        }else{
            i = i + 1;
        }
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
        delete zone_tab[potential_indice_zone].plante_tab[indice]   .point;
        zone_tab[potential_indice_zone].plante_tab.erase(zone_tab[potential_indice_zone].plante_tab.begin() + indice);
    }else if(type == 'z'){
        zone_tab.erase(zone_tab.begin() + indice);
    }
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

void Jeu::prey_spotting(int indice){
    if(herb_tab.size() == 0){
        random_move_carn(indice, carn_tab[indice].vitesse, 200);
        return;
    }
    int min = calculate_distance(0, indice, 'c');
    int indice_min = 0;
    for(int i = 1; i<herb_tab.size(); i++){
        if(herb_tab[i].height - 15 < carn_tab[indice].height){
            int d = calculate_distance(i, indice, 'c');
            if(d < pow(herb_tab[i].vitesse, 2)){
                if(carn_tab[indice].etat == 't'){
                    carn_tab[indice].etat = 'c';
                    change_the_color('r', indice);
                }
                apply_carn_move(indice, i);
                return;
            }
            if(d < min){
                min = d;
                indice_min = i;
            }
        }
    }
    if(min < pow(carn_tab[indice].range, 2)){
        if(carn_tab[indice].etat == 't'){
            carn_tab[indice].etat = 'c';
            change_the_color('r', indice);
        }
        apply_carn_move(indice, indice_min);
        return;
    }
    if(carn_tab[indice].etat == 'c'){
        carn_tab[indice].etat = 't';
        change_the_color('o', indice);
    }
    random_move_carn(indice, carn_tab[indice].vitesse, 200);
}





bool Jeu::predator_spotting(int indice){
    if(carn_tab.size() == 0) return false;
    int min = calculate_distance(indice, 0, 'h');
    int indice_min = 0;
    int speed = pow(herb_tab[indice].vitesse, 2);
    for(int i = 1; i<carn_tab.size(); i++){
        if(carn_tab[i].height > herb_tab[indice].height - 15){
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
    int i = 0;
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

int Jeu::calculate_distance(int i_herb, int i_carn, char type){
    int xc = carn_tab[i_carn].x;
    int yc = carn_tab[i_carn].y;
    int xh = herb_tab[i_herb].x;
    int yh = herb_tab[i_herb].y;
    int d = calcul_distance(xh, xc, yh, yc);
    int range;
    if(type == 'h'){
        range = herb_tab[i_herb].range;
    }else{
        range = carn_tab[i_carn].range;
    }
    if(xh<range && map_width-xc < range){
        int d2 = calcul_distance(xh + map_width, xc, yh, yc);
        if(d2<d){
            d = d2;
        }
    }else if(xc<range && map_width-xh < range){
        int d2 = calcul_distance(xh - map_width, xc, yh, yc);
        if(d2<d){
            d = d2;
        }
    }
    if(yh<range && map_height-yc < range){
        int d2 = calcul_distance(xh, xc, yh + map_height, yc);
        if(d2<d){
            d = d2;
        }
    }else if(yc<range && map_width-yh < range){
        int d2 = calcul_distance(xh, xc, yh - map_height, yc);
        if(d2<d){
            d = d2;
        }
    }
    return d;
}
bool Jeu::reperer_partenaire_carnivore(int indice, int *size){
    if(indice_repro_carn == carn_tab.size() - 2){
        return false;
    }
    int min = calcul_distance(carn_tab[indice].x, carn_tab[indice_repro_carn+1].x, carn_tab[indice].y, carn_tab[indice_repro_carn+1].y);
    int indice_min = indice_repro_carn + 1;
    if(indice == indice_repro_carn+1){
        min = calcul_distance(carn_tab[indice].x, carn_tab[indice_repro_carn+2].x, carn_tab[indice].y, carn_tab[indice_repro_carn+2].y);
        indice_min = indice_repro_carn + 2;
    }
    for(int i = indice_min + 1; i<carn_tab.size(); i++){
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

bool Jeu::reperer_partenaire_herbivore(int indice, int *size){
    if(indice_repro_herb == herb_tab.size() - 2){
        return false;
    }
    int min = calcul_distance(herb_tab[indice].x, herb_tab[indice_repro_herb+1].x, herb_tab[indice].y, herb_tab[indice_repro_herb+1].y);
    int indice_min = indice_repro_herb + 1;
    if(indice == indice_repro_herb+1){
        min = calcul_distance(herb_tab[indice].x, herb_tab[indice_repro_herb+2].x, herb_tab[indice].y, herb_tab[indice_repro_herb+2].y);
        indice_min = indice_repro_herb + 2;
    }
    for(int i = indice_min + 1; i<herb_tab.size(); i++){
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



int Jeu::find_index_predator(int x, int y){
    for(int i = 0; i<carn_tab.size(); i++){
        if(carn_tab[i].x == x && carn_tab[i].y == y){
            return i;
        }
    }
    return -1;
}
int Jeu::find_index_herbivor(int x, int y){
    for(int i = 0; i<herb_tab.size(); i++){
        if(herb_tab[i].x == x && herb_tab[i].y == y){
            return i;
        }
    }
    return -1;
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


void Jeu::apply_carn_move(int indice, int indice_proie){
    if(carn_tab[indice].random_count != -1){
        change_the_color('r', indice);
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

void Jeu::eat_herb(int indice_carn, int indice_herb){
    delete_element(indice_herb, 'h', -1);
    carn_tab[indice_carn].faim = 0;
    herb_count -= 1;
    carn_tab[indice_carn].etat = 'r';
    change_the_color('y', indice_carn);
    carn_tab[indice_carn].repro = true;
}

void Jeu::accoupler_carnivore(int indice_carn, int indice_part){
    carn_tab[indice_carn].repro = false;
    carn_tab[indice_part].repro = false;
    add_an_element(indice_carn, indice_part, 'c');
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
    if(zone_tab[indice_zone].plante_tab[indice_plante].pv == 0){
        int i = 1;
        while(i<delete_plante_tab.size() && delete_plante_tab[i] > indice_plante){
            i += 2;
        }
        i = i-1;
        delete_plante_tab.insert(delete_plante_tab.begin() + i, indice_plante);
        delete_plante_tab.insert(delete_plante_tab.begin() + i, indice_zone);
        herb_tab[indice].eating = false;
    }
}



void Jeu::move_on_plante(int indice_herb, int indice_zone, int indice_plante, int speed){
    double x1 = zone_tab[indice_zone].plante_tab[indice_plante].x;
    double y1 = zone_tab[indice_zone].plante_tab[indice_plante].y;
    double x2 = herb_tab[indice_herb].x;
    double y2 = herb_tab[indice_herb].y;
    int x3;
    int y3;
    if(x1 >= x2-largeur_plante && x1 <= x2+largeur_plante  && y1 >= y2-largeur_plante && y1 <= y2+largeur_plante){
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

void Jeu::start_eat(int indice_herb, int indice_zone, int indice_plante){
    herb_tab[indice_herb].eating = true;
    herb_tab[indice_herb].buisson = indice_plante;
    herb_tab[indice_herb].zone = indice_zone;
    herb_tab[indice_herb].id_plante = zone_tab[indice_zone].plante_tab[indice_plante].id;
    herb_tab[indice_herb].id_zone = zone_tab[indice_zone].id;
}


void Jeu::determinate_new_pos(double x1, double y1, double x2, double y2, int *x3, int *y3, int vitesse, char type){
    if(type == 'c' && calcul_distance(x1, x2, y1, y2)<vitesse*vitesse){
        *x3 = x1;
        *y3 = y1;
        return;
    }
    double m;
    if(x2<x1){
        m = (y1-y2)/(x1-x2);
    }else{
        m = (y2-y1)/(x2-x1);
    }
    bool inverse = false;
    if(atan(abs(m)) > M_PI/4){
        inverse = true;
        swap_double(&x1, &y1);
        swap_double(&x2, &y2);
        swap_int(&map_width, &map_height);
        if(x2<x1){
            m = (y1-y2)/(x1-x2);
        }else{
            m = (y2-y1)/(x2-x1);
        }
    }

    double b = y1-m*x1;
    int bx1, bx2, by1, by2;
    bx1 = 0;
    by1 = 0;
    bx2 = map_width;
    by2 = map_height;

    find_border_point(m, b, &bx1, &by1, &bx2, &by2);
    int d1 = sqrt(calcul_distance(x1, x2, y1, y2));
    int D;

    if(x1>x2){

        if(d1 > sqrt(calcul_distance(x1, bx2, y1, by2)) + sqrt(calcul_distance(x2, bx1, y2, by1))){
            if(type == 'c'){
                D = d1 + vitesse;
            }else{
                D = d1 - vitesse;
            }
        }else{
            if(type == 'c'){
                D = d1 - vitesse;
            }else{
                D = d1 + vitesse;
            }
        }
        *x3 = x1 - (D/(sqrt(m*m+1)));

    }else{
        if(d1 > sqrt(calcul_distance(x1, bx1, y1, by1)) + sqrt(calcul_distance(x2, bx2, y2, by2))){
            if(type == 'c'){
                D = d1 + vitesse;
            }else{
                D = d1 - vitesse;
            }
        }else{
            if(type == 'c'){
                D = d1 - vitesse;
            }else{
                D = d1 + vitesse;
            }
        }
        *x3 = x1 + (D/(sqrt(m*m+1)));
    }
    if(d1 <= vitesse && type == 'c') *x3 = x1;

    if(inverse){
        *y3 = get_x(*x3);
        *x3 = get_y(m**x3 + b);
        swap_int(&map_width, &map_height);
    }else{
        *y3 = get_y(m**x3 + b);
        *x3 = get_x(*x3);
    }

}


void Jeu::find_border_point(double m, double b, int *bx1, int *by1, int *bx2, int *by2){
    if(m == 0){
        *bx1 = 0;
        *bx2 = map_width;
        *by1 = b;
        *by2 = b;
        return;
    }
    int mmw = m*map_width;
    if(0 <= b && b <= map_height){
        *bx1 = 0;
        *by1 = b;
        if(0 <= mmw + b && mmw + b <=map_height){
            *bx2 = map_width;
            *by2 = mmw + b;
        }else if(m<0){
            *bx2 = (map_height-b)/m;
            *by2 = map_height;
        }else{
            *bx2 = -b/m;
            *by2 = 0;
        }
    }
    else if(0 <= mmw + b && mmw <= map_height){
        *bx2 = map_width;
        *by2 = mmw + b;
        if(0 <= b && b <= map_height){
            *bx1 = 0;
            *by1 = b;
        }else if(m>0){
            *bx1 = (map_height-b)/m;
            *by1 = map_height;
        }else{
            *bx1 = -b/m;
            *by1 = 0;
        }
    }
    else if(m>0){
        *bx1 = (map_height-b)/m;
        *by1 = map_height;
        *bx2 = -b/m;
        *by2 = 0;
    }
    else{
        *bx1 = -b/m;
        *by1 = 0;
        *bx2 = (map_height-b)/m;
        *by2 = map_height;
    }
}

void Jeu::move_creature(int x, int y, QGraphicsEllipseItem *point, int origX, int origY){
    point->setPos(x-origX , y - origY);
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


void Jeu::print_creatures(){
    int i = 0;
    while(i<herb_tab.size() && i<carn_tab.size()){
        std::cout<<"carnivore " << i << ": " << carn_tab[i].x << " " << carn_tab[i].y << std::endl;
        std::cout<<"herbivore " << i << ": " << herb_tab[i].x << " " << herb_tab[i].y << std::endl ;
        i += 1;
    }
    if(i<herb_tab.size()){
        while(i<herb_tab.size()){
            std::cout<<"herbivore " << i << ": " << herb_tab[i].x << " " << herb_tab[i].y << std::endl << std::endl;
            i += 1;
        }
    }else if(i<carn_tab.size()){
        while(i<carn_tab.size()){
            std::cout<<"carnivore " << i << ": " << carn_tab[i].x << " " << carn_tab[i].y << std::endl << std::endl;
            i += 1;
        }
    }
    else{
        std::cout << std::endl;
    }
    for(int i = 0; i <= 3 ; i++){
        std::cout << std::endl;
    }
}

int Jeu::calcul_distance_to_00(int i, char type){
    if(type == 'h'){
        return pow(herb_tab[i].x, 2) + pow(herb_tab[i].y, 2);
    }else{
        return pow(carn_tab[i].x, 2) + pow(carn_tab[i].y, 2);
    }
}

void Jeu::swap_crea(int i, int j, char type){
    if(type == 'h'){
        herbivore_t temp = herb_tab[i];
        herb_tab[i] = herb_tab[j];
        herb_tab[j] = temp;
    }else{
        carnivore_t temp = carn_tab[i];
        carn_tab[i] = carn_tab[j];
        carn_tab[j] = temp;
    }
}

int Jeu::pivoter_herb(int start, int end){
    int i = start + 1;
    int j = end;
    int pivot = sqrt(calcul_distance_to_00(start, 'h'));
    while(i != j){
        if(calcul_distance_to_00(i, 'h') <= pivot){
            i = i + 1;
        }else{
            swap_crea(i, j, 'h');
            j = j - 1;
        }
    }
    if(calcul_distance_to_00(i, 'h') > pivot){
        i = i -1;
    }
    swap_crea(start, i, 'h');
    return i;

}

int Jeu::pivoter_carn(int start, int end){
    int i = start + 1;
    int j = end;
    int pivot = calcul_distance_to_00(start, 'c');
    while(i != j){
        if(calcul_distance_to_00(i, 'c') <= pivot){
            i = i + 1;
        }else{
            swap_crea(i, j, 'c');
            j = j - 1;
        }
    }
    if(calcul_distance_to_00(i, 'c') > pivot){
        i = i -1;
    }
    swap_crea(start, i, 'c');
    return i;

}

void Jeu::quick_sort_herb(int start, int end){
    if(start < end){
        int m = pivoter_herb(start, end);
        quick_sort_herb(start, m - 1);
        quick_sort_herb(m + 1, end);
    }
}

void Jeu::quick_sort_carn(int start, int end){
    if(start < end){
        int m = pivoter_carn(start, end);
        quick_sort_carn(start, m - 1);
        quick_sort_carn(m + 1, end);
    }
}

int Jeu::get_speed(int i1, int i2, char type){
    int val;
    if(type == 'h'){
        int vitesse = herb_tab[i1].vitesse;
        if(random(0, 1) == 0){
            vitesse = herb_tab[i2].vitesse;
        }
        val = random(vitesse - 1, vitesse + 1);
        if(val < vitesse_repos_herb) val = vitesse_repos_herb;
        else if(val > vitesse_herb_max) val = vitesse_herb_max;
    }else{
        int vitesse = carn_tab[i1].vitesse;
        if(random(0, 1) == 0){
            vitesse = carn_tab[i2].vitesse;
        }
        val = random(vitesse - 1, vitesse + 1);
        if(val == vitesse_repos_carn) val = vitesse_repos_carn;
        else if(val > vitesse_carn_max) val = vitesse_carn_max;
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

void Jeu::sort_crea(){
    if(herb_tab.size() == 0) return;
    int stack = -1;
    for(int i = herb_tab.size() - 1; i>=0; i--){
        if(herb_tab[i].repro_count <= herb_tab[i].reproductible || herb_tab[i].fuite){
            if(stack == -1){
                stack = i;
            }
        }else if(stack != -1){
            swap_crea(i, stack, 'h');
            stack = i;
        }
    }
    indice_repro_herb = stack;
    stack = -1;
    if(carn_tab.size() == 0) return;
    for(int i = carn_tab.size() - 1; i>=0; i--){
        if(!carn_tab[i].repro){
            if(stack == -1){
                stack = i;
            }
        }else if(stack != -1){
            swap_crea(i, stack, 'c');
            stack = i;
        }
    }
    indice_repro_carn = stack;
}

void Jeu::change_the_color(char color, int indice){
    if(color=='y'){
        QColor color(Qt::yellow);
        QBrush brush(color);
        carn_tab[indice].point->setBrush(brush);
    }else if(color=='o'){
        QColor color(255, 165, 0);
        QBrush brush(color);
        carn_tab[indice].point->setBrush(brush);
    }else if(color == 'r'){
        QColor color(Qt::red);
        QBrush brush(color);
        carn_tab[indice].point->setBrush(brush);
    }
}

void Jeu::delete_the_dead_plante(){
    while(delete_plante_tab.size() != 0){
        delete_element(delete_plante_tab[1], 'p', delete_plante_tab[0]);
        if(zone_tab[delete_plante_tab[0]].plante_tab.size() == 0){
            for(int i = 2; i<delete_plante_tab.size(); i = i + 2){
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
    for(int i = 0; i<herb_tab.size(); i++){
        if(herb_tab[i].id_zone == id){
            herb_tab[i].eating = false;
        }
    }
}

bool Jeu::recal_id(int indice){
    if(herb_tab[indice].buisson >= zone_tab[herb_tab[indice].zone].plante_tab.size()) herb_tab[indice].buisson = zone_tab[herb_tab[indice].zone].plante_tab.size() - 1;
    while(herb_tab[indice].buisson >= 0 && zone_tab[herb_tab[indice].zone].plante_tab[herb_tab[indice].buisson].id != herb_tab[indice].id_plante){
        herb_tab[indice].buisson -= 1;
    }
    if(herb_tab[indice].buisson == -1){
        herb_tab[indice].eating = false;
        return false;
    }
    return true;
}

int Jeu::random(int min, int max){
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> random_nbr(min, max);
    return(random_nbr(generator));
}
#endif JEU_C
