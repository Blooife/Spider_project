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

bool move(StackList* a, StackList* b){ //from a to b
    if(b->Check().value - a->Check().value == 1 || b->Check().value == 0){
        b->Push(a->Pop());
        /*if(c.value == 1){
            checkPas(b->pTop);
        }*/
        return true;
    } else{
        return false;
    }
};

void drawStart(sf::RenderWindow &window, Box a){
    for(int i = 0; i < 10; i++){
        NodeStack* temp = a.box[i].bottom;
        while(temp != a.box[i].pTop){
            Card c= temp->item;
            if(!c.visible){
            c.setTexture(c.path);} else{
                c.setTexture(c.m_path);
            }
            c.setPosition(c.posX, c.posY);
            window.draw(c.m_sprite);
            temp = temp->prev;

        }
    }
    for(int i = 0; i < 10; i++){
            Card* c= &a.box[i].pTop->item;
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

#endif //SPIDER_SPIDERLOGIC_H
