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

using namespace ns;

namespace NekoNinja
{
    struct Player
    {
        static Player* self;
        
        unsigned int level{ 1 };
        unsigned int exp{ 0 };
        unsigned int expNeeded{ 100 };
        unsigned int lootboxes{ 0 };
        
        float expRatio{ 0 };
        
        std::wstring settingsPath{ base::utf16(documentsPath()) + L"Settings.nekoninja" };
        std::wstring scoresPath{ base::utf16(documentsPath()) + L"Progress.nekoninja" };
        
        Player();
        ~Player();
        void SaveData();
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
        sf::IntRect offsets;
        
        NekoInfo() { }
        NekoInfo(const std::wstring& name, const float& chance, int l = 15, int t = 15, int w = 15, int h = 15) : name(name), chance(chance), offsets(l, t, w, h) { }
    };
    struct NekoLibrary
    {
        static vector<NekoInfo> neko;
    };
    typedef NekoLibrary nl;
    
    
    
    
    struct Controller;
    struct GUIOverlay : Component
    {
        NovelSystem system;
        Controller* control{ nullptr };
        bool fontLoaded{ false };
        
        sf::RectangleShape pauseShape;
        GUIButton pauseButton;
        sf::Text pauseText;
        
        sf::Text scoreText;
        sf::Sprite pauseSprite;
        sf::Sprite heartSprite;
        sf::Sprite handSprite;
        
        bool drawResults{ true };
        sf::Text resultsText;
        sf::Text helperText;
        GUIButton resultsOkButton;
        sf::RectangleShape resultsShape;
        sf::RectangleShape resultsLine;
        sf::RectangleShape expShape;
        unsigned int prevLevel;
        float prevExpRatio;
        float results_yyStart{ 0 };
        
        sf::Text menuText;
        sf::RectangleShape menuShape;
        GUIButton menuLeaveButton;
        GUIButton menuRetryButton;
        
        float heartsXX{ 0 };
        
        GUIOverlay() { }
        GUIOverlay(Controller* control) : control(control) { }
        void Destroy() override;
        void Init() override;
        void Update(const sf::Time& elapsedTime) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void PollEvent(sf::Event& event) override;
    };
    
    
    
    
    struct NekoObject
    {
        static float gravity;
        static float yStartVelocity;
        static int yStartRandomVelocity, xStartRandomVelocity;
        
        static Controller* control;
        
        float xVelocity{ 0.f }, yVelocity{ 0.f };
        float x{ 0 }, y{ 0 };
        float nekoScale{ 1.f };
        bool tamed{ false }, offline{ false };
        
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
    
    struct GUIOverlay;
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
        float harderElapsedTime{ 0.f }, getHarderTime{ 40.f };
        std::wstring backgroundImage{ L"Data/Images/park.jpg" };
        float criticalHitPossibility{ 0.01f }, eventPossibility{ 0.35f };
        float nekoScale{ 1.f };
        static float scoreMultiplier;
        
        bool eventIsUp{ false };
        
        float attackingElapsedTime{ 0.f }, attackingAreaTime{ 0.44f };
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
