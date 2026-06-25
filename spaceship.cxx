#include <SFML/Graphics.hpp>

int main() {
    // Crear la ventana de SFML
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Space Shooter");
    window.setFramerateLimit(60);

    // Cargar la textura de la nave
    sf::Texture shipTexture;
    // shipTexture.loadFromFile("nave.png"); // Descomenta y añade tu imagen
    sf::Sprite ship(shipTexture);
    
    // Opcional: si no tienes imagen, dibuja un triángulo
    sf::CircleShape shipShape(30, 3);
    shipShape.setFillColor(sf::Color::Cyan);
    shipShape.setOrigin(30, 30);
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Detectar si la pantalla está siendo tocada
            if (event.type == sf::Event::TouchBegan) {
                // event.touch.x e y contienen las coordenadas exactas del toque
                sf::Vector2i touchPos(event.touch.x, event.touch.y);
                
                // Mover la nave a la posición del dedo
                shipShape.setPosition(static_cast<sf::Vector2f>(touchPos));
                // Si usas sprite, descomentar: ship.setPosition(touchPos.x, touchPos.y);
            }
        }

        window.clear();
        // Dibujar la nave
        window.draw(shipShape); 
        // window.draw(ship); // Descomenta si usas sprite
        window.display();
    }

    return 0;
}