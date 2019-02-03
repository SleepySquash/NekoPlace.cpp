//
//  NekoNinja.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
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

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/NovelSystem.hpp"
#include "../../Engine/StaticMethods.hpp"
#include "../../Engine/GUIInterface.hpp"
#include "NekoHelpers.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoNinja
{
    struct NekoInfo;
    struct Player
    {
        static Player* self;
        std::wstring username{ L"Хозяин228" };
        
        unsigned int level{ 1 };
        unsigned int exp{ 0 };
        unsigned int expNeeded{ 100 };
        unsigned int lootboxes{ 0 };
        
        float expRatio{ 0 };
        
        std::wstring settingsPath{ base::utf16(documentsPath()) + L"Settings.nekoninja" };
        std::wstring nekoPath{ base::utf16(documentsPath()) + L"Neko.nekoninja" };
        std::wstring scoresPath{ base::utf16(documentsPath()) + L"Progress.nekoninja" };
        
        Player();
        ~Player();
        void SaveData();
        void SaveNeko(NekoInfo* info);
        void SaveNekos();
        void AddExperience(unsigned int xp);
    };
    



    struct SceneBackground : Component
    {
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        std::wstring imagePath{ L"" };
        
        SceneBackground() { }
        SceneBackground(const std::wstring& imagePath) : imagePath(imagePath) { }
        void Destroy() override;
        void Init() override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
    
    
    
    struct NekoInfo
    {
        std::wstring name{ L"" };
        float chance{ 0.f };
        sf::Color color;
        sf::IntRect offsets;
        
        bool tamed{ false };
        unsigned int level{ 1 };
        
        NekoInfo() { }
        NekoInfo(const std::wstring& name, const float& chance, int l = 15, int t = 15, int w = 15, int h = 15, bool tamed = false, const sf::Color& ocolor = sf::Color::Transparent) : name(name), chance(chance), offsets(l, t, w, h), tamed(tamed), color(ocolor)
        {
            if (ocolor == sf::Color::Transparent)
            {
                if (chance >= 0.25)         color = sf::Color(200, 200, 200, 120);
                else if (chance >= 0.09)    color = sf::Color(106, 143, 203, 255);
                else if (chance >= 0.04)    color = sf::Color(87, 173, 98, 255);
                else if (chance >= 0.01)    color = sf::Color(218, 113, 114, 255);
                else if (chance >= 0.005)   color = sf::Color(188, 106, 167, 255);
                else                        color = sf::Color(149, 192, 95, 255);
            }
        }
    };
    struct NekoLibrary
    {
        static vector<NekoInfo> neko;
    };
    struct NekoEntity
    {
        sf::Sprite sprite;
        bool spriteLoaded{ false };
        NekoInfo* info{ nullptr };
        
        bool inited{ false };
        float x{ 0 }, y{ 0 };
        
        bool moveRight{ false }, moveLeft{ false }, moveDown{ false }, moveUp{ false };
        float xSpd{ 140 }, ySpd{ 140 };
        
        float movingTo_x{ 0 }, movingTo_y{ 0 };
        bool newPoint{ true };
        float elapsedWaiting{ 0 }, waitingTime{ 1.2f };
        
        NekoEntity(NekoInfo* info) : info(info) { }
        void Init();
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void PollEvent(sf::Event& event);
        void Resize();
        void Draw(sf::RenderWindow* window);
    };
    struct RoomLibrary
    {
        static list<NekoEntity> neko;
    };
    typedef NekoLibrary nl;
    typedef RoomLibrary rl;
    
    
    
    
    
    struct Controller;
    struct GUIOverlay : Component
    {
        NovelSystem system;
        Controller* control{ nullptr };
        bool fontLoaded{ false };
        
        sf::RectangleShape pauseShape;
        GUI::TextButton pauseButton;
        sf::Text pauseText;
        
        sf::Text scoreText;
        sf::Sprite pauseSprite;
        sf::Sprite heartSprite;
        sf::Sprite handSprite;
        
        bool drawResults{ true }, drawNewNeko{ false };
        sf::Text resultsText;
        sf::Text helperText;
        GUI::TextButton resultsOkButton;
        sf::RectangleShape resultsShape;
        sf::RectangleShape resultsLine;
        sf::RectangleShape expShape;
        unsigned int prevLevel;
        float prevExpRatio;
        float results_yyStart{ 0 }, results_yyNeko{ 0 };
        
        sf::Text menuText;
        sf::RectangleShape menuShape;
        GUI::TextButton menuLeaveButton;
        GUI::TextButton menuRetryButton;
        
        float heartsXX{ 0 };
        
        bool newNekoAvailable{ false };
        list<NekoInfo*> newNekoList;
        sf::RectangleShape newNekoShape, newNekoLine;
        sf::Sprite newNekoSprite;
        
        GUIOverlay() { }
        GUIOverlay(Controller* control) : control(control) { }
        void Destroy() override;
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void PollEvent(sf::Event& event) override;
        void GameOver();
    };
    
    
    
    
    struct NekoObject
    {
        static float gravity;
        static float yStartVelocity;
        static int yStartRandomVelocity, xStartRandomVelocity;
        
        static Controller* control;
        static unsigned int* tamedArray;
        
        float xVelocity{ 0.f }, yVelocity{ 0.f };
        float x{ 0 }, y{ 0 };
        float nekoScale{ 1.f };
        bool tamed{ false }, offline{ false };
        unsigned int nekoIndex{ 0 };
        
        int halfTheWidth{ 0 };
        sf::Sprite sprite;
        sf::Sound sound;
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
        ///////////////
        ///
        /// backgroundImage
        /// lifeScore
        /// maxlifes
        /// criticalHitPossibility
        /// comboThreshold
        /// static Scoremultiplier
        /// getHarderTime
        ///
        /// nekoScale
        /// velocityMultiplier
        /// attackingAreaTime
        /// nextAttackTime
        /// static NekoObject's GamemodeSettings()
        ///     NekoObject::gravity = 0.19f * 60 * 60;
        ///     NekoObject::yStartVelocity = -17.f * 60;
        ///     NekoObject::yStartRandomVelocity = 4*60;
        ///
        ///////////////
        
        NovelSystem system;
        Player* player;
        
        SceneBackground background;
        GUIOverlay gui;
        list<NekoObject*> nekos;
        NekoObject* firstNeko{ nullptr };
        
        int currentDifficulty{ 0 }, difficulty{ 0 };
        float harderElapsedTime{ 0.f }, getHarderTime{ 36.f };
        std::wstring backgroundImage{ L"Data/Images/park.jpg" };
        float criticalHitPossibility{ 0.01f }, eventPossibility{ 0.35f };
        float nekoScale{ 1.f };
        static float scoreMultiplier;
        
        bool eventIsUp{ false };
        
        float attackingElapsedTime{ 0.f }, attackingAreaTime{ 0.45f };
        bool attackIsDone{ true };
        int attackType{ 0 }, attackCount{ 0 };
        
        bool spawnNekos{ true };
        bool isGameOver{ false };
        bool needsInit{ true };
        
        float elapsedCombo{ 0.f }, comboThreshold{ 0.07f };
        int comboCounter{ 0 };
        sf::Vector2f lastNekoPosition;
        
        float elapsedSeconds{ 0.9f }, nextAttackTime{ 2.f }, baseAttackTime{ 1.2f }, randomAttackTime{ 1.2f };
        unsigned int score{ 0 }, topScore{ 0 }, lifes{ 3 }, maxlifes{ 3 };
        unsigned int lifeScore{ 1000 };
        
        sf::Vector2i startPoint{ 0, 0 }, endPoint{ 0, 0 };
        list<sf::Vector2i> cursor;
        bool swipeConfirmed{ false };
        float elapsedCursor{ 0.f };
        sf::Vertex line[2];
        
        Controller(bool needsInit = true) : needsInit(needsInit) { }
        void Destroy() override;
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void PollEvent(sf::Event& event) override;
        void GameOver();
        void ResetGame();
        void DefaultGamemodeSettings();
        void ReturnToMenu();
        void SpawnNeko();
        void ApplyDifficulty(const int& diff);
    };
}

#endif /* NekoNinjaComponents_hpp */
