//
//  List.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef List_hpp
#define List_hpp

namespace ns
{
    template<typename T> struct StackElement
    {
        T data;
        StackElement<T>* next;
        
        StackElement(const T& v, StackElement<T>* n) : data(v), next(n) { }
    };
    template<typename T> struct Stack
    {
        StackElement<T>* head{ nullptr };
        unsigned int size{ 0 };
        
        ~Stack()
        {
            while (head != nullptr)
            {
                StackElement<T>* next = head->next;
                delete head;
                head = next;
            }
        }
        void Push(const T& value)
        {
            ++size;
            head = new StackElement<T>(value, head);
        }
        T Pop()
        {
            if (head != nullptr)
            {
                --size;
                T value = head->data;
                StackElement<T>* next = head->next;
                
                delete head;
                head = next;
                
                return value;
            }
            return T();
        }
        void Clear()
        {
            while (head != nullptr)
            {
                StackElement<T>* next = head->next;
                delete head;
                head = next;
            }
            head = nullptr;
            size = 0;
        }
        bool IsEmpty()
        {
            return (size == 0 || head == nullptr);
        }
        T operator[](int i)
        {
            if (head != nullptr)
            {
                if (i > 0)
                {
                    StackElement<T>* ret = head;
                    for (int k = 0; k < i && ret->next != nullptr; ++k)
                        ret = ret->next;
                    
                    return ret->data;
                }
                else if (i == 0)
                    return head->data;
            }
            
            return T();
        }
    };
}

#endif /* List_hpp */
