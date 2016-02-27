#ifndef SPACEGENERATOR_H
#define SPACEGENERATOR_H

#include "rGUI.h"
#include <ctime>
#include <cmath>
#include <random>

class SpaceGenerator
{
private:
    ALLEGRO_BITMAP *refbmp;
    int orig_flags;
    clock_t t;
    std::mt19937 rndgenerator;
    std::uniform_int_distribution<int> rnddistribution;
    std::vector<bool> is_free;

    ALLEGRO_COLOR clWhite = al_map_rgb(255,255,255);
public:
    int w, h;
    int ss, bs, neb;
    std::vector<ALLEGRO_COLOR> nebc;
    ALLEGRO_BITMAP *bmp = nullptr;

    void (*Progresscallback)(int a) = nullptr;

    SpaceGenerator(int width, int height, int smallstars, int bigstars, int nebullas, std::vector<ALLEGRO_COLOR> &nebcolors);
    virtual ~SpaceGenerator();

    ALLEGRO_BITMAP* Generate();
    bool Proc(int &chance);

};

#endif // SPACEGENERATOR_H
