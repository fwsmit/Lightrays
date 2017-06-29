#include <SFML/Graphics.hpp>
#include "CollisionHandler.h"
//#include "Lightray.h"
#include <SFML/Config.hpp>
#include <iostream>
#include "vectorOperations.h"
#include <SFML/Window/ContextSettings.hpp>
#define M_PI		3.14159265358979323846

#define USE_SFML_WINDOW

float degreeToRadian(float degree)
{
    return (degree) * (M_PI/180);
}

/*sf::Vector2f getIntersect(const Lightray& first, const Lightray& second, sf::Vector2f& result)
{
    float angle_first = first.angle;
    float angle_second = second.angle;

    float steepness_first = tan(angle_first);
    float steepness_second = tan(angle_second);

    // y = a(x - Px) + Py

    sf::Vector2f positionFirst = first.vertices[0].position;
    sf::Vector2f positionSecond = second.vertices[0].position;

    float xIntersect = (steepness_first*positionFirst.x - positionFirst.y - steepness_second * positionSecond.x + positionSecond.y)/(steepness_first - steepness_second);

    printf("angles are %f and %f\n", radianToDegree(angle_first), radianToDegree(angle_second));

    printf("steepness is %f and %f\n", steepness_first, steepness_second);

    printf("x intersect is %f \n", xIntersect);
    return sf::Vector2f(300, 20);
}*/



int main()
{
    //cout << op::rotate(sf::Vector2f(1,0), degreeToRadian(180));
    //cin.get();
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16;
    settings.majorVersion = 4;
    settings.minorVersion = 5;
    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!", sf::Style::Default, settings);
    sf::CircleShape intersect(10);
    intersect.setOrigin(sf::Vector2f(intersect.getRadius(), intersect.getRadius()));
    intersect.setFillColor(sf::Color::Green);

    CollisionHandler col(window);
    Lightray* ray = col.createRay(sf::Vector2f(0,0), sf::Vector2f(100,100));
    Lightray* ray2 = col.createRay(sf::Vector2f (100, 550), sf::Vector2f(3,4));
    //Wall* wall = col.createWall(sf::Vector2f(400, 600), sf::Vector2f(300, 100));
    col.createWall(sf::Vector2f(400, 600), sf::Vector2f(230, 100));
    col.createWall(sf::Vector2f(400, 600), sf::Vector2f(400, 100));
    col.createWall(sf::Vector2f(10, 600), sf::Vector2f(40, 1000));

    /*
        Lightray li(sf::Vector2f(300 ,100), sf::Vector2f(300, 400), sqrt(pow(window.getSize().x, 2) + pow(window.getSize().y, 2)));
        Lightray li2( sf::Vector2f(405, 300), sf::Vector2f(300, 400), sqrt(pow(window.getSize().x, 2) + pow(window.getSize().y, 2)));
        Lightray liBounce(sf::Vector2f(300 ,400), sf::Vector2f(30, 0), sqrt(pow(window.getSize().x, 2) + pow(window.getSize().y, 2)));
        //Wall wall(sf::Vector2f(400, 600), sf::Vector2f(230, 100));
        //li.calculateVertices();
        //Lightray* li = col.createRay(degreeToRadian(100), sf::Vector2f(0, 0));
        //Wall* wall = col.createWall(sf::Vector2f(30, 230), sf::Vector2f(230, 100));
        //li.calculateVertices(col);
        //sf::Vector2f temp;


        //sf::VertexArray lines_array(sf::Lines);
        //lines_array.append(wall.getFirst());
        //lines_array.append(wall.getSecond());

        //col.draw();

        li.calculateVertices(col);
        li2.calculateVertices(col);

        float angle = op::angle(li.getDeltaPos(), li2.getDeltaPos());
        //cout << radianToDegree(angle) << endl;


        liBounce.vertices[1] = liBounce.vertices[0].position + newDeltaPos;


        li.vertices[0].color = sf::Color::Red;
        li.vertices[1].color = sf::Color::Green;
        liBounce.vertices[0].color = sf::Color::Red;
        liBounce.vertices[1].color = sf::Color::Blue;
        cout << li.getDeltaPos() << endl << li2.getDeltaPos() << endl;


        //liBounce.calculateVertices(col);
    */
    //cin.get();

    //cout << col.unIntersect(ray->vertices[0], ray->vertices[1], *wall) << endl;

#ifdef USE_SFML_WINDOW
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
            case sf::Event::KeyReleased:
                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;

            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // left mouse button released
                    ray->direction= sf::Vector2f(sf::Mouse::getPosition(window));
    //col.createRay(sf::Vector2f (1000, 800), sf::Vector2f(3,4));
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::JoystickButtonReleased:
                if(event.joystickButton.button == 1) {
                    window.close();
                }
                break;
            default:
                break;
            }
        }
        ray->direction = sf::Vector2f(sf::Mouse::getPosition(window));
        ray2->direction = sf::Vector2f(sf::Mouse::getPosition(window));

        window.clear();
        col.draw();
        //cout << sf::Mouse::getPosition(window).x << "   " << sf::Mouse::getPosition(window).y << endl;
        window.display();

    }
#endif

    return 0;
}
