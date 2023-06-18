#include <SFML/Graphics.hpp>
#include "math.h"
#include "vector"

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
        s.setFillColor(sf::Color::Red);
        s.setRadius(10);
    }

    void render(sf::RenderWindow &wind) {
        if (strength>0)
            wind.draw(s);
    }

    sf::Vector2f getPosition() {
        return pos;
    }

    float get_strength() {
        return strength;
    }
};


class Particle {
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::CircleShape cs;
    bool is_visible= true;


public:
    Particle(float pos_x, float pos_y, float vel_x, float vel_y) {
        pos.x = pos_x;
        pos.y = pos_y;

        vel.x = vel_x;
        vel.y = vel_y;

        cs.setPosition(pos);
        cs.setFillColor(sf::Color::Black);
        cs.setRadius(3);
    }
    ~Particle(){
        //TODO remove from array
    }

    bool getVisible(){
        return is_visible;
    }

    sf::Vector2f getPosition(){
        return pos;
    }

    void setColour(sf::Color colour){
    cs.setFillColor(colour);
    }

    void render(sf::RenderWindow &wind) {
        if (is_visible) {
            cs.setPosition(pos);
            wind.draw(cs);
        }
    }

    sf::Vector2f calculateacceleration(GravitySource &s){
        float distance_x = s.getPosition().x - pos.x;
        float distance_y = s.getPosition().y - pos.y;

        float distance = sqrt(distance_x * distance_x + distance_y * distance_y);

        float inverse_distance = 1.f / distance;

        float normalized_x = inverse_distance * distance_x;
        float normalized_y = inverse_distance * distance_y;

        float inverse_square_dropoff = inverse_distance * inverse_distance;

        sf::Vector2f acceleration;

        acceleration.x = normalized_x * s.get_strength() * inverse_square_dropoff;
        acceleration.y = normalized_y * s.get_strength() * inverse_square_dropoff;

        return acceleration ;
    }

    void update_physics(GravitySource &s1, GravitySource &s2)  {
        sf::Vector2f acceleration = calculateacceleration(s1);

        vel.x += acceleration.x;
        vel.y += acceleration.y;

        pos.x += vel.x;
        pos.y += vel.y;

        acceleration = calculateacceleration(s2);

        vel.x += acceleration.x;
        vel.y += acceleration.y;

        pos.x += vel.x;
        pos.y += vel.y;

        if (pos.x > 1080 || pos.x <0)
            is_visible = false;
        if (pos.y > 800 || pos.y <0)
            is_visible = false;
    }

};

float postionbetweentwoparticles (Particle &p1, Particle &p2){
    sf::Vector2f pos1 , pos2;
    pos1 = p1.getPosition();
    pos2 = p2.getPosition();
    float x = pos1.x - pos2.x;
    float y = pos1.y = pos2.y;

    return sqrt( (x * x) + (y * y));

}

int main() {
    sf::RenderWindow window(sf::VideoMode(1600, 1000), "My Program");
    window.setFramerateLimit(60);

    GravitySource source1(800, 500, 4000);
    GravitySource source2(400, 250, 0);

    std::vector<Particle> particles;

    particles.push_back(Particle(600, 620, 2, 2));
    particles.push_back(Particle(600, 615, 2, 2));
    particles.push_back(Particle(600, 610, 2, 2));
    particles.push_back(Particle(600, 605, 2, 2));
    particles.push_back(Particle(600, 620, 0, 2));
    particles.push_back(Particle(600, 615, 0, 2));
    particles.push_back(Particle(600, 610, 0, 2));
    particles.push_back(Particle(600, 605, 0, 2));

    int count_number_visible = particles.capacity();

    while (window.isOpen() && count_number_visible != 0 ) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) window.close();
        }


        window.clear(sf::Color::White);

        source1.render(window);
        source2.render(window);

        count_number_visible = 0;
        for (auto& particle : particles)
        {
            particle.update_physics(source1, source2);
            if (particle.getVisible() == true)
                count_number_visible++;
            particle.render(window);
        }

        //draw calls
        window.display();
    }


    return 0;
}