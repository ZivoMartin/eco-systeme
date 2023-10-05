#include "../jeu.h"

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


