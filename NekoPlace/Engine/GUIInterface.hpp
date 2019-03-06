//
//  GUIInterface.hpp
//  AlternativeTransport-MLaToA2018
//
//  Created by Никита Исаенко on 22/11/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef GUIInterface_hpp
#define GUIInterface_hpp

#include <SFML/Graphics.hpp>

#include "../Essentials/Base.hpp"
#include "StaticMethods.hpp"

namespace ns
{
    enum class Halign { Left, Center, Right };
    enum class Valign { Top, Center, Bottom };
    namespace GUI
    {
        struct Button
        {
            sf::Vector2i dot;
            int maxAlpha{ 255 };
            bool visible{ true }, active{ true };
            
            Halign halign{ Halign::Center };
            Valign valign{ Valign::Center };
            
            virtual void Draw(sf::RenderTarget* window);
            virtual void Resize(unsigned int width, unsigned int height);
            virtual bool PollEvent(sf::Event& event);
            virtual void setAlpha(const sf::Int8& alpha);
            virtual void setPosition(float x, float y);
            virtual void setVisible(bool vis);
        };
        
        struct TextButton : Button
        {
            bool fontLoaded{ false };
            
            bool onPress{ false };
            bool wasPressed{ false };
            bool ignoreWasPressed{ false };
            bool characterScale{ false };
            
            sf::Text text;
            sf::String string;
            unsigned int characterSize{ 20 };
            float thickness{ 1.f };
            
            TextButton();
            void Draw(sf::RenderTarget* window) override;
            void Resize(unsigned int width, unsigned int height) override;
            bool PollEvent(sf::Event& event) override;
            void setAlpha(const sf::Int8& alpha) override;
            void setPosition(float x, float y) override;
            void setFont(const std::wstring& fontname);
            void setString(const std::wstring& string);
            void setCharacterSize(const unsigned int size);
        };
        
        struct SpriteButton : Button
        {
            bool spriteLoaded{ false };
            
            bool onPress{ false };
            bool wasPressed{ false };
            bool ignoreWasPressed{ false };
            bool characterScale{ false };
            bool invertXAxes{ false };
            
            sf::Sprite sprite;
            std::wstring textureName{ L"" };
            float scale{ 1.f };
            
            ~SpriteButton();
            void Draw(sf::RenderTarget* window) override;
            void Resize(unsigned int width, unsigned int height) override;
            bool PollEvent(sf::Event& event) override;
            void setAlpha(const sf::Int8& alpha) override;
            void setPosition(float x, float y) override;
            void setTexture(const std::wstring& texture);
            void setTexture(sf::Texture* texture);
            void setScale(const float& scl);
        };
        
        struct SpriteButtons : Button
        {
            bool spriteLoaded{ false };
            
            bool anyButtonPressed{ false };
            unsigned long index{ 0 }, pressedIndex{ 0 };
            bool onPress{ false };
            bool ignoreWasPressed{ false };
            bool characterScale{ false };
            
            sf::Sprite sprite;
            std::wstring textureName{ L"" };
            float scale{ 1.f };
            
            ~SpriteButtons();
            void Draw(sf::RenderTarget* window) override;
            void Resize(unsigned int width, unsigned int height) override;
            bool PollEvent(sf::Event& event) override;
            void eventPolled(sf::Event& event);
            void setAlpha(const sf::Int8& alpha) override;
            void setPosition(float x, float y) override;
            void setTexture(const std::wstring& texture);
            void setTexture(sf::Texture* texture);
            void setScale(const float& scl);
        };
        
        struct RectangleButton : Button
        {
            bool fontLoaded{ false };
            
            bool onPress{ false };
            bool wasPressed{ false };
            bool ignoreWasPressed{ false };
            bool characterScale{ false };
            
            sf::RectangleShape shape;
            sf::Text text;
            sf::String string;
            unsigned int characterSize{ 20 };
            float thickness{ 1.f };
            
            RectangleButton();
            void Draw(sf::RenderTarget* window) override;
            void Resize(unsigned int width, unsigned int height) override;
            bool PollEvent(sf::Event& event) override;
            void setAlpha(const sf::Int8& alpha) override;
            void setPosition(float x, float y) override;
            void setSize(const sf::Vector2f& vec);
            void setFont(const std::wstring& fontname);
            void setString(const std::wstring& string);
            void setCharacterSize(const unsigned int size);
        };
        
        struct RectangleButtons : Button
        {
            bool fontLoaded{ false };
            
            bool anyButtonPressed{ false };
            unsigned long index{ 0 }, pressedIndex{ 0 };
            bool onPress{ false };
            bool ignoreWasPressed{ false };
            bool characterScale{ false };
            
            sf::RectangleShape shape;
            sf::Color shapeFillColor{ sf::Color(0,0,0, 170) };
            
            sf::Text text;
            sf::String string;
            unsigned int characterSize{ 20 };
            float thickness{ 1.f };
            
            RectangleButtons();
            void Draw(sf::RenderTarget* window) override;
            void Resize(unsigned int width, unsigned int height) override;
            bool PollEvent(sf::Event& event) override;
            void eventPolled(sf::Event& event);
            void setAlpha(const sf::Int8& alpha) override;
            void setPosition(float x, float y) override;
            void setSize(const sf::Vector2f& vec);
            void setFont(const std::wstring& fontname);
            void setString(const std::wstring& string);
            void setCharacterSize(const unsigned int size);
        };
    }
}

#endif /* GUIInterface_hpp */
