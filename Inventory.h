#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Game.h"
#include "Items.h" // Dołączamy Twoje przedmioty!

class Inventory : public Game
{
private:
    sf::Texture slotsTexture;
    sf::Sprite slotsSprite;

    sf::Texture selectionTexture;
    sf::Sprite selectionSprite; 

    int activeSlot = 0;         
    const int maxSlots = 5;

    float startOffsetX = -6.0f;  
    float startOffsetY = -5.0f;  
    float slotWidth = 40.0f;    

    // --- NOWOŚĆ: Przechowywanie Twoich obiektów Item ---
    std::vector<Item*> items;

    // Margines wewnątrz pojedynczego okienka (żeby ikona była wyśrodkowana)
    float itemOffsetX = 4.0f; 
    float itemOffsetY = 4.0f;

public:
    Inventory() {
        if (!slotsTexture.loadFromFile("grafiki/Sloty.png")) {
            std::cout << "!!! Error loading Sloty.png texture !!!\n";
        }
        slotsSprite.setTexture(slotsTexture);

        if (!selectionTexture.loadFromFile("grafiki/Hotbar_selection3.png")) {
            std::cout << "!!! Error loading Hotbar_selection3.png texture !!!\n";
        }
        selectionSprite.setTexture(selectionTexture);

        float slotsHeight = slotsSprite.getLocalBounds().height;
        float margin = 15.0f;

        float posX = margin;
        float posY = 528.0f - slotsHeight - margin;

        slotsSprite.setPosition(posX, posY);

        // Zabezpieczamy wektor na 5 slotów wypełnionych na razie niczym (nullptr)
        items.resize(maxSlots, nullptr);
    }

    // Destruktor - zwolni pamięć przedmiotów przy wyłączaniu gry
    ~Inventory() {
        clear();
    }

    void update(float deltaTime) override {}

    // --- ZAKTUALIZOWANA METODA DRAW ---
    void draw(sf::RenderWindow& window) override {
        // 1. Rysujemy ramkę slotów (tło)
        window.draw(slotsSprite);

        // 2. Rysujemy przedmioty, które znajdują się w slotach
        sf::Vector2f basePos = slotsSprite.getPosition();
        for (int i = 0; i < maxSlots; ++i) {
            if (items[i] != nullptr) {
                // Obliczamy matematycznie pozycję x i y dla ikony w slocie i
                float itemX = basePos.x + itemOffsetX + (i * slotWidth);
                float itemY = basePos.y + itemOffsetY;
                
                // Ustawiamy pozycję sprite'a przedmiotu i go rysujemy
                items[i]->setPosition(itemX, itemY);
                items[i]->setScale(2.0f, 2.0f);
                items[i]->draw(window); // Wywołujemy metodę draw z klasy Item!
            }
        }

        // 3. Rysujemy ramkę wyboru (na samym wierzchu)
        float frameX = basePos.x + startOffsetX + (activeSlot * slotWidth);
        float frameY = basePos.y + startOffsetY;

        selectionSprite.setPosition(frameX, frameY);
        window.draw(selectionSprite);
    }

    sf::FloatRect getBounds() override {
        return slotsSprite.getGlobalBounds();
    }

    // --- NOWE METODY DO ZARZĄDZANIA PRZEDMIOTAMI ---

    // Wrzucanie przedmiotu do konkretnego slotu
    void setItem(int slot, Item* item) {
        if (slot >= 0 && slot < maxSlots) {
            if (items[slot] != nullptr) {
                delete items[slot]; // Zapobiegamy wyciekom pamięci
            }
            items[slot] = item;
        }
    }

    // Czyszczenie ekwipunku przy restarcie/nowej grze
    void clear() {
        for (int i = 0; i < maxSlots; ++i) {
            if (items[i] != nullptr) {
                delete items[i];
                items[i] = nullptr;
            }
        }
    }

    // Pobranie przedmiotu z aktywnego slotu (do walki/używania)
    Item* getActiveItem() const {
        return items[activeSlot];
    }

    // --- TWOJE METODY LOGIKI INWENTARZA ---
    void nextSlot() {
        activeSlot = (activeSlot + 1) % maxSlots;
        std::cout << "Active slot: " << activeSlot << "\n";
    }

    void prevSlot() {
        activeSlot = (activeSlot - 1 + maxSlots) % maxSlots;
        std::cout << "Active slot: " << activeSlot << "\n";
    }

    int getActiveSlot() const {
        return activeSlot;
    }
};