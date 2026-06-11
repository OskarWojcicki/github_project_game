#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"
#include <cmath>


class Link : public Character
{
    private:
    bool move_up=false;
    bool move_down=false;
    bool move_left=false;
    bool move_right=false;
    bool patrzy_wLewo=false;
    sf::Vector2f facingDirection = sf::Vector2f(0.0f, 1.0f);
    bool isAttacking = false;
    sf::Vector2f recoilVelocity = sf::Vector2f(0.0f, 0.0f);
    float friction = 8.0f;

    sf::Texture texture_down;
    sf::Texture texture_up;
    sf::Texture texture_sides;

    public:
    Link(float x, float y): Character(x, y, 3, 100.0f)
    {
        shape.setSize(sf::Vector2f(48.0f,48.0f));
        shape.setFillColor(sf::Color::Green);

        if(!texture_down.loadFromFile("chodzenie_down.png") || !texture_up.loadFromFile("chodzenie_up.png") || !texture_sides.loadFromFile("chodzenie_sides.png"))
        {
            std::cout<<"Blad w czytywaniu postaci"<<std::endl;
        }
        sprite.setTexture(texture_down);

        klatkaStruktura = sf::IntRect(0,0,szerokosc_klatki,wysokosc_klatki);
        sprite.setTextureRect(klatkaStruktura);
        sprite.setOrigin(szerokosc_klatki/2.0f, wysokosc_klatki/2.0f);

        sprite.setScale(2.0f,2.0f);
    }

    void handleEvents(sf::Event& event)
    {
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::W) move_up = true;
            if (event.key.code == sf::Keyboard::S) move_down = true;
            if (event.key.code == sf::Keyboard::A) move_left = true;
            if (event.key.code == sf::Keyboard::D) move_right = true;
            if (event.key.code == sf::Keyboard::Space) isAttacking = true;
        }
        
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::W) move_up = false;
            if (event.key.code == sf::Keyboard::S) move_down = false;
            if (event.key.code == sf::Keyboard::A) move_left = false;
            if (event.key.code == sf::Keyboard::D) move_right = false;
            if (event.key.code == sf::Keyboard::Space) isAttacking = false;
        }
    }

    void update(float deltaTime) override
    {
        sf::Vector2f movement(0.0f, 0.0f);
        bool czy_w_ruchu = false;

        // Płynny odrzut Linka
    if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
    {
        this->shape.move(recoilVelocity * deltaTime); // zakładam, że Link też używa pola shape lub innej metody ruchu

        // Wyhamowywanie przez tarcie
        recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
        recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

        // Całkowite zatrzymanie przy małej prędkości
        if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
        {
            recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }
    }
        
        if(move_up)
        {
            movement.y -= speed;
            sprite.setTexture(texture_up);
            czy_w_ruchu=true;
            facingDirection = sf::Vector2f(0.0f, -1.0f);
        }

        if(move_down)
        {
            movement.y += speed;
            sprite.setTexture(texture_down);
            czy_w_ruchu=true;
            facingDirection = sf::Vector2f(0.0f, 1.0f);
        }    

        if(move_left)
        {
            movement.x -= speed;
            sprite.setTexture(texture_sides);
            patrzy_wLewo=true;
            czy_w_ruchu=true;
            facingDirection = sf::Vector2f(-1.0f, 0.0f);
        }

        if(move_right)
        {
            movement.x += speed;
            sprite.setTexture(texture_sides);
            patrzy_wLewo=false;
            czy_w_ruchu=true;
            facingDirection = sf::Vector2f(1.0f, 0.0f);
        }

        shape.move(movement * deltaTime);

        if(czy_w_ruchu)
        {
            licznikCzasu+=deltaTime*1.45f;
            if(licznikCzasu>=czasKlatki)
            {
                licznikCzasu = 0.0f;
                aktualnaKlatka = (static_cast<int>(aktualnaKlatka)+1)%10;
            }
        }
        else
        {
            aktualnaKlatka = 0.0f;
        }

        klatkaStruktura.top =  0;
        
        if(patrzy_wLewo && sprite.getTexture()==&texture_sides)
        {
            klatkaStruktura.left = (static_cast<int>(aktualnaKlatka)*szerokosc_klatki*0.98f) + szerokosc_klatki;
            klatkaStruktura.width = -szerokosc_klatki;
        }
        else
        {
            klatkaStruktura.left = static_cast<int>(aktualnaKlatka) * szerokosc_klatki*0.98f;
            klatkaStruktura.width = szerokosc_klatki;
        }
        sprite.setTextureRect(klatkaStruktura);
        sprite.setPosition(shape.getPosition().x + (shape.getSize().x / 2.0f), shape.getPosition().y + (shape.getSize().y / 2.0f));
    }

    sf::Vector2f getPosition() const
    {
        return shape.getPosition();
    }

    void setPosition(float x, float y)
    {
        shape.setPosition(x, y);
        sprite.setPosition(x + (shape.getSize().x / 2.0f), y + (shape.getSize().y / 2.0f));    
    }   
    bool getIsAttacking() const { return isAttacking; }
    sf::Vector2f getFacingDirection() const { return facingDirection; }

    void applyKnockback(sf::Vector2f direction, float force) {
    recoilVelocity = direction * force;
}
};
