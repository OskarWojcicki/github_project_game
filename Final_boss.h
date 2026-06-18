#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath> 
#include "Enemies.h"

        // frames[0] = sf::IntRect(0, 0, 16, 10);   // Np. Oko
        // frames[1] = sf::IntRect(13, 0, 17, 16);  // Np. Oko z kulką
        // frames[2] = sf::IntRect(30, 0, 15, 14);  // Np. Oko z małą kulką
        // frames[3] = sf::IntRect(47, 0, 12, 94);  // Np. Laser ciemny
        // frames[4] = sf::IntRect(60, 0, 14, 95);  // Np. Laser jasny
enum class Boss_state
{
    Transformacja,
    Faza_Oka,
    Faza_PolHp,
    Faza_zadymka
};

// 1. KLASA STACJONARNYCH OCZU

class Flying_eyes : public Enemy
{
private:
    const float SCALE = 3.5f; 
    sf::Clock animClock;
    sf::Clock laserCycleClock;
    sf::Vector2f startPos; 
    sf::IntRect frames[6]; 
    bool laserAktywny = false;

public:
    Flying_eyes(const sf::Texture& tex, float x, float y) : Enemy(x, y, 0.0f)
    {
        this->startPos = sf::Vector2f(x, y);
        this->sprite.setTexture(tex);
        this->hp = 2;

        // Definicje klatek
        frames[0] = sf::IntRect(0, 0, 16, 10);
        frames[1] = sf::IntRect(13, 0, 17, 16);
        frames[2] = sf::IntRect(30, 0, 15, 14);
        frames[3] = sf::IntRect(47, 0, 12, 94);
        frames[4] = sf::IntRect(60, 0, 14, 95);

        this->sprite.setScale(SCALE, SCALE);
        this->sprite.setOrigin(7.0f, 47.0f); 
        this->sprite.setPosition(startPos);
        
        // Ustawienie przezroczystości (nie widzisz ramki w finalnej grze)
        this->shape.setFillColor(sf::Color::Transparent); 
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        // 1. Reset na start każdej klatki 
        this->shape.setSize(sf::Vector2f(0.0f, 0.0f));
        this->shape.setPosition(-1000.0f, -1000.0f); 
        laserAktywny = false;

        float czas = laserCycleClock.getElapsedTime().asSeconds();
        int idx = 0;

        // 2. Logika stanów
        if (czas < 2.0f) {
            // FAZA OKA
            laserAktywny = false;
            idx = (static_cast<int>(animClock.getElapsedTime().asSeconds() * 4.0f)) % 3;
        } 
        else {
            // FAZA LASERA
            laserAktywny = true;
            idx = 3 + (static_cast<int>(animClock.getElapsedTime().asSeconds() * 4.0f) % 2);
            
            // Wymiary hitboksa
            float w = 14.0f * SCALE;
            float h = 95.0f * SCALE;
            this->shape.setSize(sf::Vector2f(w, h));
            this->shape.setOrigin(w / 2.0f, 0.0f);

            float offsetX = 20.0f;  
            float offsetY = -120.0f; 

            // Ustawiamy pozycję hitboksa względem startPos
            this->shape.setPosition(startPos.x + offsetX, startPos.y + offsetY);
        }

        if (czas > 4.0f) laserCycleClock.restart();

        // 3. Aktualizacja wizualna
        this->sprite.setTextureRect(frames[idx]);
        this->sprite.setPosition(startPos);

        // 4. Kolizja 
        if (laserAktywny && player != nullptr && this->shape.getGlobalBounds().intersects(player->getBounds()))
        {
            player->takeDamage(1);
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(this->sprite);
        
    }
};

class HealthBar {
private:
    sf::RectangleShape background; 
    sf::RectangleShape foreground; 
    float maxHp;
    sf::Text bossLabel; // Dodaj to

public:
    HealthBar(float width, float height, float max, sf::Font& font) : maxHp(max) {
        // Tło
        background.setSize(sf::Vector2f(width, height));
        background.setFillColor(sf::Color(50, 50, 50)); 

        // Pasek
        foreground.setSize(sf::Vector2f(width, height));
        foreground.setFillColor(sf::Color::Red);

        // Ustawienia tekstu
        bossLabel.setFont(font);
        bossLabel.setString("BOSS");
        bossLabel.setCharacterSize(18);
        bossLabel.setFillColor(sf::Color::White);
    }

    void update(float currentHp, sf::Vector2f screenPosition) {
        float percent = currentHp / maxHp;
        if (percent < 0) percent = 0;

        foreground.setSize(sf::Vector2f(background.getSize().x * percent, background.getSize().y));

        // Teraz pozycja jest przekazywana bezpośrednio (na sztywno)
        background.setPosition(screenPosition);
        foreground.setPosition(screenPosition);
        
        // Tekst nad paskiem
        bossLabel.setPosition(screenPosition.x, screenPosition.y - 25);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background);
        window.draw(foreground);
        window.draw(bossLabel); // Rysujemy tekst
    }
};

// ============================================================================
// 2. KLASA GŁÓWNEGO BOSSA
// ============================================================================
class Final_boss : public Enemy
{
protected:
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
    int klatka_szerokosc = 64;
    int klatka_wysokosc = 64;
    int aktualnyYOffset = 0;
    int maxHp;

public:
    Final_boss(const sf::Texture& t1, const sf::Texture& t2, const sf::Texture& tOczy, const sf::Texture& tOgien,sf::Font& font, float x, float y) 
        : Enemy(x, y, 35.0f), tex1(t1), tex2(t2), texOczu(tOczy),texOgien(tOgien), hpBar(100.0f, 15.0f, 40.0f,font)
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
                    this->shape.setSize(sf::Vector2f(80.0f, 130.0f));
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

                float currentSpeed = isEyeOpen ? (speed * 0.5f) : (speed * 1.2f);
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

                if (!oczy_zrespowane)
                {

                    float offsetX = 7.0f;
                    float offsetY = 47.0f;
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + offsetX + (rand() % 400), 100.0f + offsetY + (rand() % 300)));
                    worldObjects.push_back(new Flying_eyes(this->texOczu, 150.0f + offsetX + (rand() % 400), 100.0f + offsetY + (rand() % 300)));
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

                sf::Vector2f bossSize = shape.getSize();

                float centerX = shape.getPosition().x + (bossSize.x / 2.0f);
                float centerY = shape.getPosition().y + (bossSize.y / 2.0f);

                if (attackClock.getElapsedTime().asSeconds() >= 2.5f) {

                    sf::IntRect bossRect(0, 0, 14, 14);

                    worldObjects.push_back(new Projectile(texOgien, centerX, centerY, getDirectionToPlayer(),bossRect, 1.5f));
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