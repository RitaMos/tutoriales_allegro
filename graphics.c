#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// https://github.com/liballeg/allegro_wiki/wiki/Allegro-Vivace
// gcc graphics.c -o tuto $(pkg-config allegro-5 allegro_font-5  allegro_image-5 allegro_primitives-5  --libs --cflags)


void must_init(bool test, const char *description)
{
	// checks that each step executes successfully. If it doesn't, the failure is printed to the terminal and the program exits immediately
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

int main()
{
// *************** INICIALIZACION LIBRERIA + ENTRADA ***************
    must_init(al_init(), "allegro");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_primitives_addon(), "primitives");
	
// ******************* PUNTEROS A FUNCIONES *******************
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "queue");

	// Mejorar resolucion
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
	// Fin de mejorar resolucion
    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    must_init(disp, "display");

    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font");

    must_init(al_init_image_addon(), "image addon");
    ALLEGRO_BITMAP* mysha = al_load_bitmap("mysha.png");
    must_init(mysha, "mysha");
    
    
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
        	/* al_map_rgba_f va de 0 a 1 y permite controlar opacidad con el ultimo argumento
			al_map_rgba va de 0 a 255 y no permite controlar opacidad */
			
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world!");
			
			//al_draw_bitmap(mysha, 100, 100, 3); alternativa, no permite ponerle un color "arriba"
			al_draw_tinted_bitmap(mysha, al_map_rgba_f(1, 0, 0, 1), 100, 100, 0);
			
			al_draw_filled_triangle(35, 350, 85, 375, 35, 400, al_map_rgb_f(0, 1, 0));
			al_draw_filled_rectangle(240, 260, 340, 340, al_map_rgba_f(0, 0, 0.5, 0.5));
			//sin filled no van a estar... filled (van a ser "contornos")
			al_draw_circle(450, 370, 30, al_map_rgb_f(1, 0, 1), 7);
			// ultimo argumento = grosor de linea
			al_draw_line(440, 110, 460, 210, al_map_rgb_f(1, 0, 0), 1);
			al_draw_line(500, 220, 570, 200, al_map_rgb_f(1, 1, 0), 1);
			
			 /* dibuja un cuadrado colorido lindo pero me revienta la compu
			  ALLEGRO_VERTEX v[] = {
                { .x = 210, .y = 320, .z = 0, .color = al_map_rgb_f(1, 0, 0) },
                { .x = 330, .y = 320, .z = 0, .color = al_map_rgb_f(0, 1, 0) },
                { .x = 210, .y = 420, .z = 0, .color = al_map_rgb_f(0, 0, 1) },
                { .x = 330, .y = 420, .z = 0, .color = al_map_rgb_f(1, 1, 0) },
            };
            al_draw_prim(v, NULL, NULL, 0, 4, ALLEGRO_PRIM_TRIANGLE_STRIP); */
			
            al_flip_display();

            redraw = false;
        }
    }


// ************* LIBERO MEMORIA ALOCADA X LA LIBRERIA *************
    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
	al_destroy_bitmap(mysha);

    return 0;
}