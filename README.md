# 🎮 The Legend of Zelda

## 📝 Krótki opis projektu

Projekt akademicki przedstawiający dwuwymiarową grę typu twin-stick shooter z elementami RPG oraz roguelike, zrealizowany w ramach przedmiotu **Programowanie strukturalne i obiektowe** na Politechnice Poznańskiej. Projekt demonstruje praktyczne wykorzystanie programowania obiektowego w C++, zarządzanie stanami gry (menu, rozgrywka, game over), implementację fizyki (kolizje) oraz obsługę zasobów multimedialnych.

Głównym celem gracza jest pokonanie wszystkich potworów oraz final boss'a, unikając przy tym utraty punktów życia. 

### 🛠️ Główne założenia techniczne:
*   **Język programowania:** C++ (standard C++17)
*   **Biblioteka graficzna:** SFML 2.6.1
*   **Środowisko programistyczne:** Visual Studio Code

## 🕹️ Sterowanie

- **Poruszanie się:** `W` `A` `S` `D`
- **Atak:** `Lewy Przycisk Myszy (LPM)`
- **Interakcja z Eliksirem:** `Prawy Przycisk Myszy (PPM)`
- **Wybór przedmiotu w ekwipunku:** `Scroll Myszy`
- **Otwieranie skrzyni:** `E`

## Final Boss

Final Boss ma specyficzną, inną od pozostałych przeciwników mechanikę. Aby zadawać mu obrażenia należy atakować go tylko wtedy, gdy ma on otwarte oko. W innym przypadku nie dostaje obrażeń. Pokonanie go wiąże się z zakończeniem gry oraz przejścia do Menu.

## 👤 Autorzy
- **Oskar Wójcicki** - [OskarWojcicki](https://github.com/OskarWojcicki)
- **Dominik Zieliński** - [Dominik5062](https://github.com/Dominik5062)

## ⚠️ Zastrzeżenie prawne i wykorzystanie zasobów

### Użytek edukacyjny (Educational Use Only)
Niniejsza gra została stworzona wyłącznie jako **projekt studencki** w celach edukacyjnych. Projekt ma charakter całkowicie niekomercyjny, nie jest dystrybuowany w celach zarobkowych i nie przynosi żadnych korzyści majątkowych autorom.

### Prawa autorskie do zasobów graficznych
- Wszystkie materiały graficzne (tekstury, sprite'y, tła) oraz dźwiękowe użyte w projekcie zostały pobrane z publicznie dostępnych źródeł internetowych i wykorzystane wyłącznie w celach demonstracyjnych i edukacyjnych.
- Autor projektu nie rości sobie żadnych praw autorskich do użytych assetów graficznych i multimedialnych. Wszelkie prawa należą do ich prawowitych twórców.

## 📄 Licencja Projektu
Kod źródłowy tego projektu (pliki `.cpp`, `.h`, konfiguracja) jest dostępny na licencji **MIT**. *Uwaga: Licencja MIT dotyczy wyłącznie kodu źródłowego i nie obejmuje zewnętrznych assetów graficznych oraz dźwiękowych, które podlegają własnym licencjom.*
