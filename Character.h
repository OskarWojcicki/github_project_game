#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"

class Character :public Game
{
    protected:
    int hp;
    float speed;
    sf::RectangleShape shape;

    sf::Texture texture;
    sf::Sprite sprite;
    sf::IntRect klatkaStruktura;

    float czasKlatki = 0.12f;
    float licznikCzasu = 0.0f;
    float aktualnaKlatka = 0.0f;

    const int szerokosc_klatki= 24;
    const int wysokosc_klatki = 26;

    public:
    
    Character(float x, float y, int health, float movement_speed):hp(health), speed(movement_speed)
    {
        shape.setPosition(x,y);

        sprite.setPosition(x,y);
    }

    void update(float deltaTime) override = 0;
    
    void draw(sf::RenderWindow &window) override
    {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() override
    {
        return shape.getGlobalBounds();
    }  
    sf::Vector2f getPosition()
    {
        return shape.getPosition();
    }

    void setPosition(float x, float y)
    {
        shape.setPosition(x, y);
    }
};