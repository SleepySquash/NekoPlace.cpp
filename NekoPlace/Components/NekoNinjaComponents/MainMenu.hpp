//
//  NekoMenu.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef NekoMenu_hpp
#define NekoMenu_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "Player.hpp"

#include "NekoNinja.hpp"
#include "../VNLightComponents/Novel.hpp"
#include "Popup.hpp"
#include "ItemDatabaseCollection.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;

namespace NekoNinja
{
    struct RoomActivityPlace
    {
        bool occupied{ false };
        int x{ 0 }, y{ 0 };
        std::wstring phrase{ L"oWo! I'm doing something!!!" };
        
        RoomActivityPlace(int x, int y, const std::wstring& phrase = L"oWo! I'm doing something!!!");
    };
    struct MainMenu;
    struct Room
    {
        static int roomWidth, roomHeight;
        static float x, y, xWidth, yHeight;
        static float scale;
        static float roomScale;
        static sf::FloatRect roomGlobalBounds;
        
        static sf::Image* mask;
        static sf::Texture* dialogueTexture;
        static vector<RoomActivityPlace> nekoActivity;
        
        enum class Page{ no, neko, nekolist, questlist, nekoteam };
        Page page{ Page::no };
        sf::RectangleShape blackScreenShape;
        
        sf::Vector2i dot;
        
        bool firstTimeIn{ false }; /// { true };
        MainMenu* menu{ nullptr };
        bool hasFocusOnNeko{ false }; sf::Vector2i pressedNekoPos{ 0, 0 };
        NekoEntity* focusOnNeko{ nullptr }, *pressedNeko{ nullptr };
        
        GUI::SpriteButton nekoListButton, questListButton, nekoTeamButton, quitButton;
        GUI::SpriteButton roomBackButton;
        Page nintReturnTo{ Page::no };
        
        float ntmNekoStartXX{ 0 }, ntmNekoScaling{ 1.f };
        sf::RectangleShape ntmNekoShape; int ntmChoosing{ 0 };
        GUI::SpriteButtons ntmNekoButton;
        
        bool active{ false };
        float move_dx{ 0 }, move_dy{ 0 };
        int roomLevel{ 0 }, lastLevel{ 0 };
        sf::Sprite sprite; bool spriteLoaded{ false };
        
        float scrollSensitivity{ 0.02f };
        float prevZoom{ 1 };
        sf::Vector2i middleZoom{ 0, 0 };
        float prevDistanceZoom{ 1.f };
        
        GUI::RectangleButton bNekosButton, bQuestButton;
        
        Room();
        ~Room();
        void Switch(bool on);
        void Destroy();
        void Update(const sf::Time& elapsedTime);
        void PollEvent(sf::Event& event);
        void Resize(unsigned int width, unsigned int height);
        void Draw(sf::RenderWindow* window);
        void CalculateCameraPosition();
        void CalculateCameraScale();
        void LoadTextureLevel();
        void RecieveMessage(MessageHolder& message);
    };
    
    
    struct LocationPlace
    {
        std::wstring name, background;
        int difficulty;
        LocationPlace(const std::wstring& name = L"UNKNOWN", const std::wstring& background = L"", const int& difficulty = 0);
    };
    struct MainMenu : Component
    {
        bool active{ true };
        
        Room room;
        sf::Sprite background;
        sf::Sprite neko; std::wstring nekoTextureName{ L"" }; float nekoRelScale{ 1.f };
        bool spriteLoaded{ false }, nekoSpriteLoaded{ false };
        bool verticalOrientation{ false };
        enum class Screen { main, stage, inventory, room, account };
        Screen screen{ Screen::main };
        
        GUI::TextButton playButton;
        GUI::SpriteButton roomButton;
        GUI::TextButton accountButton;
        GUI::TextButton accountBackButton;
        GUI::TextButton resetButton;
        
        static vector<LocationPlace*> places;
        int placeSelected{ 0 }; bool locationChanged{ false };
        int difficultyStartValue{ 0 };
        GUI::TextButton walkButton, addDifficultyButton;
        GUI::SpriteButton locLeftButton, locRightButton;
        sf::Sprite locationSprite;
        sf::Text locationNameText;
        
        sf::Sprite lootboxSprite;
        sf::CircleShape lootboxesCircle; sf::Text lootboxesCountText;
        GUI::TextButton inventoryButton;
        GUI::TextButton backButton;
        
        sf::Text dialogueText;
        GUI::SpriteButton dialogueButton;
        
        sf::Text helperText;
        sf::RectangleShape backShape;
        sf::RectangleShape accountBackShape;
        sf::RectangleShape expShape;
        
        //GUI::StageButtons stageButtons;
        
        void Init() override;
        void Destroy() override;
        void Update(const sf::Time& elapsedTime) override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
    };
}

#endif /* NekoMenu_hpp */
