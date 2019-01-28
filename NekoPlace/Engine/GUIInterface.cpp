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
        if (fontLoaded)
            window->draw(text);
    }
    void GUIButton::Resize(unsigned int width, unsigned int height)
    {
        text.setCharacterSize(characterSize * gs::scale);
        text.setOutlineThickness(thickness * gs::scale);
    }
    void GUIButton::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            isPressed = (event.mouseButton.x > text.getPosition().x && event.mouseButton.x < text.getPosition().x + text.getLocalBounds().width
                         && event.mouseButton.y > text.getPosition().y && event.mouseButton.y < text.getPosition().y + text.getLocalBounds().height);
        }
    }
    bool GUIButton::IsPressed()
    {
        if (isPressed)
        {
            isPressed = false;
            //pressedAfterColor = 0.1f;
            //text.setFillColor(sf::Color::Red);
            return true;
        }
        return false;
    }
    void GUIButton::SetPosition(float x, float y)
    {
        switch (halign)
        {
            case halignEnum::left:
                text.setPosition(x, text.getPosition().y);
                break;
            case halignEnum::center:
                text.setPosition(x - text.getLocalBounds().width/2, text.getPosition().y);
                break;
            case halignEnum::right:
                text.setPosition(x - text.getLocalBounds().width, text.getPosition().y);
                break;
        }
        switch (valign)
        {
            case valignEnum::top:
                text.setPosition(text.getPosition().x, y);
                break;
            case valignEnum::center:
                text.setPosition(text.getPosition().x, y - text.getLocalBounds().height/2);
                break;
            case valignEnum::bottom:
                text.setPosition(text.getPosition().x, y - text.getLocalBounds().height);
                break;
        }
    }
    void GUIButton::SetFont(const std::wstring& fontname)
    {
        if ((fontLoaded = (fc::GetFont(fontname) != nullptr)))
            text.setFont(*fc::GetFont(fontname));
    }
    void GUIButton::SetString(const std::wstring& string)
    {
        text.setString(string);
    }
    void GUIButton::SetCharacterSize(const unsigned int size)
    {
        characterSize = size;
        text.setCharacterSize(size * gs::scale);
    }
}
