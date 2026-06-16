#include <iostream>
#include "Game.h"
#include "Background.h"
#include "Link.h"
#include "Enemies.h"
#include "Chest.h"
#include "Ptaszek.h"
#include "Inventory.h"
#include <SFML/Audio.hpp>

enum class GameState
{
    MainMenu,
    Gameplay,
    Kurtyna_lvl2,
    GameOver,
};

Link* player = nullptr;


//pokoje lvl 1
const char room1[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','F','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','F','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room2[11][15]=
{
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room3[11][15]=
{
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'}
};

const char room4[11][15]=
{
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'}
};

const char room5[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room6[11][15]=
{
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','#','F','F','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','#','F','F','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','#','F','F','P','P','P','P','P','P','P','P','P'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'}
};

const char room7[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'P','P','P','P','P','P','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','P','P','P','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','P','P','P','#','#','#','#','#','#'}
};

const char room8[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'P','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},  
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room9[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','P'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','P','P','P','P','P','P','P','P','P','P','P','P','P','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

//pokoje lvl 2
const char room10[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room11[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room12[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};
const char room13[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room14[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room15[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room16[11][15]=
{
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'F','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};
const char room17[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};
const char room18[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};


const char (*worldMap[10][10])[15]=
{
    {room9,   room8,   room7,   nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, room6,   room5,   nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, room4,   nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, room3,   nullptr, nullptr,  nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, room1,   room2,   nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},

    {nullptr, nullptr, nullptr, room18, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, nullptr, room17, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {room15, room14, room13, room16, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, room12, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
    {nullptr, nullptr, room11, room10, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
};

void Rooms(int wx, int wy, std::vector<Game*>& wordlObjects, float startX, float startY, const sf::Texture& t_drzewa, const sf::Texture& t_floor1, const sf::Texture& t_floor2, const sf::Texture& t_cien,const sf::Texture& t_slime, const sf::Texture& t_moblin_up,const sf::Texture& t_moblin_down,const sf::Texture& t_moblin_left,const sf::Texture& t_moblin_right, const sf::Texture& t_skieleton_down, const sf::Texture& t_skieleton_up,const sf::Texture& t_skieleton_sides,const sf::Texture& t_proj, const sf::Texture& t_piasek,const sf::Texture& t_wysoka_trawa,const sf::Texture& t_stone, const sf::Texture& t_kwiatek,std::vector<std::string> (&defeatedEnemies)[10][10])
{
    // 1. ZAPAMIĘTYWANIE HP: Jeśli gracz już istniał, pobieramy jego obecne punkty życia
    int currentHP = 3; // Domyślnie 10 (np. przy pierwszym uruchomieniu gry)
    if (player != nullptr)
    {
        currentHP = player->getHP();
    }

    player = nullptr;

    // Czyszczenie starego pokoju
    for(auto& object : wordlObjects)
    {
        delete object;
    }
    wordlObjects.clear();

    const char (*selectedRoom)[15] = worldMap[wy][wx];
    if(selectedRoom == nullptr)
    {
        return;
    }

    float rozmiar_kafelka = 48.0f;

    for(int row = 0; row < 11; ++row)
    {
        for(int col = 0; col < 15; ++col)
        {
            float posX = col * rozmiar_kafelka;
            float posY = row * rozmiar_kafelka;

            if(selectedRoom[row][col] == '#')
            { 
                wordlObjects.push_back(new Background(t_drzewa, posX, posY, true));
            }
            else if(selectedRoom[row][col] == 'F')
            {
                wordlObjects.push_back(new Background(t_floor1, posX, posY, false));
            }
            else if(selectedRoom[row][col] == 'S')
            {
                wordlObjects.push_back(new Background(t_floor2, posX,posY,false));
            }
            else if(selectedRoom[row][col] == 'C')
            {
                wordlObjects.push_back(new Background(t_cien, posX,posY,false));
            }
            else if(selectedRoom[row][col] == 'P')
            {
                wordlObjects.push_back(new Background(t_piasek,posX,posY,false));
            }
        }
    }

    // 2. PRZYWRACANIE HP: Tworzymy nowego Linka w nowym miejscu...
    player = new Link(startX, startY);
    
    // ... i ustawiamy mu zapamiętane punkty życia!
    player->setHP(currentHP); 
    
    wordlObjects.push_back(player);

    auto czyZyje = [&](float x, float y) -> bool {
        std::string key = std::to_string((int)x) + "_" + std::to_string((int)y);
        for(const auto& deadKey : defeatedEnemies[wy][wx]) {
            if(deadKey == key) {
                return false; // Został zabity, czyli nie żyje
            }
        }
        return true; // Nie ma go na liście martwych, czyli żyje
    };
    // --- Reszta Twojego kodu z potworami (Slime, Chest, Moblin itd.) --
    if(selectedRoom==room2)
    {
        if(czyZyje(300.0f, 400.0f))wordlObjects.push_back(new Slime(t_slime, 300.0f, 400.0f));
        if(czyZyje(400.0f, 200.0f))wordlObjects.push_back(new Slime(t_slime, 400.0f, 200.0f));
        wordlObjects.push_back(new Background(t_stone, 546.0f, 380.0f, true));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 450.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 498.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 546.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 594.0f, 100.0f, false));

        wordlObjects.push_back(new Background(t_wysoka_trawa, 450.0f, 148.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 498.0f, 196.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 546.0f, 196.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 594.0f, 148.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 498.0f, 148.0f, false));
        wordlObjects.push_back(new Background(t_stone, 546.0f, 148.0f, true));

    }
    if(selectedRoom==room5)
    {
        wordlObjects.push_back(new Chest(336.0f, 140.0f, "SWORD"));
    }
    if(selectedRoom==room3)
    {
        if(czyZyje(600.0f, 400.0f)) wordlObjects.push_back(new Slime(t_slime,600.0f, 400.0f));
        if(czyZyje(600.0f, 200.0f))wordlObjects.push_back(new Slime(t_slime,600.0f, 200.0f));
        if(czyZyje(300.0f, 200.0f))wordlObjects.push_back(new Slime(t_slime,300.0f, 200.0f));
        wordlObjects.push_back(new Background(t_kwiatek, 500.0f, 284.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 500.0f, 188.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 500.0f, 92.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 190.0f, 284.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 190.0f, 188.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 190.0f, 92.0f, false));

        
    }
    if(selectedRoom==room7)
    {
        if(czyZyje(100.0f, 100.0f))wordlObjects.push_back(new Moblin(t_moblin_up,t_moblin_down,t_moblin_left,t_moblin_right,100.0f, 100.0f));
        if(czyZyje(300.0f, 100.0f))wordlObjects.push_back(new Skieleton(t_skieleton_down,t_skieleton_up,t_skieleton_sides,t_proj,300.0f, 100.0f));
        if(czyZyje(100.0f, 200.0f))wordlObjects.push_back(new Moblin(t_moblin_up,t_moblin_down,t_moblin_left,t_moblin_right,100.0f, 200.0f));
        wordlObjects.push_back(new Background(t_stone, 536.0f, 300.0f, true));

        
    }
    if(selectedRoom==room8)
    {
        if(czyZyje(100.0f, 400.0f))wordlObjects.push_back(new Moblin(t_moblin_up,t_moblin_down,t_moblin_left,t_moblin_right,100.0f, 400.0f));
        if(czyZyje(100.0f, 300.0f))wordlObjects.push_back(new Moblin(t_moblin_up,t_moblin_down,t_moblin_left,t_moblin_right,100.0f, 300.0f));
      
    }
    if(selectedRoom==room9)
    {
        if(czyZyje(336.0f, 240.0f))wordlObjects.push_back(new Skieleton(t_skieleton_down,t_skieleton_up,t_skieleton_sides,t_proj,336.0f,240.0f));
    }
      if(selectedRoom==room1)
    {
        wordlObjects.push_back(new Background(t_wysoka_trawa, 150.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 198.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 246.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 298.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 346.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_stone, 546.0f, 380.0f, true));
        wordlObjects.push_back(new Background(t_kwiatek, 346.0f, 380.0f, false));
        wordlObjects.push_back(new Background(t_kwiatek, 300.0f, 380.0f, false));
    }
        if(selectedRoom==room4)
    {
        if(czyZyje(336.0f, 140.0f))wordlObjects.push_back(new Moblin(t_moblin_up,t_moblin_down,t_moblin_left,t_moblin_right,336.0f, 140.0f));
        wordlObjects.push_back(new Background(t_stone, 120.0f, 380.0f, true));
        wordlObjects.push_back(new Background(t_stone, 120.0f, 100.0f, true));
        wordlObjects.push_back(new Background(t_stone, 536.0f, 100.0f, true));
        wordlObjects.push_back(new Background(t_stone, 536.0f, 380.0f, true));
        

    }
        if(selectedRoom==room6)
    {
        wordlObjects.push_back(new Background(t_wysoka_trawa, 120.0f, 380.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 120.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 536.0f, 100.0f, false));
        wordlObjects.push_back(new Background(t_wysoka_trawa, 536.0f, 380.0f, false));

    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(720, 528), "The legend of Zelda");

    sf::Music backgroundMusic;
    sf::Music titleMusic;
    sf::Music gameOverMusic;

    if(!titleMusic.openFromFile("muzyka/title_screen.mp3")) 
    {
        std::cout << "Blad w ladowaniu muzyki menu" << std::endl;
    }
    else
    {
        titleMusic.setLoop(true);
        titleMusic.setVolume(80.0f);
        titleMusic.play(); 
    }

    if(!gameOverMusic.openFromFile("muzyka/muzyka_gameOver.mp3"))
    {
        std::cout << "Blad w ladowaniu muzyki Game Over" << std::endl;
    }
    else
    {
        gameOverMusic.setLoop(false); 
        gameOverMusic.setVolume(80.0f);
    }

    window.setFramerateLimit(60);

    sf::Texture tex_drzewa, tex_floor1, tex_floor2, tex_cien, tex_serduszko, tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_up,tex_skieleton_down,tex_skieleton_sides,tex_strzala, tex_piasek,tex_wysoka_trawa,tex_stone, tex_kwiatek;
    if (!tex_drzewa.loadFromFile("grafiki/pokemon_fence.png") ||
        !tex_floor1.loadFromFile("grafiki/pokemon_grass.png") ||
        !tex_floor2.loadFromFile("grafiki/floor2.png") ||
        !tex_cien.loadFromFile("grafiki/cien.png") ||
        !tex_serduszko.loadFromFile("grafiki/Serduszko2.png") ||
        !tex_slime.loadFromFile("grafiki/slime.png") ||
        !tex_moblin_up.loadFromFile("grafiki/moblin_up.png")||
        !tex_moblin_down.loadFromFile("grafiki/moblin_down.png") ||
        !tex_moblin_left.loadFromFile("grafiki/moblin_left.png") ||
        !tex_moblin_righ.loadFromFile("grafiki/moblin_right.png") ||
        !tex_skieleton_sides.loadFromFile("grafiki/skieleton_sides.png") ||
        !tex_skieleton_down.loadFromFile("grafiki/skieleton_down.png") ||
        !tex_skieleton_up.loadFromFile("grafiki/skieleton_up.png") ||
        !tex_strzala.loadFromFile("grafiki/strzala.png") ||
        !tex_piasek.loadFromFile("grafiki/piasek.png") ||
        !tex_wysoka_trawa.loadFromFile("grafiki/wysoka_trawa.png") ||
        !tex_stone.loadFromFile("grafiki/stone.png") ||
        !tex_kwiatek.loadFromFile("grafiki/kwiatek.png"))
    {
        std::cout << "!!! Blad w ladowaniu tekstur otoczenia !!!" << std::endl;
    }

    GameState currentState = GameState::MainMenu;
    int worldX = 1;
    int worldY = 4;

    sf::Clock transitionClock;

    sf::Font font;
    if(!font.loadFromFile("Triforce-y07d.ttf"))
    {
        std::cout << "!!! Blad w ladowaniu czcionki !!!" << std::endl;
    }

    sf::Text TitleText("The legends of", font, 30);
    TitleText.setFillColor(sf::Color::White);

    sf::Text TitleText2("Zelda", font, 150);
    TitleText2.setFillColor(sf::Color::White);
    
    sf::Text menuText("NACISNIJ ENTER", font, 18);
    menuText.setFillColor(sf::Color::White);

    float width1 = TitleText.getLocalBounds().width;
    TitleText.setPosition((720.0f - width1) / 2.0f, 50.0f);

    float width2 = TitleText2.getLocalBounds().width;
    TitleText2.setPosition((720.0f - width2) / 2.0f, 80.0f);

    float width3 = menuText.getLocalBounds().width;
    menuText.setPosition((720.0f - width3) / 2.0f, 400.0f);

    // --- Zmienne dla ekranu Game Over ---
    sf::Text gameOverText("GAME OVER", font, 80);
    gameOverText.setFillColor(sf::Color::Red);

    sf::Text retryText("NACISNIJ ENTER ABY SPROBOWAC PONOWNIE", font, 16);
    retryText.setFillColor(sf::Color::White);

    // Środkowanie tekstów na ekranie
    gameOverText.setPosition((720.0f - gameOverText.getLocalBounds().width) / 2.0f, 150.0f);
    retryText.setPosition((720.0f - retryText.getLocalBounds().width) / 2.0f, 350.0f);

    // Czarny prostokąt kurtyny – na początku ma wysokość 0, będzie "rosnąć" w dół
    sf::RectangleShape gameOverCurtain(sf::Vector2f(720.0f, 0.0f));
    gameOverCurtain.setFillColor(sf::Color::Black);
    float curtainHeight = 0.0f;

    std::vector<Game*> worldObjects;
    sf::Clock clock;
    Inventory playerInventory;
    std::vector<std::string> defeatedEnemies[10][10];

    while(window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }

            if(currentState == GameState::MainMenu)
            {
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Gracz wcisnal Enter. Ladowanie gry ...\n";
                    worldX = 1;
                    worldY = 4;

                    titleMusic.stop();
                    if(!backgroundMusic.openFromFile("muzyka/lvl1.mp3"))
                    {
                        std::cout << "Blad w ladowaniu muzyki" << std::endl;
                    }
                    else
                    {
                        backgroundMusic.setLoop(true);
                        backgroundMusic.setVolume(50.0f); 
                        backgroundMusic.play();
                    }

                    playerInventory.clear(); // Czyszczenie starych śmieci (np. po wcześniejszej grze)
                            
                            playerInventory.setItem(0, new Potion());    // Slot 1 (indeks 0)
                            playerInventory.setItem(1, new Sword());     // Slot 2 (indeks 1)
                            playerInventory.setItem(2, new Bow());       // Slot 3 (indeks 2)
                            playerInventory.setItem(3, new Boomerang()); // Slot 4 (indeks 3)

                    Rooms(worldX, worldY, worldObjects, 340.0f, 240.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
                    currentState = GameState::Gameplay;
                }
            }

            if (currentState == GameState::GameOver)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Restart gry... Ladowanie poziomu od nowa.\n";
        
                    gameOverMusic.stop();  
                    backgroundMusic.play();
                    // Resetujemy pozycję mapy do punktu startowego (tak jak przy nowej grze)
                    worldX = 1;
                    worldY = 4;
                    
                    for (int y = 0; y < 10; ++y)
                    {
                        for (int x = 0; x < 10; ++x)
                        {
                            defeatedEnemies[y][x].clear();
                        }
                    }
                    
                    // Ładujemy pokój startowy na nowo. Rooms automatycznie stworzy nowego Linka.
                    // Podajemy domyślne współrzędne startowe (np. środek ekranu 340, 240)
                    Rooms(worldX, worldY, worldObjects, 340.0f, 240.0f, tex_drzewa, tex_floor1, tex_floor2, tex_cien, tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
        
                    // Ponieważ Rooms tworzy nowego Linka z 10 HP, upewniamy się, że ma pełne zdrowie
                    if (player != nullptr) {
                        player->setHP(3); 
                    }

                    // Wracamy do rozgrywki
                    currentState = GameState::Gameplay;
                }
            }

            // Wewnątrz pętli while (window.pollEvent(event))
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                // Upewniamy się, że gracz kręci rolką w pionie (góra/dół)
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                {
                   // delta > 0 oznacza ruch w górę, delta < 0 ruch w dół
                  if (event.mouseWheelScroll.delta > 0)
                 {
                        playerInventory.prevSlot(); // Przewiń slot w lewo
                 }
                 else if (event.mouseWheelScroll.delta < 0)
                 {
                     playerInventory.nextSlot(); // Przewiń slot w prawo
                 }
             }
            }
            
            if (currentState == GameState::Gameplay)
            {
                if (event.type == sf::Event::MouseButtonPressed) 
                {
                    if (event.mouseButton.button == sf::Mouse::Right) 
                    {
                        Item* activeItem = playerInventory.getActiveItem();

                        if (activeItem != nullptr) 
                        {
                            std::string itemName = activeItem->getName();

                            if (itemName == "Sword") {
                                std::cout << "[PPM] Atakujesz mieczem!\n";
                                activeItem->use(player);
                            }
                            else if (itemName == "Bow") {
                                std::cout << "[PPM] Wypuszczasz strzale z luku!\n";
                            }
                            else if (itemName == "Boomerang") {
                                std::cout << "[PPM] Rzucasz bumerangiem!\n";
                            }
                            else if (itemName == "Potion") {
                                std::cout << "[PPM] Pijesz miksture lecznicza!\n";
                                activeItem->use(player);
                                playerInventory.setItem(playerInventory.getActiveSlot(), nullptr);
                            }
                        } 
                        else {
                            std::cout << "[PPM] Kliknales, ale ten slot jest pusty!\n";
                        }
                    }
                }
                // ===============================================================
                
            }

            if(currentState == GameState::Gameplay && player != nullptr)
            {
                player->handleEvents(event); 
            }


            if(currentState==GameState::Gameplay && player != nullptr)
            {
            if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::E)
            {
                sf::FloatRect playerBounds = player -> getBounds();

                for(auto& obj : worldObjects)
                {
                    Chest* chest = dynamic_cast<Chest*>(obj);
                    if(chest !=nullptr)
                    {
                        sf::FloatRect chestBounds = chest->getBounds();

                        float dist = 10.0f;
                        sf::FloatRect interactionZone(chestBounds.left - dist,chestBounds.top - dist, chestBounds.width + (dist*2), chestBounds.height + (dist*2));
                        

                        if(playerBounds.intersects(interactionZone))
                        {
                            chest -> interact(player);
                            break;
                        }
                    }
                }
                       if(worldX==0 && worldY==0)
                {
                    bool wrogowie_zyja=false;
                    for(auto& obj : worldObjects)
                    {
                        if(dynamic_cast<Enemy*>(obj) != nullptr)
                        {
                            wrogowie_zyja = true;
                            break;
                        }
                    }

                    if(!wrogowie_zyja)
                    {
                        std::cout<<"Ptaszek zabierze Linka na LVL 2"<<std::endl;
                        currentState = GameState::Kurtyna_lvl2;
                        transitionClock.restart();
                    }
                }
            }
            
            }

        }

        sf::Vector2f oldPlayerPos(0.0f, 0.0f);
        if(player != nullptr)
        {
            oldPlayerPos = player->getPosition(); 
        }
        


// --- NOWY, POPRAWIONY KOD AKTUALIZACJI W MAIN.CPP ---
for (size_t i = 0; i < worldObjects.size(); ++i)
{
    Enemy* enemy = dynamic_cast<Enemy*>(worldObjects[i]);
    if (enemy != nullptr)
    {
        enemy->updateEnemyAI(worldObjects, deltaTime);
        enemy->updateInvincibility(deltaTime);
    }
    else
    {
        worldObjects[i]->update(deltaTime);
    }
}

        if(currentState == GameState::Gameplay && player != nullptr)
        {

            sf::Vector2f playerPos = player->getPosition();

            if(playerPos.x > 690.0f)
            {
                if(worldX + 1 < 10 && worldMap[worldY][worldX + 1] != nullptr)
                {
                    worldX++;
                    Rooms(worldX, worldY, worldObjects, 20.0f, playerPos.y, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
                }
                else
                {
                    player->setPosition(690.0f, playerPos.y);
                }
            }
            
            else if(playerPos.x < 0.0f)
            {
                if(worldX - 1 >= 0 && worldMap[worldY][worldX - 1] != nullptr)
                {
                    worldX--;
                    Rooms(worldX, worldY, worldObjects, 630.0f, playerPos.y, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
                }
                else
                {
                    player->setPosition(10.0f, playerPos.y);
                }
            }
            else if(playerPos.y > 528.0f)
            {
                if(worldY + 1 < 10 && worldMap[worldY + 1][worldX] != nullptr)
                {
                    worldY++;
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 20.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
                }
                else
                {
                    player->setPosition(playerPos.x, 510.0f);
                }
            }
            else if(playerPos.y < 0.0f)
            {
                if(worldY - 1 >= 0 && worldMap[worldY - 1][worldX] != nullptr)
                {
                    worldY--;
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 500.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);
                }
                else 
                {
                    player->setPosition(playerPos.x, 10.0f);
                }
            }
        }

        if(currentState == GameState::Gameplay && player != nullptr)
        {
            sf::FloatRect playerBounds = player->getBounds();

            for(auto& object : worldObjects)
            {
                if(object == player)
                {
                    continue;
                }
                if(object->isSolid())
                {
                    sf::FloatRect wallBounds = object->getBounds();
                    sf::FloatRect overlap;

                    if(playerBounds.intersects(wallBounds, overlap)) 
                    {

                        bool wasLeft   = (oldPlayerPos.x + playerBounds.width <= wallBounds.left);
                        bool wasRight  = (oldPlayerPos.x >= wallBounds.left + wallBounds.width);
                        bool wasTop    = (oldPlayerPos.y + playerBounds.height <= wallBounds.top);
                        bool wasBottom = (oldPlayerPos.y >= wallBounds.top + wallBounds.height);

                        if(wasLeft && !wasTop && !wasBottom)
                        {
                            player->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                        }
                        else if (wasRight && !wasBottom && !wasTop)
                        {
                            player->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                        }
                        else if (wasTop && !wasLeft && !wasRight)
                        {
                            player->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                        }
                        else if (wasBottom && !wasLeft && !wasRight)
                        {
                            player->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                        }
                        else
                        {

                            if(overlap.width < overlap.height)
                            {
                                if(playerBounds.left + (playerBounds.width / 2.0f) < wallBounds.left + (wallBounds.width / 2.0f))
                                {
                                    player->setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                                }
                                else
                                {
                                    player->setPosition(wallBounds.left + wallBounds.width, playerBounds.top);
                                }
                            }
                            else
                            {
                                if(playerBounds.top + (playerBounds.height / 2.0f) < wallBounds.top + (wallBounds.height / 2.0f))
                                {
                                    player->setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
                                }
                                else
                                {
                                    player->setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
                                }

                            }
                        }
                        playerBounds = player->getBounds();
                    }
                }
            }
        }
// 2. POPRAWIONE KOLIZJE DLA WROGÓW W MAIN.CPP
        if(currentState == GameState::Gameplay)
        {
            for (auto& obj : worldObjects)
            {
                // ZABEZPIECZENIE GRACZA: Jeśli ten obiekt to gracz, ignorujemy go!
                if (obj == player) continue;

                Enemy* enemy = dynamic_cast<Enemy*>(obj);
                if (enemy != nullptr)
                {
                    sf::FloatRect enemyBounds = enemy->getBounds();

                    for (auto& object : worldObjects)
                    {
                        if (object == enemy) continue;

                        if (object->isSolid()) 
                        {
                            sf::FloatRect wallBounds = object->getBounds();
                            sf::FloatRect overlap;

                            if (enemyBounds.intersects(wallBounds, overlap))
                            {
                                if (overlap.width < overlap.height)
                                {
                                    // KOLIZJA BOCZNA (Oś X)
                                    if (enemyBounds.left < wallBounds.left)
                                    {
                                        // Odepchnięcie w lewo + margines 1.5f przeciw przyklejaniu
                                        enemy->setPosition(wallBounds.left - enemyBounds.width - 1.5f, enemy->getPosition().y);
                                    }
                                    else
                                    {
                                        // Odepchnięcie w prawo + margines 1.5f przeciw przyklejaniu
                                        enemy->setPosition(wallBounds.left + wallBounds.width + 1.5f, enemy->getPosition().y);
                                    }
                                }
                                else
                                {
                                    // KOLIZJA PIONOWA (Oś Y)
                                    if (enemyBounds.top < wallBounds.top)
                                    {
                                        // Odepchnięcie w górę
                                        enemy->setPosition(enemy->getPosition().x, wallBounds.top - enemyBounds.height - 1.5f);
                                    }
                                    else
                                    {
                                        // Odepchnięcie w dół
                                        enemy->setPosition(enemy->getPosition().x, wallBounds.top + wallBounds.height + 1.5f);
                                    }
                                }
                                // Aktualizujemy granice do kolejnych sprawdzeń
                                enemyBounds = enemy->getBounds();
                            }
                        }
                    }
                }
            }
        }
        // ==================== NOWA LOGIKA WALKI (ZABIJANIE WROGÓW) ====================
        if (currentState == GameState::Gameplay && player != nullptr)
        {
            sf::FloatRect playerBounds = player->getBounds();

            // ITERACJA OD TYŁU (od size()-1 do 0) – kluczowa do bezpiecznego usuwania obiektów!
            for (int i = static_cast<int>(worldObjects.size()) - 1; i >= 0; --i)
            {
                if (worldObjects[i] == player) continue;

                // --- ROZPOZNANIE PRZECIWNIKA ---
                Enemy* enemy = dynamic_cast<Enemy*>(worldObjects[i]);
                if (enemy != nullptr)
                {
                    // 1. Sprawdzamy, czy gracz w ogóle atakuje (klika LPM)
                    if (player->getIsAttacking())
                    {
                        // Pobieramy aktywny przedmiot
                        Item* activeItem = playerInventory.getActiveItem();

                        // 2. LOGIKA DLA MIECZA (Atak obszarowy 360 stopni wokół Linka)
                        if (activeItem != nullptr && activeItem->getName() == "Sword")
                        {
                            // Liczymy środki obiektów, żeby odległość była precyzyjna
                            sf::FloatRect pBounds = player->getBounds();
                            sf::FloatRect eBounds = enemy->getBounds();

                            sf::Vector2f playerCenter(pBounds.left + pBounds.width / 2.0f, pBounds.top + pBounds.height / 2.0f);
                            sf::Vector2f enemyCenter(eBounds.left + eBounds.width / 2.0f, eBounds.top + eBounds.height / 2.0f);

                            // Matematyczny Pitagoras: pierwiastek z (dx^2 + dy^2)
                            float dx = playerCenter.x - enemyCenter.x;
                            float dy = playerCenter.y - enemyCenter.y;
                            float distance = std::sqrt(dx * dx + dy * dy);

                            float attackRange = 100.0f; // Zasięg kołowy miecza

                            // Jeśli wróg jest wewnątrz koła ataku miecza
                            if (distance <= attackRange)
                            {
                                enemy->takeDamage(5); // 5 serduszek z miecza

                                // Odrzut działa w stronę, w którą leci wróg od środka Linka (promieniście)
                                sf::Vector2f knockbackDir = enemyCenter - playerCenter;
                                float len = std::sqrt(knockbackDir.x * knockbackDir.x + knockbackDir.y * knockbackDir.y);
                                if (len != 0.0f) knockbackDir /= len; // Normalizacja

                                enemy->applyKnockback(knockbackDir, 600.0f);
                                std::cout << "Miecz kosi dookola! HP wroga: " << enemy->getHP() << "\n";
                            }
                        }
                        // 3. LOGIKA DLA INNYCH PRZEDMIOTÓW / PIĘŚCI (Stary hitbox kierunkowy)
                        else
                        {
                            sf::FloatRect attackHitbox = player->getBounds();
                            float baseRange = 25.0f;
                            sf::Vector2f dir = player->getFacingDirection();

                            if (dir.x > 0) attackHitbox.width += baseRange;          
                            else if (dir.x < 0) { attackHitbox.left -= baseRange; attackHitbox.width += baseRange; } 
                            else if (dir.y > 0) attackHitbox.height += baseRange;    
                            else if (dir.y < 0) { attackHitbox.top -= baseRange; attackHitbox.height += baseRange; }

                            if (attackHitbox.intersects(enemy->getBounds()))
                            {
                                enemy->takeDamage(1); // 1 obrażenie bez miecza
                                
                                sf::Vector2f knockbackDir = player->getFacingDirection();
                                enemy->applyKnockback(knockbackDir, 600.0f);
                                std::cout << "Zwykly atak kierunkowy! HP wroga: " << enemy->getHP() << "\n";
                            }
                        }

                        // 4. Wspólna sekcja sprawdzania śmierci wroga po uderzeniu
                        if (enemy->isDead())
                        {
                            std::cout << "Przeciwnik zginal!\n";
                            sf::Vector2f sPos = enemy->getStartPosition();
                            std::string enemyKey = std::to_string((int)sPos.x) + "_" + std::to_string((int)sPos.y);
                            
                            defeatedEnemies[worldY][worldX].push_back(enemyKey);
                            
                            delete worldObjects[i];
                            worldObjects.erase(worldObjects.begin() + i);
                            continue; 
                        }
                    }
                    // Wróg dotyka gracza (gracz zbiera obrażenia) - to zostaje bez zmian
                    else if (player->getBounds().intersects(enemy->getBounds()))
                    {
                        if (!player->isInvincible()) 
                        {
                            player->takeDamage(1); 
                            sf::Vector2f diff = player->getPosition() - enemy->getPosition();
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            if (length != 0.0f) player->applyKnockback(diff / length, 500.0f);
                        }
                    }
                    continue; 
                }

                // --- ROZPOZNANIE POCISKU SZKIELETA --- (zostaje bez zmian na dole)
                // ... Twój niezmieniony kod pocisku ...
            
        
                // --- ROZPOZNANIE POCISKU SZKIELETA ---
                Projectile* bullet = dynamic_cast<Projectile*>(worldObjects[i]);
                if (bullet != nullptr)
                {
                    sf::FloatRect bulletBounds = bullet->getBounds();

                    if (playerBounds.intersects(bulletBounds))
                    {
                        if (!player->isInvincible())
                        {
                            player->takeDamage(2); 

                            sf::Vector2f bulletCenter(bulletBounds.left + bulletBounds.width/2.f, bulletBounds.top + bulletBounds.height/2.f);
                            sf::Vector2f diff = player->getPosition() - bulletCenter;
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            if (length != 0.0f)
                            {
                                sf::Vector2f pushDir = diff / length;
                                player->applyKnockback(pushDir, 400.0f);
                            }
                        }

                        // Bezpieczne czyszczenie pocisku
                        delete worldObjects[i];
                        worldObjects.erase(worldObjects.begin() + i);
                    }
                }
            }
        }
        
        else if(currentState==GameState::Kurtyna_lvl2)
        {
            if(transitionClock.getElapsedTime().asSeconds() >= 1.5f)
            {
                worldX = 3;
                worldY = 9;

                Rooms(worldX,worldY,worldObjects,340.0f,240.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien,tex_slime,tex_moblin_up,tex_moblin_down,tex_moblin_left,tex_moblin_righ,tex_skieleton_down,tex_skieleton_up,tex_skieleton_sides,tex_strzala,tex_piasek,tex_wysoka_trawa,tex_stone,tex_kwiatek, defeatedEnemies);

                currentState = GameState::Gameplay;
            }
        }

        if (currentState == GameState::Gameplay && player != nullptr)
        {
            if (player->getHP() <= 0)
            {
                currentState = GameState::GameOver;
                curtainHeight = 0.0f;

                backgroundMusic.stop(); 
                gameOverMusic.play();   
            }
        }

        if (currentState == GameState::GameOver)
        {
            if (curtainHeight < 528.0f)
            {
                curtainHeight += 500.0f * deltaTime;
                if (curtainHeight > 528.0f) curtainHeight = 528.0f;
                gameOverCurtain.setSize(sf::Vector2f(720.0f, curtainHeight));
            }
        }

        window.clear(sf::Color::Black);

        if(currentState == GameState::MainMenu)
        {
            window.draw(TitleText);
            window.draw(menuText);
            window.draw(TitleText2);
        }
        else if(currentState == GameState::Gameplay)
        {
            for(auto& object : worldObjects)
            {
                object->draw(window);
            }
            
            // DOPISZ TO (Rysowanie serduszek):
            if (player != nullptr)
            {
                sf::Sprite heartSprite(tex_serduszko);
                float startX = 20.0f;
                float startY = 440.0f; // dopasuj wysokość nad ekwipunek
                float odstep = 24.0f;
                for (int i = 0; i < player->getHP(); ++i)
                {
                    heartSprite.setPosition(startX + (i * odstep), startY);
                    window.draw(heartSprite);
                }
            }

            playerInventory.draw(window);
        }
        else if(currentState == GameState::Kurtyna_lvl2)
        {
            // Twój stary pusty blok (lub z napisem)
        }
        // DOPISZ TEN CAŁY NOWY WARUNEK NA KOŃCU SEKCJI RENDEROWANIA:
        else if(currentState == GameState::GameOver)
        {
            for(auto& object : worldObjects)
            {
                object->draw(window); 
            }
            window.draw(gameOverCurtain);

            if (curtainHeight >= 528.0f)
            {
                window.draw(gameOverText);
                window.draw(retryText);
            }
        }
        window.display();
    }

    for(auto& object : worldObjects)
    {
        delete object;
    }
    worldObjects.clear();

    return 0;
}