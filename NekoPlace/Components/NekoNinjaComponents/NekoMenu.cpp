//
//  NekoMenu.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoMenu.hpp"

namespace NekoNinja
{
    int Room::roomWidth{ 1280 }, Room::roomHeight{ 650 };
    float Room::x{ 0 }, Room::y{ 0 };
    float Room::scale{ 1.f };
    
    void NekoEntity::Init()
    {
        sprite.setTexture(*ic::LoadTexture(L"Data/Neko/" + info->name + L".png"));
        spriteLoaded = (sprite.getTexture() != nullptr);
        
        x = rand() % Room::roomWidth;
        y = rand() % Room::roomHeight;
        sprite.setPosition(x * Room::scale * gs::scale, y * Room::scale * gs::scale);
        
        newPoint = true;
        elapsedWaiting = (rand() % 1000)/1000.f;
        
        inited = true;
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/" + info->name + L".png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        if (elapsedWaiting > 0) elapsedWaiting -= elapsedTime.asSeconds();
        else
        {
            if (newPoint)
            {
                movingTo_x = rand() % Room::roomWidth;
                movingTo_y = rand() % Room::roomHeight;
                newPoint = false;
            }
            else
            {
                if (x < movingTo_x - 30) { moveRight = true; moveLeft = false; }
                else if (x > movingTo_x + 30) { moveLeft = true; moveRight = false; }
                else { moveLeft = false; moveRight = false; }
                
                if (y < movingTo_y - 30) { moveDown = true; moveUp = false; }
                else if (y > movingTo_y + 30) { moveUp = true; moveDown = false; }
                else { moveDown = false; moveUp = false; }
                
                if (!moveUp && !moveDown && !moveLeft && !moveRight)
                {
                    newPoint = true;
                    elapsedWaiting = waitingTime + (rand() % 1000)/1000.f;
                }
            }
            
            if (moveLeft) x -= xSpd * elapsedTime.asSeconds();
            if (moveRight) x += xSpd * elapsedTime.asSeconds();
            if (moveUp) y -= ySpd * elapsedTime.asSeconds();
            if (moveDown) y += ySpd * elapsedTime.asSeconds();
            if (moveLeft || moveRight || moveUp || moveDown)
                sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        }
    }
    void NekoEntity::PollEvent(sf::Event& event)
    {
        
    }
    void NekoEntity::Resize()
    {
        sprite.setScale(0.7 * Room::scale * gs::scale, 0.7 * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        if (spriteLoaded) window->draw(sprite);
    }
    
    
    
    Room::Room()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/room.jpg");
        if (texture) texture->setSmooth(false);
        sprite.setTexture(*texture);
        spriteLoaded = (sprite.getTexture() != nullptr);
    }
    Room::~Room() { }
    void Room::Switch(bool on)
    {
        active = on;
        if (on) for (auto& n : rl::neko) if (!n.inited) n.Init();
        Resize(gs::width, gs::height);
    }
    void Room::Destroy() { for (auto& n : rl::neko) n.Destroy(); }
    void Room::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        if (gs::inGame) { active = false; return; }
        for (auto& n : rl::neko) n.Update(elapsedTime);
        
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            x = move_dx + sf::Mouse::getPosition(*gs::window).x/(scale*gs::scale);
            y = move_dy + sf::Mouse::getPosition(*gs::window).y/(scale*gs::scale);
            sprite.setPosition(x * gs::scale * scale, y * gs::scale * scale);
            for (auto& n : rl::neko) n.Resize();
        }
        if (sf::Touch::isDown(0))
        {
            x = move_dx + sf::Touch::getPosition(0, *gs::window).x/(scale*gs::scale);
            y = move_dy + sf::Touch::getPosition(0, *gs::window).y/(scale*gs::scale);
            sprite.setPosition(x * gs::scale * scale, y * gs::scale * scale);
            for (auto& n : rl::neko) n.Resize();
        }
    }
    void Room::PollEvent(sf::Event& event)
    {
        if (!active) return;
        for (auto& n : rl::neko) n.PollEvent(event);
        
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            move_dx = x - event.mouseButton.x/(scale*gs::scale);
            move_dy = y - event.mouseButton.y/(scale*gs::scale);
        }
        else if (event.type == sf::Event::TouchBegan)
        {
            if (event.touch.finger == 0)
            {
                move_dx = x - event.touch.x/(scale*gs::scale);
                move_dy = y - event.touch.y/(scale*gs::scale);
            }
        }
    }
    void Room::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        for (auto& n : rl::neko) n.Resize();
        
        sprite.setScale(3 * gs::scale * scale, 3 * gs::scale * scale);
        sprite.setPosition(x * gs::scale * scale, y * gs::scale * scale);
    }
    void Room::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        if (spriteLoaded) window->draw(sprite);
        for (auto& n : rl::neko) n.Draw(window);
    }
    
    
    
    void MainMenu::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/MainMenu1.jpg");
        if (texture)
        {
            background.setTexture(*texture);
            background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
        }
        spriteLoaded = (texture != nullptr);
        
        texture = ic::LoadTexture(L"Data/Neko/Vanilla Full.png");
        if (texture)
        {
            neko.setTexture(*texture);
            neko.setOrigin(texture->getSize().x/2, texture->getSize().y);
        }
        spriteLoaded = (texture != nullptr);
        
        playButton.setFont(L"Pacifica.ttf");
        playButton.setCharacterSize(140);
        playButton.setString(L"Прогулка");
        playButton.valign = Valign::Bottom;
        
        accountButton.setFont(L"Pacifica.ttf");
        accountButton.setCharacterSize(72);
        accountButton.setString(L"[" + Player::self->username + L"]");
        accountButton.valign = Valign::Top;
        accountBackButton.setFont(L"Pacifica.ttf");
        accountBackButton.setCharacterSize(90);
        if (Player::self->username == L"Закрыть") accountBackButton.setString(L"[Назад]");
        else accountBackButton.setString(L"[Закрыть]");
        accountBackButton.valign = Valign::Top;
        
        resetButton.setFont(L"Pacifica.ttf");
        resetButton.setCharacterSize(48);
        resetButton.setString(L"[Reset NekoLibrary]");
        resetButton.halign = Halign::Left;
        resetButton.valign = Valign::Bottom;
        
        roomButton.setTexture(L"Data/Neko/Chocola.png");
        roomButton.sprite.setRotation(-70);
        roomBackButton.setTexture(L"Data/Neko/Vanilla.png");
        roomBackButton.sprite.setRotation(-70);
        
        dialogueText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        dialogueText.setFillColor(sf::Color::White);
        dialogueText.setOutlineColor(sf::Color::Black);
        dialogueText.setOutlineThickness(1 * gs::scale);
        dialogueText.setCharacterSize(50 * gs::scale);
        dialogueText.setString(L"С возвращением, хозяин!~ <3");
        dialogueButton.setTexture(L"Data/Images/dialogue1.png");
        dialogueButton.valign = Valign::Bottom;
        dialogueButton.characterScale = true;
        
        backShape.setFillColor(sf::Color(0, 0, 0, 185));
        accountBackShape.setFillColor(sf::Color(0, 0, 0, 185));
        expShape.setFillColor(sf::Color(30, 30, 30, 255));
        helperText.setFillColor(sf::Color::White);
        helperText.setOutlineColor(sf::Color::Black);
        helperText.setOutlineThickness(1.f);
        helperText.setCharacterSize(48);
        helperText.setString(L"Уровень:");
        helperText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        
        //ic::PreloadTexture(L"Data/Images/park.jpg", 0, false);
        ic::PreloadTexture(L"Data/Images/heartsshape.png", 0, false);
        ic::PreloadTexture(L"Data/Images/heart.png", 0, false);
        ic::PreloadTexture(L"Data/Images/groping-hand.png", 0, false);
        ic::PreloadTexture(L"Data/Images/pause_button.png", 0, false);
        /*for (int i = 0; i < nl::neko.size(); ++i)
        {
            ic::PreloadTexture(L"Data/Neko/" + nl::neko[i].name + L".png", 0, false);
            sc::PreloadSound(L"Data/Neko/" + nl::neko[i].name + L".ogg", 0, false);
        }*/
    }
    void MainMenu::Destroy()
    {
        ic::DeleteImage(L"Data/Images/MainMenu1.jpg");
        ic::DeleteImage(L"Data/Neko/Vanilla Full.png");
        
        //ic::SetDestroyable(L"Data/Images/park.jpg", true);
        ic::SetDestroyable(L"Data/Images/heartsshape.png", true);
        ic::SetDestroyable(L"Data/Images/heart.png", true);
        ic::SetDestroyable(L"Data/Images/groping-hand.png", true);
        ic::SetDestroyable(L"Data/Images/pause_button.png", true);
        /*for (int i = 0; i < nl::neko.size(); ++i)
        {
            ic::SetDestroyable(L"Data/Neko/" + nl::neko[i].name + L".png", true);
            sc::SetDestroyable(L"Data/Sounds/" + nl::neko[i].name + L".ogg", true);
        }*/
    }
    void MainMenu::Update(const sf::Time& elapsedTime)
    {
        if (screen == Screen::room) room.Update(elapsedTime);
    }
    void MainMenu::PollEvent(sf::Event& event)
    {
        if (gs::inGame) return;
        
        switch (screen)
        {
            case Screen::main:
                if (playButton.PollEvent(event)) { gs::isPause = false; lastInGame = true; entity->AddComponent<NekoNinja::Controller>(); }
                else if (accountButton.PollEvent(event)) screen = Screen::account;
                else if (resetButton.PollEvent(event))
                {
                    for (auto& n : nl::neko) if (n.name != L"Shigure") n.tamed = false;
                    Player::self->SaveNekos();
                    
                    for (auto& n : rl::neko) n.Destroy(); rl::neko.clear();
                    if (nl::neko.size() != 0) rl::neko.emplace_back(&nl::neko[0]);
                }
                else if (dialogueButton.PollEvent(event)) dialogueButton.setVisible(false);
                else if (roomButton.PollEvent(event)) { screen = Screen::room; room.Switch(true); }
                break;
                
            case Screen::account:
                if (accountBackButton.PollEvent(event)) screen = Screen::main;
                break;
                
            case Screen::room:
                room.PollEvent(event);
                if (roomBackButton.PollEvent(event)) { screen = Screen::main; room.Switch(false); }
                break;
            default: break;
        }
    }
    void MainMenu::Resize(unsigned int width, unsigned int height)
    {
        if (gs::inGame) return;
        playButton.Resize(width, height);
        playButton.setPosition(width/2, height - height/10);
        accountButton.Resize(width, height);
        accountButton.setPosition(width/2, -5*gs::scale);
        resetButton.Resize(width, height);
        resetButton.setPosition(30*gs::scale, height - 20*gs::scale);
        roomButton.Resize(width, height);
        roomButton.setPosition(width, height/3.5);
        
        dialogueButton.Resize(width, height);
        dialogueButton.setPosition(width/2, height - playButton.text.getGlobalBounds().height - 45*gs::scaley);
        if (gs::width < dialogueButton.sprite.getGlobalBounds().width)
            dialogueButton.setScale((float)gs::width/dialogueButton.sprite.getGlobalBounds().width);
        dialogueText.setOutlineThickness(1 * gs::scale);
        dialogueText.setCharacterSize(40 * gs::scale);
        dialogueText.setPosition(dialogueButton.sprite.getGlobalBounds().left + 80*gs::scale, dialogueButton.sprite.getGlobalBounds().top + 20*gs::scale);
        
        if (spriteLoaded)
        {
            float factorX = (float)width / background.getTexture()->getSize().x;
            float factorY = (float)height / background.getTexture()->getSize().y;
            float scaleFactor = factorX > factorY ? factorX : factorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
            
            neko.setScale(1.5*gs::scScale, 1.5*gs::scScale);
            if (neko.getGlobalBounds().height + 20*gs::scale > gs::height)
                neko.setPosition(width/2, neko.getGlobalBounds().height + 20*gs::scale);
            else neko.setPosition(width/2, height);
        }
        
        room.Resize(width, height);
        roomBackButton.Resize(width, height);
        roomBackButton.setPosition(width, height/3.5);
        
        
        backShape.setSize({ (float)width, (float)height });
        accountBackShape.setSize({width * 5.f/6, height * 5.f/6});
        accountBackShape.setPosition((width * 1.f/6)/2, (height * 1.f/6)/2);
        accountBackButton.Resize(width, height);
        accountBackButton.setPosition(width/2, /*accountBackShape.getPosition().y*/ -5*gs::scale);
        
        helperText.setOutlineThickness(gs::scale);
        helperText.setCharacterSize((unsigned int)(78 * gs::scale));
        helperText.setString(L"Уровень:");
        expShape.setSize({accountBackShape.getSize().x * 0.9f, helperText.getGlobalBounds().height + 10*gs::scale});
        expShape.setPosition(accountBackShape.getPosition().x + accountBackShape.getSize().x*0.05f, 0);
    }
    void MainMenu::Draw(sf::RenderWindow* window)
    {
        if (gs::inGame) return;
        if (lastInGame)
        {
            dialogueButton.setVisible(true);
            int randomDialogue = rand() % 4;
            switch (randomDialogue)
            {
                case 0: dialogueText.setString(L"Как тебе прогулка, мур?~"); break;
                case 1: dialogueText.setString(L"Чем займёмся сегодня?~~~ \n   Пойдём гулять? :3"); break;
                case 2: dialogueText.setString(L"Хозяин... Я скучала >///<"); break;
                default: break;
            }
            lastInGame = false;
        }
        
        switch (screen)
        {
            case Screen::main: case Screen::account:
                if (spriteLoaded)
                {
                    window->draw(background);
                    window->draw(neko);
                }
                playButton.Draw(window);
                accountButton.Draw(window);
                resetButton.Draw(window);
                roomButton.Draw(window);
            
                dialogueButton.Draw(window);
                if (dialogueButton.visible) window->draw(dialogueText);
                
                if (screen == Screen::account)
                {
                    //window->draw(accountBackShape);
                    window->draw(backShape);
                    accountBackButton.Draw(window);
                    
                    float yy = gs::height/3;
                    
                    helperText.setCharacterSize((unsigned int)(78 * gs::scale));
                    helperText.setString(L"Уровень:    " + std::to_wstring(Player::self->level));
                    helperText.setPosition(accountBackShape.getPosition().x + 10*gs::scalex, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 30*gs::scaley;
                    
                    expShape.setPosition(expShape.getPosition().x, yy);
                    window->draw(expShape); expShape.setFillColor(sf::Color(255, 37, 142, 255));
                    expShape.setSize({expShape.getSize().x * (Player::self->expRatio), expShape.getSize().y});
                    window->draw(expShape); expShape.setFillColor(sf::Color(30, 30, 30, 255));
                    expShape.setSize({accountBackShape.getSize().x * 0.9f, expShape.getSize().y});
                    
                    helperText.setString(std::to_wstring(Player::self->exp) + L" / " + std::to_wstring(Player::self->expNeeded));
                    helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy - 15*gs::scale);
                    window->draw(helperText); yy += expShape.getGlobalBounds().height + 5*gs::scaley;
                }
                break;
                
            case Screen::room:
                room.Draw(window);
                roomBackButton.Draw(window);
                break;
                
            default: break;
        }
    }
}
