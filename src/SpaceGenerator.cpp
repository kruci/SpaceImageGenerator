#include "SpaceGenerator.h"

SpaceGenerator::SpaceGenerator(int width, int height, int smallstars, int bigstars, int nebullas,
                               std::vector<ALLEGRO_COLOR> &nebcolors)
    : w(width), h(height), ss(smallstars), bs(bigstars), neb(nebullas), nebc(nebcolors), rndgenerator(clock()),
    rnddistribution(1,(width*height)/40), is_free(width*height, true)
{
    orig_flags = al_get_new_bitmap_flags();
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    bmp = al_create_bitmap(w,h);
    al_set_new_bitmap_flags(orig_flags);
}

SpaceGenerator::~SpaceGenerator()
{
    if(bmp != nullptr)
        al_destroy_bitmap(bmp);
}

ALLEGRO_BITMAP* SpaceGenerator::Generate()
{
    refbmp = al_get_target_bitmap();
    al_set_target_bitmap(bmp);

    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_filled_rectangle(0,0,w,h, al_map_rgb(0,0,0));

    if(Progresscallback != nullptr)
    {
        Progresscallback(0);
    }
    //stars
    for(int a = 0;a < w;a++)
    {
        for(int b = 0;b < h;b++)
        {
            if(Proc(ss) == true)
            {
                al_put_pixel(a, b, clWhite);
            }
            if(Proc(bs) == true) //255,255,204
            {
                al_draw_filled_circle(a, b, rand() % 2, clWhite);
                if(rand() % 2 == 0)
                {
                    al_draw_filled_circle(a, b, rand() % 4, al_premul_rgba(255,255,204, 240 +rand()%15));
                }
            }
        }
    }

    //nebs
    std::uniform_real_distribution<float> a_distribution(0, 0.02);
    std::uniform_int_distribution<int> w_distribution(0, w);
    std::uniform_int_distribution<int> h_distribution(0, h);

    for(int c = 0; c < nebc.size(); c++)
    {
        signed int x = w_distribution(rndgenerator), y = h_distribution(rndgenerator);
        signed int xx, yy;
        for(int t = 0;t < (float)(w*h)*((float)neb/100.0f);t++)
        {
            xx = -1 + rand()%3;
            yy = -1 + rand()%3;
            al_draw_filled_circle(x, y, 1,al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));

            x += xx;
            y += yy;
        }
        if(Progresscallback != nullptr)
        {
            Progresscallback( (c+1)* 100/(nebc.size()-1));
        }
    }

    al_set_target_bitmap(refbmp);
    return bmp;
}

bool SpaceGenerator::Proc(int &chance_from0to100)
{
    int chany = rnddistribution(rndgenerator);
    if(chany <= chance_from0to100)
    {
        return true;
    }
    return false;
}
