//
//  NekoHelpers.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoHelpers.hpp"

namespace NekoNinja
{
    HeartsShape::HeartsShape(float x, float y, float scale)
    {
        sprite.setPosition(x*gs::scalex, y*gs::scaley);
        sprite.setScale(scale, scale);
    }
    void HeartsShape::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/heartsshape.png");
        if (texture) sprite.setTexture(*texture);
        sprite.setOrigin(190, 0);
        sprite.setScale(0.3*gs::scale, 0.3*gs::scale);
    }
    void HeartsShape::Update(const sf::Time& elapsedTime)
    {
        if (elapsed < 0.4f) elapsed += elapsedTime.asSeconds();
        else
        {
            alpha -= 255 * elapsedTime.asSeconds();
            if (alpha <= 0) novelSystem->PopComponent(this);
            else sprite.setColor(sf::Color(255, 255, 255, alpha));
        }
    }
    void HeartsShape::Draw(sf::RenderWindow* window)
    {
        window->draw(sprite);
    }
    
    
    
    ComboText::ComboText(int count, float x, float y)
    {
        text.setString(L"Комбо +" + std::to_wstring(count));
        text.setPosition(x*gs::scalex, y*gs::scaley);
    }
    void ComboText::Init()
    {
        text.setCharacterSize(90 * gs::scale);
        text.setFillColor(sf::Color::Yellow);
        text.setOutlineColor(sf::Color(40, 40, 40, 255));
        text.setOutlineThickness(2.f * gs::scale);
        text.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (fc::GetFont(L"Pacifica.ttf") != nullptr);
        
        text.setPosition(text.getPosition().x - text.getLocalBounds().width/2, text.getPosition().y);
        if (text.getGlobalBounds().left < 0)
            text.setPosition(text.getGlobalBounds().left + gs::width/6, text.getPosition().y);
        else if (text.getGlobalBounds().left + text.getGlobalBounds().width > gs::width)
            text.setPosition(gs::width - text.getGlobalBounds().width - gs::width/8, text.getPosition().y);
    }
    void ComboText::Update(const sf::Time& elapsedTime)
    {
        text.setPosition(text.getPosition().x, text.getPosition().y - 50*elapsedTime.asSeconds());
        if (elapsed < 0.4f) elapsed += elapsedTime.asSeconds();
        else
        {
            alpha -= 255 * elapsedTime.asSeconds();
            if (alpha <= 0) novelSystem->PopComponent(this);
            else {
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
            }
        }
    }
    void ComboText::Draw(sf::RenderWindow* window)
    {
        if (fontLoaded) window->draw(text);
    }
    
    
    CriticalHitText::CriticalHitText(float x, float y)
    {
        text.setString(L"В кокоро!");
        text.setPosition(x*gs::scalex, y*gs::scaley);
    }
    void CriticalHitText::Init()
    {
        text.setCharacterSize(90 * gs::scale);
        text.setFillColor(sf::Color::Red);
        text.setOutlineColor(sf::Color(40, 40, 40, 255));
        text.setOutlineThickness(2.f * gs::scale);
        text.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (fc::GetFont(L"Pacifica.ttf") != nullptr);
        
        text.setPosition(text.getPosition().x - text.getLocalBounds().width/2, text.getPosition().y);
        if (text.getGlobalBounds().left < 0)
            text.setPosition(text.getGlobalBounds().left + gs::width/6, text.getPosition().y);
        else if (text.getGlobalBounds().left + text.getGlobalBounds().width > gs::width)
            text.setPosition(gs::width - text.getGlobalBounds().width - gs::width/8, text.getPosition().y);
    }
    void CriticalHitText::Update(const sf::Time& elapsedTime)
    {
        text.setPosition(text.getPosition().x, text.getPosition().y - 50*elapsedTime.asSeconds());
        if (elapsed < 0.4f) elapsed += elapsedTime.asSeconds();
        else
        {
            alpha -= 255 * elapsedTime.asSeconds();
            if (alpha <= 0) novelSystem->PopComponent(this);
            else {
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
            }
        }
    }
    void CriticalHitText::Draw(sf::RenderWindow* window)
    {
        if (fontLoaded) window->draw(text);
    }
}
