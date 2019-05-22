//
//  EventAlert.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "EventAlert.hpp"

namespace NekoNinja
{
    EventAlert::EventAlert(MessageSender* sender, const std::wstring& alert, float waiting) : sender(sender), alert(alert), event(nullptr), waiting(waiting) { }
    EventAlert::EventAlert(MessageSender* sender, EventBase* event) : sender(sender), event(event), alert(event->description + L"!"), waiting(event->alertDuration) { }
    void EventAlert::Init()
    {
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color(40, 40, 40, 255));
        fontLoaded = fc::GetFont(L"Pacifica.ttf");
        if (fontLoaded) text.setFont(*fc::GetFont(L"Pacifica.ttf"));
        
        text.setPosition(text.getPosition().x - text.getLocalBounds().width/2, text.getPosition().y);
        if (text.getGlobalBounds().left < 0)
            text.setPosition(text.getGlobalBounds().left + gs::width/6, text.getPosition().y);
        else if (text.getGlobalBounds().left + text.getGlobalBounds().width > gs::width)
            text.setPosition(gs::width - text.getGlobalBounds().width - gs::width/8, text.getPosition().y);
    }
    void EventAlert::Update(const sf::Time& elapsedTime)
    {
        if (gs::isPause) return;
        if (elapsed < 0.6f) elapsed += elapsedTime.asSeconds();
        else
        {
            if (!sent) { sender->SendMessage({"EventStart"}); sent = true; }
            alpha -= 255 * elapsedTime.asSeconds();
            if (alpha <= 0) novelSystem->PopComponent(this);
            else {
                text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
                text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
            }
        }
    }
    void EventAlert::Resize(unsigned int width, unsigned int height)
    {
        text.setCharacterSize((event ? 90 : 360) * gs::scale);
        text.setOutlineThickness(2.f * gs::scale); text.setPosition(width/2, height/2);
        text.setString(nss::GetStringWithLineBreaks(text, alert, gs::width - gs::width/9));
        text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
    }
    void EventAlert::Draw(sf::RenderWindow* window) { if (fontLoaded) window->draw(text); }
}
