#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char *argv[]) {
    // 1. Inicializar Allegro
    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro\n");
        return -1;
    }

    // 2. Inicializar periféricos y complementos
    al_init_primitives_addon();
    al_install_touch_input();
    al_install_mouse(); // Opcional, pero util si el emulador de Cxxdroid emula ratón

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        fprintf(stderr, "Error al crear la pantalla\n");
        return -1;
    }

    // 3. Crear cola de eventos
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_touch_input_event_source());

    bool running = true;
    float nave_x = SCREEN_WIDTH / 2.0f;
    float nave_y = SCREEN_HEIGHT - 100.0f;
    float velocidad = 5.0f;

    // 4. Bucle principal del juego
    while (running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        // Procesar eventos táctiles
        else if (ev.type == ALLEGRO_EVENT_TOUCH_BEGIN || ev.type == ALLEGRO_EVENT_TOUCH_MOVE) {
            // Se actualiza la posición de la nave según donde toque el usuario en la pantalla
            nave_x = ev.touch.x;
            nave_y = ev.touch.y;
        }

        // --- LÓGICA DE JUEGO (CPU y disparos) ---
        // Aquí programarías la lógica donde la CPU se mueve para esquivar o disparar

        // --- DIBUJADO EN PANTALLA ---
        al_clear_to_color(al_map_rgb(0, 0, 0));

        // Dibujar nave del jugador (triángulo)
        al_draw_filled_triangle(nave_x, nave_y - 20, 
                                nave_x - 20, nave_y + 20, 
                                nave_x + 20, nave_y + 20, 
                                al_map_rgb(0, 255, 0));

        // Dibujar nave de la CPU (triángulo invertido en la parte superior)
        al_draw_filled_triangle(SCREEN_WIDTH / 2.0f, 50, 
                                SCREEN_WIDTH / 2.0f - 20, 10, 
                                SCREEN_WIDTH / 2.0f + 20, 10, 
                                al_map_rgb(255, 0, 0));

        al_flip_display();
    }

    // Limpieza
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}