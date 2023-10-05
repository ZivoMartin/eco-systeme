TARGET = eco-systeme
CONFIG += c++11

HEADERS += src/jeu.h src/struct.h
SOURCES += src/init.cpp main.cpp src/other/print.cpp src/carn_functions/carn.cpp src/elementary_functions/move_functions.cpp src/carn_functions/move_carn.cpp
SOURCES += src/elementary_functions/get.cpp src/elementary_functions/calcul_distance.cpp src/elementary_functions/swaps.cpp src/elementary_functions/add.cpp
SOURCES += src/herb_functions/herb.cpp src/herb_functions/move_herb.cpp src/herb_functions/plante_management.cpp src/other/random.cpp  src/other/sort.cpp 
SOURCES += src/other/change_color.cpp src/carn_functions/corpse_management.cpp src/other/stats.cpp
QT += widgets
