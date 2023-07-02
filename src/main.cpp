#include <SFML/Graphics.hpp>
#include "math.h"
#include "vector"

int window_height = 1080;
int window_width = 1960;

sf::Font program_font;
sf::Color particle_origin_colour = sf::Color(192,192,192);
sf::Color particle_accellerating_colour = sf::Color(255,128,0);
sf::Color particle_decellerating_colour = sf::Color(0,0,255);
sf::Color gravity_source_colour = sf::Color(255,0,0);


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
        s.setFillColor(gravity_source_colour);
        s.setRadius(4);
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
    sf::Vector2f initial_position;
    sf::Vector2f velocity;
    float strength;
    int number_of_cyles = 0;
    float current_speed = 0;
    float previous_speed = 0;
    sf::CircleShape particle_shape;
    sf::CircleShape particle_source;
    bool is_visible = true;


public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        initial_position.x = pos_x;
        initial_position.y = pos_y;
        position.x = pos_x;
        position.y = pos_y;

        velocity.x = vel_x;
        velocity.y = vel_y;
        strength = 1;

        particle_shape.setPosition(position);
        particle_shape.setFillColor(particle_origin_colour);
        particle_shape.setRadius(3);

        particle_source.setPosition(initial_position);
        particle_source.setFillColor(particle_origin_colour);
        particle_source.setRadius(2);
    }

    ~Particle() {
        //TODO remove from array
    }

    bool getVisible() {
        return is_visible;
    }

    void setVisible(bool p_visible) {
        is_visible = p_visible;
    }

    sf::Vector2f getPosition() {
        return position;
    }

    sf::Vector2f getVelocity() {
        return velocity;
    }

    float get_strength() {
        return strength;
    }

    void setColour(sf::Color colour) {
        particle_shape.setFillColor(colour);
    }

    void render(sf::RenderWindow &wind) {
        if (is_visible) {
            particle_shape.setPosition(position);
            wind.draw(particle_shape);
        };
        particle_source.setFillColor(particle_origin_colour);
        wind.draw(particle_source);
    }

    float CalculateParticleVelocity() const {
        return sqrt((pow(velocity.x, 2.0f) + pow(velocity.y, 2.0f)));
    }

    sf::Vector2f CalculateAccelerationFromGravitySource(GravitySource &s) const {
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
        number_of_cyles++;
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

        current_speed = CalculateParticleVelocity();

        if (current_speed > previous_speed)
            setColour(particle_accellerating_colour);
        else
            setColour(particle_decellerating_colour);
        previous_speed = current_speed;

        if (position.x > window_width || position.x < 0)
            is_visible = false;
        if (position.y > window_height || position.y < 0)
            is_visible = false;
    }

    void UpdatePhysicsfromAnotherParticle(Particle &p) {
        sf::Vector2f acceleration = CalculateAccelerationFromAnotherParticle(p);

        velocity.x += acceleration.x;
        velocity.y += acceleration.y;

        position.x += velocity.x;
        position.y += velocity.y;

        is_visible = true;
        if (position.x > window_width || position.x < 0)
            is_visible = false;
        if (position.y > window_height || position.y < 0)
            is_visible = false;
    }

};


float CalculateDistanceBetweenTwoParticles(Particle &p1, Particle &p2) {
    sf::Vector2f pos1, pos2;
    pos1 = p1.getPosition();
    pos2 = p2.getPosition();
    float x = pos1.x - pos2.x;
    float y = pos1.y = pos2.y;

    return sqrt((x * x) + (y * y));

}

int main() {

    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Particle Modelling");
    window.setFramerateLimit(60);
    program_font.loadFromFile("../fonts/arial.ttf");

    GravitySource source1(window_width / 3, window_height / 2, 1000);
    GravitySource source2(window_width / 3 * 2, window_height / 2, 1000);

    std::vector<Particle> particles;


    while (window.isOpen()) {
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
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                sf::Vector2i MousePosition = sf::Mouse::getPosition();
                particles.push_back(Particle((float) MousePosition.x, (float) MousePosition.y, 1, 1));
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
                sf::Vector2i MousePosition = sf::Mouse::getPosition();
                particles.push_back(Particle((float) MousePosition.x, (float) MousePosition.y, -1.0f, -1.0f));
            }
        }
        window.clear(sf::Color::White);

        source1.render(window);
        source2.render(window);


        for (auto part = particles.begin(); part != particles.end();) {
            part->UpdatePhysicsfromGravitySources(source1, source2);
            // Check if the particle should be removed

            //now need to check to see if the particle is in the same location as another

          // for (auto part2 = particles.begin(); part != particles.end();) {
          //      if (part->getPosition().x == part2->getPosition().x &&
          //          part->getPosition().y == part2->getPosition().y) {
          //          if (part->getVelocity().x != part2->getVelocity().x ||
          //              part->getVelocity().y != part2->getVelocity().y)
          //              part->setVisible(false);
          //      }
          //  }
            if (!part->getVisible()) {
                particles.erase(part);
            } else {
                part->render(window);
                ++part;
            }
        }
        //draw calls
        window.display();
    }


    return 0;
}