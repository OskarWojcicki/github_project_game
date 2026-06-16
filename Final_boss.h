#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Enemies.h"

enum class Boss_state
{
    Transformacja,
    Faza_Oka,
    Faza_PolHp,
    Faza_zadymka
};

class Final_boss : public Enemy
{
    private:
    Boss_state aktualnyEtap;
    sf::Clock stateClock;
    sf::Clock attackClock;

    bool oko_otwarte;
    bool oczy_zrespowane;
z
    float czas_na_oko;
    int maxHp;


    public:
    Final_boss(const sf::Texture& tex, float x, float y) : Enemy (x,y,35.0f)
    {
        this->maxHp = 40;
        this->hp = this->maxHp;

        this->aktualnyEtap = Boss_state::Transformacja;
        this->oko_otwarte = false;
        this->oczy_zrespowane = false;
        this->czas_na_oko = 3.0f;

        this->sprite.setTexture(tex);

        this->shape.setSize(sf::Vector2f(32.0f,32.0f));
        this->shape.setPosition(x,y);
        this->sprite.setPosition(x,y);

        this->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        this->sprite.setScale(1.5f,1.5f);

        this->startPosition = sf::Vector2f(x,y);
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        recoilVelocity = sf::Vector2f(0.0f,0.0f);

        float czas_w_stanie = stateClock.getElapsedTime().asSeconds();

        switch(aktualnyEtap)
        {
            case Boss_state::Transformacja:
                if(czas_w_stanie > 4.0f)
                {
                    this->shape.setSize(sf::Vector2f(96.0f,96.0f));
                    this->sprite.setScale(2.5f,2.5f);

                    aktualnyEtap = Boss_state::Faza_Oka;
                    stateClock.restart();
                }
                break;
                
            case Boss_state::Faza_Oka:
                if(czas_w_stanie >= czas_na_oko)
                {
                    oko_otwarte = !oko_otwarte;
                    stateClock.restart();

                    if(oko_otwarte)
                    {
                        std::cout<<"BOSS OTWORZYL OKO"<<std::endl;
                    }
                    else
                    {
                        std::cout<<"Boss schowal oko pod pelerynke"<<std::endl;
                    }
                }
                
                this->shape.move(getDirectionToPlayer() * speed * deltaTime);

                if(this->hp <= maxHp/2)
                {
                    aktualnyEtap = Boss_state::Faza_PolHp;
                    oko_otwarte=true;
                    stateClock.restart();
                    attackClock.restart();
                }
                break;

            case Boss_state::Faza_PolHp:
                if(!oczy_zrespowane)
                {
                    std::cout<<"Lataja oczy"<<std::endl;
                    oczy_zrespowane=true;
                }

                this->shape.move(getDirectionToPlayer() * (speed * 1.3f) * deltaTime);

                if(this->hp <= maxHp / 4)
                {
                    aktualnyEtap = Boss_state::Faza_zadymka;
                }
                break;

            case Boss_state::Faza_zadymka:
                this->shape.move(getDirectionToPlayer() * (speed * 1.8f) * deltaTime);
                this->sprite.setColor(sf::Color(255, 100, 100));

                if(attackClock.getElapsedTime().asSeconds() >= 1.5f)
                {
                    std::cout<<"Boss strzela ogniem"<<std::endl;
                    sf::Vector2f dir = getDirectionToPlayer();

                    attackClock.restart();
                }
                break;
        }
        this->sprite.setPosition(this->shape.getPosition());
    }

    void takeDamage(int amount) override
    {
        if(aktualnyEtap == Boss_state::Transformacja)
        {
            return;
        }

        if(aktualnyEtap == Boss_state::Faza_Oka && !oko_otwarte)
        {
            std::cout<<"Boss odbija ataki pelerynka"<<std::endl;
            return;
        }

        if(!isInvincible())
        {
            this->hp -= amount;
            if(this->hp <0)
            {
                this->hp=0;
            }
            this->invincibilityTimer = this ->invincibilityDuration;
        }
    }
    
};


class Flying_eyes : public Enemy
{
    private:
    sf::Clock laserClock;

    public:
    Flying_eyes(const Texture& tex, float x, float y) : Enemy(x,y,70.0f)
    {
        this->hp=2;
        this->sprite.setTexture(tex);

        this-> sprite.setTextureRect(sf::IntRect(64,0,16,16));
        this->sprite.setScale(2.0f,2.0f);
        this->shape.setSize(sf::Vector2f(32.0f,32.0f));
        this->shape.setPosition(x,y);
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        this->shape.move(getDirectionToPlayer() * speed * deltaTime);
        


    }

    
};