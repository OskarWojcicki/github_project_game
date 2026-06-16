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
// 1. KLASA STACJONARNYCH OCZU - WIEŻYCZEK LASEROWYCH
// ============================================================================
// ============================================================================
// 1. KLASA STACJONARNYCH OCZU - PULSACYJNE WIEŻYCZKI LASEROWE (POPRAWIONA)
// ============================================================================
class Flying_eyes : public Enemy
{
private:
    sf::Clock animClock;
    sf::Clock laserCycleClock;      // Stoper kontrolujący fazy lasera (włączony/wyłączony)
    sf::Clock damageClock;          // Stoper zapobiegający biciu gracza w każdej klatce
    
    int klatka_szerokosc = 32;
    float maxLaserDystans = 400.0f; // Maksymalna długość lasera
    
    sf::Vector2f stalyKierunekLaseru; // Zapamiętany, niezmienny kierunek strzału
    bool laserAktywny = false;       // Czy w danej sekundzie laser w ogóle świeci i bije

public:
    Flying_eyes(const sf::Texture& tex, float x, float y) : Enemy(x, y, 0.0f)
    {
        this->hp = 2;
        this->speed = 0.0f; // Stacjonarne
        this->sprite.setTexture(tex);

        this->sprite.setTextureRect(sf::IntRect(0, 0, klatka_szerokosc, 32));
        this->sprite.setScale(2.0f, 2.0f);
        this->shape.setSize(sf::Vector2f(64.0f, 64.0f));
        this->shape.setPosition(x, y);
        this->sprite.setOrigin(klatka_szerokosc / 2.0f, 16.0f);

        // KLUCZOWE: Zapamiętujemy pozycję gracza TYLKO RAZ przy stworzeniu oka
        if (player != nullptr)
        {
            sf::Vector2f startPozycja = this->shape.getPosition() + sf::Vector2f(32.0f, 32.0f);
            sf::Vector2f graczPoz = sf::Vector2f(player->getBounds().left + player->getBounds().width / 2.0f, player->getBounds().top + player->getBounds().height / 2.0f);
            
            sf::Vector2f dir = graczPoz - startPozycja;
            float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
            if (len != 0.0f) {
                this->stalyKierunekLaseru = dir / len; // Znormalizowany wektor kierunku
            } else {
                this->stalyKierunekLaseru = sf::Vector2f(0.0f, 1.0f); // Domyślnie w dół, gdyby stał idealnie w oku
            }
        }
        else
        {
            this->stalyKierunekLaseru = sf::Vector2f(0.0f, 1.0f);
        }
    }

    void updateEnemyAI(std::vector<Game*>& worldObjects, float deltaTime) override
    {
        // Ustawienie pozycji grafiki oka
        this->sprite.setPosition(
            this->shape.getPosition().x + (this->shape.getSize().x / 2.0f),
            this->shape.getPosition().y + (this->shape.getSize().y / 2.0f)
        );

        // Animacja pulsowania oka (zawsze działa)
        int aktualna_klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.12f) % 3;
        this->sprite.setTextureRect(sf::IntRect(aktualna_klatka * klatka_szerokosc, 0, klatka_szerokosc, 32));

        // --- CYKL LASERA (2 sekundy świeci, 2 sekundy przerwy) ---
        float czasCyklu = laserCycleClock.getElapsedTime().asSeconds();
        if (czasCyklu < 2.0f)
        {
            laserAktywny = true;  // Laser strzela i rani
        }
        else if (czasCyklu < 4.0f)
        {
            laserAktywny = false; // Laser znika całkowicie, oko odpoczywa
        }
        else
        {
            laserCycleClock.restart(); // Reset cyklu po 4 sekundach
        }

        // --- SPRAWDZANIE OBRAŻEŃ (Tylko gdy laser jest aktywny!) ---
        if (laserAktywny && player != nullptr && !player->isInvincible())
        {
            sf::Vector2f startPozycja = this->shape.getPosition() + sf::Vector2f(32.0f, 32.0f);
            sf::Vector2f graczPoz = sf::Vector2f(player->getBounds().left + player->getBounds().width / 2.0f, player->getBounds().top + player->getBounds().height / 2.0f);

            // Obliczamy punkt rzutu pozycji gracza na prostą lasera (matematyczne sprawdzenie kolizji z linią)
            sf::Vector2f wektorDoGracza = graczPoz - startPozycja;
            float rzutNaKierunek = wektorDoGracza.x * stalyKierunekLaseru.x + wektorDoGracza.y * stalyKierunekLaseru.y;

            // Sprawdzamy, czy gracz znajduje się przed laserem i w obrębie jego maksymalnego zasięgu
            if (rzutNaKierunek >= 0.0f && rzutNaKierunek <= maxLaserDystans)
            {
                // Najbliższy punkt na linii lasera względem Linka
                sf::Vector2f najblizszyPunkt = startPozycja + stalyKierunekLaseru * rzutNaKierunek;
                
                // Odległość gracza od linii lasera
                float dx = graczPoz.x - najblizszyPunkt.x;
                float dy = graczPoz.y - najblizszyPunkt.y;
                float odlegloscOdLasera = std::sqrt(dx * dx + dy * dy);

                // Szerokość wiązki lasera (np. 15 pikseli bufora na dotknięcie)
                if (odlegloscOdLasera < 15.0f)
                {
                    // Przerwa w biciu (co 0.5 sekundy), żeby nie zabić Linka natychmiast
                    if (damageClock.getElapsedTime().asSeconds() >= 0.5f)
                    {
                        player->takeDamage(1);
                        std::cout << "Link przecial staly laser! HP: " << player->getHP() << "\n";
                        
                        // Odepchnięcie Linka w bok od lasera
                        player->applyKnockback(stalyKierunekLaseru, 200.0f);
                        damageClock.restart();
                    }
                }
            }
        }
    }

    void draw(sf::RenderWindow& window) override
    {
        // Rysujemy laser pod okiem TYLKO wtedy, gdy jest aktywny w cyklu
        if (laserAktywny && player != nullptr)
        {
            sf::Vector2f startPozycja = this->shape.getPosition() + sf::Vector2f(32.0f, 32.0f);
            // Koniec lasera wyliczany na stałe na podstawie zapamiętanego kierunku
            sf::Vector2f koniecLasera = startPozycja + stalyKierunekLaseru * maxLaserDystans;

            sf::Vertex laserLine[] =
            {
                sf::Vertex(startPozycja, sf::Color::Red),
                sf::Vertex(koniecLasera, sf::Color(255, 0, 0, 30)) // Przezroczysty koniec
            };
            window.draw(laserLine, 2, sf::Lines);
        }
        
        // Oko rysuje się zawsze
        window.draw(this->sprite);
    }
};
// ============================================================================
// 2. KLASA GŁÓWNEGO BOSSA (Mag) - BEZPIECZNA WERSJA
// ============================================================================
class Final_boss : public Enemy
{
private:
    Boss_state aktualnyEtap;
    sf::Clock stateClock;
    sf::Clock attackClock;
    sf::Clock animClock;

    const sf::Texture& texOczu;
    bool oko_otwarte;
    bool oczy_zrespowane;
    float czas_na_oko;
    int maxHp;

    int klatka_szerokosc = 64;
    int klatka_wysokosc = 64;

public:
    Final_boss(const sf::Texture& texBos, const sf::Texture& texOczy, float x, float y) 
        : Enemy(x, y, 35.0f), texOczu(texOczy)
    {
        this->maxHp = 40;
        this->hp = this->maxHp;
        this->speed = 45.0f;

        this->aktualnyEtap = Boss_state::Transformacja;
        this->oko_otwarte = false;
        this->oczy_zrespowane = false;
        this->czas_na_oko = 2.5f;

        // Tuta podpinana jest tekstura (sprawdź co przekazujesz w main.cpp!)
        this->sprite.setTexture(texBos);

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

        int klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.2f) % 4;

        if (aktualnyEtap == Boss_state::Faza_zadymka)
        {
            klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.1f) % 4;
        }

        switch (aktualnyEtap)
        {
            case Boss_state::Transformacja:
                this->sprite.setTextureRect(sf::IntRect(klatka * klatka_szerokosc, 0, klatka_szerokosc, klatka_wysokosc));
                
                if (czas_w_stanie > 4.0f)
                {
                    // Powiększenie po transformacji
                    this->shape.setSize(sf::Vector2f(96.0f, 96.0f));
                    this->sprite.setScale(2.5f, 2.5f);

                    aktualnyEtap = Boss_state::Faza_Oka;
                    stateClock.restart();
                }
                break;
                
            case Boss_state::Faza_Oka:
                if (czas_w_stanie >= czas_na_oko)
                {
                    oko_otwarte = !oko_otwarte;
                    stateClock.restart();
                }

                if (oko_otwarte) {
                    this->sprite.setTextureRect(sf::IntRect(klatka * klatka_szerokosc, 128, klatka_szerokosc, klatka_wysokosc));
                } else {
                    this->sprite.setTextureRect(sf::IntRect(klatka * klatka_szerokosc, 64, klatka_szerokosc, klatka_wysokosc));
                }
                
                this->shape.move(getDirectionToPlayer() * speed * deltaTime);

                if (this->hp <= maxHp / 2)
                {
                    aktualnyEtap = Boss_state::Faza_PolHp;
                    oko_otwarte = true;
                    stateClock.restart();
                    attackClock.restart();
                }
                break;

            case Boss_state::Faza_PolHp:
                this->sprite.setTextureRect(sf::IntRect(klatka * klatka_szerokosc, 128, klatka_szerokosc, klatka_wysokosc));

                if (!oczy_zrespowane)
                {
                    float randX1 = 150.0f + static_cast<float>(rand() % 400);
                    float randY1 = 100.0f + static_cast<float>(rand() % 300);
                    float randX2 = 150.0f + static_cast<float>(rand() % 400);
                    float randY2 = 100.0f + static_cast<float>(rand() % 300);

                    worldObjects.push_back(new Flying_eyes(this->texOczu, randX1, randY1));
                    worldObjects.push_back(new Flying_eyes(this->texOczu, randX2, randY2));
                    
                    oczy_zrespowane = true;
                }

                this->shape.move(getDirectionToPlayer() * (speed * 1.25f) * deltaTime);

                if (this->hp <= maxHp / 4)
                {
                    aktualnyEtap = Boss_state::Faza_zadymka;
                }
                break;

            case Boss_state::Faza_zadymka:
                int szybka_klatka = static_cast<int>(animClock.getElapsedTime().asSeconds() / 0.08f) % 4;
                this->sprite.setTextureRect(sf::IntRect(szybka_klatka * klatka_szerokosc, 128, klatka_szerokosc, klatka_wysokosc));
                
                this->shape.move(getDirectionToPlayer() * (speed * 1.7f) * deltaTime);
                this->sprite.setColor(sf::Color(255, 90, 90));

                if (attackClock.getElapsedTime().asSeconds() >= 1.3f)
                {
                    sf::Vector2f dir = getDirectionToPlayer();
                    const sf::Texture* fireTex = this->sprite.getTexture();
                    if (fireTex != nullptr)
                    {
                        worldObjects.push_back(new Projectile(*fireTex, this->shape.getPosition().x + 48.0f, this->shape.getPosition().y + 48.0f, dir));
                    }
                    attackClock.restart();
                }
                break;
        }
        
        // Klasyczne dopasowanie pozycji grafiki do hitboxu bez offsetów
        this->sprite.setPosition(this->shape.getPosition());
    }

    void takeDamage(int amount) override
    {
        if(aktualnyEtap == Boss_state::Transformacja) return;

        if(aktualnyEtap == Boss_state::Faza_Oka && !oko_otwarte)
        {
            std::cout << "Boss odbija ataki pelerynka" << std::endl;
            return;
        }

        if(!isInvincible())
        {
            this->hp -= amount;
            if(this->hp < 0) this->hp = 0;
            this->invincibilityTimer = this->invincibilityDuration;
        }
    }
};