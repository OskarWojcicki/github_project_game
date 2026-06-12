#ifndef INVENTORY_H
#define INVENTORY_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Inventory {
private:
    sf::Texture slotsTexture;
    sf::Sprite slotsSprite;

    sf::Texture selectionTexture;
    sf::Sprite selectionSprite; // Nasza złota ramka Hotbar_selection.png

    int activeSlot = 0;         // Indeks wybranego slotu (od 0 do 4)
    const int maxSlots = 5;

    // Marginesy dopasowane do Twojego paska (wartości poglądowe, dostosuj jeśli trzeba)
    float startOffsetX = -6.0f;  // Odległość od lewej krawędzi paska do 1. slotu
    float startOffsetY = -5.0f;  // Odległość od górnej krawędzi paska do slotu
    float slotWidth = 40.0f;    // Szerokość jednego slotu + odstęp (od środka do środka)

public:
    Inventory() {
        // 1. Ładowanie tła ekwipunku
        if (!slotsTexture.loadFromFile("grafiki/Sloty.png")) {
            std::cout << "!!! Error loading Sloty.png texture !!!" << std::endl;
        }
        slotsSprite.setTexture(slotsTexture);

        // 2. Ładowanie złotej ramki wyboru
        if (!selectionTexture.loadFromFile("grafiki/Hotbar_selection3.png")) {
            std::cout << "!!! Error loading Hotbar_selection.png texture !!!" << std::endl;
        }
        selectionSprite.setTexture(selectionTexture);

        // 3. Pozycjonowanie paska w lewym dolnym rogu (okno 720x528)
        float slotsHeight = slotsSprite.getLocalBounds().height;
        float margin = 15.0f;

        float posX = margin;
        float posY = 528.0f - slotsHeight - margin;

        slotsSprite.setPosition(posX, posY);
    }

    // Funkcje do zmiany aktywnego slotu za pomocą scrolla
    void nextSlot() {
        activeSlot = (activeSlot + 1) % maxSlots; // Zapętla: 4 -> 0
        std::cout << "Active slot: " << activeSlot << std::endl;
    }

    void prevSlot() {
        activeSlot = (activeSlot - 1 + maxSlots) % maxSlots; // Zapętla: 0 -> 4
        std::cout << "Active slot: " << activeSlot << std::endl;
    }

    // Zwraca aktualnie wybrany slot (przyda się później do używania przedmiotów)
    int getActiveSlot() const {
        return activeSlot;
    }

    // Renderowanie paska i złotej ramki
    void render(sf::RenderWindow& window) {
        // Najpierw rysujemy cały pasek
        window.draw(slotsSprite);

        // Pobieramy aktualną pozycję paska, żeby od niej liczyć pozycję ramki
        sf::Vector2f basePos = slotsSprite.getPosition();

        // MATEMATYKA POZYCJI RAMKI:
        // X = początek paska + margines startowy + (numer slotu * szerokość slotu)
        float frameX = basePos.x + startOffsetX + (activeSlot * slotWidth);
        float frameY = basePos.y + startOffsetY;

        selectionSprite.setPosition(frameX, frameY);
        
        // Rysujemy złotą ramkę NA pasku
        window.draw(selectionSprite);
    }
};

#endif // INVENTORY_H