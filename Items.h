#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Game.h"
#include "Link.h"


// 1. KLASA ABSTRAKCYJNA: Item
class Item : public Game 
{
protected:
    std::string name;
    sf::Texture texture;
    sf::Sprite sprite;

public:
    Item(std::string itemName) : name(itemName) {}
    virtual ~Item() {}

    virtual void use(Link* hero) = 0;

    std::string getName() const { return name; }

    void update(float deltaTime) override {
    }

    void draw(sf::RenderWindow& window) override {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() override {
        return sprite.getGlobalBounds();
    }
    
    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void setScale(float factorX, float factorY) {
        sprite.setScale(factorX, factorY);
    }
};

// 2. KLASA: Potion (Dziedziczy po Item)
class Potion : public Item 
{
private:
    int healAmount;

public:
    Potion() : Item("Potion"), healAmount(9) {
        if (!texture.loadFromFile("grafiki/Potion.png")) {
            std::cout << "Blad ladowania grafiki/Potion.png" << std::endl;
        }
        sprite.setTexture(texture);
    }

    // REALIZACJA LECZENIA:
void use(Link* hero) override{
    std::cout << "Uzyto mikstury! Przywrocono " << healAmount << " punktow zdrowia.\n";
    hero->heal(healAmount); 
}
};

// 3. KLASA ABSTRAKCYJNA: Weapon (Dziedziczy po Item)
class Weapon : public Item 
{
protected:
    int damage;

public:
    Weapon(std::string weaponName) : Item(weaponName), damage(4) {}

    virtual int getDamage() const {
        return damage;
    }

    void use(Link* hero) override {
        std::cout << "Wybrano bron: " << name << " (Obrazenia: " << getDamage() << ")\n";
    }
};

// 4. KLASY POCHODNE OD WEAPON

// --- SWORD ---
class Sword : public Weapon 
{
public:
    Sword() : Weapon("Sword") {
        if (!texture.loadFromFile("grafiki/Miecz.png")) {
            std::cout << "Blad ladowania grafiki/Miecz.png" << std::endl;
        }
        sprite.setTexture(texture);
    }

    int getDamage() const override {
        return damage; 
    }
};

// --- BOW ---
class Bow : public Weapon 
{
public:
    Bow() : Weapon("Bow") {
        if (!texture.loadFromFile("grafiki/Bow.png")) {
            std::cout << "Blad ladowania grafiki/Bow.png" << std::endl;
        }
        sprite.setTexture(texture);
    }
};

// --- BOOMERANG ---
class Boomerang : public Weapon 
{
public:
    Boomerang() : Weapon("Boomerang") {
        if (!texture.loadFromFile("grafiki/Boomerang.png")) {
            std::cout << "Blad ladowania grafiki/Boomerang.png" << std::endl;
        }
        sprite.setTexture(texture);
    }
    int getDamage() const override {
        return 3; 
    }
};