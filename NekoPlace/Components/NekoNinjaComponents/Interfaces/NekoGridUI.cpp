//
//  NekoGridUI.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 16/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoGridUI.hpp"

namespace NekoNinja
{
    void NekoGridUI::Init()
    {
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/room_buttons.png");
        quitButton.sprite.setTextureRect({ 477,0,64,64 });
        quitButton.setScale(2);
        
        nlstNekoButton.valign = Valign::Bottom;
        nlstNekoShape.setFillColor(sf::Color(0,0,0,100));
        nlstNekoShape.setOutlineColor(sf::Color::White);
    }
    void NekoGridUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event)) { nlstNekoButton.eventPolled(event); entity->SendMessage({"NekoGridUI :: Close"}); }
        else
        {
            float xx = 100*gs::scale, yy = 240*gs::scale; int i{ 0 };
            if (nekoSelection && selectPassive)
            {
                nlstNekoButton.index = i;
                nlstNekoButton.setTexture(crossTexture);
                if (crossTexture) nlstNekoButton.setScale(168.f / crossTexture->getSize().x);
                nlstNekoButton.setPosition(xx, yy);
                if (nlstNekoButton.PollEvent(event)) entity->SendMessage({"NekoUI :: Choosed", nullptr});
                ++i; xx += 178*gs::scale;
            }
            for (auto& n : rl::nekolist)
                if (!nekoSelection || (selectPassive && n->info->ability && n->info->ability->passive) || (selectActive && (!n->info->ability || !n->info->ability->passive)) || (!selectPassive && !selectActive))
                {
                    if (ic::FindTexture(L"Data/Neko/Avatar/" + n->info->name + L".png"))
                    {
                        nlstNekoButton.index = i;
                        nlstNekoButton.setTexture(ic::FindTexture(L"Data/Neko/Avatar/" + n->info->name + L".png"));
                        nlstNekoButton.setPosition(xx, yy);
                        nlstNekoButton.setScale(168.f / nlstNekoButton.sprite.getTexture()->getSize().x);
                        if (nlstNekoButton.PollEvent(event)) { entity->SendMessage({"NekoUI :: SelectNeko", n});
                            if (nekoSelection) {
                                if (n->highlightedDuringSelection) entity->SendMessage({"NekoUI :: Look"});
                                else entity->SendMessage({"NekoUI :: Choose"});
                            } else entity->SendMessage({"NekoUI :: Show"});
                            break; }
                        ++i;
                    }
                    xx += 178*gs::scale;
                    if (xx > gs::width - 100*gs::scale) { xx = 100*gs::scale; yy += 178*gs::scale; }
                }
            nlstNekoButton.eventPolled(event);
        }
    }
    void NekoGridUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        
        nlstNekoButton.Resize(width, height);
        nlstNekoShape.setSize({168*gs::scale, 168*gs::scale});
        nlstNekoShape.setOrigin(nlstNekoShape.getSize().x/2, nlstNekoShape.getSize().y);
        nlstNekoShape.setOutlineColor(sf::Color::White);
        nlstNekoShape.setOutlineThickness(2*gs::scale);
    }
    void NekoGridUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        window->draw(blackScreenShape);
        
        float xx = 100*gs::scale, yy = 240*gs::scale; int i{ 0 };
        if (nekoSelection && selectPassive)
        {
            nlstNekoShape.setPosition(xx, yy);
            nlstNekoShape.setFillColor(sf::Color(100, 100, 100, 150));
            window->draw(nlstNekoShape);
            
            nlstNekoButton.index = i;
            nlstNekoButton.setTexture(crossTexture);
            if (crossTexture) nlstNekoButton.setScale(168.f / crossTexture->getSize().x);
            nlstNekoButton.setPosition(xx, yy);
            nlstNekoButton.Draw(window); ++i;
            xx += 178*gs::scale;
        }
        for (auto& n : rl::nekolist)
            if (!nekoSelection || (selectPassive && n->info->ability && n->info->ability->passive) || (selectActive && (!n->info->ability || !n->info->ability->passive)) || (!selectPassive && !selectActive))
            {
                if (ic::FindTexture(L"Data/Neko/Avatar/" + n->info->name + L".png"))
                {
                    nlstNekoShape.setPosition(xx, yy);
                    nlstNekoShape.setFillColor(sf::Color(n->info->color.r, n->info->color.g, n->info->color.b, 150));
                    if (n->highlightedDuringSelection) {
                        nlstNekoShape.setOutlineColor(sf::Color::Yellow); nlstNekoShape.setOutlineThickness(5*gs::scale);
                        window->draw(nlstNekoShape);
                        nlstNekoShape.setOutlineColor(sf::Color::White); nlstNekoShape.setOutlineThickness(2*gs::scale);
                    } else window->draw(nlstNekoShape);
                    
                    nlstNekoButton.index = i;
                    sf::Texture* texture = ic::FindTexture(L"Data/Neko/Avatar/" + n->info->name + L".png");
                    nlstNekoButton.setTexture(texture);
                    if (texture) nlstNekoButton.setScale(168.f / texture->getSize().x);
                    nlstNekoButton.setPosition(xx, yy);
                    nlstNekoButton.Draw(window); ++i;
                }
                xx += 178*gs::scale;
                if (xx > gs::width - 100*gs::scale) { xx = 100*gs::scale; yy += 178*gs::scale; }
            }
        quitButton.Draw(window);
    }
    void NekoGridUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "NekoGridUI :: Show") { nekoSelection = false; Switch(true); }
        else if (active && message.info == "NekoGridUI :: Close") { nekoSelection = false; Switch(false); }
        else if (message.info == "NekoGridUI :: Choose") { selectActive = selectPassive = false; nekoSelection = true; Switch(true); }
        else if (message.info == "NekoGridUI :: ChooseActive") { nekoSelection = selectActive = true; selectPassive = false; Switch(true); }
        else if (message.info == "NekoGridUI :: ChoosePassive") { nekoSelection = selectPassive = true; selectActive = false; Switch(true); }
        else if (message.info == "NekoGridUI :: Switch") Switch(!active);
    }
    void NekoGridUI::Switch(const bool& on)
    {
        if (on && !active)
        {
            if (nekoSelection && selectPassive) crossTexture = ic::LoadTexture(L"Data/Images/cross.png");
            gs::PushInterface(this); active = true; entity->SortAbove(this); Resize(gs::width, gs::height);
            for (auto& n : rl::neko) ic::PreloadTexture(L"Data/Neko/Avatar/" + n->info->name + L".png", 2, true, true);
        }
        else if (active)
        {
            nlstNekoButton.anyButtonPressed = false;
            if (nekoSelection && selectPassive) { ic::DeleteImage(L"Data/Images/cross.png"); crossTexture = nullptr; }
            gs::RemoveInterface(this); active = false; Resize(gs::width, gs::height);
            for (auto& n : rl::neko) ic::DeleteImage(L"Data/Neko/Avatar/" + n->info->name + L".png");
        }
    }
}
