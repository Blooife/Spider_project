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
    Suits getsuit(){
        return suit;
    }
    int getvalue(){
        return value;
    }
    void makeVisible(){
        visible = true;
    };
    void makeUnvisible(){
        visible = false;
    };
    Card emptyCard(){
        Card c;
        c.value = 0;
        c.visible = true;
        c.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
        c.setTexture(c.m_path);
        return c;
    }
    void PrintCard()
    {
        PrintValue();

        std::cout << " of ";

        PrintSuit();

        std::cout << std::endl;
    }

    void PrintValue()
    {
        if (name == CardNames::JACK)
        {
            std::cout << "jack";
        }
        else if (name == CardNames::QUEEN)
        {
            std::cout << "queen";
        }
        else if (name == CardNames::KING)
        {
            std::cout << "king";
        }
        else if (name == CardNames::ACE)
        {
            std::cout << "ace";
        }
        else {
            std::cout << value;
        }
    }

    void PrintSuit()
    {
        if (suit == Suits::CLUBS)
        {
            std::cout << "clubs";
        }
        else if (suit == Suits::DIAMONDS)
        {
            std::cout << "diamonds";
        }
        else if (suit == Suits::HEARTS)
        {
            std::cout << "hearts";
        }
        else if (suit == Suits::SPADES)
        {
            std::cout << "spades";
        }
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
                    p->item .posY = pTop->item.posY + 15;
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
        pTop = nullptr; // поправить вершину стека
    }
    // деструктор
    ~StackList()
    {
        //Empty();
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
    void PrintAll()
    {
        for (int index = 0; index<size; index++)
        {
            std::cout << index<<" ";
            arrCards[index].PrintCard();
        }
    }

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

    void draw(sf::RenderWindow window){
         float x = 100;
         float y = 100;
         float distX = window.getSize().x % 900;
         float distY = window.getSize().y % 900;
        for(int i = 0; i<10; i++){
            box[i].pTop->item.setPosition(x,y);
            window.draw(box[i].pTop->item.m_sprite);
        }
    };
};


