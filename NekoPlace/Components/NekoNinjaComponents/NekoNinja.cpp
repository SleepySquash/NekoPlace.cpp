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
    float Controller::scoreMultiplier = 1.f;
    
    Controller* NekoObject::control = nullptr;
    unsigned int* NekoObject::tamedArray = nullptr;
    
    /*std::sort(s.begin(), s.end(), [](int a, int b) {
        return a > b;
    });*/
    vector<NekoInfo> NekoLibrary::neko = {
        NekoInfo(L"Shigure",  1.f, 155, 30, 120, 70, true),
        NekoInfo(L"Azuki",    1.f, 115, 75, 98, 80),
        NekoInfo(L"Poop",     0.68f, 20, 20, 20, 20),
        NekoInfo(L"Cinnamon", 0.5f, 144, 50, 144, 70),
        NekoInfo(L"Maple",    0.4f, 115, 90, 138, 100),
        NekoInfo(L"Coconut",  0.3f, 195, 75, 97, 80),
        NekoInfo(L"Vanilla",  0.1f, 185, 75, 167, 70),
        NekoInfo(L"Chocola",  0.1f, 168, 80, 160, 70),
    };
    list<NekoEntity> RoomLibrary::neko;
    
    Player* Player::self = new Player();
    
    
    
    
    Player::Player()
    {
        std::wifstream wif;
#ifdef _WIN32
        wif.open(settingsPath);
#else
        wif.open(base::utf8(settingsPath));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            
            std::getline(wif, line);
            level = (unsigned int)std::atol(base::utf8(line).c_str());
            std::getline(wif, line);
            exp = (unsigned int)std::atol(base::utf8(line).c_str());
            
            wif.close();
        }
        switch (level)
        {
            case 1: expNeeded = 100; break;
            case 2: expNeeded = 120; break;
            case 3: expNeeded = 160; break;
            case 4: expNeeded = 210; break;
            case 5: expNeeded = 270; break;
            case 6: expNeeded = 360; break;
            case 7: expNeeded = 420; break;
            case 8: expNeeded = 480; break;
            case 9: expNeeded = 560; break;
            case 10: expNeeded = 660; break;
            case 11: expNeeded = 760; break;
            case 12: expNeeded = 880; break;
            case 13: expNeeded = 1000; break;
            case 14: expNeeded = 1200; break;
            case 15: expNeeded = 1500; break;
            default: expNeeded = 2000; break;
        }
        expRatio = (float)exp/expNeeded;
        
#ifdef _WIN32
        wif.open(nekoPath);
#else
        wif.open(base::utf8(nekoPath));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line;
            bool nekoReading{ false };
            vector<NekoInfo>::iterator it{ nl::neko.end() };
            
            while (!wif.eof())
            {
                std::getline(wif, line);
                if (!nekoReading)
                {
                    std::wstring nekoName = L"";
                    for (int i = 0; i < line.size(); ++i) if (line[i] != 13) nekoName += line[i];
                    it = std::find_if(nl::neko.begin(), nl::neko.end(), [&nekoName](const NekoInfo& m) { return m.name == nekoName; });
                    if (it != nl::neko.end()) (*it).tamed = true;
                    nekoReading = true;
                }
                else
                {
                    if (it != nl::neko.end())
                    {
                        int pos{ 0 };
                        
                        std::wstring levelStr = nss::ParseUntil(line, L' ', pos);
                        (*it).level = base::atoi(levelStr); pos += levelStr.length() + 1;
                    }
                    nekoReading = false;
                }
            }
            
            wif.close();
        }
        
        for (auto& n : nl::neko) if (n.tamed) rl::neko.emplace_back(&n);
    }
    Player::~Player()
    {
        SaveData();
    }
    void Player::SaveData()
    {
        if (!base::FileExists(settingsPath))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(settingsPath);
#else
        wof.open(base::utf8(settingsPath));
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << level << endl;
            wof << exp << endl;
        }
    }
    void Player::SaveNeko(NekoInfo* info)
    {
        if (!base::FileExists(settingsPath))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(nekoPath, std::wofstream::app);
#else
        wof.open(base::utf8(nekoPath), std::wofstream::app);
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            wof << info->name << endl;
            wof << info->level << L' ' << endl;
        }
    }
    void Player::SaveNekos()
    {
        if (!base::FileExists(settingsPath))
            base::CreateDirectory(base::utf16(documentsPath()));
        
        std::wofstream wof;
#ifdef _WIN32
        wof.open(nekoPath);
#else
        wof.open(base::utf8(nekoPath));
#endif
        wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        if (wof.is_open())
        {
            for (auto& neko : nl::neko)
            {
                if (neko.tamed)
                {
                    wof << neko.name << endl;
                    wof << neko.level << L' ' << endl;
                }
            }
        }
    }
    void Player::AddExperience(unsigned int xp)
    {
        exp += xp;
        while (exp >= expNeeded)
        {
            exp -= expNeeded;
            ++level;
            ++lootboxes;
        }
        expRatio = (float)exp/expNeeded;
    }
    
    
    
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
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/pause_button.png");
        if (texture != nullptr) pauseSprite.setTexture(*texture);
        
        texture = ic::LoadTexture(L"Data/Images/groping-hand.png");
        if (texture != nullptr) handSprite.setTexture(*texture);
        handSprite.setOrigin(170, 136);
        
        texture = ic::LoadTexture(L"Data/Images/heart.png");
        if (texture != nullptr)
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
    }
    void GUIOverlay::Update(const sf::Time& elapsedTime)
    {
        if (control != nullptr) scoreText.setString(std::to_wstring(control->score));
        system.Update(elapsedTime);
    }
    void GUIOverlay::Resize(unsigned int width, unsigned int height)
    {
        system.Resize(width, height);
        
        scoreText.setOutlineThickness(gs::scale);
        scoreText.setCharacterSize((unsigned int)(96 * gs::scale));
        scoreText.setPosition(8*gs::scale, -10*gs::scale);
        
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
        resultsText.setPosition(gs::width/2 - resultsText.getLocalBounds().width/2, resultsShape.getPosition().y - 15*gs::scale);
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
        menuText.setCharacterSize((unsigned int)(76 * gs::scale));
        menuText.setPosition(gs::width/2 - menuText.getLocalBounds().width/2, menuShape.getPosition().y - 10*gs::scale);
        
        menuRetryButton.Resize(width, height);
        menuLeaveButton.Resize(width, height);
        menuRetryButton.setPosition(width/2, height/2 - menuRetryButton.text.getLocalBounds().height + 22*gs::scale);
        menuLeaveButton.setPosition(width/2, menuRetryButton.text.getPosition().y + menuRetryButton.text.getLocalBounds().height + 44*gs::scale);
        
        
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
        
        if (fontLoaded) window->draw(scoreText);
        window->draw(pauseSprite);
        
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
                window->draw(helperText); yy += helperText.getGlobalBounds().height + 10*gs::scaley;
                
                helperText.setCharacterSize((unsigned int)(50 * gs::scale));
                if (control->score == control->topScore) helperText.setString(L"(Новый рекорд!)");
                else helperText.setString(L"(Лучший: " + std::to_wstring(control->topScore) + L")");
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
                    if (ic::FindTexture(L"Data/Neko/" + neko->name + L".png") != nullptr)
                        newNekoSprite.setTexture(*ic::FindTexture(L"Data/Neko/" + neko->name + L".png"), true);
                    float factorScale = (90*gs::scaley)/newNekoSprite.getLocalBounds().height;
                    newNekoSprite.setScale(factorScale, factorScale);
                    newNekoSprite.setPosition(resultsShape.getPosition().x, yy);
                    
                    newNekoShape.setSize({newNekoShape.getSize().x, newNekoSprite.getGlobalBounds().height});
                    newNekoShape.setPosition(newNekoShape.getPosition().x, yy);
                    newNekoShape.setFillColor(neko->color);
                    window->draw(newNekoShape);
                    window->draw(newNekoSprite);
                    
                    helperText.setCharacterSize((unsigned int)(60 * gs::scaley));
                    helperText.setString(neko->name);
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
        if (event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchEnded)
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
        for (unsigned int i = nl::neko.size() - 1; i >= 0; --i)
        {
            if (!nl::neko[i].tamed && NekoObject::tamedArray[i])
            {
                newNekoList.push_back(&nl::neko[i]);
                rl::neko.emplace_back(&nl::neko[i]);
            }
            if (i == 0) break;
        }
        newNekoAvailable = (newNekoList.size() != 0);
        
        if (newNekoAvailable) resultsOkButton.setString(L"ДАЛЕЕ"); else resultsOkButton.setString(L"OK");
        resultsOkButton.setPosition(gs::width/2, gs::height - resultsShape.getPosition().y - 42*gs::scale);
    }
    
    
    
    
    
    
    
    void NekoObject::Init()
    {
        float possibility = (rand() % 10000) / 10000.f;
        unsigned long last = 0;
        for (; last < nl::neko.size(); ++last)
            if (nl::neko[last].chance < possibility) break;
        
        nekoIndex = rand() % last;
        nekoInfo = &nl::neko[nekoIndex];
        
        x = rand() % gs::relativeWidth;
        y = gs::relativeHeight;
        
        if (x < gs::relativeWidth/13) x = gs::relativeWidth/13;
        if (x > gs::relativeWidth - gs::relativeWidth/13) x = gs::relativeWidth - gs::relativeWidth/13;
        
        if (x < gs::relativeWidth/2) xVelocity = (rand() % xStartRandomVelocity); else xVelocity = -(rand() % xStartRandomVelocity);
        yVelocity = yStartVelocity + (rand() % yStartRandomVelocity);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + nekoInfo->name + L".png");
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
            ic::DeleteImage(L"Data/Neko/" + nekoInfo->name + L".png");
            sc::DeleteSound(L"Data/Neko/" + nekoInfo->name + L".ogg");
        }
    }
    void NekoObject::Update(const sf::Time& elapsedTime)
    {
        float yVelocity_prev = yVelocity;
        yVelocity += gravity * elapsedTime.asSeconds();
        if (y > (gs::relativeHeight + 100)) offline = true;
        
        x += xVelocity * elapsedTime.asSeconds();
        y += 0.5 * (yVelocity + yVelocity_prev) * elapsedTime.asSeconds();
        
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
                if (control->score < 10*Controller::scoreMultiplier) control->score = 0;
                else control->score -= 10*Controller::scoreMultiplier;
                --control->lifes; if (control->lifes == 0) control->GameOver();
            }
            else
            {
                ++tamedArray[nekoIndex];
                //nekoInfo->tamed = true;
                
                float possibility = (rand() % 10000) / 10000.f;
                if (possibility <= control->criticalHitPossibility)
                {
                    control->score += 100 * Controller::scoreMultiplier;
                    control->gui.system.AddComponent<CriticalHitText>(x, y);
                }
                else control->score += 10 * Controller::scoreMultiplier;
                
                if (control->lifeScore <= control->score)
                {
                    control->lifeScore += 1000;
                    if (control->lifes != control->maxlifes)
                        ++control->lifes;
                }
                ++control->comboCounter; control->elapsedCombo = 0.f;
                control->lastNekoPosition = { x, y };
                
                control->system.AddComponent<HeartsShape>(x, y, nekoScale);
                sf::SoundBuffer* buffer = sc::LoadSound(L"Data/Sounds/" + nekoInfo->name + L".ogg");
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
    void NekoObject::Draw(sf::RenderWindow* window)
    {
        window->draw(sprite);
    }



    void Controller::Destroy()
    {
        gs::inGame = false;
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
                ic::SetDestroyable(L"Data/Neko/" + nl::neko[i].name + L".png", true);
                sc::SetDestroyable(L"Data/Sounds/" + nl::neko[i].name + L".ogg", true);
            }
            ic::SetDestroyable(L"Data/Images/heartsshape.png", true);
        }
        
        gui.Destroy();
    }
    void Controller::Init()
    {
        sf::Clock clock;
        
        gs::inGame = true;
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
                ic::PreloadTexture(L"Data/Neko/" + nl::neko[i].name + L".png", 0, false);
                sc::PreloadSound(L"Data/Neko/" + nl::neko[i].name + L".ogg", 0, false);
            }
        }
        
        for (int i = 0; i < difficulty; ++i) ApplyDifficulty(++currentDifficulty);
        firstNeko = new NekoObject();
        NekoObject::tamedArray = new unsigned int[nl::neko.size()]();
        
        std::wifstream wif;
#ifdef _WIN32
        wif.open(Player::self->scoresPath);
#else
        wif.open(base::utf8(Player::self->scoresPath));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line; std::getline(wif, line);
            topScore = (unsigned int)std::atol(base::utf8(line).c_str());
        }
        
        gui.control = this;
        gui.Init();
        
        elapsedSeconds -= clock.getElapsedTime().asSeconds();
    }
    void Controller::Update(const sf::Time& elapsedTime)
    {
        if (!gs::isPause)
        {
            system.Update(elapsedTime);
            if (spawnNekos)
            {
                if (currentDifficulty <= 12)
                {
                    harderElapsedTime += elapsedTime.asSeconds();
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
                }
                
                if (attackIsDone)
                {
                    elapsedSeconds += elapsedTime.asSeconds();
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
                            attackingElapsedTime += elapsedTime.asSeconds();
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
                    elapsedCombo += elapsedTime.asSeconds();
                    if (elapsedCombo > comboThreshold)
                    {
                        if (comboCounter > 2)
                        {
                            gui.system.PrioritizeComponent<ComboText>(1, comboCounter, lastNekoPosition.x, lastNekoPosition.y);
                            score += comboCounter * 10 * scoreMultiplier;
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
                    if (!isGameOver && !(*it)->tamed && (*it)->nekoInfo->name != L"Poop") {--lifes; if (lifes <= 0) GameOver();}
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
        gui.Draw(window);
    }
    void Controller::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::LostFocus)
        {
            if (gs::pauseOnFocusLost && !isGameOver) gs::isPause = true;
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::K)
        {
            ApplyDifficulty(++currentDifficulty);
            cout << "Difficulty is now: " << currentDifficulty << endl;
        }
        else if (!gs::isPause && !isGameOver)
        {
            if (event.type == sf::Event::MouseButtonReleased || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
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
        gui.PollEvent(event);
    }
    void Controller::GameOver()
    {
        isGameOver = true;
        spawnNekos = false;
        gs::isPause = false;
        
        Player::self->AddExperience(score*0.1);
        Player::self->SaveData();
        
        gui.GameOver();
        for (unsigned int i = 0; i < nl::neko.size(); ++i)
            if (NekoObject::tamedArray[i]) nl::neko[i].tamed = true;
        Player::self->SaveNekos();
        
        if (score > topScore)
        {
            topScore = score;
            
            if (!base::FileExists(Player::self->scoresPath))
                base::CreateDirectory(base::utf16(documentsPath()));
            
            std::wofstream wof;
#ifdef _WIN32
            wof.open(Player::self->scoresPath);
#else
            wof.open(base::utf8(Player::self->scoresPath));
#endif
            wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            if (wof.is_open()) wof << topScore << endl;
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
        
        currentDifficulty = difficulty;
        nextAttackTime = 1.f;
        nekoScale = 1.f;
        baseAttackTime = 1.2f;
        attackingAreaTime = 0.44f;
        DefaultGamemodeSettings();
        
        
        for (unsigned int i = 0; i < nl::neko.size(); ++i) NekoObject::tamedArray[i] = 0;
        gui.newNekoList.clear();
        for (auto n : nekos) delete n;
        nekos.clear();
        system.clear();
    }
    void Controller::DefaultGamemodeSettings()
    {
        NekoObject::gravity = 0.19f * 60 * 60;
        NekoObject::yStartVelocity = -17.f * 60;
        NekoObject::yStartRandomVelocity = 4*60;
        NekoObject::xStartRandomVelocity = 6*60;
        Controller::scoreMultiplier = 1.f;
    }
    void Controller::ReturnToMenu()
    {
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
        
        Controller::scoreMultiplier *= 1.04;
        switch (diff)
        {
            case 1:
                nekoScale *= 0.9;
                break;
            case 4:
            case 2:
                nekoScale *= 0.95;
                attackingAreaTime *= 0.95;
                baseAttackTime *= 0.93;
                randomAttackTime *= 0.93;
                NekoObject::gravity *= 1.05; NekoObject::gravity *= 1.05;
                NekoObject::yStartVelocity *= 1.05;
                NekoObject::yStartRandomVelocity += 1;
                break;
            case 5:
            case 3:
                baseAttackTime *= 0.94;
                randomAttackTime *= 0.94;
                attackingAreaTime *= 0.96;
                break;
            default:
                nekoScale *= 0.98;
                baseAttackTime *= 0.93;
                randomAttackTime *= 0.93;
                attackingAreaTime *= 0.95;
                NekoObject::gravity *= 1.05; NekoObject::gravity *= 1.05;
                NekoObject::yStartVelocity *= 1.05;
                NekoObject::yStartRandomVelocity += 1;
                break;
        }
    }
}
