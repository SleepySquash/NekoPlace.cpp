//
//  Popup.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 03/03/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Popup.hpp"

namespace NekoNinja
{
    PopupInfo::PopupInfo(const std::wstring& desc, const std::wstring& imagePath) : desc(desc), imagePath(imagePath) { }
    Popup::Popup(const std::wstring& title, const std::wstring& desc) : title(title), desc(desc) { }
    void Popup::Init()
    {
        gs::PushInterface(this);
        
        button.setCharacterSize(100);
        button.setString(L"OK");
        button.setFont(L"Pacifica.ttf");
        button.valign = Valign::Bottom;
        
        caption.setFont(*fc::GetFont(L"Pacifica.ttf"));
        caption.setOutlineColor(sf::Color::Black);
        caption.setString(title);
        info.setFont(*fc::GetFont(L"Pacifica.ttf"));
        info.setOutlineColor(sf::Color::Black);
        
        shape.setFillColor(sf::Color(0,0,0,160));
        shape.setOutlineColor(sf::Color::White);
        backfade.setFillColor(sf::Color(0,0,0,120));
        line.setFillColor(sf::Color(255,255,255,120));
    }
    void Popup::Destroy() { gs::RemoveInterface(this); }
    void Popup::PollEvent(sf::Event& event) { if (gs::isActiveInterface(this) && button.PollEvent(event)) { entity->PopComponent(this); } }
    void Popup::Resize(unsigned int width, unsigned int height)
    {
        backfade.setSize({(float)width, (float)height});
        shape.setSize({width * 4.f/5, height * 4.f/5});
        shape.setPosition((width - shape.getSize().x)/2, (height - shape.getSize().y)/2);
        shape.setOutlineThickness(gs::scale);
        line.setSize({shape.getSize().x, 4*gs::scale});
        
        button.Resize(width, height);
        button.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y + shape.getSize().y - 35*gs::scale);
        
        caption.setPosition(shape.getPosition().x + shape.getSize().x/2, shape.getPosition().y);
        caption.setCharacterSize(80*gs::scale);
        caption.setOutlineThickness(2*gs::scale);
        caption.setScale(1, 1); caption.setOrigin(caption.getGlobalBounds().width/2, caption.getGlobalBounds().height/10);
        if (shape.getSize().x < caption.getGlobalBounds().width)
            caption.setScale((float)shape.getSize().x/(caption.getGlobalBounds().width), 1);
        line.setPosition(shape.getPosition().x, caption.getPosition().y + caption.getGlobalBounds().height + 20*gs::scale);
        
        info.setCharacterSize(60*gs::scale);
        info.setOutlineThickness(2*gs::scale);
        info.setString(nss::GetStringWithLineBreaks(info, desc, shape.getSize().x - 40*gs::scale));
        
        info.setScale(1, 1); if (shape.getSize().x < info.getGlobalBounds().width)
            info.setScale((float)shape.getSize().x/(info.getGlobalBounds().width), 1);
        info.setPosition(shape.getPosition().x + 20*gs::scale, line.getPosition().y + 20*gs::scale);
    }
    void Popup::Draw(sf::RenderWindow* window)
    {
        window->draw(backfade);
        window->draw(shape);
        window->draw(line);
        window->draw(caption);
        window->draw(info);
        button.Draw(window);
    }
}
