#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include "Character.h"
#include "Link.h"

extern Link* player;

class Projectile : public Game
{
private:
    sf::CircleShape bulletShape;
    sf::Vector2f velocity;
    float speed;
    sf::Sprite sprite;
    bool isPlayerOwned;

public:
    Projectile(const sf::Texture& tex, float x, float y, sf::Vector2f direction, sf::IntRect rect= sf::IntRect(0,0,16,9),float scale = 2.0f, bool playerOwned = false)
    {
        this->isPlayerOwned = playerOwned;
        speed = 200.0f; 

        this->sprite.setTexture(tex);

        this->sprite.setTextureRect(rect);
        this->sprite.setOrigin(15.0f, 4.0f); 
        this->sprite.setPosition(x, y);
        this->sprite.setScale(scale,scale);

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0)
        {
            velocity = (direction / length) * speed;
            float angle = (std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f) + 180.0f;
            this->sprite.setRotation(angle);
        }
    }

    bool getIsPlayerOwned() const { return isPlayerOwned; }

    void update(float deltaTime) override
    {
        sprite.move(velocity * deltaTime);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }

    bool isSolid() const override { return false; }
    
    sf::FloatRect getBounds() override 
    {
        return sprite.getGlobalBounds();
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

    virtual void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) = 0;

    void update(float deltaTime) override {} 

    void draw(sf::RenderWindow& window) override
    {
        window.draw(this->sprite);
    }

    void applyKnockback(sf::Vector2f direction, float force) {
    recoilVelocity = direction * force;
    }

    int getHP() const { return hp; }
    bool isInvincible() const { return invincibilityTimer > 0.0f; }

    virtual void takeDamage(int amount) {
        if (!isInvincible()) { 
            hp -= amount;
            if (hp < 0) hp = 0;
            invincibilityTimer = invincibilityDuration; 
        }
    }
    
    bool isDead() const { return hp <= 0; }

    void updateInvincibility(float deltaTime) {
        if (invincibilityTimer > 0.0f) {
            invincibilityTimer -= deltaTime;
        }
    }
    sf::Vector2f getStartPosition() const { return startPosition; }
    
    int hp;

protected:
    sf::Vector2f recoilVelocity = sf::Vector2f(0.0f, 0.0f); 
    float friction = 8.0f;
    float invincibilityTimer = 0.0f;       
    float invincibilityDuration = 0.4f; 
    sf::Vector2f startPosition;

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


// 1. Klasa Moblin 
class Moblin : public Enemy
{
    private:
        float czasAnimacji = 0.0f;
        int aktualna_klatka = 0;
        const int ilosc_klatek= 3;
        const float czas_na_klatke = 0.2;

        const sf::Texture* texture_down;
        const sf::Texture* texture_up;
        const sf::Texture* texture_left;
        const sf::Texture* texture_right;

public:
    Moblin(const sf::Texture& tex_up,const sf::Texture& tex_down,const sf::Texture& tex_left,const sf::Texture& tex_right, float x, float y) : Enemy(x, y, 150.0f)
    {   
        this->texture_down = &tex_down;
        this->texture_up = &tex_up;
        this->texture_left = &tex_left;
        this->texture_right = &tex_right;

        this->shape.setFillColor(sf::Color::Transparent);
        this->shape.setOutlineColor(sf::Color::Transparent); 

        this->sprite.setTexture(tex_down);

        int szerokosc_moblina = 32;
        int wysokosc_moblina = 40;
        
        this->klatkaStruktura = sf::IntRect(0, 0, szerokosc_moblina, wysokosc_moblina);
        this->sprite.setTextureRect(this->klatkaStruktura);

        this->sprite.setOrigin(szerokosc_moblina/ 2.0f, wysokosc_moblina / 2.0f);
        
        this->sprite.setScale(1.75f, 1.75f); 

        this->sprite.setPosition(
            x + (this->shape.getSize().x / 2.0f),
            y + (this->shape.getSize().y / 2.0f)
        );
        this->hp = 7;
        this->startPosition = sf::Vector2f(x, y);
    }

    

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        sf::Vector2f dir(0.0f,0.0f);
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
    else 
    {
        dir = getDirectionToPlayer();
        this->shape.move(dir * speed * deltaTime);
    }

    bool czy_w_ruchu = (dir.x !=0.0f || dir.y != 0.0f);

    int klatka_szerokosc = 32;
    int klatka_wysokosc = 40;

    if(czy_w_ruchu)
    {
        if(std::abs(dir.x) > std::abs(dir.y))
        {
            if(dir.x >0.0f)
            {
                this -> sprite.setTexture(*texture_right);
            }
            else
            {
                this->sprite.setTexture(*texture_left);
            }
            klatka_szerokosc = 43; 
            klatka_wysokosc = 32;
        }
        else
        {
            if(dir.y >0.0f)
            {
                this->sprite.setTexture(*texture_down);
            }
            else
            {
                this->sprite.setTexture(*texture_up);
            }
            klatka_szerokosc = 32; 
            klatka_wysokosc = 40;
        }
        czasAnimacji += deltaTime;
        if(czasAnimacji >= czas_na_klatke)
        {
            czasAnimacji=0.0f;
            aktualna_klatka=(aktualna_klatka + 1) % ilosc_klatek;
        }
    }
    else
    {
        aktualna_klatka=0;
        klatka_szerokosc = 32;
        klatka_wysokosc = 40;
    }
    int wielkosc = 32;
    this->klatkaStruktura.top = 0;
    this->klatkaStruktura.width = klatka_szerokosc;
    this->klatkaStruktura.height = klatka_wysokosc;
    this->klatkaStruktura.left = aktualna_klatka * klatka_szerokosc; 
    
    this->sprite.setTextureRect(this->klatkaStruktura);

    this->sprite.setOrigin(klatka_szerokosc / 2.0f, klatka_wysokosc / 2.0f);

    this->sprite.setPosition(
        this->shape.getPosition().x + (this->shape.getSize().x / 2.0f),
        this->shape.getPosition().y + (this->shape.getSize().y / 2.0f)
    );
    }
};


// 2. Klasa Slime
class Slime : public Enemy
{
    private:
        float czasAnimacji = 0.0f;
        int aktualna_klatka = 0;
        const int ilosc_klatek= 3;
        const float czas_na_klatke = 0.16;

    public:
        Slime(const sf::Texture& tex, float x, float y) : Enemy(x, y, 30.0f)
        {
        this->shape.setFillColor(sf::Color::Transparent);
        this->shape.setOutlineColor(sf::Color::Transparent); // Ukrywamy też ramkę debugowania

        this->sprite.setTexture(tex);

        int szerokosc_slima = 32;
        int wysokosc_slima = 27;
        
        this->klatkaStruktura = sf::IntRect(0, 0, szerokosc_slima, wysokosc_slima);
        this->sprite.setTextureRect(this->klatkaStruktura);

        this->sprite.setOrigin(szerokosc_slima / 2.0f, wysokosc_slima / 2.0f);
        
        this->sprite.setScale(1.2f, 1.2f); 

        this->sprite.setPosition(
            x + (this->shape.getSize().x / 2.0f),
            y + (this->shape.getSize().y / 2.0f)
        );
        this->hp = 3;
        this->startPosition = sf::Vector2f(x, y);
        }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
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
    else 
    {
        sf::Vector2f dir = getDirectionToPlayer();
        this->shape.move(dir * speed * deltaTime);
    }

    this->sprite.setPosition(
        this->shape.getPosition().x + (this->shape.getSize().x / 2.0f),
        this->shape.getPosition().y + (this->shape.getSize().y / 2.0f)
    );

    czasAnimacji+=deltaTime;

    if(czasAnimacji>=czas_na_klatke)
    {
        czasAnimacji=0.0f;
        aktualna_klatka++;

        if(aktualna_klatka >= ilosc_klatek)
        {
            aktualna_klatka = 0;
        }
        int szerokosc_slima=32;
        this->klatkaStruktura.left = aktualna_klatka * szerokosc_slima;
        this->sprite.setTextureRect(this->klatkaStruktura);

    }
    }
};


// 3. Klasa Skieleton 
class Skieleton : public Enemy
{
private:
    sf::Clock shootClock;
    const sf::Texture* texture_down;
    const sf::Texture* texture_up;
    const sf::Texture* texture_sides; 
    const sf::Texture* texture_projectile;

    float czasAnimacji = 0.0f;
    int aktualna_klatka = 0;
    const float czas_na_klatke = 0.12f;
    
    bool patrzy_wLewo = false;

public:
    Skieleton(const sf::Texture& t_down, const sf::Texture& t_up, const sf::Texture& t_sides,const sf::Texture& t_proj, float x, float y) : Enemy(x, y, 60.0f) 
    {
        this->texture_down = &t_down;
        this->texture_up = &t_up;
        this->texture_sides = &t_sides;
        this->texture_projectile = &t_proj;

        this->shape.setFillColor(sf::Color::Transparent);
        this->shape.setOutlineColor(sf::Color::Transparent);

        this->sprite.setTexture(*texture_down);
        
        this->klatkaStruktura = sf::IntRect(0, 0, 32, 32); 
        this->sprite.setTextureRect(this->klatkaStruktura);
        this->sprite.setOrigin(16.0f, 16.0f);
        this->sprite.setScale(1.75f, 1.75f);
        
        this->sprite.setPosition(x + 20.0f, y + 20.0f);
        this->hp = 5;
        this->startPosition = sf::Vector2f(x, y);
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
        {
            this->shape.move(recoilVelocity * deltaTime);
            recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
            recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

            if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
                recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }

        sf::Vector2f dir = getDirectionToPlayer();
        float czasOdStrzalu = shootClock.getElapsedTime().asSeconds();

        if (czasOdStrzalu < 2.2f && recoilVelocity.x == 0.0f && recoilVelocity.y == 0.0f)
        {
            this->shape.move(dir * speed * deltaTime);
        }
        int szerokosc_klatki=32;
        int wysokosc_klatki=32;

        if (std::abs(dir.x) > std::abs(dir.y))
        {
            this->sprite.setTexture(*texture_sides);
            patrzy_wLewo = (dir.x < 0.0f);

            szerokosc_klatki=46;
            wysokosc_klatki=37;
        }
        else
        {
            if (dir.y < 0.0f)
            {
                this->sprite.setTexture(*texture_up);
            }
            else
            {
                this->sprite.setTexture(*texture_down);
            }
            patrzy_wLewo = false;

            szerokosc_klatki=32;
            wysokosc_klatki=35;
        }

        if (czasOdStrzalu >= 2.2f)
        {
            czasAnimacji += deltaTime;
            if (czasAnimacji >= czas_na_klatke)
            {
                czasAnimacji = 0.0f;
                if (aktualna_klatka < 3) aktualna_klatka = 3;
                else if (aktualna_klatka < 5) aktualna_klatka++;
            }
        }
        else
        {
            czasAnimacji += deltaTime;
            if (czasAnimacji >= 0.2f)
            {
                czasAnimacji = 0.0f;
                aktualna_klatka = (aktualna_klatka + 1) % 3;
            }
        }

        this->klatkaStruktura.top = 0;
        this->klatkaStruktura.height = wysokosc_klatki;
        this->sprite.setScale(1.75f, 1.75f); 

        if (!patrzy_wLewo && this->sprite.getTexture() == texture_sides)
        {
            this->klatkaStruktura.left = (aktualna_klatka * szerokosc_klatki) + szerokosc_klatki;
            this->klatkaStruktura.width = -szerokosc_klatki; // Lustrzane odbicie dla chodzenia w lewo
        }
        else
        {
            this->klatkaStruktura.left = aktualna_klatka * szerokosc_klatki;
            this->klatkaStruktura.width = szerokosc_klatki;
        }

        this->sprite.setTextureRect(this->klatkaStruktura);
        this->sprite.setPosition(this->shape.getPosition().x + 20.0f, this->shape.getPosition().y + 20.0f);

        if (czasOdStrzalu >= 3.0f)
        {
            if (player != nullptr)
            {
                sf::Vector2f skeletonCenter(this->shape.getPosition().x + 20.0f, this->shape.getPosition().y + 20.0f);
                sf::Vector2f playerCenter(player->getBounds().left + (player->getBounds().width / 2.0f), player->getBounds().top + (player->getBounds().height / 2.0f));
                sf::Vector2f shootDir = playerCenter - skeletonCenter;


                float length = std::sqrt(shootDir.x * shootDir.x + shootDir.y * shootDir.y);
                sf::Vector2f spawnPos = skeletonCenter;

                if (length != 0.0f)
                {
                    spawnPos += (shootDir / length) * 38.0f;

                    recoilVelocity = -(shootDir / length) * 300.0f;
                }
                 worldObjects.push_back(new Projectile(*texture_projectile, spawnPos.x, spawnPos.y, shootDir));

            }
            aktualna_klatka = 0;
            shootClock.restart();
        }
    }
};

// KLASA: POCISK BUMERANGU
class BoomerangProjectile : public Game
{
private:
    sf::Sprite sprite;
    sf::CircleShape fallbackShape; 
    bool hasTexture = false;

    std::vector<Enemy*> targets;   
    int currentTargetIndex = 0;    
    bool returningToPlayer = false;
    float speed = 280.0f;        
    int damage = 3;                

public:
    BoomerangProjectile(sf::Texture* tex, sf::Vector2f startPos, const std::vector<Game*>& worldObjects)
    {
        if (tex != nullptr) {
            sprite.setTexture(*tex);
            sprite.setOrigin(tex->getSize().x / 2.0f, tex->getSize().y / 2.0f);
            sprite.setPosition(startPos);
            hasTexture = true;
        } else {
            fallbackShape.setRadius(12.0f);
            fallbackShape.setFillColor(sf::Color::Yellow);
            fallbackShape.setOrigin(12.0f, 12.0f);
            fallbackShape.setPosition(startPos);
        }

        for (auto* obj : worldObjects) {
            Enemy* e = dynamic_cast<Enemy*>(obj);
            if (e != nullptr && !e->isDead()) {
                targets.push_back(e);
            }
        }

        if (targets.empty()) {
            returningToPlayer = true;
        }
    }

    void update(float deltaTime) override
    {
        if (hasTexture) sprite.rotate(800.0f * deltaTime);
        else fallbackShape.rotate(800.0f * deltaTime);

        sf::Vector2f currentPos = hasTexture ? sprite.getPosition() : fallbackShape.getPosition();
        sf::Vector2f targetPos;

        if (!returningToPlayer) {
            if (currentTargetIndex >= targets.size()) {
                returningToPlayer = true;
                return;
            }

            Enemy* currentEnemy = targets[currentTargetIndex];
            
            if (currentEnemy == nullptr || currentEnemy->isDead()) {
                currentTargetIndex++;
                if (currentTargetIndex >= targets.size()) {
                    returningToPlayer = true;
                }
                return;
            }

            targetPos = currentEnemy->getPosition();

            sf::Vector2f dir = targetPos - currentPos;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist > 5.0f) {
                dir /= dist;
                currentPos += dir * speed * deltaTime;
            } else {
                currentEnemy->takeDamage(damage);
                
                if (player != nullptr) {
                    sf::Vector2f knockbackDir = currentEnemy->getPosition() - player->getPosition();
                    float len = std::sqrt(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
                    if (len != 0.0f) knockbackDir /= len;
                    currentEnemy->applyKnockback(knockbackDir, 400.0f);
                }

                std::cout << "[BUMERANG] Trafiono wroga! HP wroga: " << currentEnemy->getHP() << "\n";

                currentTargetIndex++;
                if (currentTargetIndex >= targets.size()) {
                    returningToPlayer = true;
                }
            }
        }
        else {
            if (player == nullptr) return;

            sf::FloatRect pBounds = player->getBounds();
            targetPos = sf::Vector2f(pBounds.left + pBounds.width / 2.0f, pBounds.top + pBounds.height / 2.0f);

            sf::Vector2f dir = targetPos - currentPos;
            float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

            if (dist > 10.0f) {
                dir /= dist;
                currentPos += dir * speed * deltaTime;
            } else {
                damage = -1; 
            }
        }

        if (hasTexture) sprite.setPosition(currentPos);
        else fallbackShape.setPosition(currentPos);
    }

    void draw(sf::RenderWindow& window) override
    {
        if (hasTexture) window.draw(sprite);
        else window.draw(fallbackShape);
    }

    sf::FloatRect getBounds() override
    {
        return hasTexture ? sprite.getGlobalBounds() : fallbackShape.getGlobalBounds();
    }

    bool isFinished() const { return damage == -1; }
};