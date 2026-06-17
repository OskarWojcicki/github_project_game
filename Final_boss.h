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


class HealthBar {
private:
    sf::RectangleShape background; // Szare tło
    sf::RectangleShape foreground; // Czerwony pasek
    float maxHp;
    

public:
    HealthBar(float width, float height, float max) : maxHp(max) {
        // Ustawienia tła
        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(50, 50, 50)); // Ciemnoszary

        // Ustawienia paska
        foreground.setSize(sf::Vector2f(width, height));
        foreground.setFillColor(sf::Color::Red);
    }

    void update(float currentHp, sf::Vector2f position) {
        // Obliczamy procent zdrowia (od 0.0 do 1.0)
        float percent = currentHp / maxHp;
        if (percent < 0) percent = 0;

        // Skalujemy szerokość paska
        foreground.setSize(sf::Vector2f(background.getSize().x * percent, background.getSize().y));

        // Ustawiamy pozycję (np. nad głową bossa)
        background.setPosition(position.x, position.y - 20); // 20px nad bossem
        foreground.setPosition(position.x, position.y - 20);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(foreground);
    }
};

// ============================================================================
// 2. KLASA GŁÓWNEGO BOSSA
// ============================================================================
class Final_boss : public Enemy
{
private:
    // Stan bosa
    Boss_state aktualnyEtap;
    bool isEyeOpen; // Czy oko jest otwarte (podatność)
    bool oczy_zrespowane;
    
    // UI i pomocnicze
    HealthBar hpBar;
    sf::Clock stateClock, attackClock, animClock, phaseTimer;
    float currentPhaseDuration;
    
    // Zasoby
    const sf::Texture& tex1;
    const sf::Texture& tex2;
    const sf::Texture& texOczu;
    const sf::Texture& texOgien;
    sf::Clock damageClock;
    
    // Parametry
    int maxHp;
    int klatka_szerokosc = 64;
    int klatka_wysokosc = 64;
    int aktualnyYOffset = 0;

public:
    Final_boss(const sf::Texture& t1, const sf::Texture& t2, const sf::Texture& tOczy, const sf::Texture& tOgien, float x, float y) 
        : Enemy(x, y, 35.0f), tex1(t1), tex2(t2), texOczu(tOczy),texOgien(tOgien), hpBar(100.0f, 15.0f, 40.0f)
    {
        this->maxHp = 40;
        this->hp = this->maxHp;
        this->speed = 45.0f;
        this->aktualnyEtap = Boss_state::Transformacja;
        
        // Logika oka
        this->isEyeOpen = true; 
        this->oczy_zrespowane = false;
        this->currentPhaseDuration = 2.0f; // Startowy czas fazy

        this->sprite.setTexture(tex1);
        this->shape.setSize(sf::Vector2f(48.0f, 48.0f));
        this->shape.setPosition(x, y);
        this->sprite.setPosition(x, y);
        this->sprite.setScale(1.5f, 1.5f);
        this->startPosition = sf::Vector2f(x, y);
        
        this->shape.setFillColor(sf::Color::Transparent); // Przezroczysty środek
this->shape.setOutlineColor(sf::Color::Red);       // Czerwona ramka
this->shape.setOutlineThickness(2.0f);

        this->invincibilityDuration = 0.5f; // Czas nietykalności po otrzymaniu dmg
    }

    // Funkcja sprawdzająca czy można bić bossa
    bool isVulnerable() const {
        if (aktualnyEtap == Boss_state::Faza_Oka) {
            return isEyeOpen; // Tylko gdy oko otwarte
        }
        return true; // W innych fazach jest podatny
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        if (player != nullptr && this->getBounds().intersects(player->getBounds()))
    {
        // Sprawdzamy czy minęła sekunda od ostatniego uderzenia
        if (damageClock.getElapsedTime().asSeconds() >= 1.0f) 
        {
            // Obliczamy kierunek odpychania (od Boss -> Gracz)
            sf::Vector2f dir = player->getPosition() - this->shape.getPosition();
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len > 0) dir /= len; // Normalizacja wektora

            // Zadaj obrażenia i odepchnij
            player->takeDamage(1); 
            player->applyKnockback(dir, 600.0f); // Siła 300.0f
            
            damageClock.restart();
        }
    }
        recoilVelocity = sf::Vector2f(0.0f, 0.0f);
        float czas_w_stanie = stateClock.getElapsedTime().asSeconds();
        
        int klatka = 0;
        const sf::Texture* aktywnaTex = &tex1;

        // Logika faz
        switch (aktualnyEtap)
        {
            case Boss_state::Transformacja:
                klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.2f) % 17;
                aktywnaTex = &tex1;
                aktualnyYOffset = 0;
                
                if (czas_w_stanie > 4.0f) {
                    this->shape.setSize(sf::Vector2f(160.0f, 160.0f));
                    this->sprite.setScale(2.5f, 2.5f);
                    aktualnyEtap = Boss_state::Faza_Oka;
                    stateClock.restart();
                    animClock.restart();
                    phaseTimer.restart();
                }
                break;
                
            case Boss_state::Faza_Oka:
            {
                // --- LOSOWE PRZEŁĄCZANIE (OKO / PELERYNKA) ---
                if (phaseTimer.getElapsedTime().asSeconds() >= currentPhaseDuration) {
                    isEyeOpen = !isEyeOpen;
                    phaseTimer.restart();
                    
                    if (isEyeOpen) {
                        currentPhaseDuration = 1.5f + static_cast<float>(rand() % 150) / 100.0f; // 1.5-3s otwarte
                    } else {
                        currentPhaseDuration = 3.0f + static_cast<float>(rand() % 300) / 100.0f; // 3-6s zamknięte
                    }
                }

                aktywnaTex = &tex2;

                if (isEyeOpen) {
                    // OTWARTY - klatki 0-14
                    klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.15f) % 15;
                    aktualnyYOffset = 128; 
                } else {
                    // ZAMKNIĘTY/PELERYNKA - klatki 15-16
                    klatka = 15 + (static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.4f) % 2);
                    aktualnyYOffset = 64; 
                }

                float currentSpeed = isEyeOpen ? (speed * 0.5f) : (speed * 1.8f);
                this->shape.move(getDirectionToPlayer() * currentSpeed * deltaTime);                
                if (this->hp <= maxHp / 2) { 
                    aktualnyEtap = Boss_state::Faza_PolHp; 
                    isEyeOpen = true; // W kolejnej fazie znowu podatny
                    stateClock.restart(); 
                    attackClock.restart(); 
                }
                break;
            }

            case Boss_state::Faza_PolHp:
                klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.2f) % 17;
                aktywnaTex = &tex2;
                aktualnyYOffset = 128;

                if (!oczy_zrespowane) {
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + (rand() % 400), 100.0f + (rand() % 300)));
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + (rand() % 400), 100.0f + (rand() % 300)));
                    oczy_zrespowane = true;
                }
                this->shape.move(getDirectionToPlayer() * (speed * 1.25f) * deltaTime);
                
                if (this->hp <= maxHp / 4) {
                    aktualnyEtap = Boss_state::Faza_zadymka;
                    animClock.restart();
                }
                break;

            case Boss_state::Faza_zadymka:
                klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.08f) % 17;
                aktywnaTex = &tex2;
                aktualnyYOffset = 128;
                
                this->shape.move(getDirectionToPlayer() * (speed * 1.7f) * deltaTime);
                this->sprite.setColor(sf::Color(255, 150, 150));
                if (attackClock.getElapsedTime().asSeconds() >= 2.5f) {

                    sf::IntRect bossRect(0, 0, 23, 31);

                    worldObjects.push_back(new Projectile(texOgien, shape.getPosition().x, shape.getPosition().y, getDirectionToPlayer(),bossRect, 4.0f));
                    attackClock.restart();
                }
                break;
        }

        // Aktualizacja UI i Sprite
        hpBar.update(static_cast<float>(this->hp), this->shape.getPosition());
        this->sprite.setTexture(*aktywnaTex);
        
        // Zabezpieczenie przed wyjściem poza teksturę
        if (aktualnyYOffset + klatka_wysokosc > (int)aktywnaTex->getSize().y) aktualnyYOffset = 0;
    
        this->sprite.setTextureRect(sf::IntRect(klatka * klatka_szerokosc, aktualnyYOffset, klatka_szerokosc, klatka_wysokosc));
        this->sprite.setPosition(this->shape.getPosition());
    }

    void draw(sf::RenderWindow& window) override { 
        window.draw(this->sprite); 
        hpBar.draw(window); 
    }
    
    void takeDamage(int amount) override {
        // Blokada bicia, jeśli oko zamknięte (tylko w fazie oka)
        if (!isVulnerable()) return;

        if(!isInvincible()) {
            this->hp -= amount;
            if(this->hp < 0) this->hp = 0;
            this->invincibilityTimer = this->invincibilityDuration;
        }
    }
};