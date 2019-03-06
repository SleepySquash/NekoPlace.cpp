//
//  NekoNinja.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "NekoNinja.hpp"

namespace NekoNinja
{
    float NekoObject::gravity = 0.19f * 60 * 60;
    float NekoObject::yStartVelocity = -17.f * 60;
    int NekoObject::yStartRandomVelocity = 4*60;
    int NekoObject::xStartRandomVelocity = 6*60;
    
    Controller* NekoObject::control = nullptr;
    unsigned int* NekoObject::tamedArray = nullptr;
    
    
    
    void SceneBackground::Destroy() { ic::DeleteImage(imagePath); }
    void SceneBackground::Init()
    {
        sf::Texture* texture = ic::LoadTexture(imagePath);
        if (texture != nullptr)
        {
            sprite.setTexture(*texture); spriteLoaded = true;
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
        }
    }
    void SceneBackground::Resize(unsigned int width, unsigned int height)
    {
        if (spriteLoaded)
        {
            float factorX = (float)width / sprite.getTexture()->getSize().x;
            float factorY = (float)height / sprite.getTexture()->getSize().y;
            
            float scaleFactor = factorX > factorY ? factorX : factorY;
            sprite.setScale(scaleFactor, scaleFactor);
            sprite.setPosition(width/2, height/2);
        }
    }
    void SceneBackground::Draw(sf::RenderWindow* window)
    {
        if (spriteLoaded) window->draw(sprite);
    }
    
    
    
    
    
    
    void GUIOverlay::Destroy() { ic::DeleteImage(L"Data/Images/pause_button.png"); ic::DeleteImage(L"Data/Images/heart.png"); }
    void GUIOverlay::Init()
    {
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(1.f);
        scoreText.setCharacterSize(15);
        scoreText.setString(L"0");
        scoreText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (scoreText.getFont() != nullptr);
        
        strikeText.setFillColor(sf::Color::White);
        strikeText.setOutlineColor(sf::Color::Black);
        strikeText.setOutlineThickness(1.f);
        strikeText.setCharacterSize(15);
        strikeText.setString(L"0");
        strikeText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (strikeText.getFont() != nullptr);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/pause_button.png");
        if (texture) pauseSprite.setTexture(*texture);
        
        texture = ic::LoadTexture(L"Data/Images/groping-hand.png");
        if (texture) handSprite.setTexture(*texture);
        handSprite.setOrigin(170, 136);
        
        texture = ic::LoadTexture(L"Data/Images/heart.png");
        if (texture)
        {
            heartSprite.setTexture(*texture);
            heartSprite.setTextureRect(sf::IntRect(268, 0, 268, 256));
        }
        
        prevLevel = Player::self->level;
        prevExpRatio = Player::self->expRatio;
        pauseShape.setFillColor(sf::Color(0,0,0,150));
        expShape.setFillColor(sf::Color(30, 30, 30, 255));
        resultsShape.setFillColor(sf::Color(0,0,0,150));
        resultsLine.setFillColor(sf::Color::White);
        newNekoLine.setFillColor(sf::Color::White);
        menuShape.setFillColor(sf::Color(0,0,0,150));
        newNekoShape.setFillColor(sf::Color(212, 45, 138, 255));
        
        pauseText.setFillColor(sf::Color::White);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(1.f);
        scoreText.setCharacterSize(60);
        strikeText.setCharacterSize(60);
        pauseText.setString(L"ПАУЗА");
        pauseText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (pauseText.getFont() != nullptr);
        
        resultsText.setFillColor(sf::Color::White);
        resultsText.setOutlineColor(sf::Color::Black);
        resultsText.setOutlineThickness(1.f);
        resultsText.setCharacterSize(60);
        resultsText.setString(L"ИТОГИ");
        resultsText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (resultsText.getFont() != nullptr);
        
        helperText.setFillColor(sf::Color::White);
        helperText.setOutlineColor(sf::Color::Black);
        helperText.setOutlineThickness(1.f);
        helperText.setCharacterSize(48);
        helperText.setString(L"Результаты:");
        helperText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (helperText.getFont() != nullptr);
        
        resultsOkButton.setFont(L"Pacifica.ttf");
        resultsOkButton.setString(L"OK");
        resultsOkButton.setCharacterSize(108);
        resultsOkButton.valign = Valign::Bottom;
        
        
        menuText.setFillColor(sf::Color::White);
        menuText.setOutlineColor(sf::Color::Black);
        menuText.setOutlineThickness(1.f);
        menuText.setCharacterSize(80);
        menuText.setString(L"Куда пойдём, ня?");
        menuText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (helperText.getFont() != nullptr);
        
        menuRetryButton.setFont(L"Pacifica.ttf");
        menuRetryButton.setString(L"Снова");
        menuRetryButton.setCharacterSize(108);
        menuRetryButton.valign = Valign::Top;
        menuLeaveButton.setFont(L"Pacifica.ttf");
        menuLeaveButton.setString(L"Домой");
        menuLeaveButton.setCharacterSize(108);
        menuLeaveButton.valign = Valign::Top;
        
        pauseButton.setFont(L"Pacifica.ttf");
        pauseButton.setString(L"Домой");
        pauseButton.setCharacterSize(108);
        
        
        if (Player::self->neko)
        {
            nekoButton.setTexture(L"Data/Neko/Avatar/" + Player::self->neko->info->name + L".png");
            nekoButton.halign = Halign::Right;
            nekoButton.valign = Valign::Bottom;
            nekoButShape.setFillColor(sf::Color(Player::self->neko->info->color.r, Player::self->neko->info->color.g, Player::self->neko->info->color.b, 150));
            nekoButShape.setOutlineColor(sf::Color::White);
            
            nekoButText.setFillColor(sf::Color::White);
            nekoButText.setOutlineColor(sf::Color::Black);
            nekoButText.setString(L"0");
            nekoButText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        }
    }
    void GUIOverlay::Update(const sf::Time& elapsedTime)
    {
        if (control) { scoreText.setString(std::to_string(control->score));
            if (!scoresAlreadyColored && control->score > Player::self->topScore) {
                scoreText.setFillColor(sf::Color(255, 223, 0, 255)); scoresAlreadyColored = true; }
            strikeText.setString(std::to_string(control->strike) + "x");
            if (control->abilityElapsedCooldown && !control->abilityIsUp) nekoButText.setString(std::to_string((int)control->abilityElapsedCooldown));
        }
        system.Update(elapsedTime);
    }
    void GUIOverlay::Resize(unsigned int width, unsigned int height)
    {
        system.Resize(width, height);
        
        scoreText.setOutlineThickness(gs::scale);
        scoreText.setCharacterSize((unsigned int)(96 * gs::scale));
        scoreText.setPosition(8*gs::scale, -10*gs::scale);
        
        strikeText.setOutlineThickness(gs::scale);
        strikeText.setCharacterSize((unsigned int)(96 * gs::scale));
        strikeText.setPosition(8*gs::scale, gs::height - strikeText.getGlobalBounds().height -30*gs::scale);
        
        pauseText.setOutlineThickness(gs::scale);
        pauseText.setCharacterSize((unsigned int)(108 * gs::scale));
        pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4);
        
        handSprite.setScale(0.4 * gs::scale, 0.4 * gs::scale);
        
        heartSprite.setScale(0.3 * gs::scale, 0.3 * gs::scale);
        heartsXX = 10*gs::scale;
        heartSprite.setPosition(0, 10*gs::scale + scoreText.getLocalBounds().height + 10*gs::scale);
        
        pauseSprite.setScale(gs::scale, gs::scale);
        pauseSprite.setPosition(width - pauseSprite.getLocalBounds().width*gs::scale - 10*gs::scale, 10*gs::scale);
        
        pauseShape.setSize({(float)width, (float)height});
        pauseButton.Resize(width, height);
        pauseButton.setPosition(width/2, height - height/4);
        
        
        resultsShape.setSize({width * 5.f/6, height * 5.f/6});
        resultsShape.setPosition((width * 1.f/6)/2, (height * 1.f/6)/2);
        newNekoShape.setSize({resultsShape.getSize().x, 1});
        newNekoShape.setPosition(resultsShape.getPosition().x, 0);
        newNekoLine.setSize({resultsShape.getSize().x, 2*gs::scale});
        newNekoLine.setPosition(resultsShape.getPosition().x, 0);
        
        resultsText.setOutlineThickness(gs::scale);
        resultsText.setCharacterSize((unsigned int)(108 * gs::scale));
        resultsText.setPosition(gs::width/2 - resultsText.getGlobalBounds().width/2, resultsShape.getPosition().y - 15*gs::scale);
        helperText.setOutlineThickness(gs::scale);
        helperText.setCharacterSize((unsigned int)(78 * gs::scale));
        
        helperText.setString(L"Уровень:");
        expShape.setSize({resultsShape.getSize().x * 0.94f, helperText.getGlobalBounds().height + 10*gs::scale});
        expShape.setPosition(resultsShape.getPosition().x + resultsShape.getSize().x*0.03f, 0);
        
        resultsLine.setSize({resultsShape.getSize().x, 3*gs::scale});
        resultsLine.setPosition(resultsShape.getPosition().x, resultsShape.getPosition().y + resultsText.getLocalBounds().height + 20*gs::scale);
        
        resultsOkButton.Resize(width, height);
        resultsOkButton.setPosition(width/2, height - resultsShape.getPosition().y - 42*gs::scale);
        
        
        menuShape.setSize({width * 0.76f, height * 0.5f});
        menuShape.setPosition((width * 0.12f), (height * 1.f/4));
        
        menuText.setOutlineThickness(gs::scale);
        menuText.setCharacterSize((unsigned int)(76 * gs::scale)); menuText.setScale(1, 1);
        if (menuText.getGlobalBounds().width + 4*gs::scale > menuShape.getSize().x)
            menuText.setScale(menuShape.getSize().x/(menuText.getGlobalBounds().width + 4*gs::scale), menuText.getScale().y);
        menuText.setPosition(gs::width/2 - menuText.getGlobalBounds().width/2, menuShape.getPosition().y - 10*gs::scale);
        
        menuRetryButton.Resize(width, height);
        menuLeaveButton.Resize(width, height);
        menuRetryButton.setPosition(width/2, height/2 - menuRetryButton.text.getGlobalBounds().height + 22*gs::scale);
        menuLeaveButton.setPosition(width/2, menuRetryButton.text.getPosition().y + menuRetryButton.text.getGlobalBounds().height + 44*gs::scale);
        
        
        results_yyNeko = resultsLine.getPosition().y + 10*gs::scaley;
        float blockHeight;
        {
            float yy = 0;
            
            helperText.setCharacterSize((unsigned int)(78 * gs::scale));
            helperText.setString(L"Результат:");
            helperText.setPosition(resultsShape.getPosition().x + 10*gs::scalex, yy);
            yy += helperText.getGlobalBounds().height + 5*gs::scaley;
            
            helperText.setCharacterSize((unsigned int)(85 * gs::scale));
            helperText.setString(L"9");
            helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
            yy += helperText.getGlobalBounds().height + 10*gs::scaley;
            
            helperText.setCharacterSize((unsigned int)(50 * gs::scale));
            helperText.setString(L"(Лучший: 9)");
            helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
            yy += helperText.getGlobalBounds().height + 5*gs::scaley;
            
            helperText.setCharacterSize((unsigned int)(78 * gs::scale));
            helperText.setString(L"Уровень:");
            helperText.setPosition(resultsShape.getPosition().x + 10*gs::scalex, yy);
            yy += helperText.getGlobalBounds().height + 30*gs::scaley;
            
            expShape.setPosition(expShape.getPosition().x, yy);
            
            helperText.setString(L"9");
            helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy - 15*gs::scale);
            yy += expShape.getGlobalBounds().height + 5*gs::scaley;
            
            helperText.setCharacterSize((unsigned int)(64 * gs::scale));
            helperText.setString(L"Доступно лутбоксов!");
            helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
            yy += helperText.getGlobalBounds().height + 5*gs::scaley;
            
            blockHeight = yy;
        }
        results_yyStart = resultsLine.getPosition().y + (resultsShape.getSize().y - resultsLine.getPosition().y - resultsOkButton.text.getGlobalBounds().height + 25*gs::scale)/2 - blockHeight/2 + 10*gs::scaley;
        
        
        if (Player::self->neko)
        {
            nekoButton.Resize(width, height);
            nekoButton.setPosition(width - 10*gs::scale, height - 10*gs::scale);
            nekoButShape.setSize({168*gs::scale, 168*gs::scale});
            nekoButShape.setPosition(nekoButton.sprite.getPosition());
            nekoButShape.setOrigin(nekoButShape.getSize().x, nekoButShape.getSize().y);
            nekoButShape.setOutlineThickness(2*gs::scale);
            
            nekoButText.setOutlineThickness(2*gs::scale);
            nekoButText.setCharacterSize(94*gs::scale);
            nekoButText.setPosition(nekoButShape.getPosition().x - nekoButShape.getSize().x/2, nekoButShape.getPosition().y - nekoButShape.getSize().y/2);
            nekoButText.setOrigin(nekoButText.getLocalBounds().width/2, nekoButText.getLocalBounds().height/2);
        }
    }
    void GUIOverlay::Draw(sf::RenderWindow* window)
    {
        system.Draw(window);
        
        float xx = heartsXX;
        for (int i = 0; i < control->lifes; ++i)
        {
            heartSprite.setPosition(xx, heartSprite.getPosition().y);
            window->draw(heartSprite);
            xx += heartSprite.getLocalBounds().width * heartSprite.getScale().x + 5*gs::scale;
        }
        if (control->lifes != control->maxlifes) heartSprite.setTextureRect(sf::IntRect(0, 0, 268, 256));
        for (int i = control->lifes; i < control->maxlifes; ++i)
        {
            heartSprite.setPosition(xx, heartSprite.getPosition().y);
            window->draw(heartSprite);
            xx += heartSprite.getLocalBounds().width * heartSprite.getScale().x + 5*gs::scale;
        }
        if (control->lifes != control->maxlifes) heartSprite.setTextureRect(sf::IntRect(268, 0, 268, 256));
        
        
        if (gs::isPause)
        {
            window->draw(pauseShape);
            window->draw(pauseText);
            pauseButton.Draw(window);
        }
        
        if (fontLoaded) { window->draw(scoreText); window->draw(strikeText); }
        window->draw(pauseSprite);
        
        window->draw(nekoButShape);
        if (control->abilityIsUp) { nekoButton.sprite.setColor(sf::Color::Green); nekoButton.Draw(window); }
        else if (control->abilityElapsedCooldown) {
            nekoButton.sprite.setColor(sf::Color(150,150,150,255)); nekoButton.Draw(window); window->draw(nekoButText); }
        else { nekoButton.sprite.setColor(sf::Color::White); nekoButton.Draw(window); }
        
        if (!gs::isPause && !control->isGameOver)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                handSprite.setPosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
                window->draw(handSprite);
            }
            else if (sf::Touch::isDown(0))
            {
                handSprite.setPosition(sf::Touch::getPosition(0, *window).x, sf::Touch::getPosition(0, *window).y);
                window->draw(handSprite);
            }
        }
        
        if (control->isGameOver)
        {
            if (drawResults)
            {
                window->draw(resultsShape);
                window->draw(resultsLine);
                window->draw(resultsText);
                
                float yy = results_yyStart;
                
                helperText.setCharacterSize((unsigned int)(78 * gs::scale));
                helperText.setString(L"Результат:");
                helperText.setPosition(resultsShape.getPosition().x + 10*gs::scalex, yy);
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 5*gs::scaley;
                
                helperText.setCharacterSize((unsigned int)(85 * gs::scale));
                helperText.setString(std::to_wstring(control->score));
                helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
                if (control->score == Player::self->topScore) helperText.setFillColor(sf::Color(255, 223, 0, 255));
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 10*gs::scaley;
                if (control->score == Player::self->topScore) helperText.setFillColor(sf::Color::White);
                
                helperText.setCharacterSize((unsigned int)(50 * gs::scale));
                if (control->score == Player::self->topScore) helperText.setString(L"(Новый рекорд!)");
                else helperText.setString(L"(Лучший: " + std::to_wstring(Player::self->topScore) + L")");
                helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 5*gs::scaley;
                
                helperText.setCharacterSize((unsigned int)(78 * gs::scale));
                helperText.setString(L"Уровень:");
                helperText.setPosition(resultsShape.getPosition().x + 10*gs::scalex, yy);
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 30*gs::scaley;
                
                expShape.setPosition(expShape.getPosition().x, yy);
                window->draw(expShape); expShape.setFillColor(sf::Color(255, 37, 142, 255));
                expShape.setSize({expShape.getSize().x * (Player::self->expRatio), expShape.getSize().y});
                window->draw(expShape); expShape.setFillColor(sf::Color(30, 30, 30, 255));
                expShape.setSize({resultsShape.getSize().x * 0.94f, expShape.getSize().y});
                
                if (prevLevel != Player::self->level)
                {
                    helperText.setString(std::to_wstring(prevLevel) + L" -> " + std::to_wstring(Player::self->level));
                    helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy - 15*gs::scale);
                    window->draw(helperText); yy += expShape.getGlobalBounds().height + 5*gs::scaley;
                    
                    helperText.setCharacterSize((unsigned int)(64 * gs::scale));
                    helperText.setString(L"Доступно " + std::to_wstring(Player::self->level - prevLevel) + L" лутбоксов!");
                    if (helperText.getGlobalBounds().width + 4*gs::scale > resultsShape.getSize().x)
                        helperText.setScale(resultsShape.getSize().x/(helperText.getGlobalBounds().width + 4*gs::scale), helperText.getScale().y);
                    helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 5*gs::scaley;
                }
                else
                {
                    if (Player::self->expRatio != prevExpRatio)
                    {
                        expShape.setFillColor(sf::Color(248, 142, 181, 255));
                        expShape.setSize({expShape.getSize().x * (Player::self->expRatio), expShape.getSize().y});
                        window->draw(expShape); expShape.setFillColor(sf::Color(255, 37, 142, 255));
                        expShape.setSize({resultsShape.getSize().x * 0.94f * (prevExpRatio), expShape.getSize().y});
                        window->draw(expShape); expShape.setFillColor(sf::Color(30, 30, 30, 255));
                        expShape.setSize({resultsShape.getSize().x * 0.94f, expShape.getSize().y});
                    }
                    
                    helperText.setString(std::to_wstring(Player::self->level));
                    helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy - 15*gs::scale);
                    window->draw(helperText); yy += expShape.getGlobalBounds().height + 5*gs::scaley;
                }
                resultsOkButton.Draw(window);
            }
            else if (drawNewNeko)
            {
                window->draw(resultsShape);
                window->draw(resultsLine);
                window->draw(resultsText);
                
                float yy = results_yyNeko;
                
                helperText.setCharacterSize((unsigned int)(68 * gs::scale));
                helperText.setString(L"Получено:");
                helperText.setPosition(resultsShape.getPosition().x + 10*gs::scalex, yy - 5*gs::scale);
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 15*gs::scaley;
                
                newNekoLine.setPosition(newNekoLine.getPosition().x, yy);
                window->draw(newNekoLine); yy += 2*gs::scale;
                
                for (auto neko : newNekoList)
                {
                    if (ic::FindTexture(L"Data/Neko/Avatar/" + neko->name + L".png"))
                        newNekoSprite.setTexture(*ic::FindTexture(L"Data/Neko/Avatar/" + neko->name + L".png"), true);
                    else if (ic::FindTexture(L"Data/Neko/Chibi/" + neko->name + L".png"))
                        newNekoSprite.setTexture(*ic::FindTexture(L"Data/Neko/Chibi/" + neko->name + L".png"), true);
                    float factorScale = (90*gs::scaley)/newNekoSprite.getLocalBounds().height;
                    newNekoSprite.setScale(factorScale, factorScale);
                    newNekoSprite.setPosition(resultsShape.getPosition().x, yy);
                    
                    newNekoShape.setSize({newNekoShape.getSize().x, newNekoSprite.getGlobalBounds().height});
                    newNekoShape.setPosition(newNekoShape.getPosition().x, yy);
                    newNekoShape.setFillColor(neko->color);
                    window->draw(newNekoShape);
                    window->draw(newNekoSprite);
                    
                    helperText.setCharacterSize((unsigned int)(60 * gs::scaley));
                    helperText.setString(neko->display); helperText.setScale(1, 1);
                    if (helperText.getGlobalBounds().width + 4*gs::scale > newNekoShape.getSize().x)
                        helperText.setScale(newNekoShape.getSize().x/(helperText.getGlobalBounds().width + 4*gs::scale), helperText.getScale().y);
                    helperText.setPosition((resultsShape.getPosition().x + newNekoSprite.getGlobalBounds().width) + (resultsShape.getSize().x - newNekoSprite.getGlobalBounds().width)/2 - helperText.getGlobalBounds().width/2, yy - 10*gs::scaley);
                    window->draw(helperText); float mereYY = helperText.getGlobalBounds().height;
                    
                    helperText.setCharacterSize((unsigned int)(32 * gs::scaley)); helperText.setString(L"кошкодевочка");
                    helperText.setPosition((resultsShape.getPosition().x + newNekoSprite.getGlobalBounds().width) + (resultsShape.getSize().x - newNekoSprite.getGlobalBounds().width)/2 - helperText.getGlobalBounds().width/2, yy + mereYY - gs::scaley);
                    window->draw(helperText);
                    
                    yy += 90*gs::scaley;
                    newNekoLine.setPosition(newNekoLine.getPosition().x, yy);
                    window->draw(newNekoLine); yy += 2*gs::scaley;
                }
                
                resultsOkButton.Draw(window);
            }
            else
            {
                window->draw(menuShape);
                window->draw(menuText);
                menuLeaveButton.Draw(window);
                menuRetryButton.Draw(window);
            }
        }
    }
    void GUIOverlay::PollEvent(sf::Event& event)
    {
        if (!gs::isPause && control && !control->isGameOver && control->ability && !control->abilityIsUp && control->abilityElapsedCooldown == 0 && nekoButton.PollEvent(event)) control->ability->OnAction();
        else if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded)
        {
            if (control->isGameOver)
            {
                if (drawResults)
                {
                    if (resultsOkButton.PollEvent(event))
                    {
                        drawResults = false; drawNewNeko = newNekoAvailable; resultsOkButton.setString(L"OK");
                        resultsOkButton.setPosition(gs::width/2, gs::height - resultsShape.getPosition().y - 42*gs::scale);
                    }
                } else if (drawNewNeko) { if (resultsOkButton.PollEvent(event)) { drawNewNeko = false; } }
                else
                {
                    if (menuLeaveButton.PollEvent(event)) control->ReturnToMenu();
                    else if (menuRetryButton.PollEvent(event)) {
                        drawResults = true; prevLevel = Player::self->level; prevExpRatio = Player::self->expRatio; control->ResetGame(); }
                }
            } else if (gs::isPause && pauseButton.PollEvent(event)) control->ReturnToMenu();
        }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
        {
            if (!control->isGameOver)
            {
                sf::Vector2i dot;
                if (event.type == sf::Event::MouseButtonPressed) dot = {event.mouseButton.x, event.mouseButton.y};
                else dot = {event.touch.x, event.touch.y};
                
                sf::IntRect rect(pauseSprite.getPosition().x, pauseSprite.getPosition().y,
                                 pauseSprite.getPosition().x + pauseSprite.getLocalBounds().width*gs::scale,
                                 pauseSprite.getPosition().y + pauseSprite.getLocalBounds().height*gs::scale);
                if (rect.contains(dot.x, dot.y)) gs::isPause = !gs::isPause;
                if (gs::isPause) pauseButton.PollEvent(event);
            }
            else
            {
                if (drawResults) resultsOkButton.PollEvent(event);
                else if (drawNewNeko) resultsOkButton.PollEvent(event);
                else { menuLeaveButton.PollEvent(event); menuRetryButton.PollEvent(event); }
            }
        }
    }
    void GUIOverlay::GameOver()
    {
        for (unsigned long i = nl::neko.size() - 1; i >= 0; --i)
        {
            if (!nl::neko[i]->tamed && NekoObject::tamedArray[i])
            {
                newNekoList.push_back(nl::neko[i]);
                rl::neko.emplace_back(new NekoEntity(nl::neko[i], rl::neko.size()));
            }
            if (i == 0) break;
        }
        newNekoAvailable = (newNekoList.size() != 0);
        if (newNekoAvailable && !Player::self->roomUnlocked) { Player::self->roomUnlocked = true; control->entity->AddComponent<Popup>(L"Новая кошкодевочка!", L"Теперь Вам доступна комната с кошкодевочками! Нажмите в главном меню на Чоколу справа, чтобы попасть туда."); }
        
        if (newNekoAvailable) resultsOkButton.setString(L"ДАЛЕЕ"); else resultsOkButton.setString(L"OK");
        resultsOkButton.setPosition(gs::width/2, gs::height - resultsShape.getPosition().y - 42*gs::scale);
        
        scoresAlreadyColored = false;
        scoreText.setFillColor(sf::Color::White);
    }
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    void NekoObject::Init()
    {
        float possibility = (rand() % 10000) / 10000.f;
        unsigned long last = 0;
        for (; last < nl::neko.size(); ++last)
            if (nl::neko[last]->chance < possibility) break;
        
        do { nekoIndex = rand() % last; } while (nl::neko[nekoIndex]->difficultyAvailable > control->currentDifficulty);
        nekoInfo = nl::neko[nekoIndex];
        
        x = rand() % gs::relativeWidth;
        y = gs::relativeHeight;
        
        if (x < gs::relativeWidth/13) x = gs::relativeWidth/13;
        if (x > gs::relativeWidth - gs::relativeWidth/13) x = gs::relativeWidth - gs::relativeWidth/13;
        
        if (x < gs::relativeWidth/2) xVelocity = (rand() % xStartRandomVelocity); else xVelocity = -(rand() % xStartRandomVelocity);
        yVelocity = yStartVelocity + (rand() % yStartRandomVelocity);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Chibi/" + nekoInfo->name + L".png");
        if (texture != nullptr)
        {
            sprite.setTexture(*texture);
            halfTheWidth = texture->getSize().x / 2;
            sprite.setOrigin(halfTheWidth, 0);
        }
    }
    void NekoObject::Destroy()
    {
        offline = true;
        if (nekoInfo)
        {
            ic::DeleteImage(L"Data/Neko/Chibi/" + nekoInfo->name + L".png");
            sc::DeleteSound(L"Data/Neko/Nya/" + nekoInfo->name + L".ogg");
        }
    }
    void NekoObject::Update(const sf::Time& elapsedTime)
    {
        float yVelocity_prev = yVelocity;
        yVelocity += gravity * elapsedTime.asSeconds() * nns::velocityMultiplier;
        if (y > (gs::relativeHeight + 100)) offline = true;
        
        x += xVelocity * elapsedTime.asSeconds() * nns::velocityMultiplier;
        y += 0.5 * (yVelocity + yVelocity_prev) * elapsedTime.asSeconds() * nns::velocityMultiplier;
        
        sprite.setPosition(x * gs::scalex, y * gs::scaley);
    }
    void NekoObject::CheckInterception(sf::Vector2<int>& point)
    {
        if (control->isGameOver) return;
        
        sf::FloatRect rect = sprite.getGlobalBounds();
        rect.left += nekoInfo->offsets.left * sprite.getScale().x;
        rect.width -= nekoInfo->offsets.width * sprite.getScale().x;
        rect.top += nekoInfo->offsets.top * sprite.getScale().x;
        rect.height -= nekoInfo->offsets.height * sprite.getScale().x;
        tamed = rect.contains(point.x, point.y);
        if (tamed)
        {
            sprite.setColor(sf::Color(255,255,255,160));
            if (nekoInfo->name == L"Poop")
            {
                if (control->strike > control->maxStrike) control->maxStrike = control->strike;
                control->strike = 0;
                if (control->score < 10 * nns::scoreMultiplier) control->score = 0;
                else control->score -= 10 * nns::scoreMultiplier;
                --control->lifes; if (control->lifes == 0) control->GameOver();
            }
            else
            {
                float possibility = (rand() % 10000) / 10000.f;
                if (possibility <= control->criticalHitPossibility)
                {
                    control->score += 100 * nns::scoreMultiplier;
                    control->gui.system.AddComponent<CriticalHitText>(x, y);
                }
                else control->score += 10 * nns::scoreMultiplier;
                
                if (control->lifeScore <= control->score) {
                    control->lifeScore += 1000;
                    if (control->lifes != control->maxlifes) ++control->lifes; }
                ++control->strike; ++control->comboCounter; control->elapsedCombo = 0.f;
                control->lastNekoPosition = { x, y };
                
                if (nekoInfo->levelNeeded <= Player::self->level) {
                    ++tamedArray[nekoIndex];
                    control->system.AddComponent<HeartsShape>(x, y, nekoScale); }
                sf::SoundBuffer* buffer = sc::LoadSound(L"Data/Neko/Nya/" + nekoInfo->name + L".ogg");
                if (buffer)
                {
                    sound.setBuffer(*buffer);
                    sound.setVolume(gs::maxVolumeGlobal * gs::maxVolumeSound);
                    sound.play();
                }
            }
        }
    }
    void NekoObject::Resize()
    {
        sprite.setPosition(x * gs::scalex, y * gs::scaley);
        sprite.setScale(nekoScale * gs::scale, nekoScale * gs::scale);
    }
    void NekoObject::Draw(sf::RenderWindow* window) { window->draw(sprite); }

    
    
    
    
    
    
    
    
    
    


    Controller::Controller(const ControllerSettings& settings) : needsInit(true), difficulty(settings.difficulty), backgroundImage(settings.backgroundImage) { }
    void Controller::Destroy()
    {
        entity->SendMessage(MessageHolder("NekoNinjaComponents :: NekoNinja :: Returning to the menu"));
        gs::isPause = false;
        DefaultGamemodeSettings();
        if (firstNeko) delete firstNeko;
        
        background.Destroy();
        for (auto n : nekos) delete n;
        nekos.clear();
        if (needsInit)
        {
            for (int i = 0; i < nl::neko.size(); ++i)
            {
                ic::SetDestroyable(L"Data/Neko/Chibi/" + nl::neko[i]->name + L".png", true);
                sc::SetDestroyable(L"Data/Neko/Nya/" + nl::neko[i]->name + L".ogg", true);
            }
            ic::SetDestroyable(L"Data/Images/heartsshape.png", true);
        }
        
        gui.Destroy();
    }
    void Controller::Init()
    {
        Player::self->Init();
        sf::Clock clock;
        
        NekoObject::control = this;
        line[0] = sf::Vertex(sf::Vector2f(10, 10), sf::Color::Red);
        line[1] = sf::Vertex(sf::Vector2f(150, 150), sf::Color::Red);
        
        background.imagePath = backgroundImage;
        background.Init();
        
        if (needsInit)
        {
            ic::PreloadTexture(L"Data/Images/park.jpg", 0, false);
            ic::PreloadTexture(L"Data/Images/heartsshape.png", 0, false);
            ic::PreloadTexture(L"Data/Images/heart.png", 0, false);
            ic::PreloadTexture(L"Data/Images/groping-hand.png", 0, false);
            ic::PreloadTexture(L"Data/Images/pause_button.png", 0, false);
            for (int i = 0; i < nl::neko.size(); ++i)
            {
                ic::PreloadTexture(L"Data/Neko/Chibi/" + nl::neko[i]->name + L".png", 0, false);
                //TODO: sc::PreloadSound(L"Data/Neko/Nya/" + nl::neko[i]->name + L".ogg", 0, false);
            }
        }
        
        firstNeko = new NekoObject();
        NekoObject::tamedArray = new unsigned int[nl::neko.size()]();
        if (Player::self->neko && Player::self->activeNekoUnlocked) ability = Player::self->neko->info->ability;
        
        //topScore and topStrike reading
        
        gui.control = this; gui.Init();
        ResetGame();
        elapsedSeconds -= clock.getElapsedTime().asSeconds();
    }
    void Controller::ApplyGameSettings() { for (int i = 0; i < difficulty; ++i) ApplyDifficulty(++currentDifficulty); }
    void Controller::Update(const sf::Time& elapsedTime)
    {
        if (!gs::isPause && !gs::ignoreEvent)
        {
            if (ability)
            {
                abilityIsUp = ability->isActive;
                if (abilityIsUp) { ability->OnUpdate(elapsedTime); if (ability->isActive) abilityElapsedCooldown = ability->cooldownTime; }
                else
                {
                    if (abilityElapsedCooldown > 0) abilityElapsedCooldown -= elapsedTime.asSeconds();
                    else { abilityElapsedCooldown = 0; }
                }
            }
            
            system.Update(elapsedTime);
            if (spawnNekos)
            {
                harderElapsedTime += elapsedTime.asSeconds() * nns::getHarderTimeMultiplier;
                if (harderElapsedTime > getHarderTime)
                {
                    float possibility = (rand() % 1000)/1000.f;
                    if (possibility >= eventPossibility)
                    {
                        cout << "Event placeholder..." << endl;
                        
                        ApplyDifficulty(++currentDifficulty);
                        harderElapsedTime = 0.f;
                    }
                    else
                    {
                        cout << "Getting harder" << endl;
                        ApplyDifficulty(++currentDifficulty);
                        harderElapsedTime = 0.f;
                    }
                }
                
                if (attackIsDone)
                {
                    elapsedSeconds += elapsedTime.asSeconds() * nns::timeMultiplier;
                    if (elapsedSeconds > nextAttackTime)
                    {
                        elapsedSeconds = 0.f;
                        nextAttackTime = baseAttackTime + (rand() % (int)(randomAttackTime*1000))/1000.f;
                        attackType = rand() % 3;
                        switch (attackType)
                        {
                            case 0:
                                SpawnNeko();
                                attackCount = 3 + rand() % 3;
                                attackingElapsedTime = 0.f;
                                attackIsDone = false;
                                break;
                                
                            default:
                                attackCount = 1 + rand() % 5;
                                for (int i = 0; i < attackCount; ++i) SpawnNeko();
                                attackIsDone = true;
                                break;
                        }
                    }
                }
                else
                {
                    switch (attackType)
                    {
                        case 0:
                            attackingElapsedTime += elapsedTime.asSeconds() * nns::getHarderTimeMultiplier;
                            if (attackingElapsedTime >= attackingAreaTime)
                            {
                                SpawnNeko(); --attackCount;
                                if (attackCount <= 0) attackIsDone = true;
                                attackingElapsedTime = 0;
                            }
                            break;
                        default: attackIsDone = true; break;
                    }
                }
                
                if (comboCounter != 0)
                {
                    elapsedCombo += elapsedTime.asSeconds() * nns::getHarderTimeMultiplier;
                    if (elapsedCombo > comboThreshold)
                    {
                        if (comboCounter > 2)
                        {
                            gui.system.PrioritizeComponent<ComboText>(1, comboCounter, lastNekoPosition.x, lastNekoPosition.y);
                            score += comboCounter * 10 * nns::scoreMultiplier;
                            if (lifeScore <= score)
                            {
                                lifeScore += 1000;
                                if (lifes != maxlifes) ++lifes;
                            }
                        }
                        comboCounter = 0;
                    }
                }
            }
            
            for (auto it = nekos.begin(); it != nekos.end(); ++it)
            {
                if ((*it)->offline) {
                    if (!isGameOver && !(*it)->tamed && (*it)->nekoInfo->name != L"Poop") {
                        if (strike > maxStrike) maxStrike = strike;
                        strike = 0; --lifes; if (lifes <= 0) GameOver();}
                    (*it)->Destroy(); delete (*it);
                    it = nekos.erase(it); /*score += 100;*/
                }
                else (*it)->Update(elapsedTime);
            }
            
            if (cursor.size() > 60)
            {
                auto it = cursor.begin(); std::advance(it, cursor.size() - 60);
                cursor.erase(cursor.begin(), it);
            }
            
            if (cursor.size() > 0)
            {
                elapsedCursor += elapsedTime.asSeconds();
                while (elapsedCursor > 0.04f)
                {
                    if (cursor.size() > 0)
                    {
                        elapsedCursor -= 0.04f;
                        cursor.erase(cursor.begin());
                    }
                    else elapsedCursor = 0.f;
                }
            }
        }
        gui.Update(elapsedTime);
    }
    void Controller::Resize(unsigned int width, unsigned int height)
    {
        background.Resize(width, height);
        system.Resize(width, height);
        for (auto& n : nekos) n->Resize();
        gui.Resize(width, height);
    }
    void Controller::Draw(sf::RenderWindow* window)
    {
        if (gs::ignoreDraw) return;
        
        background.Draw(window);
        system.Draw(window);
        for (auto& n : nekos) n->Draw(window);
        for (auto it = cursor.begin(); it != cursor.end(); ++it)
        {
            if ((*it).x != -1)
            {
                auto it1 = it; std::advance(it1, 1);
                if (it1 != cursor.end())
                {
                    if ((*it1).x != -1)
                    {
                        line[0].position.x = (*it).x;
                        line[0].position.y = (*it).y;
                        line[1].position.x = (*it1).x;
                        line[1].position.y = (*it1).y;
                        window->draw(line, 2, sf::Lines);
                    }
                }
            }
        }
        if (ability && abilityIsUp) ability->OnDraw(window);
        gui.Draw(window);
    }
    void Controller::PollEvent(sf::Event& event)
    {
#if defined(SFML_SYSTEM_IOS) || defined(SFML_SYSTEM_ANDROID)
        if (event.type == sf::Event::MouseLeft) if (gs::pauseOnFocusLost && !isGameOver) gs::isPause = true;
#endif
        if (event.type == sf::Event::LostFocus) { if (gs::pauseOnFocusLost && !isGameOver) gs::isPause = true; }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::K)
        {
            ApplyDifficulty(++currentDifficulty);
            cout << "Difficulty is now: " << currentDifficulty << endl;
        }
        else if (!gs::isPause && !gs::ignoreEvent && !isGameOver)
        {
            bool abilityTurnOn{ false };
            if (ability && !abilityIsUp && abilityElapsedCooldown == 0)
            {
                abilityTurnOn = (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Button::Right);
                if (!abilityTurnOn && event.type == sf::Event::TouchBegan)
                {
                    if (event.touch.finger == 0) { countdownLastTouchedMoment = true; lastTouchedMoment = 0.03f; }
                    else if (event.touch.finger == 1) { if (countdownLastTouchedMoment && lastTouchedMoment > 0) abilityTurnOn = true; }
                }
            }
            if (abilityTurnOn) ability->OnAction();
            else if (event.type == sf::Event::MouseButtonReleased || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
                swipeConfirmed = false;
            else if (event.type == sf::Event::MouseMoved || event.type == sf::Event::TouchMoved)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || (sf::Touch::isDown(0) && event.touch.finger == 0))
                {
                    sf::Vector2i dot;
                    if (event.type == sf::Event::MouseMoved) dot = {event.mouseMove.x, event.mouseMove.y};
                    else dot = {event.touch.x, event.touch.y};
                    
                    endPoint.x = dot.x;
                    endPoint.y = dot.y;
                    
                    if (!swipeConfirmed)
                    {
                        cursor.push_back({ -1, -1 });
                        startPoint.x = dot.x;
                        startPoint.y = dot.y;
                        
                        swipeConfirmed = true;
                    }
                    
                    if (pow(startPoint.x - endPoint.x, 2) + pow(startPoint.y - endPoint.y, 2) >= 36*gs::scale)
                    {
                        if (abs(startPoint.x - endPoint.x) > abs(startPoint.y - endPoint.y))
                        {
                            sf::Vector2<int> point{ startPoint.x, startPoint.y };
                            int steps = 0;
                            if (startPoint.x - endPoint.x != 0) steps = (startPoint.y - endPoint.y) / (abs(startPoint.x - endPoint.x)/8.f);
                            if (startPoint.x < endPoint.x)
                            {
                                while (point.x < endPoint.x)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n->tamed) { n->CheckInterception(point); }
                                    point.x += 8; point.y -= steps;
                                }
                            }
                            else
                            {
                                while (point.x > endPoint.x)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n->tamed) { n->CheckInterception(point); }
                                    point.x -= 8; point.y -= steps;
                                }
                            }
                        }
                        else
                        {
                            sf::Vector2<int> point{ startPoint.x, startPoint.y };
                            int steps = 0;
                            if (startPoint.y - endPoint.y != 0) steps = (startPoint.x - endPoint.x) / (abs(startPoint.y - endPoint.y)/8.f);
                            if (startPoint.y < endPoint.y)
                            {
                                while (point.y < endPoint.y)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n->tamed) { n->CheckInterception(point); }
                                    point.y += 8; point.x -= steps;
                                }
                            }
                            else
                            {
                                while (point.y > endPoint.y)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n->tamed) { n->CheckInterception(point); }
                                    point.y -= 8; point.x -= steps;
                                }
                            }
                        }
                    }
                    
                    startPoint.x = endPoint.x;
                    startPoint.y = endPoint.y;
                } else swipeConfirmed = false;
            }
        }
        if (!gs::ignoreEvent) gui.PollEvent(event);
    }
    void Controller::GameOver()
    {
        if (strike > maxStrike) maxStrike = strike;
        isGameOver = true;
        spawnNekos = false;
        gs::isPause = false;
        
        Player::self->AddExperience(score*0.1);
        Player::self->SaveData();
        Player::self->lootboxes += Player::self->level - gui.prevLevel;
        if (gui.prevLevel < 6 && Player::self->level >= 6) {
            if (Player::self->neko) ability = Player::self->neko->info->ability;
            Player::self->activeNekoUnlocked = true; entity->AddComponent<Popup>(L"Вы достигли 6-го уровня!", L"Теперь Вы можете использовать активную способность кошкодевочки."); }
        if (gui.prevLevel < 20 && Player::self->level >= 20) { ++Player::self->passiveNekosUnlocked; entity->AddComponent<Popup>(L"Вы достигли 20-го уровня!", L"Теперь Вы можете добавить в отряд на прогулку ещё одну кошкодевочку с пассивной способностью!"); }
        if (gui.prevLevel < 35 && Player::self->level >= 35) { ++Player::self->passiveNekosUnlocked; entity->AddComponent<Popup>(L"Вы достигли 35-го уровня!", L"Теперь Вы можете добавить в отряд на прогулку вторую кошкодевочку с пассивной способностью!"); }
        
        gui.GameOver();
        for (unsigned int i = 0; i < nl::neko.size(); ++i)
            if (NekoObject::tamedArray[i]) nl::neko[i]->tamed = true;
        Player::self->SaveNekos();
        
        if (ability && abilityIsUp) { abilityIsUp = false; ability->OnEnd(); abilityElapsedCooldown = 0.f; }
        
        if (score > Player::self->topScore || maxStrike > Player::self->topStrike || score != 0)
        {
            Player::self->totalScore += score;
            if (score > Player::self->topScore) { Player::self->topScore = score; }
            if (maxStrike > Player::self->topStrike) { Player::self->topStrike = maxStrike; }
            
            if (!base::FileExists(Player::self->scoresPath))
                base::CreateDirectory(base::utf16(documentsPath()));
            
            std::wofstream wof;
    #ifdef _WIN32
            wof.open(Player::self->scoresPath);
    #else
            wof.open(base::utf8(Player::self->scoresPath));
    #endif
            wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            if (wof.is_open()) wof << Player::self->topScore << endl
                << Player::self->topStrike << endl << Player::self->totalScore << endl << Player::self->lootboxes << endl;
        }
    }
    void Controller::ResetGame()
    {
        gs::isPause = false;
        lifes = maxlifes;
        spawnNekos = true;
        isGameOver = false;
        score = 0;
        comboCounter = 0;
        lifeScore = 1000;
        
        /////////////// HELPER NEKO DEPENDENT
        /// lifeScore
        /// maxlifes
        /// criticalHitPossibility
        /// comboThreshold
        /// static Scoremultiplier
        /// getHarderTime
        /////////////// HELPER NEKO DEPENDENT
        
        elapsedSeconds = 0.f; harderElapsedTime = 0.f;
        currentDifficulty = difficulty;
        nextAttackTime = 1.f;
        nekoScale = 1.f;
        baseAttackTime = 1.2f;
        attackingAreaTime = 0.5f;
        nns::timeMultiplier = 1.f;
        DefaultGamemodeSettings();
        
        
        for (unsigned int i = 0; i < nl::neko.size(); ++i) NekoObject::tamedArray[i] = 0;
        gui.newNekoList.clear();
        for (auto& n : nekos) delete n;
        nekos.clear();
        system.clear();
        
        ApplyGameSettings();
        for (auto& n : Player::self->passiveNeko) if (n->info->ability) n->info->ability->OnAction();
    }
    void Controller::DefaultGamemodeSettings()
    {
        nns::scoreMultiplier = 1.f;
        nns::velocityMultiplier = 1.f;
    }
    void Controller::ReturnToMenu()
    {
        if (ability && abilityIsUp) ability->OnEnd();
        gs::isPause = false;
        entity->PopComponent(this);
        entity->system->Resize(gs::width, gs::height);
    }
    void Controller::SpawnNeko()
    {
        if (firstNeko) { nekos.push_back(firstNeko); firstNeko = nullptr; }
        else nekos.push_back(new NekoObject());
        nekos.back()->nekoScale = nekoScale;
        nekos.back()->Init(); nekos.back()->Resize();
    }
    void Controller::ApplyDifficulty(const int &diff)
    {
        /// nekoScale
        /// velocitymultiplier
        /// attackingAreaTime
        /// baseAttackTime
        /// static NekoObject's GamemodeSettings()
        ///     NekoObject::gravity = 0.19f * 60 * 60;
        ///     NekoObject::yStartVelocity = -17.f * 60;
        ///     NekoObject::yStartRandomVelocity = 4*60;
        
        nns::scoreMultiplier *= 1.05;
        switch (diff)
        {
            case 1:
                nekoScale *= 0.91;
                break;
            case 4:
            case 2:
                nekoScale *= 0.95;
                attackingAreaTime *= 0.95;
                /*baseAttackTime *= 0.93;
                randomAttackTime *= 0.93;*/
                nns::timeMultiplier *= 1.12;
                nns::velocityMultiplier *= 1.05;
                break;
            case 5:
            case 3:
                /*baseAttackTime *= 0.94;
                randomAttackTime *= 0.94;*/
                attackingAreaTime *= 0.96;
                nns::timeMultiplier *= 1.12;
                break;
            default:
                nekoScale *= 0.98;
                /*baseAttackTime *= 0.93;
                randomAttackTime *= 0.93;*/
                attackingAreaTime *= 0.95;
                nns::timeMultiplier *= 1.11;
                nns::velocityMultiplier *= 1.05;
                break;
        }
    }
}
