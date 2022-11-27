//
// Created by Asus on 25.11.2022.
//

#ifndef SPIDER_SPIDERLOGIC_H
#define SPIDER_SPIDERLOGIC_H
#include "iostream"
#include "Cards.cpp"

bool dealt(Box* a, Deck* deck){
    if((*deck).size>0){
        for(int i = 0;i<10; i++){
            a->box[i].Push((*deck).pop());
        }
        if((*deck).size<10){
            deck->m_path="C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
        }
        return true;
    } else{

        return false;
    }
};

bool checkPas(NodeStack* pTop){
    NodeStack*  temp = pTop;
    Card c;
    int i = 0;
    while (temp->next->item.value != 0 && i < 13)
        if(temp->next->item.value - temp->item.value == 1 && temp->item.suit==temp->next->item.suit){
            i++;
        }
    if(i == 12){
        return true;
    } else{
        return false;
    }
};
NodeStack* del(NodeStack* a){
    if(a->prev == nullptr){
        a->next->prev = nullptr;
//        delete a;
        return nullptr;
    }else
    if(a->next == nullptr){
        a->prev->next = nullptr;
    //    delete a;
        return nullptr;
    } else{
        NodeStack* buf = a->next;
        a->prev->next = a->next;
     //   delete buf;
         return buf;
    }
}
bool move(NodeStack* col, StackList* a, StackList* b){ //from a to b
    StackList temp;
    if(b->Check().value - col->item.value == 1 || b->Check().value == 0){
    while (a->pTop != col){
        temp.Push(a->Pop());
    };
    temp.Push(a->Pop());

        while (temp.pTop){
            b->Push(temp.Pop());
  //          col->next->prev = col->prev;
//            col = col->prev;
           // delete col;
        }
        return true;
    } else{
        return false;
    }
};

void drawStart(sf::RenderWindow &window, Box a){
        for (int i = 0; i < 10; i++) {
            NodeStack *temp = a.box[i].bottom;
            while (temp != a.box[i].pTop) {
                Card c = temp->item;
                if (!c.visible) {
                    c.setTexture(c.path);
                } else {
                    c.setTexture(c.m_path);
                }
                c.setPosition(c.posX, c.posY);
                window.draw(c.m_sprite);
                temp = temp->prev;

            }
        }
        for (int i = 0; i < 10; i++) {
            Card *c = &a.box[i].pTop->item;
            (*c).visible = true;
            (*c).setTexture((*c).m_path);
            (*c).setPosition((*c).posX, (*c).posY);
            window.draw((*c).m_sprite);
        }
};
Tile chosen(sf::RenderWindow &window, Card* a){
    Tile up;
    up.m_path = "C:/Users/Asus/Desktop/spider/resource/chosen1.png";
    up.setPosition(a->posX-2, a->posY);
    up.setTexture(up.m_path);
    up.m_sprite.setColor(sf::Color(0,0,0,100));
    return up;
}
bool clickInRange(sf::Event::MouseButtonEvent event, sf::IntRect rect)
{
    return rect.contains(event.x, event.y);
}

bool releasedInRange(sf::Vector2i  pixelPos, sf::IntRect rect)
{
    return rect.contains(pixelPos.x, pixelPos.y);
}

bool canDrag(NodeStack* col){

    while (col->prev){
        if(!(col->item.suit == col->prev->item.suit && col->item.value - col->prev->item.value == 1)){
            return false;
        }
        col = col->prev;
    }
    return true;
}

int getColomn(sf::Vector2i  pixelPos, float x, float cardWidth){
    for(int i = 0; i<10; i++){
        if(pixelPos.x >= x-10 && pixelPos.x <= x + 95){
            return i;
        }
        x += cardWidth;
    }
    return -1;
}

#endif //SPIDER_SPIDERLOGIC_H
