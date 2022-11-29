//
// Created by Asus on 25.11.2022.
//

#ifndef SPIDER_SPIDERLOGIC_H
#define SPIDER_SPIDERLOGIC_H
#include "iostream"
#include "Cards.cpp"

bool checkPas(StackList* b){
    NodeStack*  temp = b->pTop;
    Card c;
    int i = 0;
    while (temp->next->item.visible  && i < 12) {
        if (temp->next->item.value - temp->item.value == 1 && temp->item.suit == temp->next->item.suit) {
            i++;
        } else {
            break;
        }
        temp = temp->next;
    }
    if(i == 12){
        for(int i = 0; i < 13; i++){
            b->Pop();
        }
        b->pTop->item.visible = true;
        return true;
    } else{
        return false;
    }
};

bool dealt(Box* a, Deck* deck){
    if((*deck).size>0){
        for(int i = 0;i<10; i++){
            Card c = (*deck).pop();
            a->box[i].Push(c);
            if(c.value == 1){
                checkPas(&a->box[i]);
            }
        }
        if((*deck).size<10){
            deck->m_path="C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
        }
        return true;
    } else{

        return false;
    }
};

bool move(NodeStack* col, StackList* a, StackList* b){ //from a to b
    StackList temp;
    if(b->Check().value - col->item.value == 1 || b->Check().value == 0){
    while (a->pTop != col){
        temp.Push(a->Pop());
    };
    temp.Push(a->Pop());
    a->pTop->item.visible = true;

        while (temp.pTop){
            b->Push(temp.Pop());
        }
        temp.pTop = nullptr;
        if(b->pTop->item.value == 1) {
            checkPas(b);
        }
        return true;
    } else{
        return false;
    }
};

void drawStart(sf::RenderWindow &window, Box* a, int chBox){
        NodeStack *temp;
        for (int i = 0; i < chBox; i++) {
           temp = (*a).box[i].bottom;
            while (temp != nullptr) {
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
        for (int i = chBox+1; i < 10; i++) {
            temp = (*a).box[i].bottom;
            while (temp != nullptr) {
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
        if(chBox != -1) {
            temp = (*a).box[chBox].bottom;
            while (temp != nullptr) {
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
        /*if(col){
            col->item.setPosition(col->item.posX, col->item.posY);
            col->item.setTexture(col->item.m_path);
            window.draw(col->item.m_sprite);
        }*/

       /* for (int i = 0; i < 10; i++) {
            Card *c = &a.box[i].pTop->item;
            (*c).visible = true;
            (*c).setTexture((*c).m_path);
            (*c).setPosition((*c).posX, (*c).posY);
            window.draw((*c).m_sprite);
        }*/
};
 void setColorIfCardChosen(Tile *up, Tile *down, NodeStack* col, float cardHeight){
     (*up).setPosition(col->item.posX-2, col->item.posY-2);
     (*up).setTexture((*up).m_path);
    // (*up).m_sprite.setColor(sf::Color(0,0,0,100));
     (*up).m_sprite.setColor(sf::Color(254,254,34,200));
     float y=0;
     NodeStack* temp = col;
     while (temp->prev){
         y+=20;
     }
     (*down).setPosition(col->item.posX-2, col->item.posY+20+y+cardHeight/2);
     (*down).setTexture((*down).m_path);
     (*down).m_sprite.setColor(sf::Color(0,0,0,100));
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
    if(col->item.value == 0){
        return false;
    }
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
