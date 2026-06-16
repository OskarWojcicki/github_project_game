#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath> 
#include "Enemies.h"

enum class Boss_state
{
    Transformacja,
    Faza_Oka,
    Faza_PolHp,
    Faza_zadymka
};

// ============================================================================
// 1. KLASA STACJONARNYCH OCZU
// ============================================================================
class Flying_eyes : public Enemy
{
private:
    sf::Clock animClock;
    sf::Clock laserCycleClock;      
    sf::Clock damageClock;          
    
    int klatka_szerokosc = 32;
    float maxLaserDystans = 400.0f; 
    
    sf::Vector2f stalyKierunekLaseru; 
    bool laserAktywny = false;       

public:
    Flying_eyes(const sf::Texture& tex, float x, float y) : Enemy(x, y, 0.0f)
    {
        this->hp = 2;
        this->speed = 0.0f; 
        this->sprite.setTexture(tex);

        this->sprite.setTextureRect(sf::IntRect(0, 0, klatka_szerokosc, 32));
        this->sprite.setScale(2.0f, 2.0f);
        this->shape.setSize(sf::Vector2f(64.0f, 64.0f));
        this->shape.setPosition(x, y);
        this->sprite.setOrigin(klatka_szerokosc / 2.0f, 16.0f);

        if (player != nullptr)
        {
            sf::Vector2f startPozycja = this->shape.getPosition() + sf::Vector2f(32.0f, 32.0f);
            sf::Vector2f graczPoz = sf::Vector2f(player->getBounds().left + player->getBounds().width / 2.0f, player->getBounds().top + player->getBounds().height / 2.0f);
            
            sf::Vector2f dir = graczPoz - startPozycja;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len != 0.0f) {
                this->stalyKierunekLaseru = dir / len; 
            } else {
                this->stalyKierunekLaseru = sf::Vector2f(0.0f, 1.0f); 
            }
        }
        else
        {
            this->stalyKierunekLaseru = sf::Vector2f(0.0f, 1.0f);
        }
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        this->sprite.setPosition(
            this->shape.getPosition().x + (this->shape.getSize().x / 2.0f),
            this->shape.getPosition().y + (this->shape.getSize().y / 2.0f)
        );

        int aktualna_klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.12f) % 3;
        this->sprite.setTextureRect(sf::IntRect(aktualna_klatka * klatka_szerokosc, 0, klatka_szerokosc, 32));

        float czasCyklu = laserCycleClock.getElapsedTime().asSeconds();
        if (czasCyklu < 2.0f) { laserAktywny = true; }
        else if (czasCyklu < 4.0f) { laserAktywny = false; }
        else { laserCycleClock.restart(); }

        if (laserAktywny && player != nullptr && !player->isInvincible())
        {
            sf::Vector2f startPozycja = this->shape.getPosition() + sf::Vector2f(32.0f, 32.0f);
            sf::Vector2f graczPoz = sf::Vector2f(player->getBounds().left + player->getBounds().width / 2.0f, player->getBounds().top + player->getBounds().height / 2.0f);

            sf::Vector2f wektorDoGracza = graczPoz - startPozycja;
            float rzutNaKierunek = wektorDoGracza.x * stalyKierunekLaseru.x + wektorDoGracza.y * stalyKierunekLaseru.y;

            if (rzutNaKierunek >= 0.0f && rzutNaKierunek <= maxLaserDystans)
            {
                sf::Vector2f najblizszyPunkt = startPozycja + stalyKierunekLaseru * rzutNaKierunek;
                float dx = graczPoz.x - najblizszyPunkt.x;
                float dy = graczPoz.y - najblizszyPunkt.y;
                float odlegloscOdLasera = std::sqrt(dx * dx + dy * dy);

                if (odlegloscOdLasera < 15.0f)
                {
                    if (damageClock.getElapsedTime().asSeconds() >= 0.5f)
                    {
                        player->takeDamage(1);
                        player->applyKnockback(stalyKierunekLaseru, 200.0f);
                        damageClock.restart();
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(this->sprite);
    }
};

// ============================================================================
// 2. KLASA GŁÓWNEGO BOSSA
// ============================================================================
class Final_boss : public Enemy
{
private:
    Boss_state aktualnyEtap;
    sf::Clock stateClock, attackClock, animClock;
    const sf::Texture& tex1;
    const sf::Texture& tex2;
    const sf::Texture& texOczu;
    bool oko_otwarte, oczy_zrespowane;
    float czas_na_oko;
    int maxHp;
    int klatka_szerokosc = 64;
    int klatka_wysokosc = 64;

public:
    Final_boss(const sf::Texture& t1, const sf::Texture& t2, const sf::Texture& tOczy, float x, float y) 
        : Enemy(x, y, 35.0f), tex1(t1), tex2(t2), texOczu(tOczy)
    {
        this->maxHp = 40; // Twoje oryginalne HP
        this->hp = this->maxHp;
        this->speed = 45.0f;
        this->aktualnyEtap = Boss_state::Transformacja;
        this->oko_otwarte = false;
        this->oczy_zrespowane = false;
        this->czas_na_oko = 2.5f;

        this->sprite.setTexture(tex1);
        this->shape.setSize(sf::Vector2f(48.0f, 48.0f));
        this->shape.setPosition(x, y);
        this->sprite.setPosition(x, y);
        this->sprite.setTextureRect(sf::IntRect(0, 0, klatka_szerokosc, klatka_wysokosc));
        this->sprite.setScale(1.5f, 1.5f);
        this->startPosition = sf::Vector2f(x, y);
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        float czas_w_stanie = stateClock.getElapsedTime().asSeconds();
        
        // Animacja
        int klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.2f) % 34;
        if (aktualnyEtap == Boss_state::Faza_zadymka)
             klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.08f) % 34;

        // 1. Wybór tekstury
        const sf::Texture* aktywnaTex = (klatka < 17) ? &tex1 : &tex2;
        this->sprite.setTexture(*aktywnaTex);

        // 2. Ustalenie wysokości (Y)
        int yOffset = 0;
        switch (aktualnyEtap)
        {
            case Boss_state::Transformacja: yOffset = 0; break;
            case Boss_state::Faza_Oka:      yOffset = oko_otwarte ? 128 : 64; break;
            case Boss_state::Faza_PolHp:    yOffset = 128; break;
            case Boss_state::Faza_zadymka:  yOffset = 128; break;
        }

        // 3. BEZPIECZNIK: Sprawdzenie, czy tekstura w ogóle ma tyle pikseli w pionie
        if (yOffset + klatka_wysokosc > (int)aktywnaTex->getSize().y)
        {
            yOffset = 0; // Jeśli plik jest za krótki, zresetuj do zera, żeby nie znikał
        }

        // 4. BEZPIECZNIK: Sprawdzenie, czy X nie wychodzi poza szerokość pliku
        // Jeśli tekstura jest węższa niż 17 klatek, używamy modulo
        int iloscKlatekWPoziomie = aktywnaTex->getSize().x / klatka_szerokosc;
        if (iloscKlatekWPoziomie < 1) iloscKlatekWPoziomie = 1;
        
        int xPos = (klatka % iloscKlatekWPoziomie) * klatka_szerokosc;
        this->sprite.setTextureRect(sf::IntRect(xPos, yOffset, klatka_szerokosc, klatka_wysokosc));

        // Logika faz
        switch (aktualnyEtap)
        {
            case Boss_state::Transformacja:
                if (czas_w_stanie > 4.0f) {
                    this->shape.setSize(sf::Vector2f(96.0f, 96.0f));
                    this->sprite.setScale(2.5f, 2.5f);
                    aktualnyEtap = Boss_state::Faza_Oka;
                    stateClock.restart();
                }
                break;
                
            case Boss_state::Faza_Oka:
                if (czas_w_stanie >= czas_na_oko) { oko_otwarte = !oko_otwarte; stateClock.restart(); }
                this->shape.move(getDirectionToPlayer() * speed * deltaTime);
                
                // Powrót do Twojego oryginalnego HP
                if (this->hp <= maxHp / 2) { 
                    aktualnyEtap = Boss_state::Faza_PolHp; 
                    oko_otwarte = true; 
                    stateClock.restart(); 
                    attackClock.restart(); 
                }
                break;

            case Boss_state::Faza_PolHp:
                if (!oczy_zrespowane) {
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + (rand() % 400), 100.0f + (rand() % 300)));
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + (rand() % 400), 100.0f + (rand() % 300)));
                    oczy_zrespowane = true;
                }
                this->shape.move(getDirectionToPlayer() * (speed * 1.25f) * deltaTime);
                
                // Powrót do Twojego oryginalnego HP
                if (this->hp <= maxHp / 4) aktualnyEtap = Boss_state::Faza_zadymka;
                break;

            case Boss_state::Faza_zadymka:
                this->shape.move(getDirectionToPlayer() * (speed * 1.7f) * deltaTime);
                this->sprite.setColor(sf::Color(255, 150, 150));
                if (attackClock.getElapsedTime().asSeconds() >= 1.3f) {
                    worldObjects.push_back(new Projectile(*this->sprite.getTexture(), shape.getPosition().x, shape.getPosition().y, getDirectionToPlayer()));
                    attackClock.restart();
                }
                break;
        }
        this->sprite.setPosition(this->shape.getPosition());
    }

    void draw(sf::RenderWindow& window) override { window.draw(this->sprite); }
    
    void takeDamage(int amount) override {
        if(aktualnyEtap == Boss_state::Transformacja) return;
        if(aktualnyEtap == Boss_state::Faza_Oka && !oko_otwarte) return;
        if(!isInvincible()) {
            this->hp -= amount;
            if(this->hp < 0) this->hp = 0;
            this->invincibilityTimer = this->invincibilityDuration;
        }
    }
};