//
//  Novel.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 24/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Novel_hpp
#define Novel_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/NovelSystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "../NekoNinjaComponents/NekoBase.hpp"
#include "../NekoNinjaComponents/NekoLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

using namespace ns;
using namespace NekoNinja;

namespace ns
{
    namespace VNLightComponents
    {
        struct Novel;
        struct Waiting : public NovelObject
        {
            float currentTime{ 0.f }, waitingTime{ 1.f };
            Novel* novel{ nullptr };
            
            void Update(const sf::Time& elapsedTime) override;
        };
        
        struct RectangleShape : NovelObject
        {
            sf::RectangleShape shape;
            Novel* novel{ nullptr };
            
            bool visible{ true };
            sf::Int8 alpha{ 0 }; int maxAlpha{ 160 };
            float currentTime{ 0.f }, appearTime{ 0.f }, disappearTime{ 0.f };
            
            enum modeEnum {appearing, existing, disappearing, deprecated};
            modeEnum mode{ appearing };
            
            enum sendMessageBackEnum {noMessage, atAppearance, atDisappearing, atDeprecated};
            sendMessageBackEnum sendMessageBack{ atAppearance };
            
            void Init() override;
            void Resize(unsigned int width, unsigned int height) override;
            void Update(const sf::Time& elapsedTime) override;
            void Draw(sf::RenderWindow* window) override;
        };
        
        struct Background : NovelObject
        {
            sf::Sprite sprite;
            Novel* novel{ nullptr };
            std::wstring imagePath{ L"" };
            
            bool spriteLoaded{ false }; bool visible{ true };
            sf::Int8 alpha{ 0 }; int maxAlpha{ 255 };
            float currentTime{ 0.f }, appearTime{ 0.6f }, disappearTime{ 0.6f };
            
            float defaultPositionX{ 0.f }, defaultPositionY{ 0.f };
            
            enum modeEnum {appearing, existing, disappearing, deprecated};
            modeEnum mode{ appearing };
            
            enum sendMessageBackEnum {noMessage, atAppearance, atDisappearing, atDeprecated};
            sendMessageBackEnum sendMessageBack{ atAppearance };
            
            float scaleX{ 1.f }, scaleY{ 1.f };
            
            bool doParallax{ gs::isParallaxEnabled };
            float parallaxPower { gs::defaultParallaxBackground };
            
            void LoadImage(const sf::String&);
            void Resize(unsigned int width, unsigned int height) override;
            void Update(const sf::Time& elapsedTime) override;
            void Draw(sf::RenderWindow* window) override;
            void Destroy() override;
            void PollEvent(sf::Event& event) override;
            void CalculateScale(unsigned int width, unsigned int height);
            void CalculateParallax(int mouseX, int mouseY);
        };
        
        struct Dialogue : NovelObject
        {
            sf::Text text;
            std::wstring textString{ L"" }, printingString{ L"" }, currentString{ L"" };
            
            sf::Sprite dialogueSprite, nameSprite;
            sf::Text charText;
            std::wstring charString{ L"" };
            NekoBase* nekoInfo{ nullptr };
            Novel* novel{ nullptr };
            
            bool drawCharacterName{ false }, fontLoaded{ false };
            sf::Int8 alpha{ 0 }; int maxAlpha{ 255 };
            float currentTime{ 0.f }, waitingTime{ 2.f }, appearTime{ 0.6f }, disappearTime{ 0.6f };
            
            bool visible{ true };
            enum modeEnum {appearing, waiting, waitingForTime, waitingForInput, waitingForChoose, disappearing, deprecated};
            modeEnum mode{ appearing };
            enum sendMessageBackEnum {noMessage, atAppearance, atDisappearing, atDeprecated};
            sendMessageBackEnum sendMessageBack{ atDisappearing };
            modeEnum afterAppearSwitchTo{ waitingForInput };
            
            unsigned int textAppearPos{ 0 }, textAppearMax{ 0 }, textAppearI{ 0 };
            float characterInSecond{ 0.04f }, elapsedCharacterSum{ 0 };
            enum class textAppearModeEnum {fading, printing};
            textAppearModeEnum textAppearMode{ textAppearModeEnum::printing };
            
            sf::IntRect workingArea;
            bool forcePressInsideDialogue{ true };
            std::wstring fontName{ L"Pacifica.ttf" };
            unsigned int characterSize{ 30 };
            float outlineThickness{ 1.f };
            
            Dialogue(Novel* novel);
            void Init() override;
            void Update(const sf::Time& elapsedTime) override;
            void PollEvent(sf::Event& event) override;
            void Draw(sf::RenderWindow* window) override;
            void Destroy() override;
            void Resize(unsigned int width, unsigned int height) override;
            void SetGroup(const list<Dialogue*>::iterator& element);
            void SetNeko(NekoBase* info);
            void SetCharacterName(const sf::String& characterName);
            void SetDialogue(const sf::String& dialogue);
        };
        
        struct Character : NovelObject
        {
            sf::Sprite sprite;
            std::wstring imagePath{ L"" };
            
            Novel* novel{ nullptr };
            vector<NekoBase*>::iterator nekoIterator;
            NekoBase* nekoInfo{ nullptr };
            sf::String state{ "" };
            
            bool spriteLoaded{ false };
            sf::Int8 alpha{ 0 };
            float currentTime{ 0.f };
            
            float defaultPositionX{ 0.f }, defaultPositionY{ 0.f };
            
            enum modeEnum {appearing, existing, disappearing, deprecated};
            modeEnum mode{ appearing };
            enum sendMessageBackEnum {noMessage, atAppearance, atDisappearing, atDeprecated};
            sendMessageBackEnum sendMessageBack{ atAppearance };
            modeEnum afterAppearSwitchTo{ existing };
            
            float scaleX{ 1.f }, scaleY{ 1.f }, relScale{ 1.f };
            
            enum positionEnum { custom, left, cleft, center, cright, right };
            positionEnum position { center };
            
            bool doParallax{ gs::isParallaxEnabled };
            float parallaxPower { gs::defaultParallaxNormal };
            
            int maxAlpha{ 255 };
            float appearTime{ 0.6f }, disappearTime{ 0.6f };
            bool visible{ true };
            
            void LoadState(const sf::String& state);
            void Resize(unsigned int width, unsigned int height) override;
            void Update(const sf::Time& elapsedTime) override;
            void Draw(sf::RenderWindow* window) override;
            void Destroy() override;
            void PollEvent(sf::Event& event) override;
            void CalculateParallax(int mouseX, int mouseY);
        };
        
        struct Novel : Component
        {
            NovelSystem layers;
            
            bool eof{ false };
            nss::CommandSettings command;
            
            std::wstring folderPath{ L"Data/Images/" };
            list<NovelObject*> onHold;
            list<std::wstring> lines;
            int preloadLinesAmount{ 12 };
            
            Novel();
            void Init() override;
            void Destroy() override;
            void Update(const sf::Time& elapsedTime) override;
            void PollEvent(sf::Event& event) override;
            void Resize(unsigned int width, unsigned int height) override;
            void Draw(sf::RenderWindow* window) override;
            void OnHold(NovelObject* component);
            void UnHold(NovelObject* component);
            std::wstring GetFolderPath();
        };
    }
}

#endif /* Novel_hpp */
