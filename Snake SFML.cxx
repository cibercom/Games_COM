#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Configuración global del tablero
const int TAMANO_BLOCK = 20;     // Tamaño en píxeles de cada cuadro (serpiente/comida)
const int ANCHO_TABLERO = 30;    // Número de cuadros de ancho
const int ALTO_TABLERO = 20;     // Número de cuadros de alto

enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

struct Posicion {
    int x, y;
};

int main() {
    // Inicializar generador de números aleatorios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Crear la ventana de juego
    sf::RenderWindow window(sf::VideoMode(ANCHO_TABLERO * TAMANO_BLOCK, ALTO_TABLERO * TAMANO_BLOCK), "Snake Game - SFML");
    window.setFramerateLimit(60);

    // Estado de la serpiente
    std::vector<Posicion> snake = { {15, 10}, {14, 10}, {13, 10} }; // Cuerpo inicial
    Direccion dir = DERECHA;

    // Estado de la comida
    Posicion comida = { std::rand() % ANCHO_TABLERO, std::rand() % ALTO_TABLERO };

    // Variables de control de tiempo (para regular la velocidad de la serpiente)
    sf::Clock clock;
    float temporizador = 0.0f;
    float retrasoMovimiento = 0.15f; // Segundos por paso (menor = más rápido)

    bool gameOver = false;

    // Bucle principal del juego
    while (window.isOpen()) {
        float tiempoTranscurrido = clock.restart().asSeconds();
        temporizador += tiempoTranscurrido;

        // 1. Manejo de Eventos (Entradas)
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && !gameOver) {
                if ((event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) && dir != ABAJO)
                    dir = ARRIBA;
                if ((event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) && dir != ARRIBA)
                    dir = ABAJO;
                if ((event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::A) && dir != DERECHA)
                    dir = IZQUIERDA;
                if ((event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::D) && dir != IZQUIERDA)
                    dir = DERECHA;
            }
        }

        // 2. Actualización de la Lógica (Movimiento y Colisiones)
        if (temporizador > retrasoMovimiento && !gameOver) {
            temporizador = 0.0f;

            // Calcular nueva posición de la cabeza
            Posicion nuevaCabeza = snake.front();
            if (dir == ARRIBA)    nuevaCabeza.y -= 1;
            if (dir == ABAJO)   nuevaCabeza.y += 1;
            if (dir == IZQUIERDA) nuevaCabeza.x -= 1;
            if (dir == DERECHA)   nuevaCabeza.x += 1;

            // Colisión con los bordes (Paredes)
            if (nuevaCabeza.x < 0 || nuevaCabeza.x >= ANCHO_TABLERO || nuevaCabeza.y < 0 || nuevaCabeza.y >= ALTO_TABLERO) {
                gameOver = true;
            }

            // Colisión con su propio cuerpo
            for (const auto& segmento : snake) {
                if (nuevaCabeza.x == segmento.x && nuevaCabeza.y == segmento.y) {
                    gameOver = true;
                }
            }

            if (!gameOver) {
                // Insertar nueva cabeza al inicio del cuerpo
                snake.insert(snake.begin(), nuevaCabeza);

                // Comprobar si come la comida
                if (nuevaCabeza.x == comida.x && nuevaCabeza.y == comida.y) {
                    // Generar nueva comida en posición aleatoria
                    comida.x = std::rand() % ANCHO_TABLERO;
                    comida.y = std::rand() % ALTO_TABLERO;
                } else {
                    // Si no come, remueve el último segmento (simula avance)
                    snake.pop_back();
                }
            }
        }

        // 3. Renderizado (Dibujo en Pantalla)
        window.clear(sf::Color(30, 30, 30)); // Fondo gris oscuro

        // Dibujar Comida (Cuadrado Rojo)
        sf::RectangleShape formaComida(sf::Vector2f(TAMANO_BLOCK - 1, TAMANO_BLOCK - 1));
        formaComida.setFillColor(sf::Color::Red);
        formaComida.setPosition(comida.x * TAMANO_BLOCK, comida.y * TAMANO_BLOCK);
        window.draw(formaComida);

        // Dibujar Serpiente (Verde)
        for (size_t i = 0; i < snake.size(); ++i) {
            sf::RectangleShape formaSegmento(sf::Vector2f(TAMANO_BLOCK - 1, TAMANO_BLOCK - 1));
            // La cabeza es de un verde más brillante
            formaSegmento.setFillColor(i == 0 ? sf::Color(50, 205, 50) : sf::Color(34, 139, 34));
            formaSegmento.setPosition(snake[i].x * TAMANO_BLOCK, snake[i].y * TAMANO_BLOCK);
            window.draw(formaSegmento);
        }

        // Si es Game Over, oscurece la pantalla ligeramente
        if (gameOver) {
            sf::RectangleShape capaGameOver(sf::Vector2f(ANCHO_TABLERO * TAMANO_BLOCK, ALTO_TABLERO * TAMANO_BLOCK));
            capaGameOver.setFillColor(sf::Color(0, 0, 0, 150)); // Negro translúcido
            window.draw(capaGameOver);
        }

        window.display();
    }

    return 0;
}