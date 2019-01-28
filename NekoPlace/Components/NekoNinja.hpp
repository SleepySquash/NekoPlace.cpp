//
//  NekoNinjaComponents.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoNinjaComponents_hpp
#define NekoNinjaComponents_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../Essentials/ResourcePath.hpp"
#include "../Engine/EntitySystem.hpp"
#include "../Engine/StaticMethods.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;

namespace NekoNinja
{
    struct SceneBackground : Component
    {
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        
        void Destroy() override;
        void Init() override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
    
    
    
    struct NekoInfo
    {
        std::wstring name{ L"" };
        unsigned int chance{ 0 };
        sf::IntRect offsets;
        
        NekoInfo() { }
        NekoInfo(const std::wstring& name, const unsigned int& chance, int l = 15, int t = 15, int w = 15, int h = 15) : name(name), chance(chance), offsets(l, t, w, h) { }
    };
    struct NekoLibrary
    {
        static vector<NekoInfo> neko;
    };
    typedef NekoLibrary nl;
    
    
    
    
    struct Controller;
    struct NekoObject
    {
        static float gravity;
        static float yStartVelocity;
        static int yStartRandomVelocity, xStartRandomVelocity;
        
        static Controller* control;
        
        float xVelocity{ 0.f }, yVelocity{ 0.f };
        float x{ 0 }, y{ 0 };
        bool tamed{ false }, offline{ false };
        
        int halfTheWidth{ 0 };
        sf::Sprite sprite;
        NekoInfo* nekoInfo{ nullptr };
        
        void Init();
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void CheckInterception(sf::Vector2<int>& point);
        void Resize();
        void Draw(sf::RenderWindow* window);
    };
    
    struct Controller : Component
    {
        list<NekoObject> nekos;
        std::wstring backgroundImage{ L"Data/Images/park.jpg" };
        
        bool spawnNekos{ true };
        bool isGameOver{ false };
        
        float elapsedSeconds{ 0.f };
        unsigned int score{ 0 }, topScore{ 0 }, lifes{ 3 }, maxlifes{ 4 };
        
        sf::Vector2<int> startPoint{ 0, 0 }, endPoint{ 0, 0 };
        list<sf::Vector2<int>> cursor;
        bool swipeConfirmed{ false };
        float elapsedCursor{ 0.f };
        sf::Vertex line[2];
        
        void Destroy() override;
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void PollEvent(sf::Event& event) override;
        void GameOver();
        void ResetGame();
        void DefaultGamemodeSettings();
        
        // Свайп реализуется следующим образом: в некий вектор добавляются точки (например, в sf::Lines), которые говорят о текущем положении нажатого пальца. При отпускании этого пальца, точки стираются. Также точки стираются, начиная с конца, если с момента их появления прошло больше, скажем, 0.2 секунд. Коллизия проверяется относительно этих точек. И добавление этих точек (как и уничтожение) должно происходить через определённые промежутки времени для оптимизации работы свайпа.
    };
    
    
    
    struct GUIOverlay : Component
    {
        Controller* control{ nullptr };
        bool fontLoaded{ false };
        
        sf::RectangleShape pauseShape;
        sf::Text pauseText;
        
        sf::Text scoreText;
        sf::Sprite pauseSprite;
        sf::Sprite heartSprite;
        sf::Sprite handSprite;
        
        float heartsXX{ 0 };
        
        GUIOverlay(Controller* control) : control(control) { }
        void Destroy() override;
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Draw(sf::RenderWindow* window) override;
        void Resize(unsigned int width, unsigned int height) override;
        void PollEvent(sf::Event& event) override;
    };
}

#endif /* NekoNinjaComponents_hpp */
