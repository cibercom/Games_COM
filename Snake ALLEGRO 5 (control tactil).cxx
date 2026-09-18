#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <time.h>

#define TAM_BLOQUE 20
#define ANCHO_PANTALLA 800
#define ALTO_PANTALLA 600
#define MAX_SERPIENTE 100

typedef struct {
    int x, y;
} Punto;

enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

int main(int argc, char **argv) {
    al_init();
    al_init_primitives_addon();
    
    // Inicializar inputs: Teclado (como respaldo) y Táctil
    al_install_keyboard();
    al_install_touch_input(); // Activa soporte touch nativo

    ALLEGRO_DISPLAY *display = al_create_display(ANCHO_PANTALLA, ALTO_PANTALLA);
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 8.0); // Velocidad del juego (8 FPS)
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    
    // Registrar el origen de eventos táctiles si está disponible
    if (al_is_touch_input_installed()) {
        al_register_event_source(queue, al_get_touch_input_event_source());
    }

    // Estado del juego
    Punto serpiente[MAX_SERPIENTE];
    int tam_serpiente = 3;
    enum Direccion dir = DERECHA;

    // Posición inicial
    for(int i = 0; i < tam_serpiente; i++) {
        serpiente[i].x = 10 - i;
        serpiente[i].y = 15;
    }

    srand(time(NULL));
    Punto comida = { rand() % (ANCHO_PANTALLA / TAM_BLOQUE), rand() % (ALTO_PANTALLA / TAM_BLOQUE) };

    bool salir = false;
    bool redibujar = true;
    al_start_timer(timer);

    while (!salir) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            salir = true;
        }
        
        // --- PROCESAMIENTO TÁCTIL (Control por Cuadrantes) ---
        else if (event.type == ALLEGRO_EVENT_TOUCH_BEGIN) {
            float tx = event.touch.x;
            float ty = event.touch.y;

            // Determinar la dirección según las diagonales de la pantalla
            if (tx > ty * ((float)ANCHO_PANTALLA / ALTO_PANTALLA)) {
                if (tx > ANCHO_PANTALLA - (ty * ((float)ANCHO_PANTALLA / ALTO_PANTALLA))) {
                    if (dir != IZQUIERDA) dir = DERECHA;
                } else {
                    if (dir != ABAJO) dir = ARRIBA;
                }
            } else {
                if (tx > ANCHO_PANTALLA - (ty * ((float)ANCHO_PANTALLA / ALTO_PANTALLA))) {
                    if (dir != ARRIBA) dir = ABAJO;
                } else {
                    if (dir != DERECHA) dir = IZQUIERDA;
                }
            }
        }
        
        // --- RESPALDO POR TECLADO ---
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:    if (dir != ABAJO) dir = ARRIBA; break;
                case ALLEGRO_KEY_DOWN:  if (dir != ARRIBA) dir = ABAJO; break;
                case ALLEGRO_KEY_LEFT:  if (dir != DERECHA) dir = IZQUIERDA; break;
                case ALLEGRO_KEY_RIGHT: if (dir != IZQUIERDA) dir = DERECHA; break;
                case ALLEGRO_KEY_ESCAPE: salir = true; break;
            }
        }

        // --- LÓGICA DEL JUEGO ---
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            redibujar = true;

            // Mover cuerpo
            for (int i = tam_serpiente - 1; i > 0; i--) {
                serpiente[i] = serpiente[i - 1];
            }

            // Mover cabeza
            if (dir == ARRIBA)    serpiente[0].y--;
            if (dir == ABAJO)   serpiente[0].y++;
            if (dir == IZQUIERDA)  serpiente[0].x--;
            if (dir == DERECHA) serpiente[0].x++;

            // Colisión con los bordes (Pantalla infinita / Envoltura)
            if (serpiente[0].x < 0) serpiente[0].x = (ANCHO_PANTALLA / TAM_BLOQUE) - 1;
            if (serpiente[0].x >= (ANCHO_PANTALLA / TAM_BLOQUE)) serpiente[0].x = 0;
            if (serpiente[0].y < 0) serpiente[0].y = (ALTO_PANTALLA / TAM_BLOQUE) - 1;
            if (serpiente[0].y >= (ALTO_PANTALLA / TAM_BLOQUE)) serpiente[0].y = 0;

            // Colisión con la comida
            if (serpiente[0].x == comida.x && serpiente[0].y == comida.y) {
                if (tam_serpiente < MAX_SERPIENTE) {
                    tam_serpiente++;
                }
                comida.x = rand() % (ANCHO_PANTALLA / TAM_BLOQUE);
                comida.y = rand() % (ALTO_PANTALLA / TAM_BLOQUE);
            }

            // Autocolisión (Game Over básico: reinicia tamaño)
            for (int i = 1; i < tam_serpiente; i++) {
                if (serpiente[0].x == serpiente[i].x && serpiente[0].y == serpiente[i].y) {
                    tam_serpiente = 3; 
                }
            }
        }

        // --- RENDERIZADO ---
        if (redibujar && al_is_event_queue_empty(queue)) {
            al_clear_to_color(al_map_rgb(20, 20, 20));

            // Dibujar comida (Rojo)
            al_draw_filled_rectangle(comida.x * TAM_BLOQUE, comida.y * TAM_BLOQUE,
                                     (comida.x + 1) * TAM_BLOQUE, (comida.y + 1) * TAM_BLOQUE,
                                     al_map_rgb(255, 50, 50));

            // Dibujar serpiente (Verde)
            for (int i = 0; i < tam_serpiente; i++) {
                al_draw_filled_rectangle(serpiente[i].x * TAM_BLOQUE, serpiente[i].y * TAM_BLOQUE,
                                         (serpiente[i].x + 1) * TAM_BLOQUE, (serpiente[i].y + 1) * TAM_BLOQUE,
                                         al_map_rgb(50, 255, 50));
            }

            al_flip_display();
            redibujar = false;
        }
    }

    // Limpieza
    al_destroy_event_queue(queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    return 0;
}