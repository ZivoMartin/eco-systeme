#include "../jeu.h"

void Jeu::get_stats(){
    float averages[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    get_averages(averages);
    fprintf(direct_data, "%d\t", x_stats);
    for(int i=0; i<8; i++){
        fprintf(direct_data, "%f\t", averages[i]);
    }
    fprintf(direct_data, "%lu\t%lu\n", herb_tab.size(), carn_tab.size());
    fflush(direct_data);
}

void Jeu::get_averages(float averages[]){
    int size_herb = herb_tab.size();
    int size_carn = carn_tab.size();
    for(int i = 0; i<size_herb; i++){
        averages[0] += herb_tab[i].vitesse;
        averages[1] += herb_tab[i].height;
        averages[2] += herb_tab[i].range;
        averages[3] += herb_tab[i].starve_death;
    }
    for(int i = 0; i<size_carn; i++){
        averages[4] += carn_tab[i].vitesse;
        averages[5] += carn_tab[i].height;
        averages[6] += carn_tab[i].range;
        averages[7] += carn_tab[i].starve_death;
    }
    averages[0] = (averages[0]/size_herb)*2.3;
    averages[4] = (averages[4]/size_carn)*2.3;
    averages[1] = (averages[1]/size_herb)/3;
    averages[5] = (averages[5]/size_carn)/3;
    averages[2] = (averages[2]/size_herb)/30;
    averages[6] = (averages[6]/size_carn)/30;
    averages[3] = (averages[3]/size_herb)/100;
    averages[7] = (averages[7]/size_carn)/100;
}

void Jeu::display_stats(){
    char const *gnu_commands_herb = "/usr/bin/gnuplot/gnu_commands_herb";
    char const *gnu_commands_carn = "/usr/bin/gnuplot/gnu_commands_carn";
    char const *gnu_commands_nb_crea = "/usr/bin/gnuplot/gnu_commands_nb_crea";

    char const *gnu_exe = "/usr/bin/gnuplot/gnuplot_exe";
    char command_herb[500];
    char command_carn[500];
    char command_nb_crea[500];
    FILE *commands_herb = fopen(gnu_commands_herb, "w");
    FILE *commands_carn = fopen(gnu_commands_carn, "w");
    FILE *commands_nb_crea = fopen(gnu_commands_nb_crea, "w");

    if(commands_herb != nullptr){
        fprintf(commands_herb, "set grid; \n");
        fprintf(commands_herb, "set xrange[0:%d]; \n", x_stats);
        fprintf(commands_herb, "set yrange[0:30]; \n");
        fprintf(commands_herb, "set key box \n");
        fprintf(commands_herb, "set multiplot \n");
        fprintf(commands_herb, "set size 1, 1 \n");
        fprintf(commands_herb, "set origin 0.0, 0.0 \n");
        fprintf(commands_herb, "plot 'data.txt' u 1:2 title 'speed' w l lt rgb 'red' lw 1 axes x1y1, ");
        fprintf(commands_herb, "'data.txt' u 1:3 title 'height' w l lt rgb 'green' lw 1 axes x1y1, ");
        fprintf(commands_herb, "'data.txt' u 1:4 title 'range' w l lt rgb 'blue' lw 1 axes x1y1, ");
        fprintf(commands_herb, "'data.txt' u 1:5 title 'starve' w l lt rgb 'black' lw 1 axes x1y1; \n");
        fprintf(commands_herb, "unset multiplot; \n");
        fclose(commands_herb);
        sprintf(command_herb, "%s -persist %s", gnu_exe, gnu_commands_herb);
        system(command_herb);
    }else{
        std::cout << "le fichier herbivore n'a pas pu s'ouvrir" << std::endl;
    }
    if(commands_carn != nullptr){
        fprintf(commands_carn, "set grid; \n");
        fprintf(commands_carn, "set xrange[0:%d]; \n", x_stats);
        fprintf(commands_carn, "set yrange[0:30]; \n");
        fprintf(commands_carn, "set key box \n");
        fprintf(commands_carn, "set multiplot \n");
        fprintf(commands_carn, "set size 1, 1 \n");
        fprintf(commands_carn, "set origin 0.0, 0.0 \n");
        fprintf(commands_carn, "plot 'data.txt' u 1:6 title 'speed' w l lt rgb 'red' lw 1 axes x1y1, ");
        fprintf(commands_carn, "'data.txt' u 1:7 title 'height' w l lt rgb 'green' lw 1 axes x1y1, ");
        fprintf(commands_carn, "'data.txt' u 1:8 title 'range' w l lt rgb 'blue' lw 1 axes x1y1, ");
        fprintf(commands_carn, "'data.txt' u 1:9 title 'starve' w l lt rgb 'black' lw 1 axes x1y1; \n");
        fprintf(commands_carn, "unset multiplot; \n");
        fclose(commands_carn);
        sprintf(command_carn, "%s -persist %s", gnu_exe, gnu_commands_carn);
        system(command_carn);
    }else{
        std::cout << "le fichier carnivore n'a pas pu s'ouvrir" << std::endl;
    }
    if(commands_nb_crea != nullptr){
        fprintf(commands_nb_crea, "set grid; \n");
        fprintf(commands_nb_crea, "set xrange[0:%d]; \n", x_stats);
        fprintf(commands_nb_crea, "set yrange[0:1000]; \n");
        fprintf(commands_nb_crea, "set key box \n");
        fprintf(commands_nb_crea, "set multiplot \n");
        fprintf(commands_nb_crea, "set size 1, 1 \n");
        fprintf(commands_nb_crea, "set origin 0.0, 0.0 \n");
        fprintf(commands_nb_crea, "plot 'data.txt' u 1:11 title 'carnivores' w l lt rgb 'red' lw 1 axes x1y1, ");
        fprintf(commands_nb_crea, "'data.txt' u 1:10 title 'herbivores' w l lt rgb 'green' lw 1 axes x1y1; \n");
        fprintf(commands_nb_crea, "unset multiplot; \n");
        fclose(commands_nb_crea);
        sprintf(command_nb_crea, "%s -persist %s", gnu_exe, gnu_commands_nb_crea);
        system(command_nb_crea);
    }else{
        std::cout << "le fichier nb_crea n'a pas pu s'ouvrir" << std::endl;
    }
}