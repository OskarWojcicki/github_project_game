#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Character.h"
#include <cmath>
#include <SFML/Audio.hpp>

class Link : public Character
{
private:
    bool move_up = false;
    bool move_down = false;
    bool move_left = false;
    bool move_right = false;
    bool patrzy_wLewo = false;
    
    sf::Vector2f facingDirection = sf::Vector2f(0.0f, 1.0f);
    bool isAttacking = false;
    sf::Vector2f recoilVelocity = sf::Vector2f(0.0f, 0.0f);
    float friction = 8.0f;
    bool interactPressed = false;
    int hp = 10;
    float invincibilityTimer = 0.0f;
    const float invincibilityDuration = 1.0f; 

    sf::Texture texture_down;
    sf::Texture texture_up;
    sf::Texture texture_sides;

    sf::Texture tex_bow_down;
    sf::Texture tex_bow_up;
    sf::Texture tex_bow_sides;
    bool isDrawingBow = false; 

    sf::Texture tex_sword_up;
    sf::Texture tex_sword_down;
    sf::Texture tex_sword_sides;
    bool isSlashingSword = false; 

    sf::SoundBuffer hurtBuffer;
    sf::Sound hurtSound;

public:
    Link(float x, float y): Character(x, y, 3, 120.0f)
    {
        shape.setSize(sf::Vector2f(48.0f, 48.0f));
        shape.setFillColor(sf::Color::Green);

        if(!texture_down.loadFromFile("grafiki/chodzenie_down.png") || 
           !texture_up.loadFromFile("grafiki/chodzenie_up.png") || 
           !texture_sides.loadFromFile("grafiki/chodzenie_sides.png") ||
           !tex_bow_down.loadFromFile("grafiki/lucznik_down.png") || 
           !tex_bow_up.loadFromFile("grafiki/lucznik_up.png") || 
           !tex_bow_sides.loadFromFile("grafiki/lucznik_sides.png") ||
           !tex_sword_up.loadFromFile("grafiki/miecznik_up.png")||
           !tex_sword_down.loadFromFile("grafiki/miecznik_down.png")||
           !tex_sword_sides.loadFromFile("grafiki/miecznik_sides.png"))
        {
            std::cout << "Blad w czytywaniu grafik postaci!" << std::endl;
        }

        if (!hurtBuffer.loadFromFile("muzyka/taking_dmg.mp3"))
        {
            std::cout << "Blad w czytywaniu dzwieku taking_dmg.mp3!" << std::endl;
        }
        else
        {
            hurtSound.setBuffer(hurtBuffer);
            hurtSound.setVolume(100.0f);
        }
        
        sprite.setTexture(texture_down);
        klatkaStruktura = sf::IntRect(0, 0, szerokosc_klatki, wysokosc_klatki);
        sprite.setTextureRect(klatkaStruktura);
        sprite.setOrigin(szerokosc_klatki / 2.0f, wysokosc_klatki / 2.0f);
        sprite.setScale(2.0f, 2.0f);
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

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                isAttacking = true;
            }
        }
    }

    void stopAttack() { this->isAttacking = false; }

    void update(float deltaTime) override
    {
        sf::Vector2f movement(0.0f, 0.0f);
        bool czy_w_ruchu = false;

        if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
        {
            this->shape.move(recoilVelocity * deltaTime);
            recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
            recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

            if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
                recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }
        
        if (!isDrawingBow && !isSlashingSword)
        {
            if(move_up) { movement.y -= speed; czy_w_ruchu = true; facingDirection = sf::Vector2f(0.0f, -1.0f); }
            if(move_down) { movement.y += speed; czy_w_ruchu = true; facingDirection = sf::Vector2f(0.0f, 1.0f); }    
            if(move_left) { movement.x -= speed; patrzy_wLewo = true; czy_w_ruchu = true; facingDirection = sf::Vector2f(-1.0f, 0.0f); }
            if(move_right) { movement.x += speed; patrzy_wLewo = false; czy_w_ruchu = true; facingDirection = sf::Vector2f(1.0f, 0.0f); }
            shape.move(movement * deltaTime);
        }

        // SYSTEM DOBIERANIA AKTUALNEJ TEKSTURY
        if (isSlashingSword)
        {
            if (facingDirection.y < 0.0f) sprite.setTexture(tex_sword_up);
            else if (facingDirection.y > 0.0f) sprite.setTexture(tex_sword_down);
            else sprite.setTexture(tex_sword_sides);
        }
        else if (isDrawingBow)
        {
            if (facingDirection.y < 0.0f) sprite.setTexture(tex_bow_up);
            else if (facingDirection.y > 0.0f) sprite.setTexture(tex_bow_down);
            else sprite.setTexture(tex_bow_sides);
        }
        else
        {
            if (move_up) sprite.setTexture(texture_up);
            else if (move_down) sprite.setTexture(texture_down);
            else if (move_left || move_right) sprite.setTexture(texture_sides);
        }

        // Animacja
        bool stan_animowany = czy_w_ruchu || isDrawingBow || isSlashingSword;
        if(stan_animowany) 
        {
            licznikCzasu += deltaTime * 1.45f;
            if(licznikCzasu >= czasKlatki)
            {
                licznikCzasu = 0.0f;
                if (isDrawingBow || isSlashingSword) {
                    int nastepnaKlatka = static_cast<int>(aktualnaKlatka) + 1;
                    if (nastepnaKlatka >= 10) { isDrawingBow = false; isSlashingSword = false; aktualnaKlatka = 0.0f; } 
                    else { aktualnaKlatka = static_cast<float>(nastepnaKlatka); }
                } 
                else { aktualnaKlatka = (static_cast<int>(aktualnaKlatka) + 1) % 10; }
            }
        }
        else { aktualnaKlatka = 0.0f; }

        // DYNAMICZNE USTAWIANIE ROZMIARU WYCINKI
        double szerokoscWycinki = szerokosc_klatki; 
        double wysokoscWycinki = wysokosc_klatki; 

        if (isSlashingSword) { szerokoscWycinki = 33; wysokoscWycinki = 39; }
        else if (isDrawingBow && sprite.getTexture() == &tex_bow_sides) { szerokoscWycinki = 26.1; wysokoscWycinki = 26; }

        klatkaStruktura.top = 0;
        klatkaStruktura.height = wysokoscWycinki;
        
        // LOGIKA STRONY: Teraz wspólna dla wszystkich!
        bool aktualnie_boki = (sprite.getTexture() == &texture_sides || 
                               sprite.getTexture() == &tex_bow_sides || 
                               sprite.getTexture() == &tex_sword_sides);

        if(patrzy_wLewo && aktualnie_boki)
        {
            klatkaStruktura.left = (static_cast<int>(aktualnaKlatka) * szerokoscWycinki * 0.98f) + szerokoscWycinki;
            klatkaStruktura.width = -szerokoscWycinki;
        }
        else
        {
            klatkaStruktura.left = static_cast<int>(aktualnaKlatka) * szerokoscWycinki * 0.98f;
            klatkaStruktura.width = szerokoscWycinki;
        }

        sprite.setTextureRect(klatkaStruktura);
        sprite.setPosition(shape.getPosition().x + (shape.getSize().x / 2.0f), shape.getPosition().y + (shape.getSize().y / 2.0f));

        if (invincibilityTimer > 0.0f) invincibilityTimer -= deltaTime;
    }

    // Gettery i Settery
    sf::Vector2f getPosition() const { return shape.getPosition(); }
    void setPosition(float x, float y) { shape.setPosition(x, y); sprite.setPosition(x + (shape.getSize().x / 2.0f), y + (shape.getSize().y / 2.0f)); }
    bool getIsAttacking() const { return isAttacking; }
    sf::Vector2f getFacingDirection() const { return facingDirection; }
    void applyKnockback(sf::Vector2f direction, float force) { recoilVelocity = direction * force; }
    bool isInteractPressed() const { return interactPressed; }
    void resetInteractPressed() { interactPressed = false; }
    int getHP() const { return hp; }
    bool isInvincible() const { return invincibilityTimer > 0.0f; }

    void takeDamage(int amount) {
        if (!isInvincible()) {
            hp -= amount;
            if (hp < 0) hp = 0;
            invincibilityTimer = invincibilityDuration;
            hurtSound.play();
        }
    }

    void heal(int amount) { hp += amount; if (hp > 10) hp = 10; }
    void setHP(int newHP) { hp = newHP; }
    void setDrawingBow(bool drawing) { this->isDrawingBow = drawing; }
    bool getIsDrawingBow() const { return isDrawingBow; }
    void setSlashingSword(bool slashing) { this->isSlashingSword = slashing; }
    bool getIsSlashingSword() const { return isSlashingSword; }
};