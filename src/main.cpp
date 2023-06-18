


#include <SFML/Graphics.hpp>


class GravitySource {
    sf::Vector2f pos;
    float strength;
    sf::CircleShape s;


public:
    GravitySource(float pos_x, float pos_y, float strength) {
        pos.x = pos_x;
        pos.y = pos_y;
        this->strength = strength;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(4);
    }

    void render(sf::RenderWindow &wind) {
        wind.draw(s);
    }

    sf::Vector2f get_pos() {
        return pos;
    }

    float get_strength() {
        return strength;
    }
};


class Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape s;


public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        pos.x = pos_x;
        pos.y = pos_y;

        vel.x = vel_x;
        vel.y = vel_y;

        s.setPosition(pos);
        s.setFillColor(sf::Color::White);
        s.setRadius(4);
    }

    void render(sf::RenderWindow &wind) {

        wind.draw(s);
    }

    void update_physics(GravitySource &s) {


    }

};


int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "My Program");
    window.setFramerateLimit(60);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }


        window.clear();

        //draw calls
        window.display();
    }


    return 0;
}