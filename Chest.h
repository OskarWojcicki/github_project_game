#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"
#include "Items.h"
class Link;

class Chest : public Game
{
private:
    sf::Sprite sprite;
    sf::Texture texture;
    bool isOpened;
    std::string reward_type;

    const int szerokosc_klatki = 32;
    const int wysokosc_klatki = 38;

public:
    Chest(float x, float y, std::string reward) : isOpened(false), reward_type(reward)
    {
        if(!texture.loadFromFile("grafiki/chest.png"))
        {
            std::cout<<"Blad w ladowaniu skrzynek!!!"<<std::endl;
        }
        sprite.setTexture(texture);

        sprite.setTextureRect(sf::IntRect(0,0,szerokosc_klatki,wysokosc_klatki));

        sprite.setScale(1.8f,1.8f);
        sprite.setPosition(x,y);
    }

    void update(float deltaTime) override {}

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }

    bool isSolid() const override
    {
        return true;
    }

    sf::FloatRect getBounds() override
    {

        sf::FloatRect rzeczywisteGranice = sprite.getGlobalBounds();

        float marginesX = 15.0f;
        float marginesY = 0.5f;

        return sf::FloatRect(rzeczywisteGranice.left + marginesX,
        rzeczywisteGranice.top + marginesY,
        rzeczywisteGranice.width - (marginesX * 2),
        rzeczywisteGranice.height - (marginesY * 2));
    }

    void setPosition(float x,float y) 
    {
        sprite.setPosition(x,y);
    }

    sf::Vector2f getPosition() 
    {
        return sprite.getPosition();
    }

    Item* interact(Link* playerObj)
    {
        if(isOpened)
        {
            return nullptr;
        }
        isOpened = true;
        
        sprite.setTextureRect(sf::IntRect(96,0,szerokosc_klatki,wysokosc_klatki));
        
        // Tworzymy odpowiedni obiekt na podstawie reward_type
        if (reward_type == "Sword") {
            return new Sword();
        }
        else if (reward_type == "Bow") {
            return new Bow();
        }
        else if (reward_type == "Boomerang") {
            // Jeśli Boomerang wymaga argumentów, podaj je tutaj, np. new Boomerang()
            return new Boomerang(); 
        }
        
        return nullptr;
    }
};