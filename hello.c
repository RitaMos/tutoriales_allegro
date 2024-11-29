#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>

// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace
// gcc hello.c -o hello $(pkg-config allegro-5 allegro_font-5 --libs --cflags)

//WAAAAAAAAAAAAAAAAAAAAAAAAAAAA

int main()
{

// *************** INICIALIZACION LIBRERIA + ENTRADA ***************
    if(!al_init())
    {
        printf("couldn't initialize allegro\n");
        return 1;
    }
    if(!al_install_keyboard())
    {
        printf("couldn't initialize keyboard\n");
        return 1;
    }
    
// ******************* PUNTEROS A FUNCIONES *******************
    // define timer q va a disparar un evento cada 1/30 segundos
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    if(!timer)
    {
        printf("couldn't initialize timer\n");
        return 1;
    }
    //define fila de eventos (aca se van a guardar a medida q se registran)
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if(!queue)
    {
        printf("couldn't initialize queue\n");
        return 1;
    }
    //crea ventana
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    if(!disp)
    {
        printf("couldn't initialize display\n");
        return 1;
    }
    //define fuente de texto
    ALLEGRO_FONT* font = al_create_builtin_font();
    if(!font)
    {
        printf("couldn't initialize font\n");
        return 1;
    }
    
    
// ********************* CONFIGURACION INICIAL *********************
    /* Define qué eventos puede detectar. Cuando se detecte un evento del teclado, del display o del timer, se van a guardar en la fila */
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    bool done = false; //cuando done=true, va a salir del ciclo
    bool redraw = false;
    /* true --> tiene q renderear una frame
    Es lo mismo en que modo arranca */
    ALLEGRO_EVENT event; //aca se guarda d forma temporal cualquier evento q llegue

    al_start_timer(timer); //empieza a correr el timer
  
  
// **************************** ARRANCA ****************************
    while(1)
    {
        al_wait_for_event(queue, &event);//espera evento

        switch(event.type)//si llega un evento...
        {
            case ALLEGRO_EVENT_TIMER://...y es del timer, entonces arranca el juego y tiene q renderear
                // GAME LOGIC GOES HERE
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN: //si se apreto una tecla o se cerro el display, sale del loop
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)) //rendereo
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
            al_flip_display();

            redraw = false;
        }
    }


// ************* LIBERO MEMORIA ALOCADA X LA LIBRERIA *************
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);

    return 0;
}
