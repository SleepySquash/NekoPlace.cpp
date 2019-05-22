//
//  NekoInterfaceUI.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 16/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoInterfaceUI.hpp"

namespace NekoNinja
{
    void NekoInterfaceUI::Init()
    {
        nintText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nintText.setOutlineColor(sf::Color::Black);
        nintNameText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nintNameText.setOutlineColor(sf::Color::Black);
        nintAbilityText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        nintAbilityText.setOutlineColor(sf::Color::Black);
        nintShape.setFillColor(sf::Color(0,0,0,80));
        nintAbilityShape.setFillColor(sf::Color(0,0,0,80));
        nintCloseButton.setString(L"Закрыть");
        nintCloseButton.setFont(L"Pacifica.ttf");
        nintCloseButton.setCharacterSize(70);
        nintCloseButton.valign = Valign::Bottom;
        
        nintSelectMainButton.setString(L"В отряд");
        nintSelectMainButton.setFont(L"Pacifica.ttf");
        nintSelectMainButton.setCharacterSize(70);
        
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/room_buttons.png");
        quitButton.sprite.setTextureRect({ 477,0,64,64 });
        quitButton.setScale(2);
        
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/heart.png");
        if (texture)
        {
            nintHeart.setTexture(*texture);
            nintHeart.setTextureRect(sf::IntRect(268, 0, 268, 256));
            nintHeart.setOrigin(texture->getSize().x, 0);
        }
        
        nintTalkButton.setFont(L"Pacifica.ttf");
        nintTalkButton.setCharacterSize(69);
        nintTalkButton.setString(L"Поговорить");
        nintWardrobeButton.setFont(L"Pacifica.ttf");
        nintWardrobeButton.setCharacterSize(54);
        nintWardrobeButton.setString(L"Гардероб");
        nintWardrobeButton.halign = Halign::Left;
        nintGiftButton.setFont(L"Pacifica.ttf");
        nintGiftButton.setCharacterSize(54);
        nintGiftButton.setString(L"Подарок");
        nintGiftButton.halign = Halign::Right;
        nintActionButton.setFont(L"Pacifica.ttf");
        nintActionButton.setString(L"Действие");
        nintActionButton.setCharacterSize(54);
        nintChoose.setFont(L"Pacifica.ttf");
        nintChoose.setString(L"Выбрать");
        nintChoose.setCharacterSize(78);
        nintActionButton.valign = nintTalkButton.valign = nintGiftButton.valign = nintWardrobeButton.valign = nintChoose.valign = Valign::Bottom;
    }
    void NekoInterfaceUI::PollEvent(sf::Event& event)
    {
        if (!active || !gs::isActiveInterface(this)) return;
        
        if (quitButton.PollEvent(event)) {
            if (nintAbility) ic::DeleteImage(L"Data/Neko/Ability/Icon/" + nintAbility->name + L".jpg");
            ic::DeleteImage(L"Data/Neko/Person/" + nintNeko->info->name + L".png");
            entity->SendMessage({"NekoUI :: Close"}); }
        else if (looking == lookingMode::select) { if (nintChoose.PollEvent(event)) entity->SendMessage({"NekoUI :: Choosed", nintNeko}); }
        else if (looking == lookingMode::look) { if (nintChoose.PollEvent(event)) entity->SendMessage({"NekoUI :: Close"}); }
        else if (nintTalkButton.PollEvent(event))
        {
            nintDontDrawPersonNeko = true;
            nintNeko->info->NovelTalkTo(entity);
        }
        else if (nintActionButton.PollEvent(event)) { }
        else if (nintGiftButton.PollEvent(event)) entity->SendMessage({"Inventory :: Show"});
        else if (nintWardrobeButton.PollEvent(event)) { }
        /*else if (nintSelectMainButton.PollEvent(event) && Player::self->neko != nintNeko)
        {
            ic::DeleteImage(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
            Player::self->neko = (nintNeko);
            if (Player::self->neko && menu)
            {
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
                if (texture) {
                    menu->neko.setTexture(*texture, true);
                    menu->neko.setOrigin(texture->getSize().x/2 + Player::self->neko->info->personSprite_offsetX, texture->getSize().y + Player::self->neko->info->personSprite_offsetY);
                    menu->nekoRelScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * Player::self->neko->info->novelScale;
                }
                menu->nekoSpriteLoaded = texture;
                menu->Resize(gs::width, gs::height);
            }
            Player::self->SaveRoom();
        }*/
    }
    void NekoInterfaceUI::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        
        nintCloseButton.Resize(width, height);
        if (gs::verticalOrientation)
        {
            nintBodyNeko.setScale(1.4 * nintBodyNekoScale * gs::scScale, 1.4 * nintBodyNekoScale * gs::scScale);
            nintShape.setSize({gs::width*4.55f/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition((gs::width - nintShape.getSize().x)/2, (gs::height - nintShape.getSize().y)/2);
            
            if (nintBodyNeko.getGlobalBounds().height + 20*gs::scale > gs::height)
                nintBodyNeko.setPosition(width/2,nintBodyNeko.getGlobalBounds().height + 20*gs::scale);
            else nintBodyNeko.setPosition(width/2, height);
        }
        else
        {
            nintBodyNeko.setScale(1.3 * nintBodyNekoScale * gs::scScale, 1.3 * nintBodyNekoScale * gs::scScale);
            nintShape.setSize({3*gs::width/5.f, gs::height*4.3f/5.f});
            nintShape.setPosition(gs::width - 3*gs::width/5.f, (gs::height - nintShape.getSize().y)/2);
            
            if (nintBodyNeko.getGlobalBounds().height + 20*gs::scale > gs::height)
                nintBodyNeko.setPosition(width/5, nintBodyNeko.getGlobalBounds().height + 20*gs::scale);
            else nintBodyNeko.setPosition(width/5, height);
        }
        
        nintCloseButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + nintShape.getGlobalBounds().height - 20*gs::scaley);
        nintNameText.setCharacterSize(80 * gs::scale);
        nintNameText.setOrigin(nintNameText.getGlobalBounds().width/2, 0);
        nintNameText.setOutlineThickness(gs::scale);
        nintText.setOutlineThickness(gs::scale);
        
        nintHeart.setScale(0.25*gs::scale, 0.25*gs::scale);
        nintNameText.setScale(1, 1);
        if (nintNameText.getLocalBounds().width + nintHeart.getGlobalBounds().width + 45*gs::scale > nintShape.getSize().x)
            nintNameText.setScale(nintShape.getSize().x/(nintNameText.getLocalBounds().width + nintHeart.getGlobalBounds().width + 45*gs::scale), nintNameText.getScale().y);
        nintNameText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + gs::scaley);
        
        nintHeart.setPosition(nintNameText.getGlobalBounds().left + 45*gs::scale, nintShape.getPosition().y + 25*gs::scale);
        if (nintNameText.getPosition().x < nintShape.getPosition().x) //TODO: add +texture.size().x
            nintHeart.setPosition(nintShape.getPosition().x + 10*gs::scale, nintShape.getPosition().y + 25*gs::scale);
        nintYYInfoStart = nintNameText.getGlobalBounds().top + nintNameText.getGlobalBounds().height;
        
        nintSelectMainButton.Resize(width, height);
        nintSelectMainButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getGlobalBounds().top + nintShape.getGlobalBounds().height/2);
        
        if (nintAbility)
        {
            nintAbilityShape.setOutlineThickness(4 * gs::scale);
            nintAbilityShape.setSize({ nintShape.getSize().x*9.5f/10.f, 120*gs::scale });
            if (gs::verticalOrientation) nintAbilityShape.setSize({ nintAbilityShape.getSize().x, 140*gs::scale });
            nintAbilityShape.setPosition(nintShape.getPosition().x + (nintShape.getSize().x - nintShape.getSize().x*9.5f/10.f)/2, 0);
            if (nintAbilitySpriteLoaded)
            {
                float nintSpriteScale = (float)(nintAbilityShape.getSize().y)/nintAbilitySprite.getLocalBounds().width;
                nintAbilitySprite.setScale(nintSpriteScale, nintSpriteScale);
                nintAbilitySprite.setPosition(nintAbilityShape.getPosition().x, 0);
            }
            
            nintAbilityText.setOutlineThickness(gs::scale);
            nintAbilityText.setCharacterSize(27 * gs::scale);
            nintAbilityDescription = nss::GetStringWithLineBreaks(nintAbilityText, nintAbility->description, nintAbilityShape.getSize().x - nintAbilitySprite.getGlobalBounds().width - 10*gs::scalex);
        }
        
        // TODO: if ((yy) > положение_куда_ставим_кнопки) { ставим кнопки в yy; }
        /*if (gs::verticalOrientation)
         nintWardrobeButton.characterSize = nintGiftButton.characterSize = nintActionButton.characterSize = 52;
         else nintWardrobeButton.characterSize = nintGiftButton.characterSize = nintActionButton.characterSize = 58;*/
        nintTalkButton.Resize(width, height);
        nintWardrobeButton.Resize(width, height);
        nintGiftButton.Resize(width, height);
        nintActionButton.Resize(width, height);
        
        if (nintShape.getSize().x - (10*gs::scale + nintWardrobeButton.text.getGlobalBounds().width + nintGiftButton.text.getGlobalBounds().width) < nintWardrobeButton.text.getGlobalBounds().width)
        {
            nintTalkButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*2*1.5);
            nintActionButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*1.4);
        }
        else
        {
            nintTalkButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - nintWardrobeButton.text.getGlobalBounds().height*1.7);
            nintActionButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        }
        nintWardrobeButton.setPosition(nintShape.getPosition().x, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        nintGiftButton.setPosition(nintShape.getPosition().x + nintShape.getSize().x, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        
        nintChoose.Resize(width, height);
        if (looking == lookingMode::select) nintChoose.setString(L"Выбрать");
        else if (looking == lookingMode::look) nintChoose.setString(L"Закрыть");
        nintChoose.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2, nintShape.getPosition().y + nintShape.getSize().y - 10*gs::scale);
        
        if (nintNeko)
        {
            nintText.setCharacterSize(44 * gs::scale);
            nintDescriptionText = nss::GetStringWithLineBreaks(nintText, L"Пообщайтесь с кошкодевочкой, чтобы узнать её лучше~"/*nintNeko->info->characterDescription*/, nintShape.getSize().x - 10*gs::scalex, 10*gs::scale);
        }
    }
    void NekoInterfaceUI::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        
        window->draw(blackScreenShape);
        if (!nintDontDrawPersonNeko) window->draw(nintBodyNeko);
        window->draw(nintShape);
        quitButton.Draw(window);
        
        window->draw(nintHeart);
        window->draw(nintNameText);
        
        float yy = nintYYInfoStart;
        nintText.setCharacterSize(30 * gs::scale);
        nintText.setString(L"(Здесь будет мой статус)");
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2 - nintText.getGlobalBounds().width/2, yy);
        window->draw(nintText); yy += nintText.getGlobalBounds().height + 10*gs::scale;
        
        nintText.setCharacterSize(50 * gs::scale);
        nintText.setString(nintMoodText);
        nintText.setPosition(nintShape.getPosition().x, yy);
        nintText.setFillColor(nintMoodColor); window->draw(nintText);
        nintText.setString(nintRarityText);
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x - nintText.getGlobalBounds().width, yy);
        nintText.setFillColor(nintRarityColor); window->draw(nintText);
        nintText.setFillColor(sf::Color::White);
        yy += nintText.getGlobalBounds().height + 25*gs::scale;
        
        nintText.setCharacterSize(44 * gs::scale);
        nintText.setString(nintDescriptionText);
        nintText.setPosition(nintShape.getPosition().x + nintShape.getSize().x/2 - nintText.getGlobalBounds().width/2, yy);
        window->draw(nintText); yy += nintText.getGlobalBounds().height + 30*gs::scale;
        
        if (nintAbility)
        {
            float localyy = yy;
            nintAbilityShape.setPosition(nintAbilityShape.getPosition().x, yy);
            window->draw(nintAbilityShape);
            nintAbilitySprite.setPosition(nintAbilityShape.getPosition().x, yy);
            if (nintAbilitySpriteLoaded) window->draw(nintAbilitySprite);
            
            nintAbilityText.setCharacterSize(39 * gs::scale);
            nintAbilityText.setString(nintAbility->display);
            nintAbilityText.setPosition(nintAbilityShape.getPosition().x + nintAbilitySprite.getGlobalBounds().width + (nintAbilityShape.getSize().x - nintAbilitySprite.getGlobalBounds().width)/2 - nintAbilityText.getGlobalBounds().width/2, yy - 5*gs::scale);
            window->draw(nintAbilityText); localyy += nintAbilityText.getGlobalBounds().height /*- 1*gs::scale*/;
            
            nintAbilityText.setCharacterSize(27 * gs::scale);
            nintAbilityText.setString(nintAbilityDescription);
            nintAbilityText.setPosition(nintAbilityShape.getPosition().x + nintAbilitySprite.getGlobalBounds().width, localyy);
            window->draw(nintAbilityText);
            
            yy += nintAbilityShape.getSize().y + 10*gs::scale;
        }
        
        switch (looking)
        {
            case lookingMode::room:
                nintTalkButton.Draw(window);
                nintWardrobeButton.Draw(window);
                nintGiftButton.Draw(window);
                nintActionButton.Draw(window);
                break;
            case lookingMode::select: nintChoose.Draw(window); break;
            case lookingMode::look: nintChoose.Draw(window); break;
        }
    }
    void NekoInterfaceUI::RecieveMessage(MessageHolder& message)
    {
        if (!active && message.info == "NekoUI :: Show") { looking = lookingMode::room; Switch(true); }
        else if (active && message.info == "NekoUI :: Close") { Switch(false); }
        else if (message.info == "NekoUI :: Choose") { looking = lookingMode::select; Switch(true); }
        else if (message.info == "NekoUI :: Look") { looking = lookingMode::look; Switch(true); }
        else if (message.info == "NekoUI :: SelectNeko") { OpenNekoInterface(reinterpret_cast<NekoEntity*>(message.address)); }
        else if (message.info == "NekoUI :: Switch") Switch(!active);
        else if (active && message.info == "NovelComponents :: Novel :: Destroying") nintDontDrawPersonNeko = false;
    }
    void NekoInterfaceUI::Switch(const bool& on)
    {
        if (on && !active) { gs::PushInterface(this); active = true; entity->SortAbove(this); Resize(gs::width, gs::height); }
        else if (active) { gs::RemoveInterface(this); active = false; Resize(gs::width, gs::height); if (nintNeko) nintNeko->beingActionedWith = false; }
    }
    void NekoInterfaceUI::OpenNekoInterface(NekoEntity* entity)
    {
        nintNeko = entity;
        entity->beingActionedWith = true; entity->drawDialogue = false;
        nintDontDrawPersonNeko = false;
        nintNameText.setString(entity->info->display);
        nintNameText.setOrigin(nintNameText.getGlobalBounds().width/2, 0);
        
        if ((nintAbility = nintNeko->info->ability))
        {
            nintAbilityShape.setOutlineColor(nintAbility->passive ? sf::Color(60, 60, 255, 255) : sf::Color(255, 60, 60, 255));
            sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Ability/Icon/" + nintAbility->name + L".jpg");
            if ((nintAbilitySpriteLoaded = texture)) { nintAbilitySprite.setTexture(*texture, true); }
        }
        
        nintMoodText = L"Счастье с:"; nintMoodColor = sf::Color::Green;
        float chance = nintNeko->info->chance;
        if (chance >= 0.09)          { nintRarityColor = sf::Color(200, 200, 200, 120); nintRarityText = L"Обычная"; }
        else if (chance >= 0.05)     { nintRarityColor = sf::Color(106, 143, 203, 255); nintRarityText = L"Необычная"; }
        else if (chance >= 0.008)    { nintRarityColor = sf::Color(87, 173, 98, 255); nintRarityText = L"Редкая"; }
        else if (chance >= 0.005)    { nintRarityColor = sf::Color(218, 113, 114, 255); nintRarityText = L"Оч. редкая"; }
        else if (chance >= 0.0012)   { nintRarityColor = sf::Color(188, 106, 167, 255); nintRarityText = L"Эпическая"; }
        else if (chance >= 0.0006)   { nintRarityColor = sf::Color(244, 200, 68, 255); nintRarityText = L"Легенда"; }
        else if (chance == 0    )    { nintRarityColor = sf::Color(244, 200, 68, 255); nintRarityText = L"Единственная"; }
        else                         { nintRarityColor = sf::Color(255, 180, 180, 255); nintRarityText = L"Сверхлегенда"; }
        /*if (chance >= 0.25)         { nintRarityColor = sf::Color(200, 200, 200, 120); nintRarityText = L"Обычная"; }
         else if (chance >= 0.08)    { nintRarityColor = sf::Color(106, 143, 203, 255); nintRarityText = L"Необычная"; }
         else if (chance >= 0.02)    { nintRarityColor = sf::Color(87, 173, 98, 255); nintRarityText = L"Редкая"; }
         else if (chance >= 0.008)   { nintRarityColor = sf::Color(218, 113, 114, 255); nintRarityText = L"Оч. редкая"; }
         else if (chance >= 0.004)   { nintRarityColor = sf::Color(188, 106, 167, 255); nintRarityText = L"Эпическая"; }
         else if (chance >= 0.001)   { nintRarityColor = sf::Color(244, 200, 68, 255); nintRarityText = L"Легенда"; }
         else if (chance == 0    )   { nintRarityColor = sf::Color(244, 200, 68, 255); nintRarityText = L"Единственная"; }
         else                        { nintRarityColor = sf::Color(255, 180, 180, 255); nintRarityText = L"Сверхлегенда"; }*/
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + nintNeko->info->name + L".png");
        if (texture)
        {
            nintBodyNeko.setTexture(*texture, true);
            nintBodyNeko.setOrigin(texture->getSize().x/2 + nintNeko->info->personSprite_offsetX, texture->getSize().y + nintNeko->info->personSprite_offsetY);
            nintBodyNekoScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * nintNeko->info->personScale;
        }
        Resize(gs::width, gs::height);
    }
}
