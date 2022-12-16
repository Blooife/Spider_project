#include <iostream>
#include "Tile.h"
#include "stdlib.h"

using namespace std;

enum class Suits
{
    SPADES,
    HEARTS,
    CLUBS,
    DIAMONDS,
};

enum class CardNames
{
    ACE = 1,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK = 11,
    QUEEN = 12,
    KING = 13,
};

struct Card : public Tile
{
    CardNames name;
    Suits suit;
    std:: string path = "C:/Users/Asus/Desktop/spider/resource/cards/card_back.bmp";
    int value;
    float posX, posY;
    bool visible = false;
    Card emptyCard(){
        Card c;
        c.value = 0;
        c.visible = true;
        c.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
        c.setTexture(c.m_path);
        return c;
    }
};

struct NodeStack
{
    Card item;
    NodeStack* next, *prev;
};

// Шаблонный класс Стек на базе односвязного списка

class StackList
{
private:


public:
    NodeStack* pTop; // указатель на вершину стека
    NodeStack* bottom;
    // конструктор по умолчанию
    StackList() { pTop = nullptr; bottom = nullptr; }
    // поместить элемент в стек
    // элемент помещается на начало списка
    void Push(Card item)
    {
        NodeStack* p;
        p = new NodeStack;
        p->item = item;
        if(pTop != nullptr){
            p->item .posX = pTop->item.posX;
            if(pTop->next == nullptr){
                p->item.posY = pTop->item.posY;
            } else{
                if(pTop->item.visible){
                    p->item .posY = pTop->item.posY + 20;
                }
                else{
                    p->item .posY = pTop->item.posY + 10;
                }
            }
        }
        p->next = pTop; // p указывает на 1-й элемент
        if(pTop == nullptr){
            bottom = p;
            bottom->next = nullptr;
        }
        else{
            pTop->prev= p;
        }
        // 2. Перенаправить pTop на p
        pTop = p;
        pTop->prev = nullptr;

    }

    int Count()
    {
        if (pTop == nullptr)
            return 0;
        else
        {
            NodeStack* p = pTop;
            int count = 0;

            while (p != nullptr)
            {
                count++;
                p = p->next;
            }
        }
    }
    void Empty()
    {
        NodeStack* p; // дополнительный указатель
        NodeStack* p2;

        p = pTop;

        while (p != nullptr)
        {
            p2 = p; // сделать копию из p
            p = p->next; // перейти на следующий элемент стека
            delete p2; // удалить память, выделенную для предыдущего элемента
        }
       // pTop = nullptr; // поправить вершину стека

    }
    // деструктор
    ~StackList()
    {
        //Empty();
        NodeStack* p; // дополнительный указатель
        NodeStack* p2;

        p = pTop;

        while (p != nullptr)
        {
            p2 = p; // сделать копию из p
            p = p->next; // перейти на следующий элемент стека
            delete p2; // удалить память, выделенную для предыдущего элемента
        }
         pTop = nullptr; // поправить вершину стека
         bottom = nullptr;
    }

    // метод, вытягивающий элемент со стека
    Card Pop()
    {
        // проверка, пуст ли стек?
          if (pTop == nullptr){
             Card c;
              return c.emptyCard();
          }


        NodeStack* p2; // дополнительный указатель
        Card item;
        item = pTop->item;

        // перенаправить указатели pTop, p2
        p2 = pTop;
        pTop = pTop->next;
        if(pTop != nullptr)
        pTop->prev = nullptr;
        // Освободить память, выделенную под 1-й элемент
        delete p2;

        // вернуть item
        return item;
    }

    Card Check(){
        if (pTop == nullptr){
            Card c;
            return c.emptyCard();
        }
        return pTop->item;
    }
};

struct Deck: public Tile
{
    int size = 104;
    Card arrCards[104];
    float posX;
    float posY;

    void SetupCards(int complx)
    {
        int border, times, index=0;
        switch (complx) {
            case 1: {
                border = (int)Suits::SPADES;
                times = 8;
                break;
            }
            case 2: {
                border = (int)Suits::HEARTS;
                times = 4;
                break;
            }
            case 3: {
                border = (int)Suits::DIAMONDS;
                times = 2;
                break;
            }
        }

        for(int i = 1; i<=times; i++){
        for (int col = (int)Suits::SPADES; col <= border; col++)
        {
            for (int row = (int)CardNames::ACE; row <= (int)CardNames::KING; row++)
            {
                Card c;
                c.suit = (Suits)col;
                c.name = (CardNames)row;
                c.value = (int)c.name;
                c.visible = true;
                c.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card" + std::to_string(col)+std::to_string(row)+".bmp";
                c.setTexture(c.m_path);
                arrCards[index] = c;
                index++;
            }
        }
        }
    };

    void shuffle(){
        srand(time(NULL));
        for(int i = 0; i < size; i++){
            int k = rand() % size;
            int l = rand() % size;
            Card temp = arrCards[k];
            arrCards[k] = arrCards[l];
            arrCards[l] = temp;
        }

        for(int i = 103; i>59;i--){
            arrCards[i].visible = false;
        }
    };

    Card pop(){
        size -= 1;
        return arrCards[size];
    };


};

class Box{
public:
     StackList box[10];
     void init(Deck* deck, float x, float cardWidth){
         Card c;
         c = c.emptyCard();
         for(int i = 0; i<10; i++){
             box[i].Push(c);
             box[i].pTop->item.posX = x;
             box[i].pTop->item.posY = 30;
             x+=cardWidth;
         }
         for(int i = 0; i < 10; i++){
             for(int j = 0; j<4; j++){
                  c = deck->pop();
                 box[i].Push(c);
               //  box[i].pTop->item.visible = false;
             }
         }
         for(int i = 0; i < 4; i++){
                 box[i].Push((*deck).pop());
         }
         for(int i = 0; i < 10; i++){
             box[i].Push((*deck).pop());
         }
     }
    Box& operator=(const Box* a)
    {
        for(int i =0; i<10; i++){
            this->box[i].pTop = a->box[i].pTop;
            this->box[i].bottom = a->box[i].bottom;
        }
        return *this;
    }
};






