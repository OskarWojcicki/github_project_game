#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Character.h"
#include "Link.h"

extern Link* player;

// Ponieważ worldObjects w main trzyma wskaźniki na Game*, musimy mieć dostęp do tej listy,
// aby Szkielet mógł fizycznie DODAĆ wystrzelony pocisk do świata gry.
// W tym celu przekażemy referencję do worldObjects w metodzie update().



// Nowa klasa reprezentująca pocisk (kuleleczkę) wystrzeloną przez Szkieleta
class Projectile : public Game
{
private:
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float speed;

public:
    Projectile(float x, float y, sf::Vector2f direction)
    {
        speed = 200.0f; // Pocisk leci dość szybko
        
        shape.setRadius(6.0f);
        shape.setFillColor(sf::Color::White); // Biała kuleczka
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(1.0f);
        shape.setOrigin(6.0f, 6.0f); // Środek kółka
        shape.setPosition(x, y);

        // Normalizujemy wektor kierunku pocisku
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0)
        {
            velocity = (direction / length) * speed;
        }
    }

    void update() override
    {
        // Ruch pocisku (sztywne dt = 1/60 tak jak reszta)
        shape.move(velocity * (1.0f / 60.0f));
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }

    // Wymagane przez silnik (pocisk nie jest ścianą)
    bool isSolid() const override { return false; }
    
    sf::FloatRect getBounds() override 
    {
        return shape.getGlobalBounds();
    }
};


// Klasa bazowa (Abstrakcyjna)
class Enemy : public Character
{
public:
    Enemy(float x, float y, float speed) : Character(x, y, 1, speed)
    {
        this->shape.setSize(sf::Vector2f(40.0f, 40.0f));
        this->shape.setOutlineColor(sf::Color::Black);
        this->shape.setOutlineThickness(2.0f);
        this->shape.setPosition(x, y);
    }

    // Zmieniamy sygnaturę update, aby móc wrzucać nowe pociski do wektora obiektów gry
    virtual void update(std::vector<Game*>& worldObjects) = 0;

    // Przeciążenie starej metody, by kod się kompilował jeśli klasa bazowa Character tego wymaga
    void update() override {} 

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

    void update(std::vector<Game*>& worldObjects) override
    {
        sf::Vector2f dir = getDirectionToPlayer();
        this->shape.move(dir * speed * (1.0f / 60.0f));
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

    void update(std::vector<Game*>& worldObjects) override
    {
        sf::Vector2f dir = getDirectionToPlayer();
        this->shape.move(dir * speed * (1.0f / 60.0f));
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
    Skieleton(float x, float y) : Enemy(x, y, 0.0f) // Dałem mu prędkość 60, żeby Link mógł go dogonić
    {
        this->shape.setFillColor(sf::Color(169, 169, 169));
    }

void update(std::vector<Game*>& worldObjects) override
{
    // Pobieramy deltaTime (załóżmy standardowe ~1/60 sekundy, jeśli nie przekazujesz go w argumencie, 
    // możemy użyć stałej wartości lub najlepiej, jeśli masz dostęp do zegara gry. 
    // Na potrzeby stałego FPS 60, 1.0f / 60.0f da idealną płynność):
    float dt = 1.0f / 60.0f; 

    // 1. OBSŁUGA PŁYNNEGO ODRZUTU (Wywoływane w każdej klatce)
    if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
    {
        // Przesuwamy szkieleta o aktualną prędkość odrzutu
        this->shape.move(recoilVelocity * dt);

        // Zmniejszamy prędkość odrzutu (tarciem/oporami), aby szkielet płynnie zatrzymał się
        recoilVelocity.x -= recoilVelocity.x * friction * dt;
        recoilVelocity.y -= recoilVelocity.y * friction * dt;

        // Jeśli prędkość jest już minimalna, zerujemy ją całkowicie
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
            // Pobieramy granice (wymiary) obu postaci, aby znaleźć ich środki
            sf::FloatRect skeletonBounds = this->shape.getGlobalBounds();
            sf::FloatRect playerBounds = player->getBounds();

            // POPRAWKA CELOWANIA: Obliczamy dokładny ŚRODEK szkieleta i ŚRODEK gracza
            sf::Vector2f skeletonCenter(
                skeletonBounds.left + (skeletonBounds.width / 2.0f),
                skeletonBounds.top + (skeletonBounds.height / 2.0f)
            );

            sf::Vector2f playerCenter(
                playerBounds.left + (playerBounds.width / 2.0f),
                playerBounds.top + (playerBounds.height / 2.0f)
            );

            // Kierunek strzału wyliczany OD ŚRODKA DO ŚRODKA
            sf::Vector2f shootDir = playerCenter - skeletonCenter;

            // Tworzymy pocisk ze środka szkieleta
            worldObjects.push_back(new Projectile(skeletonCenter.x, skeletonCenter.y, shootDir));

            // AKTYWACJA ODRZUTU:
            float length = std::sqrt(shootDir.x * shootDir.x + shootDir.y * shootDir.y);
            if (length != 0.0f)
            {
                sf::Vector2f normalizedDir = shootDir / length;

                // Nadajemy dużą prędkość początkową w przeciwnym kierunku.
                // Wartość 400.0f sprawi, że szkielet gwałtownie odskoczy i zacznie zwalniać.
                float recoilForce = 450.0f; 
                recoilVelocity = -normalizedDir * recoilForce;
            }
        }
        shootClock.restart();
    }
}
};