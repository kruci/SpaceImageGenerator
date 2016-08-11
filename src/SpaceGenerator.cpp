#include "SpaceGenerator.h"

SpaceGenerator::SpaceGenerator(int width, int height, int smallstars, int bigstars, int nebullas,
                               std::vector<ALLEGRO_COLOR> &nebcolors)
    : w(width), h(height), ss(smallstars), bs(bigstars), neb(nebullas), nebc(nebcolors), rndgenerator(clock()),
    rnddistribution(1,70000)
{
    //orig_flags = al_get_new_bitmap_flags();
    //al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP);
    bmp = al_create_bitmap(w,h);
    //al_set_new_bitmap_flags(orig_flags);

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

    for (int i = 0; i < w; ++i)
    {
        delete[] isfree[i];
    }
    delete[] isfree;
}

void SpaceGenerator::someshit(int x, int y, long size_pixels, ALLEGRO_COLOR col)
{
    for (int a = w-1; a>= 0; --a)
    {
            for (int b = h-1; b>= 0; --b)
            {
                isfree[a][b] = true;
            }
    }

    //size_pixels = size_pixels/2;

    while(size_pixels > 0)
    {
        /*if(rand() % size_pixels == 0)
        {
            col = al_premul_rgba(rand() %256,rand() %256,rand() %256,rand() %30);
        }*/

        x += -1 + rand() %3;
        y += -1 + rand() %3;
        /*x += -2 + rand() %5;
        y += -2 + rand() %5;*/
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
            //al_draw_filled_circle(x,y,3, col);
            isfree[x][y] = false;
            --size_pixels;
        }
    }
}

ALLEGRO_BITMAP* SpaceGenerator::Generate()
{
    time_t st = clock();
    refbmp = al_get_target_bitmap();
    al_lock_bitmap(bmp,  al_get_bitmap_format(bmp), ALLEGRO_LOCK_WRITEONLY);
    al_set_target_bitmap(bmp);

    al_clear_to_color(al_map_rgb(255,255,255));
    al_draw_filled_rectangle(0,0,w,h, al_map_rgb(0,0,0));

    std::uniform_real_distribution<float> a_distribution(0, 0.05);//0, 0.05
    std::uniform_int_distribution<int> w_distribution(0, w);
    std::uniform_int_distribution<int> h_distribution(0, h);

    if(Progresscallback != nullptr)
    {
        Progresscallback(0);
    }
    //stars
    int trash = rand() % max_bckground_it + min_bckground_it;
    for(int c = 0; c < trash; ++c)
    {
        signed int x = w_distribution(rndgenerator), y = h_distribution(rndgenerator);

        //someshit(x, y, rand() % (w*h), al_map_rgb(rand()%10, rand()%10,1+rand()%20));
        someshit(x, y, rand() % (w*h), al_map_rgb(rand()%12, rand()%12,1+rand()%26));
        Progresscallback(c +1);
        //someshit(x, y, rand() % (w*h), al_premul_rgba(1+rand()%255, 1+rand()%255,1+rand()%255, 1+rand()%25));
    }

    Stars *strs = new Stars(w,h);
    int b1,b2,b3;
    al_get_blender(&b1,&b2,&b3);

    //int r = 0;
    for(int a = 0;a < w;++a)
    {
        for(int b = 0;b < h;++b)
        {
            if(Proc(ss) == true)
            {
                al_put_pixel(a, b, clWhite);
            }
            if(Proc(bs) == true) //255,255,204
            {
                strs->sv.push_back(new Stars::star(a, b, (float)(rand()%1000)/1000.0 +0.40, al_map_rgb(rand()%200 +55,rand()%200 +55,rand()%200 +55)));
            }
        }
    }
    al_draw_bitmap(strs->Render(), 0,0,0);
    al_set_blender(b1,b2,b3);
    delete strs;
    //trash = rand() % 3;
    /*for(int c = 0; c < 1; c++)
    {
        signed int x = w_distribution(rndgenerator), y = h_distribution(rndgenerator);

        //someshit(x, y, rand() % (w*h), al_map_rgb(rand()%10, rand()%10,1+rand()%20));
        //someshit(x, y, rand() % (w*h), al_premul_rgba(1+rand()%10, 1+rand()%10,1+rand()%250, 1+rand()%60));
        someshit(x, y, (rand() % ( (w*h)/100)), al_premul_rgba(255,255,255, 1+rand()%120));
    }*/

    //nebs
    for(int c = 0; c < (int)nebc.size(); ++c)
    {
        signed int x = w_distribution(rndgenerator), y = h_distribution(rndgenerator);
        //signed int xx, yy;
        for(int t = 0;t < (float)(w*h)*((float)neb/100.0f); ++t)
        {
            /*xx = -1 + rand()%3;
            yy = -1 + rand()%3;*/
            //al_draw_filled_circle(x, y, 1,al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));
            al_put_blended_pixel(x, y, al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));

            al_put_blended_pixel(x-1, y, al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));
            al_put_blended_pixel(x+1, y, al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));
            al_put_blended_pixel(x, y-1, al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));
            al_put_blended_pixel(x, y+1, al_premul_rgba_f(nebc[c].r, nebc[c].g, nebc[c].b, a_distribution(rndgenerator)));
            /*x += xx;
            y += yy;*/
            x += -1 + rand()%3;
            y += -1 + rand()%3;;
        }
        if(Progresscallback != nullptr)
        {
            Progresscallback( (float)(c+1)* 100.0f/(float)(nebc.size()-1));
        }
    }

    al_set_target_bitmap(refbmp);
    al_unlock_bitmap(bmp);

    std::cout << ((float)clock() - (float)st)/(float)CLOCKS_PER_SEC << "s" << std::endl;

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

void SpaceGenerator::blank(int a)
{

}
