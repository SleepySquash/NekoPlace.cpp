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
    GUIButton::GUIButton()
    {
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
    }
    void GUIButton::Update(const sf::Time &elapsedTime)
    {
        /*if (pressedAfterColor)
         {
         pressedAfterColor -= elapsedTime.asSeconds();
         if (pressedAfterColor <= 0)
         {
         text.setFillColor(sf::Color::White);
         pressedAfterColor = 0.f;
         }
         }*/
    }
    void GUIButton::Draw(sf::RenderTarget* window)
    {
        if (fontLoaded) window->draw(text);
    }
    void GUIButton::Resize(unsigned int width, unsigned int height)
    {
        text.setCharacterSize(characterSize * gs::scale);
        text.setOutlineThickness(thickness * gs::scale);
    }
    bool GUIButton::PollEvent(sf::Event& event)
    {
        if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || event.type == sf::Event::TouchEnded)
        {
            if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
            else dot = { event.touch.x, event.touch.y };
            
            if (wasPressed) text.setFillColor(sf::Color::White);
            bool constains = text.getGlobalBounds().contains(dot.x, dot.y);
            if (constains) event = sf::Event(); // So that no button will be clicked being the underlaying.
            
            return constains;
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
    void GUIButton::setPosition(float x, float y)
    {
        switch (halign)
        {
            case halignEnum::left: text.setPosition(x, text.getPosition().y); break;
            case halignEnum::center: text.setPosition(x - text.getGlobalBounds().width/2, text.getPosition().y); break;
            case halignEnum::right: text.setPosition(x - text.getGlobalBounds().width, text.getPosition().y); break;
        }
        switch (valign)
        {
            case valignEnum::top: text.setPosition(text.getPosition().x, y); break;
            case valignEnum::center: text.setPosition(text.getPosition().x, y - text.getGlobalBounds().height/2); break;
            case valignEnum::bottom: text.setPosition(text.getPosition().x, y - text.getGlobalBounds().height); break;
        }
    }
    void GUIButton::setFont(const std::wstring& fontname)
    {
        if ((fontLoaded = (fc::GetFont(fontname) != nullptr)))
            text.setFont(*fc::GetFont(fontname));
    }
    void GUIButton::setString(const std::wstring& string)
    {
        text.setString(string);
    }
    void GUIButton::setCharacterSize(const unsigned int size)
    {
        characterSize = size;
        text.setCharacterSize(size * gs::scale);
    }
}
