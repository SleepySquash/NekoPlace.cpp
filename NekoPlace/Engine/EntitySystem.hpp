//
//  EntitySystem.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef EntitySystem_hpp
#define EntitySystem_hpp

#include <iostream>
#include <list>
using std::list;

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "StaticMethods.hpp"

namespace ns
{
    struct Component;
    struct Entity;
    struct EntitySystem;
    
    struct MessageHolder
    {
        std::string info;
        std::wstring path;
        
        MessageHolder();
        MessageHolder(const std::string& info, const std::wstring& path = L"") : info(info), path(path) { }
    };
    
    struct Component
    {
        Entity* entity{ nullptr };
        bool offline{ false };
        int priority{ 0 };
        
        virtual ~Component();
        virtual void Init();
        virtual void Update(const sf::Time&);
        virtual void Draw(sf::RenderWindow*);
        virtual void Resize(unsigned int width, unsigned int height);
        virtual void PollEvent(sf::Event& event);
        virtual void RecieveMessage(MessageHolder& message);
        virtual void Destroy();
        void SetPriority(int priority);
        void SetEntity(Entity* entity);
        Entity* GetEntity();
    };
    
    struct Entity
    {
        EntitySystem* system{ nullptr };
        bool offline{ false };
        list<Component*> components;
        
        Entity();
        Entity(EntitySystem* system);
        void Update(const sf::Time& elapsedTime);
        void Draw(sf::RenderWindow* window);
        void Resize(unsigned int width, unsigned int height);
        void PollEvent(sf::Event& event);
        void PopComponent(Component* component);
        void SendMessage(MessageHolder message);
        void RecieveMessage(MessageHolder& message);
        void Destroy();
        void SetEntitySystem(EntitySystem* system);
        template<typename T, typename ...Args> T* AddComponent(Args... args)
        {
            T* component = new T(args...);
            components.push_back(component);
            
            component->SetEntity(this);
            component->Init();
            component->Resize(gs::width, gs::height);
            
            return component;
        }
        template<typename T, typename ...Args> T* PrioritizeComponent(int priority, Args... args)
        {
            T* component = new T(args...);
            component->priority = priority;
            
            bool done{ false };
            for (list<Component*>::iterator it = components.begin(); it != components.end() && !done; ++it)
                if ((*it)->priority > priority)
                {
                    components.insert(it, component);
                    done = true;
                }
            if (!done)
                components.push_back(component);
            
            component->SetEntity(this);
            component->Init();
            component->Resize(gs::width, gs::height);
            
            return component;
        }
    };
    
    struct EntitySystem
    {
        list<Entity*> entities;
        
        EntitySystem();
        void Update(const sf::Time& elapsedTime);
        void Draw(sf::RenderWindow* window);
        void Resize(unsigned int width, unsigned int height);
        void PollEvent(sf::Event& event);
        Entity* AddEntity();
        void PopEntity(Entity* entity);
        void SendMessage(MessageHolder message);
        void clear();
    };
}

#endif /* EntitySystem_hpp */
