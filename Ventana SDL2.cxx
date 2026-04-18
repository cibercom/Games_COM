#include <SDL2/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "No se pudo inicializar SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Crear ventana
    SDL_Window* window = SDL_CreateWindow("Ejemplo SDL2 Cxxdroid",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Ventana no creada: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Crear renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Pintar fondo azul
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // R, G, B, A
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    // Esperar 5 segundos
    SDL_Delay(5000);

    // Limpiar y cerrar
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}