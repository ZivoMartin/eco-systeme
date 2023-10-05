#include "../jeu.h"


void Jeu::change_the_color(char color, int indice, char type){
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
    }else if(color=='c'){
        QColor color(173, 216, 230);
        QBrush brush(color);
        if(type == 'c'){
            carn_tab[indice].point->setBrush(brush);
        }else{
            herb_tab[indice].point->setBrush(brush);
        }
    }
}
