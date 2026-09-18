#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Configuración global del juego
const int ANCHO_VENTANA = 600;
const int ALTO_VENTANA = 600;
const int TAM_CASILLA = 30; // Tamaño de cada bloque de la serpiente y comida

enum class Direccion { NINGUNA, ARRIBA, ABAJO, IZQUIERDA, DERECHA };

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode(ANCHO_VENTANA, ALTO_VENTANA), "Snake con Control Tactil (SFML)");
    window.setFramerateLimit(60);

    // Lógica interna de la serpiente
    std::vector<sf::Vector2i> snake = { {10, 10}, {10, 11}, {10, 12} };
    Direccion dirActual = Direccion::ARRIBA;
    Direccion dirNueva = Direccion::ARRIBA;

    // Posición de la comida
    sf::Vector2i comida = { std::rand() % (ANCHO_VENTANA / TAM_CASILLA), std::rand() % (ALTO_VENTANA / TAM_CASILLA) };

    // Reloj para controlar la velocidad del movimiento de la serpiente
    sf::Clock clock;
    float delayMovimiento = 0.15f; // Segundos por paso

    // Variables para capturar el control táctil (Swipe)
    sf::Vector2i inicioToque(0, 0);
    bool tocando = false;
    const float umbralDeslizamiento = 30.0f; // Píxeles mínimos para considerar un swipe válido

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // --- INICIO CAPTURA CONTROL TÁCTIL ---
            // Soporta tanto pantallas táctiles como el Mouse (para pruebas en PC)
            if (event.type == sf::Event::TouchBegan) {
                inicioToque = sf::Vector2i(event.touch.x, event.touch.y);
                tocando = true;
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                inicioToque = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                tocando = true;
            }

            if (tocando && (event.type == sf::Event::TouchEnded || event.type == sf::Event::MouseButtonReleased)) {
                sf::Vector2i finToque;
                if (event.type == sf::Event::TouchEnded) {
                    finToque = sf::Vector2i(event.touch.x, event.touch.y);
                } else {
                    finToque = sf::Vector2i(event.mouseButton.x, event.mouseButton.y);
                }

                // Calcular la diferencia del movimiento
                float dx = static_cast<float>(finToque.x - inicioToque.x);
                float dy = static_cast<float>(finToque.y - inicioToque.y);

                // Verificar si superó la distancia mínima para que no sea un toque accidental
                if (std::sqrt(dx * dx + dy * dy) > umbralDeslizamiento) {
                    if (std::abs(dx) > std::abs(dy)) {
                        // Movimiento Horizontal
                        if (dx > 0 && dirActual != Direccion::IZQUIERDA) dirNueva = Direccion::DERECHA;
                        else if (dx < 0 && dirActual != Direccion::DERECHA) dirNueva = Direccion::IZQUIERDA;
                    } else {
                        // Movimiento Vertical
                        if (dy > 0 && dirActual != Direccion::ARRIBA) dirNueva = Direccion::ABAJO;
                        else if (dy < 0 && dirActual != Direccion::ABAJO) dirNueva = Direccion::ARRIBA;
                    }
                }
                tocando = false;
            }
            // --- FIN CAPTURA CONTROL TÁCTIL ---
        }

        // Actualización temporal de la lógica del juego
        if (clock.getElapsedTime().asSeconds() >= delayMovimiento) {
            clock.restart();
            dirActual = dirNueva;

            // Calcular nueva posición de la cabeza
            sf::Vector2i nuevaCabeza = snake.front();
            if (dirActual == Direccion::ARRIBA) nuevaCabeza.y--;
            else if (dirActual == Direccion::ABAJO) nuevaCabeza.y++;
            else if (dirActual == Direccion::IZQUIERDA) nuevaCabeza.x--;
            else if (dirActual == Direccion::DERECHA) nuevaCabeza.x++;

            // Insertar nueva cabeza
            snake.insert(snake.begin(), nuevaCabeza);

            // Verificar si come
            if (nuevaCabeza == comida) {
                comida = { std::rand() % (ANCHO_VENTANA / TAM_CASILLA), std::rand() % (ALTO_VENTANA / TAM_CASILLA) };
            } else {
                // Si no come, remueve la cola para simular el avance continuo
                snake.pop_back();
            }

            // Colisiones contra las paredes o cuerpo (Game Over simplificado: reiniciar)
            if (nuevaCabeza.x < 0 || nuevaCabeza.x >= ANCHO_VENTANA / TAM_CASILLA ||
                nuevaCabeza.y < 0 || nuevaCabeza.y >= ALTO_VENTANA / TAM_CASILLA) {
                snake = { {10, 10}, {10, 11}, {10, 12} };
                dirNueva = Direccion::ARRIBA;
            }
            for (size_t i = 1; i < snake.size(); ++i) {
                if (nuevaCabeza == snake[i]) {
                    snake = { {10, 10}, {10, 11}, {10, 12} };
                    dirNueva = Direccion::ARRIBA;
                }
            }
        }

        // --- RENDERIZADO ---
        window.clear(sf::Color(30, 30, 30));

        // Dibujar comida
        sf::RectangleShape rectComida(sf::Vector2f(TAM_CASILLA - 2, TAM_CASILLA - 2));
        rectComida.setFillColor(sf::Color::Red);
        rectComida.setPosition(static_cast<float>(comida.x * TAM_CASILLA), static_cast<float>(comida.y * TAM_CASILLA));
        window.draw(rectComida);

        // Dibujar serpiente
        for (size_t i = 0; i < snake.size(); ++i) {
            sf::RectangleShape rectCuerpo(sf::Vector2f(TAM_CASILLA - 2, TAM_CASILLA - 2));
            rectCuerpo.setFillColor(i == 0 ? sf::Color::Green : sf::Color(0, 200, 0));
            rectCuerpo.setPosition(static_cast<float>(snake[i].x * TAM_CASILLA), static_cast<float>(snake[i].y * TAM_CASILLA));
            window.draw(rectCuerpo);
        }

        window.display();
    }

    return 0;
}
