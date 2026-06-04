#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Character.h"
#include "Link.h"

extern Link* player;

// Nowa klasa reprezentująca pocisk (kuleleczkę) wystrzeloną przez Szkieleta
class Projectile : public Game
{
private:
    sf::CircleShape bulletShape;
    sf::Vector2f velocity;
    float speed;

public:
    Projectile(float x, float y, sf::Vector2f direction)
    {
        speed = 200.0f; // Pocisk leci dość szybko
        
        bulletShape.setRadius(6.0f);
        bulletShape.setFillColor(sf::Color::White); // Biała kuleczka
        bulletShape.setOutlineColor(sf::Color::Black);
        bulletShape.setOutlineThickness(1.0f);
        bulletShape.setOrigin(6.0f, 6.0f); // Środek kółka
        bulletShape.setPosition(x, y);

        // Normalizujemy wektor kierunku pocisku
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0)
        {
            velocity = (direction / length) * speed;
        }
    }

    // Dopasowane do sygnatury z Game.h (przyjmuje deltaTime)
    void update(float deltaTime) override
    {
        bulletShape.move(velocity * deltaTime);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(bulletShape);
    }

    // Wymagane przez silnik (pocisk nie jest ścianą)
    bool isSolid() const override { return false; }
    
    // Dopasowane do sygnatury z Game.h (brak słówka const)
    sf::FloatRect getBounds() override 
    {
        return bulletShape.getGlobalBounds();
    }
};


// Klasa bazowa (Abstrakcyjna)
class Enemy : public Character
{
public:
    Enemy(float x, float y, float speed) : Character(x, y, 1, speed)
    {
        // Korzystamy z pola 'shape' odziedziczonego z klasy Character!
        this->shape.setSize(sf::Vector2f(40.0f, 40.0f));
        this->shape.setOutlineColor(sf::Color::Black);
        this->shape.setOutlineThickness(2.0f);
        this->shape.setPosition(x, y);
    }

    // Nowa wirtualna metoda dla sztucznej inteligencji, która potrzebuje dostępu do wektora obiektów świata
    virtual void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) = 0;

    // Spełniamy wymaganie czystej funkcji wirtualnej z Game.h/Character.h
    void update(float deltaTime) override {} 

    void draw(sf::RenderWindow& window) override
    {
        window.draw(this->shape);
    }

protected:
    // Zwraca wektor ruchu W STRONĘ gracza
    sf::Vector2f getDirectionToPlayer()
    {
        if (player == nullptr) return sf::Vector2f(0.f, 0.f);
        
        sf::Vector2f enemyPos = this->shape.getPosition();
        sf::Vector2f playerPos = player->getPosition();
        sf::Vector2f direction = playerPos - enemyPos;
        
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 2.0f)
        {
            return sf::Vector2f(direction.x / length, direction.y / length);
        }
        return sf::Vector2f(0.f, 0.f);
    }
};


// 1. Klasa Moblin (Brązowy kwadracik)
class Moblin : public Enemy
{
public:
    Moblin(float x, float y) : Enemy(x, y, 80.0f)
    {
        this->shape.setFillColor(sf::Color(139, 69, 19));
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        sf::Vector2f dir = getDirectionToPlayer();
        this->shape.move(dir * speed * deltaTime);
    }
};


// 2. Klasa Slime (Fioletowy kwadracik)
class Slime : public Enemy
{
public:
    Slime(float x, float y) : Enemy(x, y, 80.0f)
    {
        this->shape.setFillColor(sf::Color(128, 0, 128));
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        sf::Vector2f dir = getDirectionToPlayer();
        this->shape.move(dir * speed * deltaTime);
    }
};


// 3. Klasa Skieleton (Szary kwadracik - Ucieka i strzela!)
class Skieleton : public Enemy
{
private:
    sf::Clock shootClock;
    sf::Vector2f recoilVelocity = sf::Vector2f(0.0f, 0.0f); // Aktualna prędkość odrzutu
    float friction = 8.0f;

public:
    Skieleton(float x, float y) : Enemy(x, y, 60.0f) 
    {
        this->shape.setFillColor(sf::Color(169, 169, 169));
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        // 1. OBSŁUGA PŁYNNEGO ODRZUTU (Wywoływane w każdej klatce)
        if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
        {
            this->shape.move(recoilVelocity * deltaTime);

            recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
            recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

            if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
            {
                recoilVelocity = sf::Vector2f(0.0f, 0.0f);
            }
        }

        // 2. STRZELANIE I POCHODZENIE ODRZUTU (Co 3 sekundy)
        if (shootClock.getElapsedTime().asSeconds() >= 3.0f)
        {
            if (player != nullptr)
            {
                sf::FloatRect skeletonBounds = this->shape.getGlobalBounds();
                sf::FloatRect playerBounds = player->getBounds();

                // Obliczamy dokładny ŚRODEK szkieleta i ŚRODEK gracza
                sf::Vector2f skeletonCenter(
                    skeletonBounds.left + (skeletonBounds.width / 2.0f),
                    skeletonBounds.top + (skeletonBounds.height / 2.0f)
                );

                sf::Vector2f playerCenter(
                    playerBounds.left + (playerBounds.width / 2.0f),
                    playerBounds.top + (playerBounds.height / 2.0f)
                );

                sf::Vector2f shootDir = playerCenter - skeletonCenter;

                // Spawnowanie pocisku
                worldObjects.push_back(new Projectile(skeletonCenter.x, skeletonCenter.y, shootDir));

                // AKTYWACJA ODRZUTU:
                float length = std::sqrt(shootDir.x * shootDir.x + shootDir.y * shootDir.y);
                if (length != 0.0f)
                {
                    sf::Vector2f normalizedDir = shootDir / length;
                    float recoilForce = 450.0f; 
                    recoilVelocity = -normalizedDir * recoilForce;
                }
            }
            shootClock.restart();
        }
    }
};