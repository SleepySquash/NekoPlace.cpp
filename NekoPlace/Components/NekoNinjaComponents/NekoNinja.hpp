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
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "Player.hpp"
#include "NekoLibrary.hpp"
#include "NekoNinjaSettings.hpp"
#include "EventDatabaseCollection.hpp"

#include "GameplayEffects.hpp"
#include "Interfaces/Popup.hpp"
#include "EventAlert.hpp"

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
        sf::Text strikeText;
        sf::Sprite pauseSprite;
        sf::Sprite heartSprite;
        sf::Sprite handSprite;
        
        sf::RectangleShape nekoButShape, nekoCooldownShape;
        sf::Text nekoButText;
        GUI::SpriteButton nekoButton;
        
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
        
        bool scoresAlreadyColored{ false };
        bool newNekoAvailable{ false }; bool newNekoReadNovels{ false };
        vector<NekoBase*> newNekoList; vector<NekoBase*>::iterator newNekoIterator;
        vector<NekoEntity*> newEntityList;
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
        static Controller* control;
        static GUIOverlay* gui;
        static unsigned int* tamedArray;
        
        float xVelocity{ 0.f }, yVelocity{ 0.f }, gravity{ 0.f };
        float x{ 0 }, y{ 0 };
        float nekoScale{ 1.f }, relScale{ 1.f }, hitboxScale{ 1.f };
        bool tamed{ false }, offline{ false }, newneko{ false }, offsetYYUp{ false };
        unsigned int nekoIndex{ 0 };
        float spriteHeight{ 0 };
        sf::FloatRect hitbox;
        
        int halfTheWidth{ 0 };
        sf::Sprite sprite;
        sf::Sound sound;
        NekoBase* nekoInfo{ nullptr };
        
        void Init();
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void CheckInterception(const sf::Vector2<int>& point);
        void Tame();
        void Resize();
        void Draw(sf::RenderWindow* window);
    };
    
    struct ControllerSettings
    {
        std::wstring backgroundImage;
        int difficulty;
        
        ControllerSettings(const int& difficulty = 0, const std::wstring& backgroundImage = L"Data/Images/park.jpg") : difficulty(difficulty), backgroundImage(backgroundImage) { }
    };
    struct Controller : Component, MessageSender
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
        float harderElapsedTime{ 0.f }, getHarderTime{ nns::difficultyLevelUpTime };
        std::wstring backgroundImage{ L"Data/Images/park.jpg" };
        float criticalHitPossibility{ 0.01f }, eventPossibility{ 0.5f };
        float nekoScale{ 1.f };//, timeMultiplier{ 1.f };
        
        bool eventIsUp{ false }; EventBase* event{ nullptr };
        std::string previousEvent{ "" }; int eventsInRow{ 0 };
        
        float attackingElapsedTime{ 0.f }, attackingAreaTime{ 0.5f };
        bool attackIsDone{ true };
        int attackType{ 0 }, attackCount{ 0 };
        
        bool spawnNekos{ true }, isGameOver{ false }, needsInit{ true };
        AbilityBase* ability{ nullptr }; bool abilityIsUp{ false }, abilityOnCooldown{ false }, abilityIsCooldownBased{ false };
        unsigned long abilityNekoCounter{ std::numeric_limits<unsigned long>::infinity() }; int abilityCooldownGUI{ 0 };
        float abilityElapsedCooldown{ 0.f }, lastTouchedMoment{ 0.f };
        bool countdownLastTouchedMoment{ false };
        
        float elapsedCombo{ 0.f }, comboThreshold{ 0.07f };
        int comboCounter{ 0 };
        sf::Vector2f lastNekoPosition;
        
        float elapsedSeconds{ 0.9f }, nextAttackTime{ 2.f }, baseAttackTime{ 1.2f }, randomAttackTime{ 1.2f };
        unsigned int score{ 0 }, maxlifes{ nns::maxLifes }, lifes{ maxlifes }, strike{ 0 }, maxStrike{ 0 };
        unsigned int lifeScore{ nns::lifeScoreDelta };
        unsigned long nekoCounter{ 0 };
        
        sf::Vector2i startPoint{ 0, 0 }, endPoint{ 0, 0 };
        list<sf::Vector2i> cursor; bool cursorEnabled{ false };
        bool swipeConfirmed{ false };
        float elapsedCursor{ 0.f };
        sf::Vertex line[2];
        
        Controller(const ControllerSettings& settings = ControllerSettings());
        void Destroy() override;
        void Init() override;
        void ApplyGameSettings();
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
        void UpdateCooldownCounter();
        void ActivateAbility();
        void SendMessage(MessageHolder message) override;
    };
}

#endif /* NekoNinjaComponents_hpp */
