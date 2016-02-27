#include "rGUI.h"
#include <ctime>
#include <cmath>
#include <random>
#include "include/SpaceGenerator.h"

#define TIME_B clock_t cccccc = clock();
#define TIME_E std::cout << (clock() - cccccc) << std::endl;

const float FPS = 60;
const int SCREEN_W = 900;
const int SCREEN_H = 600;

std::vector<rGUI::Widget*>widgets;
std::vector<ALLEGRO_COLOR> nebscbacols;
int nebullacolor_scba;
rGUI::ProgressBar *pbar = nullptr;

inline int error_message(std::string error_string)
{
    return al_show_native_message_box(al_get_current_display(), "ERROR", "", error_string.c_str(), nullptr, ALLEGRO_MESSAGEBOX_ERROR);
}

void Nebulacollorscba_Button()
{

}

void Pbar_callback(int val)
{
    pbar->value = val;
    ALLEGRO_BITMAP *refb = al_get_target_bitmap();
    al_set_target_backbuffer(al_get_current_display());
    pbar->Print();
    al_set_target_bitmap(refb);
    al_flip_display();
}

int main(int argc, char **argv)
{

    if(!al_init())
    {
        error_message("al_init()");
    }
    if(!al_init_primitives_addon())
    {
        error_message("al_init_primitives_addon()");
        return false;
    }
    if(!al_install_keyboard())
    {
        error_message("al_install_keyboard()");
        return false;
    }
    if(!al_install_mouse())
    {
        error_message("al_install_mouse()");
        return false;
    }
    if(!al_init_image_addon())
    {
        error_message("al_init_image_addon()");
        return false;
    }
    if(!al_install_audio())
    {
        error_message("al_install_audio()");
        return false;
    }
    if(!al_init_acodec_addon())
    {
        error_message("al_init_acodec_addon()");
        return false;
    }
    al_init_font_addon(); // returns void
    if(!al_init_ttf_addon())
    {
        error_message("al_init_ttf_addon()");
        return false;
    }

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        fprintf(stderr, "failed to create timer!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "failed to create display!\n");
        al_destroy_timer(timer);
        return -1;
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        fprintf(stderr, "failed to create event_queue!\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());


    al_clear_to_color(al_map_rgb(0,0,0));

    al_flip_display();

    al_start_timer(timer);

    rGUI::mouse_state = new ALLEGRO_MOUSE_STATE;
    rGUI::keyboard_state = new ALLEGRO_KEYBOARD_STATE;

    rGUI::Theme tmh;
    tmh.c_text = al_map_rgb(255,255,255);
    tmh.c_outline = al_map_rgb(150,150,150);
    tmh.c_background = al_map_rgb(200,200,200);
    tmh.c_clicking = al_map_rgba(0,0,0,125);
    tmh.added_thickness = 1;
    tmh.roundx = 0;
    tmh.roundy = 0;

    int generate_button_poz = 0;
    widgets.push_back(new rGUI::Button(SCREEN_W - 110, SCREEN_H - 100, 100, 40, "Generate", "Calibri.ttf", &tmh));
    widgets.push_back(new rGUI::Button(SCREEN_W - 110, SCREEN_H - 50, 100, 40, "Save", "Calibri.ttf", &tmh));
    ALLEGRO_FILECHOOSER *savediag = al_create_native_file_dialog("",
                        "Save that beautiful masterpiece", "*.*;*.bmp;*.png;*.jpg", ALLEGRO_FILECHOOSER_SAVE);

    ///Small stars
    widgets.push_back(new rGUI::Label(SCREEN_W - 270, 10, 265, 40, "Small stars", "Calibri.ttf", 0, &tmh, false));
    int sspoz =  widgets.size();
    widgets.push_back(new rGUI::SlideBar(SCREEN_W - 115, 15, 100, 30, 0, 100, &tmh, false));

    ///Big stars
    widgets.push_back(new rGUI::Label(SCREEN_W - 270, 60, 265, 40, "Big stars", "Calibri.ttf", 0, &tmh, false));
    int bspoz = widgets.size();
    widgets.push_back(new rGUI::SlideBar(SCREEN_W - 115, 65, 100, 30, 0, 100, &tmh, false));

    ///Nebulas
    widgets.push_back(new rGUI::Label(SCREEN_W - 270, 110, 265, 40, "Dust", "Calibri.ttf", 0, &tmh, false));
    int nbpoz = widgets.size();
    widgets.push_back(new rGUI::SlideBar(SCREEN_W - 115, 115, 100, 30, 0, 100, &tmh, false));

    ///Nebulas Colors
    std::mt19937 generator (clock());
    std::uniform_int_distribution<int> distribution(0,255);
    int slidebar_R = widgets.size() + 1;
    widgets.push_back(new rGUI::Label(SCREEN_W - 270, 170, 265, 210,
                    "Nebulas colours             R            G            B", "Calibri.ttf", 30 ,0, &tmh, false, true));
    /*R*/widgets.push_back(new rGUI::SlideBar(SCREEN_W - 250, 230, 30, 100, 0, 255, &tmh, true, false));
         widgets[widgets.size()-1]->wd_c_background = al_map_rgb(255,0,0);
         widgets[widgets.size()-1]->wd_c_outline = al_map_rgb(100,0,0);
         ((rGUI::SlideBar*)widgets[widgets.size()-1])->Set_value(distribution(generator));
    /*G*/widgets.push_back(new rGUI::SlideBar(SCREEN_W - 150, 230, 30, 100, 0, 255, &tmh, true, false));
         widgets[widgets.size()-1]->wd_c_background = al_map_rgb(0,255,0);
         widgets[widgets.size()-1]->wd_c_outline = al_map_rgb(0,100,0);
         ((rGUI::SlideBar*)widgets[widgets.size()-1])->Set_value(distribution(generator));
    /*B*/widgets.push_back(new rGUI::SlideBar(SCREEN_W - 48, 230, 30, 100, 0, 255, &tmh, true, false));
         widgets[widgets.size()-1]->wd_c_background = al_map_rgb(0,0,255);
         widgets[widgets.size()-1]->wd_c_outline = al_map_rgb(0,0,100);
         ((rGUI::SlideBar*)widgets[widgets.size()-1])->Set_value(distribution(generator));
    widgets.push_back(new rGUI::Button(SCREEN_W - 260, 335, 245, 40, "Add", "Calibri.ttf", &tmh));

    /// W & H
    int width_inpf_poz = widgets.size()+1;
    rGUI::Theme thmwh = tmh;
    thmwh.roundx = 3;
    thmwh.roundy = 3;
    thmwh.c_background = al_map_rgb(255,255,255);
    thmwh.c_text = al_map_rgb(0,0,0);
    /*w*/widgets.push_back(new rGUI::Label(SCREEN_W - 120, 390, 115, 50, "Width", "Calibri.ttf",14, 1, &tmh, false, true));
    widgets.push_back(new rGUI::InputField(SCREEN_W - 110, 407, 100, 30, "Calibri.ttf", &thmwh, FPS, false));
    ((rGUI::InputField*)widgets[widgets.size()-1])->Set_text("1600");
    /*H*/widgets.push_back(new rGUI::Label(SCREEN_W - 120, 445, 115, 50, "Height", "Calibri.ttf",14, 1, &tmh, false, true));
     int hight_inpf_poz = widgets.size();
     widgets.push_back(new rGUI::InputField(SCREEN_W - 110, 462, 100, 30, "Calibri.ttf", &thmwh, FPS, false));
    ((rGUI::InputField*)widgets[widgets.size()-1])->Set_text("900");

    ///Nebulas Colors scba
    nebullacolor_scba = widgets.size();
    widgets.push_back(new rGUI::ScrollableArea(SCREEN_W - 270, 390, 130, 185, 130, 185, &tmh, 15));

    ///Bitmap scba
    int bitmapscbapoz = widgets.size();
    ALLEGRO_BITMAP *space = nullptr;
    SpaceGenerator *spg = nullptr;
    widgets.push_back(new rGUI::ScrollableArea(10,10, SCREEN_W - 300, SCREEN_H - 35, 1000, 1000, &tmh, 15));

    ///Progressbar
    thmwh.roundx = 3;
    thmwh.roundy = 3;
    thmwh.c_outline = al_map_rgb(225,225,225);
    thmwh.c_background = al_map_rgb(255,255,255);
    thmwh.c_text = al_map_rgb(0,50,200);
    pbar = new rGUI::ProgressBar(20, (SCREEN_H-40)/2.0f, 580, 40, 0, &thmwh, false);


    float scale = 1.0f;
    while(1)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        al_get_mouse_state(rGUI::mouse_state);
        al_get_keyboard_state(rGUI::keyboard_state);

        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }

        float aaa = 1;

        for(int a = 0;a < (int)widgets.size();a++)
        {
            widgets[a]->Input(ev, aaa, aaa);
        }
        if(widgets[generate_button_poz]->wd_md->md_just_clicked == true)
        {
            nebscbacols.clear();
            for(int a = 1;a < ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.size();a+=2)
            {
                nebscbacols.push_back( ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->
                widgets[a-1]->wd_c_background);
            }
            if(spg != nullptr)
            {
                delete spg;
                spg = nullptr;
            }
            pbar->value = 0;
            spg = new SpaceGenerator(stoi(((rGUI::InputField*)widgets[width_inpf_poz])->Get_text()),
                                                     stoi(((rGUI::InputField*)widgets[hight_inpf_poz])->Get_text()),
                                                     ((rGUI::SlideBar*)widgets[sspoz])->value,
                                                     ((rGUI::SlideBar*)widgets[bspoz])->value,
                                                     ((rGUI::SlideBar*)widgets[nbpoz])->value,
                                                     nebscbacols);
            spg->Progresscallback = Pbar_callback;
            space = spg->Generate();

            if(((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets.size() > 0)
            {
                for(int h = 0;h < ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets.size();h++)
                {
                    delete ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets[h];
                }
                ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets.clear();
            }

            ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets.push_back(new rGUI::BitmapButton(0,0, space, &tmh, false));
            ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets[((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->widgets.size()-1]->
                wd_md->md_active = false;
            ((rGUI::ScrollableArea*)widgets[bitmapscbapoz])->I_added_new_widgets();
        }
        else if(widgets[slidebar_R+3]->wd_md->md_just_clicked == true)
        {
            int njk = ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.size();
            ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.push_back(new rGUI::Widget(5 ,
                                                                        10 + 30*floor(njk/2),55,20,&tmh,false));
            ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->
                widgets[((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.size()-1]->wd_c_background =
                widgets[slidebar_R+3]->wd_c_background;

            ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.push_back(new rGUI::Button(70,
                                                        10 + 30*floor(njk/2),50,20, "Remove","Calibri.ttf",&tmh));

            ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->I_added_new_widgets();
        }
        else if(widgets[generate_button_poz+1]->wd_md->md_just_clicked == true)
        {
            al_show_native_file_dialog(display, savediag);
            if(al_get_native_file_dialog_count(savediag) != 0 && space != nullptr)
            {
                std::string savepath = al_get_native_file_dialog_path(savediag, 0);
                al_save_bitmap(savepath.c_str(), space);
            }

        }
        for(int a = 1;a < ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.size();)
        {
            if(((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[a]->wd_md->md_just_clicked == true)
            {
                delete ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[a-1];
                delete ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[a];
                ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.erase(
                    ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.begin()+a-1,
                    ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.begin()+a+1);

                for(int m = a ; m < ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets.size();m+=2)
                {
                    ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[m-1]->orig_y1 =
                        ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[m]->orig_y1 = 10 + 30*floor(m/2);
                    ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[m-1]->orig_y2 =
                        ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->widgets[m]->orig_y2 = 10 + 30*floor(m/2) + 20;
                }
                ((rGUI::ScrollableArea*)widgets[nebullacolor_scba])->I_added_new_widgets();
            }
            else
            {
                a +=2;
            }
        }


        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            al_clear_to_color(al_map_rgb(255,255,255));

            widgets[slidebar_R]->wd_c_background = al_map_rgb( ((rGUI::SlideBar*)widgets[slidebar_R])->value,0,0);
            widgets[slidebar_R+1]->wd_c_background = al_map_rgb( 0,((rGUI::SlideBar*)widgets[slidebar_R+1])->value,0);
            widgets[slidebar_R+2]->wd_c_background = al_map_rgb( 0,0,((rGUI::SlideBar*)widgets[slidebar_R+2])->value);
            widgets[slidebar_R+3]->wd_c_background = al_map_rgb( ((rGUI::SlideBar*)widgets[slidebar_R])->value,
                                                                 ((rGUI::SlideBar*)widgets[slidebar_R+1])->value,
                                                                 ((rGUI::SlideBar*)widgets[slidebar_R+2])->value);

            for(int a = 0;a < (int)widgets.size();a++)
            {
                widgets[a]->Print();
            }

            al_flip_display();
        }
    }

    for(int a = 0;a < (int)widgets.size();a++)
    {
        delete widgets[a];
    }
    widgets.clear();

    delete rGUI::keyboard_state;
    delete rGUI::mouse_state;

    /*if(space != nullptr)
        al_destroy_bitmap(space);*/
    if(spg != nullptr)
        delete spg;
    if(pbar != nullptr)
        delete pbar;

    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
