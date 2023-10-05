#include "src/jeu.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    Jeu w(nullptr, &scene, &view);


    return a.exec();
}
