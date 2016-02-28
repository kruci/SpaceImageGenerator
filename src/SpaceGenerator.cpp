#include "SpaceGenerator.h"

SpaceGenerator::SpaceGenerator(int width, int height, int smallstars, int bigstars, int nebullas,
                               std::vector<ALLEGRO_COLOR> &nebcolors)
    : w(width), h(height), ss(smallstars), bs(bigstars), neb(nebullas), nebc(nebcolors), rndgenerator(clock()),
    rnddistribution(1,70000)
{
    orig_flags = al_get_new_bitmap_flags();
    al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    bmp = al_create_bitmap(w,h);
    al_set_new_bitmap_flags(orig_flags);

    isfree = new bool*[w];
    for (int i = 0; i < w; ++i)
    {
        isfree[i] = new bool[h];
    }
}

SpaceGenerator::~SpaceGenerator()
{
    if(bmp != nullptr)
        al_destroy_bitmap(bmp);
}

void SpaceGenerator::someshit(int x, int y, long size_pixels, ALLEGRO_COLOR col)
{
    for (int a = 0; a < w; a++)
    {
            for (int b = 0; b < h; b++)
            {
                isfree[a][b] = true;
            }
    }

    while(size_pixels > 0)
    {
        x += -1 + rand() %3;
        y += -1 + rand() %3;
        if(x < 0)
        {
            x = 0;
        }
        else if(x >= w)
        {
            x = w-1;
        }

        if(y < 0)
        {
            y = 0;
        }
        else if(y >= h)
        {
            y = h-1;
        }

        if(isfree[x][y] == true)
        {
            al_put_pixel(x, y, col);
            isfree[x][y] = false;
            size_pixels--;
        }
    }
}

ALLEGRO_BITMAP* SpaceGenerator::Generate()
{
    refbmp = al_get_target_bitmap();
    al_lock_bitmap(bmp,  al_get_bitmap_format(bmp), ALLEGRO_LOCK_WRITEONLY);
    al_set_target_bitmap(bmp);

    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_filled_rectangle(0,0,w,h, al_map_rgb(0,0,0));

    std::uniform_real_distribution<float> a_distribution(0, 0.05);
    std::uniform_int_distribution<int> w_distribution(0, w);
    std::uniform_int_distribution<int> h_distribution(0, h);

    if(Progresscallback != nullptr)
    {
        Progresscallback(0);
    }
    //stars
    for(int c = 0; c < rand() % 10; c++)
    {
        signed int x = w_distribution(rndgenerator), y = h_distribution(rndgenerator);

        someshit(x, y, rand() % (w*h), al_map_rgb(rand()%20, 0,rand()%20));
        //someshit(x, y, rand() % (w*h), al_map_rgba(rand()%256,rand()%256,rand()%256, 1));
        //someshit(x, y, rand() % (w*h), al_premul_rgba(rand()%256,rand()%256,rand()%256, rand()%10));
    }

    int r = 0;
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
                r = 1+ rand() % 2;
                al_draw_filled_ellipse(a,b, 1 + rand() %2, 1 + rand() %2,clWhite);
                al_draw_filled_circle(a -1 + rand() % 3, b -1 + rand() % 3, r - rand()%2, al_map_rgb(255,255,255));
                if(rand() % 2 == 0)
                {
                    r = rand() % 4;
                    al_draw_filled_circle(a, b, r, al_premul_rgba(255,255,150+rand()%10, 230 +rand()%15));
                    al_draw_filled_circle(a -1 + rand() % 3, b -1 + rand() % 3, r- rand()%4, al_premul_rgba(255,255,50+rand()%200, 230 +rand()%15));
                }
            }
        }
    }

    //nebs


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
            Progresscallback( (float)(c+1)* 100.0f/(float)(nebc.size()-1));
        }
    }

    al_set_target_bitmap(refbmp);
    al_unlock_bitmap(bmp);
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
