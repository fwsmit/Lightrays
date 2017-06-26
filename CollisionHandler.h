#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "Lightray.h"
#include "Wall.h"

using namespace std;

class CollisionHandler
{
    const float maxLenght;
    vector<Lightray> rays;
    sf::RenderWindow& window;
    vector<Wall> walls;
    bool getIntersectWall(const sf::Vector2f& begin, const sf::Vector2f& end, const Wall& second, sf::Vector2f& result, bool debugPrint = false);
    void update();
    void update(Lightray& ray);

    /** \brief makes sure the line between vertices a and b don't intersect any walls
     *
     * \return true if the vertices were changed, false if not
     *
     */
public:
    bool unIntersect(sf::Vertex& a, sf::Vertex& b, Wall& wall);
    bool getIntersectWall(const sf::Vector2f& begin, const sf::Vector2f& end, sf::Vector2f& result, Wall& hitResult, bool debugPrint = false);
    CollisionHandler(sf::RenderWindow& _window);
    virtual ~CollisionHandler();
    Lightray* createRay(sf::Vector2f starting_position, sf::Vector2f _direction);
    Wall* createWall(sf::Vector2f _first, sf::Vector2f _second);
    void draw();


protected:

private:
};

#endif // COLLISIONHANDLER_H
