#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <iostream>

int main(int argc, char *argv[]) {
    // 1. Inicializar Allegro
    if (!al_init()) return -1;
    
    // 2. Inicializar el módulo táctil y de ratón
    al_install_touch_input();
    al_install_mouse();
    al_init_primitives_addon();

    // 3. Crear display (ventana)
    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) return -1;

    // 4. Configurar eventos
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_touch_input_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());

    bool running = true;
    float naveX = 400.0f, naveY = 500.0f; // Posición inicial de la nave
    float destinoX = naveX;

    // 5. Bucle principal del juego
    while (running) {
        ALLEGRO_EVENT event;
        while (al_get_next_event(queue, &event)) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }
            // Detectar toque en pantalla o clic de ratón
            if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN || event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                destinoX = event.touch.x; // Actualiza el destino hacia donde se tocó
            }
        }

        // Mover la nave suavemente hacia el destino
        naveX += (destinoX - naveX) * 0.1f;

        // 6. Renderizar gráficos
        al_clear_to_color(al_map_rgb(0, 0, 0));
        
        // Dibujar nave (un triángulo básico)
        al_draw_filled_triangle(naveX, naveY - 20, naveX - 20, naveY + 20, naveX + 20, naveY + 20, al_map_rgb(0, 255, 0));

        al_flip_display();
        al_rest(0.01); // Pausa breve para controlar los FPS
    }

    // 7. Destruir recursos
    al_destroy_display(display);
    al_destroy_event_queue(queue);
    return 0;
}