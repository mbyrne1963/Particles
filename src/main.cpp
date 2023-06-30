#include <SFML/Graphics.hpp>
#include "math.h"
#include "vector"

class GravitySource {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float strength;
    sf::CircleShape s;


public:
    GravitySource(float p_x, float p_y, float p_strength) {
        position.x = p_x;
        position.y = p_y;
        this->strength = p_strength;

        s.setPosition(position);
        s.setFillColor(sf::Color::Red);
        s.setRadius((strength / 1000));
    }

    void render(sf::RenderWindow &wind) {
        if (strength > 0)
            wind.draw(s);
    }

    sf::Vector2f getPosition() {
        return position;
    }

    float get_strength() {
        return strength;
    }

    void increase_strength(float increase) {
        s.setRadius((strength / 1000));
        strength += increase;
    }
};

class Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    float strength;
    sf::CircleShape cs;
    bool is_visible = true;


public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        position.x = pos_x;
        position.y = pos_y;

        velocity.x = vel_x;
        velocity.y = vel_y;
        strength=1;

        cs.setPosition(position);
        cs.setFillColor(sf::Color::Black);
        cs.setRadius(3);
    }

    ~Particle() {
        //TODO remove from array
    }

    bool getVisible() {
        return is_visible;
    }

    sf::Vector2f getPosition() {
        return position;
    }

    float get_strength() {
        return strength;
    }


    void setColour(sf::Color colour) {
        cs.setFillColor(colour);
    }

    void render(sf::RenderWindow &wind) {
        if (is_visible) {
            cs.setPosition(position);
            wind.draw(cs);
        }
    }

    sf::Vector2f CalculateAccelerationFromGravitySource(GravitySource &s) {
        float distance_x = s.getPosition().x - position.x;
        float distance_y = s.getPosition().y - position.y;

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        float inverse_distance = 1.f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance * inverse_distance;

        sf::Vector2f acceleration;

        acceleration.x = normalized_x * s.get_strength() * inverse_square_dropoff;
        acceleration.y = normalized_y * s.get_strength() * inverse_square_dropoff;

        return acceleration;
    }

    sf::Vector2f CalculateAccelerationFromAnotherParticle(Particle &p) {
        float distance_x = p.getPosition().x - position.x;
        float distance_y = p.getPosition().y - position.y;

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        float inverse_distance = 1.f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance * inverse_distance;

        sf::Vector2f acceleration;

        acceleration.x = normalized_x * p.get_strength() * inverse_square_dropoff;
        acceleration.y = normalized_y * p.get_strength() * inverse_square_dropoff;

        return acceleration;
    }

    void UpdatePhysicsfromGravitySources(GravitySource &s1, GravitySource &s2) {
        sf::Vector2f acceleration = CalculateAccelerationFromGravitySource(s1);

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;

        position.x += velocity.x;
        position.y += velocity.y;

        acceleration = CalculateAccelerationFromGravitySource(s2);

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;

        position.x += velocity.x;
        position.y += velocity.y;

        if (position.x > 1980 || position.x < 0)
            is_visible = false;
        if (position.y > 1080 || position.y < 0)
            is_visible = false;
    }

    void UpdatePhysicsfromAnotherParticle(Particle &p) {
        sf::Vector2f acceleration = CalculateAccelerationFromAnotherParticle( p);

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;

        position.x += velocity.x;
        position.y += velocity.y;

        is_visible = true;
        if (position.x > 1980 || position.x < 0)
            is_visible = false;
        if (position.y > 1080 || position.y < 0)
            is_visible = false;
    }

};

float postionbetweentwoparticles(Particle &p1, Particle &p2) {
    sf::Vector2f pos1, pos2;
    pos1 = p1.getPosition();
    pos2 = p2.getPosition();
    float x = pos1.x - pos2.x;
    float y = pos1.y = pos2.y;

    return sqrt((x * x) + (y * y));

}

int main() {
    sf::RenderWindow window(sf::VideoMode(1960, 1080), "Particle Modelling");
    window.setFramerateLimit(60);

    GravitySource source1(800, 500, 0);
    GravitySource source2(600, 300, 4000);

    std::vector<Particle> particles;

    for (int i = 0; i <= 100; i++) {
        particles.push_back(Particle(1000 - i, 400, 1, 2));
    }

    int count_number_visible = particles.capacity();

    while (window.isOpen() && count_number_visible != 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
            if (sf::Keyboard::isKeyPressed((sf::Keyboard::Add))) {
                source2.increase_strength(1000);
                source2.render(window);
            }
            if (sf::Keyboard::isKeyPressed((sf::Keyboard::Subtract))) {
                source2.increase_strength(-1000);
                source2.render(window);
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                    sf::Vector2i MousePosition = sf::Mouse::getPosition();
                    particles.push_back(Particle((float) MousePosition.x,(float) MousePosition.y, 1.0f, 1.0f));
            }
            }


        window.clear(sf::Color::White);

        source1.render(window);
        source2.render(window);

        count_number_visible = 0;

        for (auto part = particles.begin(); part != particles.end();) {
            part->UpdatePhysicsfromGravitySources(source1, source2);
            // Check if the particle should be removed
            if (!part->getVisible()) {
                particles.erase(part);
            } else {
                count_number_visible++;
                part->render(window);
                ++part;
            }
        }
    //draw calls
    window.display();
}


return 0;
}