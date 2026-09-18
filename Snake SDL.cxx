#include <SDL2/SDL.h>
#include <vector>
#include <cstdlib>
#include <ctime>

// Configuración global del juego
const int ANCHO_PANTALLA = 640;
const int ALTO_PANTALLA  = 480;
const int TAMANO_CELDA   = 20; // Tamaño de cada bloque de la cuadrícula

struct Segmento {
    int x, y;
};

// Direcciones de movimiento
enum class Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

int main(int argc, char* argv[]) {
    // 1. Inicializar SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("No se pudo inicializar SDL: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Snake Game (SDL2)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ANCHO_PANTALLA, ALTO_PANTALLA,
        SDL_WINDOW_SHOWN
    );

    if (!ventana) {
        SDL_Log("No se pudo crear la ventana: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderizador = SDL_CreateRenderer(ventana, -1, SDL_RENDERER_ACCELERATED);
    if (!renderizador) {
        SDL_Log("No se pudo crear el renderizador: %s", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return 1;
    }

    // Inicializar generador de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // 2. Estado Inicial del Juego
    std::vector<Segmento> serpiente = {
        { ANCHO_PANTALLA / 2, ALTO_PANTALLA / 2 },
        { ANCHO_PANTALLA / 2 - TAMANO_CELDA, ALTO_PANTALLA / 2 },
        { ANCHO_PANTALLA / 2 - (2 * TAMANO_CELDA), ALTO_PANTALLA / 2 }
    };

    Direccion dirActual = Direccion::DERECHA;
    Direccion dirSiguiente = Direccion::DERECHA;

    // Colocar la primera comida en una posición alineada a la cuadrícula
    Segmento comida;
    comida.x = (std::rand() % (ANCHO_PANTALLA / TAMANO_CELDA)) * TAMANO_CELDA;
    comida.y = (std::rand() % (ALTO_PANTALLA / TAMANO_CELDA)) * TAMANO_CELDA;

    bool enEjecucion = true;
    SDL_Event evento;
    
    // Variables para controlar la velocidad de actualización (FPS independientes de la lógica)
    Uint32 tiempoAnterior = SDL_GetTicks();
    const Uint32 retrasoFotograma = 100; // Milisegundos por paso (ajusta para cambiar dificultad)

    // 3. Bucle Principal del Juego
    while (enEjecucion) {
        // Manejo de Eventos (Entrada de usuario)
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                enEjecucion = false;
            } else if (evento.type == SDL_KEYDOWN) {
                switch (evento.key.keysym.sym) {
                    case SDLK_UP:
                        if (dirActual != Direccion::ABAJO) dirSiguiente = Direccion::ARRIBA;
                        break;
                    case SDLK_DOWN:
                        if (dirActual != Direccion::ARRIBA) dirSiguiente = Direccion::ABAJO;
                        break;
                    case SDLK_LEFT:
                        if (dirActual != Direccion::DERECHA) dirSiguiente = Direccion::IZQUIERDA;
                        break;
                    case SDLK_RIGHT:
                        if (dirActual != Direccion::IZQUIERDA) dirSiguiente = Direccion::DERECHA;
                        break;
                    case SDLK_ESCAPE:
                        enEjecucion = false;
                        break;
                }
            }
        }

        // Controlar la velocidad del movimiento de la serpiente
        Uint32 tiempoActual = SDL_GetTicks();
        if (tiempoActual - tiempoAnterior >= retrasoFotograma) {
            tiempoAnterior = tiempoActual;
            dirActual = dirSiguiente;

            // Calcular nueva posición de la cabeza
            Segmento nuevaCabeza = serpiente.front();
            switch (dirActual) {
                case Direccion::ARRIBA:    nuevaCabeza.y -= TAMANO_CELDA; break;
                case Direccion::ABAJO:     nuevaCabeza.y += TAMANO_CELDA; break;
                case Direccion::IZQUIERDA: nuevaCabeza.x -= TAMANO_CELDA; break;
                case Direccion::DERECHA:   nuevaCabeza.x += TAMANO_CELDA; break;
            }

            // Detección de Colisiones con los bordes (Game Over)
            if (nuevaCabeza.x < 0 || nuevaCabeza.x >= ANCHO_PANTALLA ||
                nuevaCabeza.y < 0 || nuevaCabeza.y >= ALTO_PANTALLA) {
                SDL_Log("¡Game Over! Chocaste con la pared.");
                enEjecucion = false;
            }

            // Detección de Colisiones consigo misma
            for (const auto& segmento : serpiente) {
                if (nuevaCabeza.x == segmento.x && nuevaCabeza.y == segmento.y) {
                    SDL_Log("¡Game Over! Te mordiste a ti mismo.");
                    enEjecucion = false;
                }
            }

            if (!enEjecucion) break;

            // Insertar la nueva cabeza al frente de la serpiente
            serpiente.insert(serpiente.begin(), nuevaCabeza);

            // Verificar si come el fruto
            if (nuevaCabeza.x == comida.x && nuevaCabeza.y == comida.y) {
                // Generar nueva comida asegurando que no aparezca encima de la serpiente
                bool comidaEnSerpiente;
                do {
                    comidaEnSerpiente = false;
                    comida.x = (std::rand() % (ANCHO_PANTALLA / TAMANO_CELDA)) * TAMANO_CELDA;
                    comida.y = (std::rand() % (ALTO_PANTALLA / TAMANO_CELDA)) * TAMANO_CELDA;
                    
                    for (const auto& segmento : serpiente) {
                        if (comida.x == segmento.x && comida.y == segmento.y) {
                            comidaEnSerpiente = true;
                            break;
                        }
                    }
                } while (comidaEnSerpiente);
            } else {
                // Si no comió, se elimina el último segmento para simular movimiento estándar
                serpiente.pop_back();
            }
        }

        // 4. Renderizado de Gráficos
        // Limpiar pantalla (Fondo negro)
        SDL_SetRenderDrawColor(renderizador, 0, 0, 0, 255);
        SDL_RenderClear(renderizador);

        // Dibujar comida (Bloque rojo)
        SDL_SetRenderDrawColor(renderizador, 220, 50, 50, 255);
        SDL_Rect rectComida = { comida.x, comida.y, TAMANO_CELDA, TAMANO_CELDA };
        SDL_RenderFillRect(renderizador, &rectComida);

        // Dibujar serpiente (Verde)
        for (size_t i = 0; i < serpiente.size(); ++i) {
            // El color de la cabeza es un tono ligeramente más claro
            if (i == 0) {
                SDL_SetRenderDrawColor(renderizador, 50, 230, 50, 255);
            } else {
                SDL_SetRenderDrawColor(renderizador, 34, 139, 34, 255);
            }
            SDL_Rect rectSegmento = { serpiente[i].x, serpiente[i].y, TAMANO_CELDA - 1, TAMANO_CELDA - 1 }; // -1 para marcar división visual
            SDL_RenderFillRect(renderizador, &rectSegmento);
        }

        // Mostrar lo que se ha dibujado en pantalla
        SDL_RenderPresent(renderizador);
        
        // Pausa ligera para reducir uso de CPU innecesario
        SDL_Delay(1);
    }

    // 5. Limpieza de memoria
    SDL_DestroyRenderer(renderizador);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}