#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"


class Link : public Character
{
    private:
    bool move_up=false;
    bool move_down=false;
    bool move_left=false;
    bool move_right=false;

    public:
    Link(float x, float y): Character(x, y, 3, 400.0f)
    {
        shape.setSize(sf::Vector2f(40.0f,40.0f));
        shape.setFillColor(sf::Color::Green);
    }

    void handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::W) move_up = true;
            if (event.key.code == sf::Keyboard::S) move_down = true;
            if (event.key.code == sf::Keyboard::A) move_left = true;
            if (event.key.code == sf::Keyboard::D) move_right = true;
        }
        
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::W) move_up = false;
            if (event.key.code == sf::Keyboard::S) move_down = false;
            if (event.key.code == sf::Keyboard::A) move_left = false;
            if (event.key.code == sf::Keyboard::D) move_right = false;
        }
    }

    void update() override
    {
        sf::Vector2f movement(0.0f, 0.0f);

        if(move_up)
        {
            movement.y -= speed;
        }

        if(move_down)
        {
            movement.y += speed;
        }    

        if(move_left)
        {
            movement.x -= speed;
        }

        if(move_right)
        {
            movement.x += speed;
        }

        shape.move(movement * (1.0f / 60.0f));
    }

    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }

    void setPosition(float x, float y)
    {
        shape.setPosition(x, y);
    }   
};
