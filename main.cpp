#include <iostream>
#include "Game.h"
#include "Background.h"
#include "Link.h"
#include "Enemies.h"
#include "Chest.h"
#include "Ptaszek.h"
#include "Inventory.h"

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
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','F'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','F','F','F','F','F','F','F','F','F','F','F','F','F','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room2[11][15]=
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

const char room3[11][15]=
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

const char room4[11][15]=
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

const char room5[11][15]=
{
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
    {'#','C','C','C','C','C','C','C','C','C','C','C','C','C','#'},
    {'#','C','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'#','C','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'S','S','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'S','S','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'S','S','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'#','C','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'#','C','S','S','S','S','S','S','S','S','S','S','S','C','#'},
    {'#','C','C','C','C','C','C','C','C','C','C','C','C','C','#'},
    {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

const char room6[11][15]=
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
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room7[11][15]=
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
    {'#','#','#','#','#','#','F','F','F','#','#','#','#','#','#'}
};

const char room8[11][15]=
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

const char room9[11][15]=
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

void Rooms(int wx, int wy, std::vector<Game*>& wordlObjects, float startX, float startY, const sf::Texture& t_drzewa, const sf::Texture& t_floor1, const sf::Texture& t_floor2, const sf::Texture& t_cien)
{
    // 1. ZAPAMIĘTYWANIE HP: Jeśli gracz już istniał, pobieramy jego obecne punkty życia
    int currentHP = 10; // Domyślnie 10 (np. przy pierwszym uruchomieniu gry)
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
        }
    }

    // 2. PRZYWRACANIE HP: Tworzymy nowego Linka w nowym miejscu...
    player = new Link(startX, startY);
    
    // ... i ustawiamy mu zapamiętane punkty życia!
    player->setHP(currentHP); 
    
    wordlObjects.push_back(player);

    // --- Reszta Twojego kodu z potworami (Slime, Chest, Moblin itd.) ---
    if(selectedRoom==room2)
    {
        wordlObjects.push_back(new Slime(600.0f, 400.0f));
        wordlObjects.push_back(new Slime(600.0f, 200.0f));
    }
    if(selectedRoom==room5)
    {
        wordlObjects.push_back(new Chest(336.0f, 240.0f, "SWORD"));
    }
    if(selectedRoom==room3)
    {
        wordlObjects.push_back(new Slime(600.0f, 400.0f));
        wordlObjects.push_back(new Slime(600.0f, 200.0f));
        wordlObjects.push_back(new Slime(300.0f, 200.0f));
    }
    if(selectedRoom==room7)
    {
        wordlObjects.push_back(new Moblin(100.0f, 100.0f));
        wordlObjects.push_back(new Skieleton(400.0f, 300.0f));
    }
    if(selectedRoom==room8)
    {
        wordlObjects.push_back(new Moblin(100.0f, 400.0f));
        wordlObjects.push_back(new Moblin(100.0f, 300.0f));
        wordlObjects.push_back(new Moblin(100.0f, 200.0f));
    }
    if(selectedRoom==room9)
    {
        wordlObjects.push_back(new Skieleton(336.0f,240.0f));
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(720, 528), "The legend of Zelda");
    window.setFramerateLimit(60);

    sf::Texture tex_drzewa, tex_floor1, tex_floor2, tex_cien, tex_serduszko;
    if (!tex_drzewa.loadFromFile("grafiki/drzewa.png") ||
        !tex_floor1.loadFromFile("grafiki/floor1.png") ||
        !tex_floor2.loadFromFile("grafiki/floor2.png") ||
        !tex_cien.loadFromFile("grafiki/cien.png") ||
        !tex_serduszko.loadFromFile("grafiki/Serduszko2.png"))
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

                    Rooms(worldX, worldY, worldObjects, 340.0f, 240.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien);
                    currentState = GameState::Gameplay;
                }
            }

            if (currentState == GameState::GameOver)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                {
                    std::cout << "Restart gry... Ladowanie poziomu od nowa.\n";
        
                    // Resetujemy pozycję mapy do punktu startowego (tak jak przy nowej grze)
                    worldX = 1;
                    worldY = 4;
        
                    // Ładujemy pokój startowy na nowo. Rooms automatycznie stworzy nowego Linka.
                    // Podajemy domyślne współrzędne startowe (np. środek ekranu 340, 240)
                    Rooms(worldX, worldY, worldObjects, 340.0f, 240.0f, tex_drzewa, tex_floor1, tex_floor2, tex_cien);
        
                    // Ponieważ Rooms tworzy nowego Linka z 10 HP, upewniamy się, że ma pełne zdrowie
                    if (player != nullptr) {
                        player->setHP(10); 
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
                    Rooms(worldX, worldY, worldObjects, 20.0f, playerPos.y, tex_drzewa,tex_floor1,tex_floor2,tex_cien);
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
                    Rooms(worldX, worldY, worldObjects, 630.0f, playerPos.y, tex_drzewa,tex_floor1,tex_floor2,tex_cien);
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
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 20.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien);
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
                    Rooms(worldX, worldY, worldObjects, playerPos.x, 500.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien);
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
// 3. LOGIKA WALKI: ZASIĘG GRACZA, ATAK POTWORÓW ORAZ POCISKI
        if (currentState == GameState::Gameplay && player != nullptr)
        {
            sf::FloatRect playerBounds = player->getBounds();

            // Hitbox ataku Linka
            sf::FloatRect attackHitbox = playerBounds;
            float attackRange = 25.0f; 

            if (player->getIsAttacking())
            {
                sf::Vector2f dir = player->getFacingDirection();
                if (dir.x > 0) attackHitbox.width += attackRange;          
                else if (dir.x < 0) { attackHitbox.left -= attackRange; attackHitbox.width += attackRange; } 
                else if (dir.y > 0) attackHitbox.height += attackRange;    
                else if (dir.y < 0) { attackHitbox.top -= attackRange; attackHitbox.height += attackRange; } 
            }

            for (size_t i = 0; i < worldObjects.size(); ++i)
            {
                if (worldObjects[i] == player) continue;

                // --- ROZPOZNANIE PRZECIWNIKA (Moblin, Slime, Skieleton) ---
                Enemy* enemy = dynamic_cast<Enemy*>(worldObjects[i]);
                if (enemy != nullptr)
                {
                    sf::FloatRect enemyBounds = enemy->getBounds();

                    if (player->getIsAttacking() && attackHitbox.intersects(enemyBounds))
                    {
                        sf::Vector2f knockbackDir = player->getFacingDirection();
                        enemy->applyKnockback(knockbackDir, 600.0f); 
                        std::cout << "Link slashed an enemy!\n";
                    }
                    else if (playerBounds.intersects(enemyBounds))
                    {
                        if (!player->isInvincible()) 
                        {
                            player->takeDamage(1); // Traci 1 HP

                            sf::Vector2f diff = player->getPosition() - enemy->getPosition();
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            if (length != 0.0f)
                            {
                                sf::Vector2f pushDir = diff / length;
                                player->applyKnockback(pushDir, 500.0f);
                            }
                        }
                        std::cout << "Enemy collided with Link!\n";
                    }
                    continue; 
                }

                // --- ROZPOZNANIE POCISKU SZKIELETA ---
                Projectile* bullet = dynamic_cast<Projectile*>(worldObjects[i]);
                if (bullet != nullptr)
                {
                    sf::FloatRect bulletBounds = bullet->getBounds();

                    if (playerBounds.intersects(bulletBounds))
                    {
                        if (!player->isInvincible())
                        {
                            player->takeDamage(2); // Pocisk zabiera 2 HP

                            sf::Vector2f bulletCenter(bulletBounds.left + bulletBounds.width/2.f, bulletBounds.top + bulletBounds.height/2.f);
                            sf::Vector2f diff = player->getPosition() - bulletCenter;
                            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
                            if (length != 0.0f)
                            {
                                sf::Vector2f pushDir = diff / length;
                                player->applyKnockback(pushDir, 400.0f);
                            }
                        }

                        // Pocisk niszczy się niezależnie od tego, czy zadał obrażenia
                        delete worldObjects[i];
                        worldObjects.erase(worldObjects.begin() + i);
                        --i; 
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

                Rooms(worldX,worldY,worldObjects,340.0f,240.0f, tex_drzewa,tex_floor1,tex_floor2,tex_cien);

                currentState = GameState::Gameplay;
            }
        }

        if (currentState == GameState::Gameplay && player != nullptr)
        {
            if (player->getHP() <= 0)
            {
                currentState = GameState::GameOver;
                curtainHeight = 0.0f;
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

            playerInventory.render(window);
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