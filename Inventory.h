#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Game.h"
#include "Items.h"

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

    std::vector<Item*> items;

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

        items.resize(maxSlots, nullptr);
    }

    ~Inventory() {
        clear();
    }

    void update(float deltaTime) override {}

    void draw(sf::RenderWindow& window) override {

        window.draw(slotsSprite);

        sf::Vector2f basePos = slotsSprite.getPosition();
        for (int i = 0; i < maxSlots; ++i) {
            if (items[i] != nullptr) {

                float itemX = basePos.x + itemOffsetX + (i * slotWidth);
                float itemY = basePos.y + itemOffsetY;
                
                items[i]->setPosition(itemX, itemY);
                items[i]->setScale(2.0f, 2.0f);
                items[i]->draw(window);
            }
        }

        float frameX = basePos.x + startOffsetX + (activeSlot * slotWidth);
        float frameY = basePos.y + startOffsetY;

        selectionSprite.setPosition(frameX, frameY);
        window.draw(selectionSprite);
    }

    sf::FloatRect getBounds() override {
        return slotsSprite.getGlobalBounds();
    }

    void setItem(int slot, Item* item) {
        if (slot >= 0 && slot < maxSlots) {
            if (items[slot] != nullptr) {
                delete items[slot];
            }
            items[slot] = item;
        }
    }

    void clear() {
        for (int i = 0; i < maxSlots; ++i) {
            if (items[i] != nullptr) {
                delete items[i];
                items[i] = nullptr;
            }
        }
    }

    Item* getActiveItem() const {
        return items[activeSlot];
    }

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

    bool addItem(Item* item) {
    if (item == nullptr) return false;
    
    for (int i = 0; i < maxSlots; ++i) {
        if (items[i] == nullptr) {
            items[i] = item;
            std::cout << "Dodano przedmiot " << item->getName() << " do slotu " << i << std::endl;
            return true;
        }
    }
    
    std::cout << "Brak miejsca w ekwipunku na: " << item->getName() << std::endl;
    return false;}
};