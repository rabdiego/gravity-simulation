#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <ctime>

sf::Color getColor(int index) {
    switch (index) {
        case 0:
            return sf::Color::Red;
            break;
        case 1:
            return sf::Color::Green;
            break;
        case 2:
            return sf::Color::Blue;
            break;
        case 3:
            return sf::Color::Yellow;
            break;
        case 4:
            return sf::Color::Magenta;
            break;
        case 5:
            return sf::Color::Cyan;
            break;
        default:
            return sf::Color::White;
            break;
    }
}

double getDistance(int a[2], int b[2]) {
    return sqrt(pow((a[0] - b[0]), 2) + pow((a[1] - b[1]), 2));
}

double getAngle(int a[2], int b[2]) {
    double angle = atan2((a[0] - b[0]), (a[1] - b[1]));
    return 3*M_PI_2 - angle;
}

class CelestialObject {
    public:
        int radius;
        int position[2];
        sf::Color color;

        void setParameters(int radius, int x, int y, int color) {
            this->radius = radius;
            position[0] = x;
            position[1] = y;
            this->color = getColor(color);
        }

        sf::CircleShape getShape() {
            sf::CircleShape shape(radius);
            shape.setOrigin(radius, radius);
            shape.setFillColor(color);
            shape.setPosition(position[0], position[1]);
            return shape;
        }
};

class Star : public CelestialObject {
    public:
        int mass;

        void setMass(int mass) {
            this->mass = mass;
        }
};

class Planet : public CelestialObject {
    public:
        double velocity[2];
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Gravity Simulator");
    window.setFramerateLimit(60);

    Star sun;
    sun.setParameters(20, 500, 300, 3);
    sun.setMass(10000);

    std::vector <Planet> planets;

    sf::Vector2i mouseVector;

    double distance, angle, mouseAngle, acceleration;
    int mousePosition[2], click = 0;
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                srand(time(NULL));
                
                mouseVector = sf::Mouse::getPosition(window);
                mousePosition[0] = mouseVector.x;
                mousePosition[1] = mouseVector.y;
                mouseAngle = getAngle(mousePosition, sun.position);

                Planet planet;
                planet.setParameters(5, mousePosition[0], mousePosition[1], rand()%6);

                planets.push_back(planet);
                planets[click].velocity[0] = 5*cos(mouseAngle + M_PI_2);
                planets[click].velocity[1] = 5*sin(mouseAngle + M_PI_2);
                
                click++;
            }
        }

        for (int i = 0; i < planets.size(); i++) {
            distance = getDistance(planets[i].position, sun.position);
            angle = getAngle(planets[i].position, sun.position);
            acceleration = sun.mass/pow(distance, 2);

            planets[i].velocity[0] += cos(angle)*acceleration;
            planets[i].velocity[1] += sin(angle)*acceleration;
            planets[i].position[0] += planets[i].velocity[0];
            planets[i].position[1] += planets[i].velocity[1];
        }

        window.clear(sf::Color::Black);
        
        window.draw(sun.getShape());
        for (int i = 0; i < planets.size(); i++) {
            window.draw(planets[i].getShape());
        }

        window.display();
    }
}
