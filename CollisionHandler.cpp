#include "CollisionHandler.h"
#include <cmath>
#include <assert.h>
#include "vectorOperations.h"
#include <iostream>

CollisionHandler::CollisionHandler(sf::RenderWindow& _window) :  maxLenght (op::length(_window.getSize())), window(_window)
{
    //ctor
}
CollisionHandler::~CollisionHandler()
{
    //dtor
}

Lightray* CollisionHandler::createRay(sf::Vector2f starting_position, sf::Vector2f _direction)
{
    std::unique_ptr<Lightray> ray(new Lightray(starting_position, _direction, maxLenght));
    rays.push_back(std::move(ray));
    std::cout << "created a ray" << std::endl;
    return rays.back().get();
}

Wall* CollisionHandler::createWall(sf::Vector2f _first, sf::Vector2f _second)
{
    std::unique_ptr<Wall> wall(new Wall(_first, _second));
    walls.push_back(std::move(wall));
    std::cout << "created a wall" << std::endl;
    return walls.back().get();

}

void CollisionHandler::draw()
{
    update();
    sf::VertexArray lines_array(sf::Lines);
    for(size_t i = 0; i < walls.size(); i++) {
        lines_array.append((*walls[i]).getLeft());
        lines_array.append((*walls[i]).getRight());
    }

    for(size_t i = 0; i < rays.size(); i++) {
        //rays[i].calculateVertices(walls);
        window.draw((*rays[i]).getDrawable());
    }
    window.draw(lines_array);
}

/*bool CollisionHandler::getIntersectWall(const sf::Vector2f& begin, const sf::Vector2f& end, sf::Vector2f& result, Wall& hitResult, bool debugPrint)
{
    for(int i = 0; i < walls.size(); i++) {
        if(getIntersectWall(begin, end, walls[i], result, debugPrint)) {
            hitResult = walls[i];
            return true;
        }
    }
    return false;
}*/

bool CollisionHandler::isInBounds(const sf::Vector2f& begin, const sf::Vector2f& end, const sf::Vector2f& test)
{
    return !(test.x < min(begin.x, end.x) || test.x > max(begin.x, end.x) /*don't know if the last part is needed*/ || test.y < min (begin.y, end.y) || test.y > max(begin.y, end.y));
}


bool CollisionHandler::getIntersectWall(const sf::Vector2f& begin, const sf::Vector2f& end, const Wall& second, sf::Vector2f& result, bool debugDraw, bool debugPrint)
{
    //if(debugPrint) std::cout << "in getIntersectWall b.position = " <<  end << std::endl;
    // y = a(x - Px) + Py

    sf::Vector2f deltaPos_first = end - begin;
    sf::Vector2f deltaPos_second = second.getRight() - second.getLeft();


    // paralel lines
    if(deltaPos_first.x == 0 && deltaPos_second.x == 0) {
        return false;
    } else {
        // one of the lines is vertical
        if(deltaPos_first.x == 0 || deltaPos_second.x ==0) {
            float x_vertical; // x of the vertical line
            sf::Vector2f point_normal; // point of the normal line
            float steepness; // steepness of the normal line
            bool isWall = false;
            if(deltaPos_first.x == 0) {
                // first is vertical
                steepness = deltaPos_second.y/deltaPos_second.x;
                x_vertical = begin.x;
                point_normal = second.getLeft();
            } else {
                //deltaPos_seocnd == 0
                // second is vertical
                isWall = true;
                steepness = deltaPos_first.y/deltaPos_first.x;
                x_vertical = second.getLeft().x;
                point_normal = begin;
            }
            float xIntersect = x_vertical;
            float yIntersect = steepness * (xIntersect - point_normal.x) + point_normal.y;
            if(isWall) {
                float highestY = second.getTop().y; //max(second.getFirst().y, second.getSecond().y);
                float lowestY = second.getBottom().y; //min(second.getFirst().y, second.getSecond().y);
                if(yIntersect > highestY || yIntersect < lowestY) {
                    return false;
                }
            }
            result = sf::Vector2f(xIntersect, yIntersect);
            drawDebugCircle(result, debugDraw);
            if(debugPrint) {
                printf("x intersect is %f \n", xIntersect);
                printf("lowest x = %f \n", min(begin.x, end.x));
                printf("highest x = %f \n", max(begin.x, end.x));
            }
            return isInBounds(begin, end, result);
        }
    }
    float steepness_first = deltaPos_first.y/deltaPos_first.x;
    float steepness_second = deltaPos_second.y/deltaPos_second.x;


    sf::Vector2f positionFirst = begin;
    sf::Vector2f positionSecond = second.getLeft();

    if(debugPrint) {
        printf("steepness wall: %f \n ray: %f\n", steepness_first, steepness_second);
    }

    float steep_difference = (steepness_first - steepness_second);

    if(steep_difference == 0) return false;

    float xIntersect = (steepness_first*positionFirst.x - positionFirst.y - steepness_second * positionSecond.x + positionSecond.y)/steep_difference;


    //check if intersection is in wall bounds
    //printf("xIntersect: %f, first x %f, second x %f \n", xIntersect, second.getFirst().x, second.getSecond().x);
    if(xIntersect < second.getLeft().x || xIntersect > second.getRight().x) {
        return false;
    }

    //check if intersection is in ray bounds
    if(xIntersect < min(begin.x, end.x) || xIntersect > max(begin.x, end.x)) {
        return false;
    }
    float yIntersect = steepness_first * (xIntersect - positionFirst.x) + positionFirst.y;

    if(debugPrint) {
        printf("x intersect is %f \n", xIntersect);
        printf("lowest x = %f \n", min(begin.x, end.x));
        printf("highest x = %f \n", max(begin.x, end.x));
    }

    result = sf::Vector2f(xIntersect, yIntersect);

    drawDebugCircle(result, debugDraw);
    return isInBounds(begin, end, result);
}

void CollisionHandler::update()
{
    for(int i = 0; i < rays.size(); i++) {
        update(*rays[i]);
    }
}

void CollisionHandler::drawDebugCircle(sf::Vector2f position, bool debugDraw)
{
    if(debugDraw) {
        sf::CircleShape debugCircle(10);
        debugCircle.setOrigin(debugCircle.getRadius(), debugCircle.getRadius());
        debugCircle.setFillColor(sf::Color::Green);
        debugCircle.setPosition(position);
        window.draw(debugCircle);
    }
}


void CollisionHandler::update(Lightray& ray)
{
    ray.calculateVertices(*this);

    // you can't use copy or const reference, because the vertices are changed
    if(unIntersect(ray.vertices[0], ray.vertices[1])) {

        /*
        sf::Vector2f relativeDeltaPos = li.getDeltaPos() - li2.getDeltaPos();
        sf::Vector2f newDeltaPos = -relativeDeltaPos;
        newDeltaPos += li2.getDeltaPos();
        ray.vertices[2].position = ray.vertices[1].position + newDeltaPos;
        */
    }
}

bool CollisionHandler::unIntersect(sf::Vertex& a, sf::Vertex& b)
{
    sf::Vector2f result;
    bool changed = false;
    //std::cout << "wall size " << walls.size() << std::endl;
    for(auto& currWall : walls) {
        //std::cout << "in unintersect b.position = " <<  b.position << std::endl;
        if(getIntersectWall(a.position, b.position, *currWall, result, false, false)) {
            b.position = result;
            changed = true;
        }
    }
    return changed;
}


