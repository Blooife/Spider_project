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
    bool visible;
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

    // конструктор копіювання
   /* StackList(const StackList& SL)
    {
        NodeStack* p; // дополнительные указатели
        NodeStack* p2;
        NodeStack* p3;

        // Инициализировать pTop
        pTop = nullptr;
        p3 = nullptr;

        p = SL.pTop; // указатель p движется по списку SL.pTop->...
        while (p != nullptr)
        {
            // 1. Сформировать узел p2
            try {
                // попытка выделения памяти
                p2 = new NodeStack;
            }
            catch (bad_alloc e)
            {
                // если память не выделена, то выход
                cout << e.what() << endl;
                return;
            }
            p2->item = p->item;
            p2->next = nullptr;

            // 2. pTop = pTop + p2
            if (pTop == nullptr) // создать очередь
            {
                pTop = p2;
                p3 = p2;
            }
            else
            {
                p3->next = p2;
                p3 = p3->next;
            }

            // 3. Перейти на следующий элемент
            p = p->next;
        }
    }*/

    // поместить элемент в стек
    // элемент помещается на начало списка
    void Push(Card item)
    {
        NodeStack* p;

        // 1. Сформировать элемент
        try {
            p = new NodeStack; // попытка выделить память
        }
        catch(bad_alloc e)
        {
            // если память не выделилась, то выход
            cout << e.what() << endl;
            return;
        }
        p->item = item;
        if(pTop != nullptr){
            if(pTop->next == nullptr){
                p->item.posX = pTop->item.posX;
                p->item.posY = pTop->item.posY;
            } else{
            p->item .posX = pTop->item.posX;
            p->item .posY = pTop->item.posY + 20;}
        }
        p->next = pTop; // p указывает на 1-й элемент
        if(pTop == nullptr){
            bottom = p;
        }
        else{
            pTop->prev= p;
        }
        // 2. Перенаправить pTop на p
        pTop = p;

    }

    // Количество элементов в стеке
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

    // очищает стек - удаляет все элементы из стека
    void Empty()
    {
        NodeStack* p; // дополнительный указатель
        NodeStack* p2;

        p = pTop;

        while (!p)
        {
            p2 = p; // сделать копию из p
            p = p->next; // перейти на следующий элемент стека
            delete p2; // удалить память, выделенную для предыдущего элемента
        }
        pTop = nullptr; // поправить вершину стека
    }

    // оператор копирования
    StackList& operator=(const StackList& LS)
    {
        // есть ли элементы в стеке?
        if (pTop != nullptr) Empty();

        NodeStack* p; // дополнительный указатель
        NodeStack* p2;
        NodeStack* p3;

        // Инициализировать pTop
        pTop = nullptr;
        p3 = nullptr;

        p = LS.pTop; // указатель p двигается по списку SL.pTop->...
        while (p != nullptr)
        {
            // 1. Сформировать узел p2
            try {
                // попытка выделить память
                p2 = new NodeStack;
            }
            catch (bad_alloc e)
            {
                // если память не выделена, то выход
                cout << e.what() << endl;
                return *this;
            }
            p2->item = p->item;
            p2->next = nullptr;

            // 2. pTop = pTop + p2
            if (pTop == nullptr) // создать стек
            {
                pTop = p2;
                p3 = p2;
            }
            else
            {
                p3->next = p2;
                p3 = p3->next;
            }

            // 3. Перейти на следующий элемент
            p = p->next;
        }
        return *this;
    }

    // деструктор
    ~StackList()
    {
        Empty();
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
    };

    Card pop(){
        size -= 1;
        return arrCards[size];
    };
};

class Box{
public:
     StackList box[10];
     int size = 54;
     void init(Deck* deck, float x){
         Card c;
         c = c.emptyCard();
         for(int i = 0; i<10; i++){
             box[i].Push(c);
             box[i].pTop->item.posX = x;
             box[i].pTop->item.posY = 30;
             x+=90;
         }
         for(int i = 0; i < 4; i++){
             for(int j = 0; j<6; j++){
                 box[i].Push((*deck).pop());
             }
         }
         for(int i = 4; i < 10; i++){
             for(int j = 0; j<5; j++){
                 box[i].Push((*deck).pop());
             }
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


