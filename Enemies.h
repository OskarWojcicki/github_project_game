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
    sf::Sprite sprite;

public:
    Projectile(const sf::Texture& tex, float x, float y, sf::Vector2f direction)
    {
        speed = 200.0f; // Pocisk leci dość szybko
        
        // bulletShape.setRadius(6.0f);
        // bulletShape.setFillColor(sf::Color::White); // Biała kuleczka
        // bulletShape.setOutlineColor(sf::Color::Black);
        // bulletShape.setOutlineThickness(1.0f);
        // bulletShape.setOrigin(6.0f, 6.0f); // Środek kółka
        // bulletShape.setPosition(x, y);

        this->sprite.setTexture(tex);

        this->sprite.setTextureRect(sf::IntRect(0, 0, 16, 9));
        this->sprite.setOrigin(15.0f, 4.0f); // Środek strzały
        this->sprite.setPosition(x, y);
        this->sprite.setScale(2.0f, 2.0f);

        // Normalizujemy wektor kierunku pocisku
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0)
        {
            velocity = (direction / length) * speed;
            float angle = (std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f) + 180.0f;
            this->sprite.setRotation(angle);
        }
    }

    // Dopasowane do sygnatury z Game.h (przyjmuje deltaTime)
    void update(float deltaTime) override
    {
        // bulletShape.move(velocity * deltaTime);
        sprite.move(velocity * deltaTime);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sprite);
    }

    // Wymagane przez silnik (pocisk nie jest ścianą)
    bool isSolid() const override { return false; }
    
    // Dopasowane do sygnatury z Game.h (brak słówka const)
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
        window.draw(this->sprite);
    }

    void applyKnockback(sf::Vector2f direction, float force) {
    recoilVelocity = direction * force;
    }

    int getHP() const { return hp; }
    bool isInvincible() const { return invincibilityTimer > 0.0f; }

    virtual void takeDamage(int amount) {
        if (!isInvincible()) { // Obrażenia wchodzą tylko, gdy wróg NIE JEST odporny
            hp -= amount;
            if (hp < 0) hp = 0;
            invincibilityTimer = invincibilityDuration; // Włączenie ochrony
        }
    }
    
    bool isDead() const { return hp <= 0; }

    // --- METODA UPDATE DLA ODLICZANIA CZASU ---
    void updateInvincibility(float deltaTime) {
        if (invincibilityTimer > 0.0f) {
            invincibilityTimer -= deltaTime;
        }
    }
    sf::Vector2f getStartPosition() const { return startPosition; }

protected:
    sf::Vector2f recoilVelocity = sf::Vector2f(0.0f, 0.0f); // Aktualna prędkość odrzutu
    float friction = 8.0f;
    int hp;
    float invincibilityTimer = 0.0f;       // Aktualny czas odporności
    float invincibilityDuration = 0.4f; // Jak długo wróg jest odporny po ciosie (np. 0.4 sekundy)
    sf::Vector2f startPosition;
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
        this->shape.setOutlineColor(sf::Color::Transparent); // Ukrywamy też ramkę debugowania

        this->sprite.setTexture(tex_down);

        int szerokosc_moblina = 32;
        int wysokosc_moblina = 40;
        
        this->klatkaStruktura = sf::IntRect(0, 0, szerokosc_moblina, wysokosc_moblina);
        this->sprite.setTextureRect(this->klatkaStruktura);

        // Środek sprajta ustawiamy na jego centrum
        this->sprite.setOrigin(szerokosc_moblina/ 2.0f, wysokosc_moblina / 2.0f);
        
        // Ponieważ obiekt ma 32x32, a hitboxy masz na 40x40 lub 48x48, 
        // możemy go delikatnie przeskalować, żeby lepiej pasował do świata gry
        this->sprite.setScale(1.75f, 1.75f); 

        // Pozycja startowa sprajta na środku hitboxu shape
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
// A. Jeśli potwór dostał cios i ma prędkość odrzutu – przesuń go siłą odrzutu
    if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
    {
        this->shape.move(recoilVelocity * deltaTime);

        // Wyhamowywanie odrzutu przez tarcie
        recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
        recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

        // Jeśli odrzut jest już ledwo widoczny, zatrzymaj go całkowicie
        if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
        {
            recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }
    }
    // B. Jeśli nie ma odrzutu, potwór wykonuje swój normalny ruch w stronę gracza
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
    this->klatkaStruktura.left = aktualna_klatka * klatka_szerokosc; // Skok mnoży się przez aktualną szerokość!
    
    this->sprite.setTextureRect(this->klatkaStruktura);

    // Aktualizacja origin na wypadek, gdyby wymiary się zmieniły
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

        // Środek sprajta ustawiamy na jego centrum
        this->sprite.setOrigin(szerokosc_slima / 2.0f, wysokosc_slima / 2.0f);
        
        // Ponieważ obiekt ma 32x32, a hitboxy masz na 40x40 lub 48x48, 
        // możemy go delikatnie przeskalować, żeby lepiej pasował do świata gry
        this->sprite.setScale(1.2f, 1.2f); 

        // Pozycja startowa sprajta na środku hitboxu shape
        this->sprite.setPosition(
            x + (this->shape.getSize().x / 2.0f),
            y + (this->shape.getSize().y / 2.0f)
        );
        this->hp = 3;
        this->startPosition = sf::Vector2f(x, y);
        }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
// A. Jeśli potwór dostał cios i ma prędkość odrzutu – przesuń go siłą odrzutu
    if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
    {
        this->shape.move(recoilVelocity * deltaTime);

        // Wyhamowywanie odrzutu przez tarcie
        recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
        recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

        // Jeśli odrzut jest już ledwo widoczny, zatrzymaj go całkowicie
        if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
        {
            recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }
    }
    // B. Jeśli nie ma odrzutu, potwór wykonuje swój normalny ruch w stronę gracza
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

        // Ukrywamy debugowy kwadrat
        this->shape.setFillColor(sf::Color::Transparent);
        this->shape.setOutlineColor(sf::Color::Transparent);

        // Ustawiamy domyślną teksturę na start (patrzy w dół)
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
        // 1. OBSŁUGA PŁYNNEGO ODRZUTU (Wywoływane w każdej klatce)
        if (recoilVelocity.x != 0.0f || recoilVelocity.y != 0.0f)
        {
            this->shape.move(recoilVelocity * deltaTime);
            recoilVelocity.x -= recoilVelocity.x * friction * deltaTime;
            recoilVelocity.y -= recoilVelocity.y * friction * deltaTime;

            if (std::sqrt(recoilVelocity.x * recoilVelocity.x + recoilVelocity.y * recoilVelocity.y) < 10.0f)
                recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        }

        // 2. LOGIKA PORUSZANIA I WYBORU TEKSTURY
        sf::Vector2f dir = getDirectionToPlayer();
        float czasOdStrzalu = shootClock.getElapsedTime().asSeconds();

        // Jeśli łucznik nie naciąga łuku (czas < 2.2s) i nie ma odrzutu -> idzie
        if (czasOdStrzalu < 2.2f && recoilVelocity.x == 0.0f && recoilVelocity.y == 0.0f)
        {
            this->shape.move(dir * speed * deltaTime);
        }
        int szerokosc_klatki=32;
        int wysokosc_klatki=32;

        // Dynamicznie podmieniamy tekstury przekazane z maina
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

        // 3. SEKCJA ANIMACJI (0-2: chód, 3-5: napinanie łuku)
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

        // 4. USTAWIANIE STRUKTURY KLATKI
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

        // 5. STRZAŁ (Zostawiamy na razie Twoją białą kuleczkę jako pocisk)
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