//
//  GUIInterface.cpp
//  AlternativeTransport-MLaToA2018
//
//  Created by Никита Исаенко on 22/11/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "GUIInterface.hpp"

namespace ns
{
    namespace GUI
    {
        void Button::Draw(sf::RenderTarget* window) { }
        void Button::Resize(unsigned int width, unsigned int height) { }
        bool Button::PollEvent(sf::Event& event) { return false; }
        void Button::setAlpha(const sf::Int8& alpha) { }
        void Button::setPosition(float x, float y) { }
        void Button::setVisible(bool vis) { visible = vis; active = vis; }
        
        
        
        TextButton::TextButton()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
        }
        void TextButton::Draw(sf::RenderTarget* window) { if (fontLoaded && visible) window->draw(text); }
        void TextButton::Resize(unsigned int width, unsigned int height)
        {
            text.setCharacterSize(characterSize * gs::scale);
            text.setOutlineThickness(thickness * gs::scale);
        }
        bool TextButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    text.setFillColor(sf::Color::White);
                    bool constains = text.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = text.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed) text.setFillColor(sf::Color::Yellow);
            }
            return false;
        }
        void TextButton::setAlpha(const sf::Int8& alpha)
        {
            if (fontLoaded)
            {
                unsigned char realAlpha = sf::Int8((unsigned char)alpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, realAlpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, realAlpha));
            }
        }
        void TextButton::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); break;
                case Halign::Center: text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y); break;
                case Halign::Right: text.setOrigin(text.getLocalBounds().width, text.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); break;
                case Valign::Center: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom: text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
            text.setPosition(x, y);
        }
        void TextButton::setFont(const std::wstring& fontname)
        {
            if ((fontLoaded = (fc::GetFont(fontname) != nullptr)))
                text.setFont(*fc::GetFont(fontname));
        }
        void TextButton::setString(const std::wstring& string) { text.setString(string); }
        void TextButton::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
        
        
        
        SpriteButton::~SpriteButton() { if (textureName != L"") ic::DeleteImage(textureName); }
        void SpriteButton::Draw(sf::RenderTarget* window) { if (spriteLoaded && visible) window->draw(sprite); }
        void SpriteButton::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale) sprite.setScale(scale * gs::scScale, scale * gs::scScale);
            else sprite.setScale(scale * gs::scale, scale * gs::scale);
        }
        bool SpriteButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    sprite.setColor(sf::Color::White);
                    bool constains = sprite.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed) sprite.setColor(sf::Color::Yellow);
            }
            return false;
        }
        void SpriteButton::setAlpha(const sf::Int8& alpha)
        {
            if (spriteLoaded)
            {
                unsigned char realAlpha = sf::Int8((unsigned char)alpha * ((float)maxAlpha/255));
                sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, realAlpha));
            }
        }
        void SpriteButton::setPosition(float x, float y)
        {
            switch (halign)
            {
                case Halign::Left: sprite.setOrigin(0, sprite.getOrigin().y); break;
                case Halign::Center: sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getOrigin().y); break;
                case Halign::Right: sprite.setOrigin(sprite.getLocalBounds().width, sprite.getOrigin().y); break;
            }
            switch (valign)
            {
                case Valign::Top: sprite.setOrigin(sprite.getOrigin().x, 0); break;
                case Valign::Center: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height/2); break;
                case Valign::Bottom: sprite.setOrigin(sprite.getOrigin().x, sprite.getLocalBounds().height); break;
            }
            sprite.setPosition(x, y);
        }
        void SpriteButton::setTexture(const std::wstring& texture) { textureName = texture; setTexture(ic::LoadTexture(texture)); }
        void SpriteButton::setTexture(sf::Texture* texture) { if ((spriteLoaded = (texture != nullptr))) sprite.setTexture(*texture, true); }
        void SpriteButton::setScale(const float& scl) { scale = scl; Resize(gs::width, gs::height); }
    }
}
