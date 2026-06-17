#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"
#include <cmath>
#include <SFML/Audio.hpp>



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
    bool interactPressed = false;
    int hp = 10;
    float invincibilityTimer = 0.0f;
    const float invincibilityDuration = 1.0f; // 1 sekunda nieśmiertelności po oberwaniu

    sf::Texture texture_down;
    sf::Texture texture_up;
    sf::Texture texture_sides;
    sf::SoundBuffer hurtBuffer;
    sf::Sound hurtSound;

    public:
    Link(float x, float y): Character(x, y, 3, 120.0f)
    {
        shape.setSize(sf::Vector2f(48.0f,48.0f));
        shape.setFillColor(sf::Color::Green);

        if(!texture_down.loadFromFile("grafiki/chodzenie_down.png") || !texture_up.loadFromFile("grafiki/chodzenie_up.png") || !texture_sides.loadFromFile("grafiki/chodzenie_sides.png"))
        {
            std::cout<<"Blad w czytywaniu postaci"<<std::endl;
        }
        if (!hurtBuffer.loadFromFile("muzyka/taking_dmg.mp3"))
        {
            std::cout << "Blad w czytywaniu dzwieku link_hurt.wav!" << std::endl;
        }
        else
        {
            hurtSound.setBuffer(hurtBuffer);
            hurtSound.setVolume(100.0f); // Głośność od 0 do 100
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
            if (event.key.code == sf::Keyboard::E) interactPressed = true; 
        }
        
        if (event.type == sf::Event::KeyReleased)
        {
            if (event.key.code == sf::Keyboard::W) move_up = false;
            if (event.key.code == sf::Keyboard::S) move_down = false;
            if (event.key.code == sf::Keyboard::A) move_left = false;
            if (event.key.code == sf::Keyboard::D) move_right = false;
            if (event.key.code == sf::Keyboard::Space) isAttacking = false;
            if (event.key.code == sf::Keyboard::E) interactPressed = false;
        }

        // TYLKO LPM AKTYWUJE STAN ATAKU
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                isAttacking = true;
            }
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

        if (invincibilityTimer > 0.0f) {
        invincibilityTimer -= deltaTime;}
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

    bool isInteractPressed() const
    {
        return interactPressed;
    }

    void resetInteractPressed()
    {
        interactPressed = false;
    }

    int getHP() const { return hp; }
    bool isInvincible() const { return invincibilityTimer > 0.0f; }

    void takeDamage(int amount)
     {
        if (!isInvincible()) {
            hp -= amount;
        if (hp < 0) hp = 0; // Życie nie może spaść poniżej zera
        invincibilityTimer = invincibilityDuration; // Uruchomienie ochrony

        hurtSound.play();
        std::cout << "Link stracil " << amount << " HP! Pozostalo: " << hp << "\n";
        }
    }
    void heal(int amount) {
    hp += amount;
    if (hp > 10) hp = 10;
    }
    
    void setHP(int newHP) { hp = newHP; }
};