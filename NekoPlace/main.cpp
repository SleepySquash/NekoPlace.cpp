
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

// TODO: Current game's timer

#ifdef __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE
        #include <SFML/Main.hpp>
    #endif
#endif

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "Essentials/ResourcePath.hpp"
#include "Essentials/Base.hpp"
#include "Engine/StaticMethods.hpp"
#include "Engine/EntitySystem.hpp"

#include "Components/EssentialComponents.hpp"
#include "Components/NekoNinjaComponents/NekoNinja.hpp"
#include "Components/NekoNinjaComponents/NekoMenu.hpp"

using std::cout;
using std::cin;
using std::endl;

using namespace ns;



void CalculateScaleRatios(unsigned int width, unsigned int height)
{
    float factorX = (float)width / gs::relativeWidth; gs::scalex = factorX;
    float factorY = (float)height / gs::relativeHeight; gs::scaley = factorY;
    
    float ratioFactorX = (float)width/(float)height;
    float ratioFactorY = (float)height/(float)width;
    
    gs::scale = factorX > factorY ? factorX : factorY;
    gs::scScale = gs::scale;
    if (ratioFactorY > 1)
    {
        float m = gs::scale;
        if (ratioFactorY < 1.2)
            gs::scale = gs::scale - m*(ratioFactorY - 1)*1.5;
        else if (ratioFactorY < 2)
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(ratioFactorY - 1.2)*0.17;
        else if (ratioFactorY < 2.46)
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(2 - 1.2)*0.17 - m*(ratioFactorY - 2)*0.12;
        else if (ratioFactorY < 3)
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(ratioFactorY - 2.46)*0.07;
        else if (ratioFactorY < 4.8)
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(ratioFactorY - 3)*0.04;
        else if (ratioFactorY < 8)
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(4.8 - 3)*0.04 - m*(ratioFactorY - 4.8)*0.02;
        else
            gs::scale = gs::scale - m*(1.2 - 1)*1.5 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(4.8 - 3)*0.04 - m*(8 - 4.8)*0.02;
        
        m = gs::scScale;
        if (ratioFactorY < 1.2)
            gs::scScale = gs::scScale - m*(ratioFactorY - 1)*0.72;
        else if (ratioFactorY < 2)
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(ratioFactorY - 1.2)*0.17;
        else if (ratioFactorY < 2.46)
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(2 - 1.2)*0.17 - m*(ratioFactorY - 2)*0.12;
        else if (ratioFactorY < 3)
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(ratioFactorY - 2.46)*0.07;
        else if (ratioFactorY < 4.8)
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(ratioFactorY - 3)*0.04;
        else if (ratioFactorY < 8)
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(4.8 - 3)*0.04 - m*(ratioFactorY - 4.8)*0.02;
        else
            gs::scScale = gs::scScale - m*(1.2 - 1)*0.72 - m*(2 - 1.2)*0.17 - m*(2.46 - 2)*0.12 - m*(3 - 2.46)*0.07 - m*(4.8 - 3)*0.04 - m*(8 - 4.8)*0.02;
    }
    else if (ratioFactorX > 1)
    {
        float m = gs::scale;
        if (ratioFactorX < 1.9)
            gs::scale = gs::scale - m*(ratioFactorX - 1)*0.175;
        else if (ratioFactorX < 3)
            gs::scale = gs::scale - m*(1.9 - 1)*0.175 - m*(ratioFactorX - 1.9)*0.27;
        else if (ratioFactorX < 4.24)
            gs::scale = gs::scale - m*(1.9 - 1)*0.175 - m*(3 - 1.9)*0.27 - m*(ratioFactorX - 3)*0.15;
        else if (ratioFactorX < 7)
            gs::scale = gs::scale - m*(1.9 - 1)*0.175 - m*(3 - 1.9)*0.27 - m*(4.24 - 3)*0.15 - m*(ratioFactorX - 4.24)*0.05;
        else if (ratioFactorX < 11)
            gs::scale = gs::scale - m*(1.9 - 1)*0.175 - m*(3 - 1.9)*0.27 - m*(4.24 - 3)*0.15 - m*(7 - 4.24)*0.05 - m*(ratioFactorX - 7)*0.02;
        else
            gs::scale = gs::scale - m*(1.9 - 1)*0.175 - m*(3 - 1.9)*0.27 - m*(4.24 - 3)*0.15 - m*(7 - 4.24)*0.05 - m*(11 - 7)*0.02;
    }
    if (gs::scale < 0.18) gs::scale = 0.18;
}



void SetResolutionClass()
{
    if (gs::resolutionClassSetting == -1)
    {
        unsigned int width;
        unsigned int height;
        
        if (sf::VideoMode::getDesktopMode().width >= sf::VideoMode::getDesktopMode().height)
        {
            width = sf::VideoMode::getDesktopMode().width;
            height = sf::VideoMode::getDesktopMode().height;
        }
        else
        {
            width = sf::VideoMode::getDesktopMode().height;
            height = sf::VideoMode::getDesktopMode().width;
        }
        
        if (width <= 640 && height <= 480)
            gs::resolutionClass = 0;
        else if (width <= 1024 && height <= 768)
            gs::resolutionClass = 1;
        else if (width <= 1920 && height <= 1080)
            gs::resolutionClass = 2;
        else
            gs::resolutionClass = 3;
        
#ifdef SFML_SYSTEM_IOS
        std::string device = iOSDeviceName();
        if (nss::Command(device, "iPhone"))
        {
            int version = std::atoi(nss::ParseUntil(device, ',', 6).c_str());
            if (version <= 4)
                gs::resolutionClass = 0;
            else if (version <= 7)
                gs::resolutionClass = 1;
            else if (version == 8)
            {
                int underVersion = std::atoi(nss::ParseUntil(device, '\0', 8).c_str());
                if (underVersion == 4)
                    gs::resolutionClass = 1;
                else
                    gs::resolutionClass = 2;
            }
        }
        else if (nss::Command(device, "iPad"))
        {
            int version = std::atoi(nss::ParseUntil(device, ',', 4).c_str());
            if (version <= 2)
                gs::resolutionClass = 0;
        }
        else if (nss::Command(device, "iPod"))
        {
            int version = std::atoi(nss::ParseUntil(device, ',', 4).c_str());
            if (version <= 3)
                gs::resolutionClass = 0;
            else if (version <= 4)
                gs::resolutionClass = 1;
            else if (version <= 6)
                gs::resolutionClass = 1;
        }
#endif
        
        cout << "main :: GlobalSettings :: Autocomputed ResolutionClass is " << gs::resolutionClass << "@x." << endl;
    }
    else
        gs::resolutionClass = gs::resolutionClassSetting;
}



int main()
{
#ifdef _WIN32
    std::system("chcp 1251");
#endif
    
    sf::ContextSettings settings;
    settings.antialiasingLevel = 0;
#ifdef SFML_SYSTEM_IOS
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "NekoPlace", sf::Style::Default);
#else
    #ifdef SFML_SYSTEM_ANDROID
        sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "NekoPlace", sf::Style::Default);
    #else
        sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width >= 1280 ? 1280 : sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height >= 880 ? 800 : sf::VideoMode::getDesktopMode().height - 80), "NekoPlace", sf::Style::Default, settings);
    #endif
#endif
    gs::window = &window;
    gs::width = window.getSize().x;
    gs::height = window.getSize().y;
    
    gs::relativeWidth = 1280;
    gs::relativeHeight = 800;
    CalculateScaleRatios(gs::width, gs::height);
    SetResolutionClass();
    
    window.setFramerateLimit(gs::framerateLimit);
    window.setVerticalSyncEnabled(gs::isVerticalSyncEnabled);
    
#ifdef _WIN32
    sf::Image icon;
    if (icon.loadFromFile(resourcePath() + "Data/Images/icon.jpg"))
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
#endif

    
    
    
    EntitySystem system;
    srand((unsigned int)time(nullptr));
    
    ///----------------------------------------------------------
    /// \brief Entity to hold Neko Ninja components
    ///
    /// Entity holds components like Neko Ninja controller, its background etc.
    ///
    ///----------------------------------------------------------
    Entity* Chocola = system.AddEntity();
    {
        Chocola->AddComponent<NekoNinja::MainMenu>();
    }
    
    ///----------------------------------------------------------
    /// \brief Entity to hold essential components
    ///
    /// Entity holds components like always-on debug UI layer, system's components and other essential stuff.
    /// It also may hold components like NovelComponent that runs the novel, cuz it has to be essential component.
    ///
    ///----------------------------------------------------------
    Entity* Shimakaze = system.AddEntity();
    {
        Shimakaze->AddComponent<EssentialComponents::DebugComponent>("Update 0 build 4");
    }
    
    bool displayWindow{ true };
    sf::Clock clock;
    window.setActive();
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed: window.close(); break;
                case sf::Event::GainedFocus: displayWindow = true; window.setFramerateLimit(gs::framerateLimit); break;
                case sf::Event::LostFocus: displayWindow = false; system.PollEvent(event); window.setFramerateLimit(gs::framerateNoFocus); break;
                    
                case sf::Event::TouchEnded:
                case sf::Event::TouchMoved:
                case sf::Event::TouchBegan:
                case sf::Event::MouseButtonReleased:
                case sf::Event::MouseMoved:
                case sf::Event::MouseButtonPressed:
                    system.PollEvent(event);
                    break;
                    
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Key::K: system.PollEvent(event); break;
                        default: break;
                    }
                    break;
                    
                case sf::Event::Resized:
                    gs::width = event.size.width;
                    gs::height = event.size.height;
                    CalculateScaleRatios(event.size.width, event.size.height);
                    system.Resize(event.size.width, event.size.height);
                    window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                    break;
                    
                default: break;
            }
        }
        
#ifdef SFML_SYSTEM_IOS
        if (displayWindow)
        {
            system.Update(clock.restart());
            
            window.clear();
            system.Draw(&window);
            window.display(); //TODO: Might crash there if app is not running
        }
#else
        system.Update(clock.restart());
        
        window.clear();
        system.Draw(&window);
        window.display();
#endif
    }
    
    system.clear();
    return EXIT_SUCCESS;
}
