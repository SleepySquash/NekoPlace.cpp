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
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
            }
            else
            {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
            if (gs::width < text.getGlobalBounds().width)
                text.setScale((float)gs::width/(text.getGlobalBounds().width), 1);
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
                    
                    wasPressed = false;
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
            if (invertXAxes) sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
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
                    
                    wasPressed = false;
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
        
        
        
        
        
        SpriteButtons::~SpriteButtons() { if (textureName != L"") ic::DeleteImage(textureName); }
        void SpriteButtons::Draw(sf::RenderTarget* window)
        {
            if (spriteLoaded && visible)
            {
                if (anyButtonPressed && index == pressedIndex) sprite.setColor(sf::Color::Yellow);
                window->draw(sprite);
                if (anyButtonPressed && index == pressedIndex) sprite.setColor(sf::Color::White);
            }
        }
        void SpriteButtons::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale) sprite.setScale(scale * gs::scScale, scale * gs::scScale);
            else sprite.setScale(scale * gs::scale, scale * gs::scale);
        }
        bool SpriteButtons::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (pressedIndex == index && (anyButtonPressed || ignoreWasPressed))
                {
                    sprite.setColor(sf::Color::White);
                    bool constains = sprite.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) { pressedIndex = -1; anyButtonPressed = false; event = sf::Event(); }
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool wasPressed = sprite.getGlobalBounds().contains(dot.x, dot.y);
                if (wasPressed) { pressedIndex = index; anyButtonPressed = true; }
            }
            return false;
        }
        void SpriteButtons::eventPolled(sf::Event& event) { if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) anyButtonPressed = false; }
        void SpriteButtons::setAlpha(const sf::Int8& alpha)
        {
            if (spriteLoaded)
            {
                unsigned char realAlpha = sf::Int8((unsigned char)alpha * ((float)maxAlpha/255));
                sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, realAlpha));
            }
        }
        void SpriteButtons::setPosition(float x, float y)
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
        void SpriteButtons::setTexture(const std::wstring& texture) { textureName = texture; setTexture(ic::LoadTexture(texture)); }
        void SpriteButtons::setTexture(sf::Texture* texture) { if ((spriteLoaded = (texture != nullptr))) sprite.setTexture(*texture, true); }
        void SpriteButtons::setScale(const float& scl) { scale = scl; Resize(gs::width, gs::height); }
        
        
        
        
        
        
        
        RectangleButton::RectangleButton()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            shape.setFillColor(sf::Color::White);
            shape.setOutlineColor(sf::Color::Black);
        }
        void RectangleButton::Draw(sf::RenderTarget* window) { if (fontLoaded && visible) { window->draw(shape); window->draw(text); } }
        void RectangleButton::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
            } else {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
            if (shape.getSize().x < text.getGlobalBounds().width)
                text.setScale(shape.getSize().x/(text.getGlobalBounds().width), 1);
        }
        bool RectangleButton::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (wasPressed || ignoreWasPressed)
                {
                    text.setFillColor(sf::Color::White); shape.setFillColor(sf::Color::White);
                    bool constains = shape.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
                    
                    wasPressed = false;
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                wasPressed = shape.getGlobalBounds().contains(dot.x, dot.y);
                if (onPress) return wasPressed;
                if (wasPressed) { shape.setFillColor(sf::Color::Yellow); text.setFillColor(sf::Color::Yellow); }
            }
            return false;
        }
        void RectangleButton::setAlpha(const sf::Int8& alpha)
        {
            if (fontLoaded)
            {
                unsigned char realAlpha = sf::Int8((unsigned char)alpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, realAlpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, realAlpha));
                shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, realAlpha));
                shape.setOutlineColor(sf::Color(shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, realAlpha));
            }
        }
        void RectangleButton::setPosition(float x, float y)
        {
            shape.setPosition(x, y);
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); text.setPosition(x, text.getPosition().y); break;
                case Halign::Center:
                    text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x/2, text.getPosition().y); break;
                case Halign::Right:
                    text.setOrigin(text.getLocalBounds().width, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x, text.getPosition().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); text.setPosition(text.getPosition().x, y); break;
                case Valign::Center:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y/2);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
        }
        void RectangleButton::setSize(const sf::Vector2f& vec) { shape.setSize(vec); }
        void RectangleButton::setFont(const std::wstring& fontname)
        {
            if ((fontLoaded = (fc::GetFont(fontname) != nullptr)))
                text.setFont(*fc::GetFont(fontname));
        }
        void RectangleButton::setString(const std::wstring& string) { text.setString(string); }
        void RectangleButton::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
        
        
        
        
        
        
        RectangleButtons::RectangleButtons()
        {
            text.setFillColor(sf::Color::White);
            text.setOutlineColor(sf::Color::Black);
            shape.setFillColor(shapeFillColor);
            shape.setOutlineColor(sf::Color::White);
        }
        void RectangleButtons::Draw(sf::RenderTarget* window)
        {
            if (fontLoaded && visible)
            {
                if (shape.getSize().x < text.getGlobalBounds().width) { text.setScale(shape.getSize().x/(text.getGlobalBounds().width), 1); }
                if (anyButtonPressed && index == pressedIndex) {
                    shape.setFillColor(sf::Color::Yellow); text.setFillColor(sf::Color::Yellow); }
                window->draw(shape); window->draw(text);
                if (anyButtonPressed && index == pressedIndex) {
                    shape.setFillColor(shapeFillColor); text.setFillColor(sf::Color::White); }
                text.setScale(1, 1);
            }
        }
        void RectangleButtons::Resize(unsigned int width, unsigned int height)
        {
            if (characterScale)
            {
                text.setCharacterSize(characterSize * gs::scScale);
                text.setOutlineThickness(thickness * gs::scScale);
                shape.setOutlineThickness(thickness * gs::scScale);
            } else {
                text.setCharacterSize(characterSize * gs::scale);
                text.setOutlineThickness(thickness * gs::scale);
                shape.setOutlineThickness(thickness * gs::scale);
            }
            text.setScale(1, 1);
        }
        bool RectangleButtons::PollEvent(sf::Event& event)
        {
            if (!active) return false;
            if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (pressedIndex == index && (anyButtonPressed || ignoreWasPressed))
                {
                    text.setFillColor(sf::Color::White); shape.setFillColor(shapeFillColor);
                    bool constains = shape.getGlobalBounds().contains(dot.x, dot.y);
                    if (constains) { pressedIndex = -1; anyButtonPressed = false; event = sf::Event(); }
                    
                    return constains;
                }
            }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchBegan)
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool wasPressed = shape.getGlobalBounds().contains(dot.x, dot.y);
                if (wasPressed) { pressedIndex = index; anyButtonPressed = true; }
            }
            return false;
        }
        void RectangleButtons::eventPolled(sf::Event& event) { if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded) anyButtonPressed = false; }
        void RectangleButtons::setAlpha(const sf::Int8& alpha)
        {
            if (fontLoaded)
            {
                unsigned char realAlpha = sf::Int8((unsigned char)alpha * ((float)maxAlpha/255));
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, realAlpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, realAlpha));
                shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, realAlpha));
                shape.setOutlineColor(sf::Color(shape.getOutlineColor().r, shape.getOutlineColor().g, shape.getOutlineColor().b, realAlpha));
            }
        }
        void RectangleButtons::setPosition(float x, float y)
        {
            shape.setPosition(x, y);
            switch (halign)
            {
                case Halign::Left: text.setOrigin(0, text.getOrigin().y); text.setPosition(x, text.getPosition().y); break;
                case Halign::Center:
                    text.setOrigin(text.getLocalBounds().width/2, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x/2, text.getPosition().y); break;
                case Halign::Right:
                    text.setOrigin(text.getLocalBounds().width, text.getOrigin().y);
                    text.setPosition(x + shape.getSize().x, text.getPosition().y); break;
            }
            switch (valign)
            {
                case Valign::Top: text.setOrigin(text.getOrigin().x, 0); text.setPosition(text.getPosition().x, y); break;
                case Valign::Center:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y/2);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height/2); break;
                case Valign::Bottom:
                    text.setPosition(text.getPosition().x, y + shape.getSize().y);
                    text.setOrigin(text.getOrigin().x, text.getLocalBounds().height); break;
            }
        }
        void RectangleButtons::setSize(const sf::Vector2f& vec) { shape.setSize(vec); }
        void RectangleButtons::setFont(const std::wstring& fontname)
        {
            if ((fontLoaded = (fc::GetFont(fontname) != nullptr)))
                text.setFont(*fc::GetFont(fontname));
        }
        void RectangleButtons::setString(const std::wstring& string) { text.setString(string); }
        void RectangleButtons::setCharacterSize(const unsigned int size)
        {
            characterSize = size;
            text.setCharacterSize(size * gs::scale);
        }
    }
}
