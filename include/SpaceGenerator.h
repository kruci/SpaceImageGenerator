#ifndef SPACEGENERATOR_H
#define SPACEGENERATOR_H

#include "rGUI.h"
#include "allegro5/allegro_opengl.h"
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

    ALLEGRO_COLOR clWhite = al_map_rgb(255,255,255);
    void someshit(int x, int y, long size_pixels,ALLEGRO_COLOR col);
    bool **isfree;
    void blank(int a);
public:
    int w, h;
    int ss, bs, neb;
    std::vector<ALLEGRO_COLOR> nebc;
    int max_bckground_it = 3;
    int min_bckground_it = 0;
    ALLEGRO_BITMAP *bmp = nullptr;

    void (*Progresscallback)(int a) = nullptr;

    SpaceGenerator(int width, int height, int smallstars, int bigstars, int nebullas, std::vector<ALLEGRO_COLOR> &nebcolors);
    virtual ~SpaceGenerator();

    ALLEGRO_BITMAP* Generate();
    bool Proc(int &chance);

};

class Stars
{
private:
std::string vertexshaderstring = "uniform mat4 al_projview_matrix;"
"attribute vec4 al_pos;"
"void main(void) {"
"gl_Position = al_projview_matrix*al_pos;"
"}";

std::string pixelshaderstring = "uniform vec2 lightpos;"
"uniform vec3 lightColor;"
"uniform vec3 lightAttenuation;"
"uniform float radius;"
"uniform float sh;"
"uniform sampler2D texture;"
"void main()"
"{"
"	vec2 pixel=gl_FragCoord.xy;"
"   pixel.y = sh - pixel.y;"
"	vec2 aux=lightpos-pixel;"
"	float distance=length(aux);"
"	float attenuation=1.0/(lightAttenuation.x+lightAttenuation.y*distance+lightAttenuation.z*distance*distance);"
"	vec4 color=vec4(attenuation,attenuation,attenuation,1.0)*vec4(lightColor,1.0);"
"	gl_FragColor = color;"
"}";

public:
    struct star{
        star(float _x, float _y, float _z, ALLEGRO_COLOR co) : x(_x), y(_y), z(_z), col(co){}
        float x,y,z;
        ALLEGRO_COLOR col;
    };
    float w,h;
    std::vector<star*> sv;
    ALLEGRO_BITMAP *bmp = nullptr;
    ALLEGRO_BITMAP *refbmp;
    ALLEGRO_SHADER *Shader = nullptr;

    Stars(float _w, float _h) : w(_w), h(_h)
    {
        Shader = al_create_shader(ALLEGRO_SHADER_GLSL);
        al_attach_shader_source(Shader, ALLEGRO_VERTEX_SHADER, vertexshaderstring.c_str());
        al_attach_shader_source(Shader, ALLEGRO_PIXEL_SHADER, pixelshaderstring.c_str());
        al_build_shader(Shader);
        std::cout << al_get_shader_log(Shader) << std::endl;

        bmp = al_create_bitmap(w,h);
    }

    ~Stars()
    {
        if(bmp != nullptr)
        {
            al_destroy_bitmap(bmp);
        }
        if(Shader != nullptr)
        {
            al_destroy_shader(Shader);
        }

        for(int a = sv.size()-1;a >= 0;--a)
        {
            delete sv[a];
        }
        sv.clear();
    }

    ALLEGRO_BITMAP* Render()
    {
        refbmp = al_get_target_bitmap();
        al_set_target_bitmap(bmp);

        float lightpos[2];
        float lightAttenuation[3];
        float lightColor[3];

        al_clear_to_color(al_map_rgba(0,0,0,0));
        al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE,  ALLEGRO_ONE);
        al_use_shader(Shader);
        al_set_shader_float("sh", h);
        al_set_shader_float("radius", 0);

        for(int a = 0;a < (int)sv.size();++a)
        {
            lightpos[0] = sv[a]->x;
            lightpos[1] = sv[a]->y;

            lightColor[0] = sv[a]->col.r;
            lightColor[1] = sv[a]->col.g;
            lightColor[2] = sv[a]->col.b;

            lightAttenuation[0] = 0.0;
            lightAttenuation[1] = 0.0;
            lightAttenuation[2] = sv[a]->z;

            al_set_shader_float_vector("lightpos", 2, lightpos, 1);
            al_set_shader_float_vector("lightAttenuation", 3, lightAttenuation, 1);
            al_set_shader_float_vector("lightColor", 3, lightColor, 1);
            al_draw_filled_rectangle(lightpos[0] - 100 ,lightpos[1] -100 ,lightpos[0] + 100 ,lightpos[1] + 100 , al_map_rgba(0,0,0,0));
        }
        al_set_target_bitmap(refbmp);
        return bmp;
    }
};

#endif // SPACEGENERATOR_H
