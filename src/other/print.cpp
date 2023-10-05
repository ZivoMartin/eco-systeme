#include "../jeu.h"

void Jeu::print_max_speed(){
    int t_vc[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int size = carn_tab.size();
    for(int i = 1; i<size; i++){
        t_vc[carn_tab[i].vitesse - 3] += 1;
    }
    for(unsigned int i = 0; i<10; i++){
        std::cout << "carn a la vitesse " << i + 3 << " : " << t_vc[i] << std::endl;
    }
    std::cout << "SIZE CARN: " << carn_tab.size() << std::endl;
    int t_vh[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    size = herb_tab.size();
    for(int i = 1; i<size; i++){
        t_vh[herb_tab[i].vitesse - 3] += 1;
    }
    for(unsigned int i = 0; i<10; i++){
        std::cout << "herb a la vitesse" << i + 3 << " : " << t_vh[i] << std::endl;
    }
    std::cout << "SIZE HERB : " << herb_tab.size()<<std::endl << std::endl;
}

void Jeu::print_creatures(){
    unsigned int i = 0;
    unsigned int size_herb = herb_tab.size();
    unsigned int size_carn = carn_tab.size();
    while(i<size_herb && i<carn_tab.size()){
        std::cout<<"carnivore " << i << ": " << carn_tab[i].x << " " << carn_tab[i].y << std::endl;
        std::cout<<"herbivore " << i << ": " << herb_tab[i].x << " " << herb_tab[i].y << std::endl ;
        i += 1;
    }
    if(i<size_herb){
        while(i<size_herb){
            std::cout<<"herbivore " << i << ": " << herb_tab[i].x << " " << herb_tab[i].y << std::endl << std::endl;
            i += 1;
        }
    }else if(i<size_carn){
        while(i<size_carn){
            std::cout<<"carnivore " << i << ": " << carn_tab[i].x << " " << carn_tab[i].y << std::endl << std::endl;
            i += 1;
        }
    }
    else{
        std::cout << std::endl;
    }
    for(unsigned int i = 0; i <= 3 ; i++){
        std::cout << std::endl;
    }
}
