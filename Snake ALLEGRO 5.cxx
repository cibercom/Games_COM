#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

// Parámetros de la pantalla y cuadrícula
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define GRID_SIZE     20
#define MAX_SNAKE     ((SCREEN_WIDTH / GRID_SIZE) * (SCREEN_HEIGHT / GRID_SIZE))

// Direcciones posibles
enum Direction { UP, DOWN, LEFT, RIGHT };

// Estructura de coordenadas
typedef struct {
    int x, y;
} Point;

int main(int argc, char *argv[]) {
    // 1. Inicialización de Allegro 5
    if (!al_init()) {
        fprintf(stderr, "Error al inicializar Allegro 5.\n");
        return -1;
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "Error al inicializar el addon de primitivas.\n");
        return -1;
    }
    if (!al_install_keyboard()) {
        fprintf(stderr, "Error al inicializar el teclado.\n");
        return -1;
    }

    // Configuración del temporizador (FPS controla la velocidad del juego)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 8.0); // 8 movimientos por segundo
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    if (!timer || !display || !event_queue) {
        fprintf(stderr, "Error al crear recursos de Allegro.\n");
        return -1;
    }

    // Registrar fuentes de eventos
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // 2. Estado inicial del juego
    srand(time(NULL));
    Point snake[MAX_SNAKE];
    int snake_length = 3;
    
    // Posición inicial de la serpiente en el centro
    for (int i = 0; i < snake_length; i++) {
        snake[i].x = (SCREEN_WIDTH / 2) / GRID_SIZE - i;
        snake[i].y = (SCREEN_HEIGHT / 2) / GRID_SIZE;
    }

    enum Direction dir = RIGHT;
    enum Direction next_dir = RIGHT; // Evita giros de 180° inmediatos

    // Posición inicial de la comida
    Point food;
    food.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
    food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);

    bool running = true;
    bool redraw = true;
    bool game_over = false;

    al_start_timer(timer);

    // 3. Bucle principal del juego
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_UP:    if (dir != DOWN)  next_dir = UP;    break;
                case ALLEGRO_KEY_DOWN:  if (dir != UP)    next_dir = DOWN;  break;
                case ALLEGRO_KEY_LEFT:  if (dir != RIGHT) next_dir = LEFT;  break;
                case ALLEGRO_KEY_RIGHT: if (dir != LEFT)  next_dir = RIGHT; break;
                case ALLEGRO_KEY_ESCAPE: running = false;                   break;
            }
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            if (!game_over) {
                dir = next_dir;

                // Calcular la nueva posición de la cabeza
                Point next_head = snake[0];
                switch (dir) {
                    case UP:    next_head.y--; break;
                    case DOWN:  next_head.y++; break;
                    case LEFT:  next_head.x--; break;
                    case RIGHT: next_head.x++; break;
                }

                // Colisión con los bordes de la pantalla
                if (next_head.x < 0 || next_head.x >= (SCREEN_WIDTH / GRID_SIZE) ||
                    next_head.y < 0 || next_head.y >= (SCREEN_HEIGHT / GRID_SIZE)) {
                    game_over = true;
                }

                // Colisión consigo misma
                for (int i = 0; i < snake_length; i++) {
                    if (next_head.x == snake[i].x && next_head.y == snake[i].y) {
                        game_over = true;
                    }
                }

                if (!game_over) {
                    // Desplazar el cuerpo de la serpiente
                    for (int i = snake_length - 1; i > 0; i--) {
                        snake[i] = snake[i - 1];
                    }
                    snake[0] = next_head;

                    // Si come la fruta
                    if (snake[0].x == food.x && snake[0].y == food.y) {
                        if (snake_length < MAX_SNAKE) {
                            snake_length++;
                        }
                        // Generar nueva comida de forma aleatoria
                        food.x = rand() % (SCREEN_WIDTH / GRID_SIZE);
                        food.y = rand() % (SCREEN_HEIGHT / GRID_SIZE);
                    }
                }
            }
            redraw = true;
        }

        // 4. Renderizado de gráficos
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;
            al_clear_to_color(al_map_rgb(20, 20, 20)); // Fondo oscuro

            if (!game_over) {
                // Dibujar comida (Cuadrado rojo)
                al_draw_filled_rectangle(
                    food.x * GRID_SIZE, food.y * GRID_SIZE,
                    (food.x + 1) * GRID_SIZE, (food.y + 1) * GRID_SIZE,
                    al_map_rgb(255, 50, 50)
                );

                // Dibujar serpiente (Cuadrados verdes)
                for (int i = 0; i < snake_length; i++) {
                    al_draw_filled_rectangle(
                        snake[i].x * GRID_SIZE, snake[i].y * GRID_SIZE,
                        (snake[i].x + 1) * GRID_SIZE, (snake[i].y + 1) * GRID_SIZE,
                        al_map_rgb(50, 200, 50)
                    );
                }
            } else {
                // Pantalla de Game Over (Fondo rojo tenue)
                al_clear_to_color(al_map_rgb(100, 20, 20));
            }

            al_flip_display();
        }
    }

    // 5. Destrucción de recursos y limpieza
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_timer(timer);

    return 0;
}