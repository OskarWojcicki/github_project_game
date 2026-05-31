#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

class Character :public Game
{
    protected:
    int hp;
    float speed;
    sf::RectangleShape shape;

    public:
    
    Character(float x, float y, int health, float movement_speed):hp(health), speed(movement_speed)
    {
        shape.setPosition(x,y);
    }

    void update(float deltaTime) override = 0;
    
    void draw(sf::RenderWindow &window) override
    {
        window.draw(shape);
    }

    sf::FloatRect getBounds() override
    {
        return shape.getGlobalBounds();
    }  
};