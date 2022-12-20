//
// Created by Asus on 25.11.2022.
//

#ifndef SPIDER_SPIDERLOGIC_H
#define SPIDER_SPIDERLOGIC_H
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
        while (temp.pTop){
            b->Push(temp.Pop());
        }
        temp.pTop = nullptr;
        return true;
    } else{
        return false;
    }
};

bool canMove(NodeStack col, StackList* b){
    if(b->pTop->item.value - col.item.value==1){
        return true;
    } if(b->pTop->item.value == 0){
        return true;
    }
    return false;
}

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
};
 sf::ConvexShape setColorIfCardChosen(NodeStack* col, float width, float height){

     sf::ConvexShape convex;
     convex.setPointCount(4);
     float startx = col->item.posX, starty = col->item.posY;

     convex.setOutlineThickness(3.f);
     float y=0;
     NodeStack* temp = col;
     while (temp->prev){
         y+=20;
         temp = temp->prev;
     }
     y = y +height;
     convex.setPoint(0, sf::Vector2f(startx, starty));
     convex.setPoint(1, sf::Vector2f(startx+width, starty));
     convex.setPoint(2, sf::Vector2f(startx+width, starty+y));
     convex.setPoint(3, sf::Vector2f(startx, starty+y));
     convex.setFillColor(sf::Color(0,0,0,0));
     convex.setOutlineColor(sf::Color(0,0,0,100));
     return convex;
 }
bool clickInRange(sf::Event::MouseButtonEvent event, sf::IntRect rect)
{
    return rect.contains(event.x, event.y);
}

bool releasedInRange(sf::Vector2i  pixelPos, sf::IntRect rect)
{
    return rect.contains(pixelPos.x, pixelPos.y);
}

bool canDrag(NodeStack col){
    if(col.item.value == 0){
        return false;
    }
    while (col.prev){
        if(!(col.item.suit == col.prev->item.suit && col.item.value - col.prev->item.value == 1)){
            return false;
        }
        col = *col.prev;
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

bool hint(Box *a, StackList* res, int &j){
     NodeStack temp;
     for(int i =0; i<10; i++){
         temp = *a->box[i].pTop;
         if(temp.item.value != 0){
             while (temp.item.visible && canDrag(temp)){
                 temp = *temp.next;
             }
             temp = *temp.prev;
             for(j = 0; j < 10; j++){
                 if(i==j){
                     continue;
                 }
                 bool isAbleToMove = canMove(temp, &a->box[j]);
                 if (isAbleToMove) {
                     temp = *temp.next;
                     while (temp.prev) {
                         res->Push(temp.prev->item);
                         temp = *temp.prev;
                     }
                     return true;
                 }
             }
         }
    }

    return false;
 }


 void drawCollected(sf::RenderWindow &window, Tile* collected){

 for(int i=0; i<8; i++){
     if(!collected[i].m_path.empty()){
         collected[i].setTexture(collected[i].m_path);
        window.draw(collected[i].m_sprite);
     }
 }
 };

 void saveCurCond(Box a, StackList* prev){
     for(int j = 0; j<10; j++){
         NodeStack* temp = a.box[j].bottom;
         NodeStack* pr = prev[j].bottom;
         if(a.box[j].Count() < prev[j].Count()){
             int k = - a.box[j].Count() + prev[j].Count();
             while(k > 0){
                 prev[j].Pop();
                 k--;
             }
         }
         while (temp){
             if(pr){
                 pr->item = temp->item;
                 pr = pr->prev;
             }else{
                 prev[j].Push(temp->item);
             }
             temp = temp->prev;
         }
     }
 }

 void saveToFileCollected(Tile collected[8],int size){
     FILE* file;
     file = fopen("C:\\Users\\Asus\\Desktop\\spider\\someInfo", "w");
     fprintf(file, "%d\n", size);
     for(int i = 0; i<size; i++){
         fprintf(file,"%s\n", collected[i].m_path.c_str());
     }
     fclose(file);
 }

int readCollected(Tile collected[8]){
     FILE* file;
     file = fopen("C:\\Users\\Asus\\Desktop\\spider\\someInfo", "r");
     if(file){
         int size;
         fscanf(file, "%d\n", &size);
         for(int i = 0; i<size; i++){
             char s[200];
             fscanf(file, "%s\n", &s);
             collected[i].m_path = s;
         }
         fclose(file);
         return size;
     } else return -1;
 }

 void saveToFileBox(Box* a){
     FILE* box;
     box = fopen("C:\\Users\\Asus\\Desktop\\spider\\box.txt", "w");
     if(box)
     for(int i = 0; i < 10; i++){
         NodeStack * temp = a->box[i].bottom->prev;
         fprintf(box, "%d\n", a->box[i].Count());
         fprintf(box, "%f\n", a->box[i].bottom->item.posX);
         fprintf(box, "%f\n", a->box[i].bottom->item.posY);
         while(temp){
             fprintf(box, "%d\n", temp->item.value);
             fprintf(box, "%d\n", (CardNames)temp->item.name);
             fprintf(box, "%d\n%d\n", (int)temp->item.visible, (Suits)temp->item.suit);
             temp = temp->prev;
            // a->box[i].Pop();
         }
     }
     fclose(box);
 }

 void saveToFileDeck(Deck* deck){
     FILE* fdeck;
     fdeck = fopen("C:\\Users\\Asus\\Desktop\\spider\\deck.txt", "w");
     if(fdeck)
     {
         fprintf(fdeck, "%d\n", deck->size);
         int i = deck->size-1;
         while (i >= 0){
             fprintf(fdeck, "%d\n", deck->arrCards[i].value);
             fprintf(fdeck, "%d\n", (CardNames)deck->arrCards[i].name);
             fprintf(fdeck, "%d\n%d\n", (int)deck->arrCards[i].visible, (Suits)deck->arrCards[i].suit);
             i--;
         }
     }
 }

void readDeck(Deck* deck){
    FILE* fdeck;
    fdeck = fopen("C:\\Users\\Asus\\Desktop\\spider\\deck.txt", "r");
    if(fdeck)
    {
        fscanf(fdeck, "%d\n", &deck->size);
        int i = deck->size-1;
        while (i >= 0){
            fscanf(fdeck, "%d\n", &deck->arrCards[i].value);
            fscanf(fdeck, "%d\n", &deck->arrCards[i].name);
            fscanf(fdeck, "%d\n%d\n", &deck->arrCards[i].visible, &deck->arrCards[i].suit);
            deck->arrCards[i].m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card" +
                    std::to_string((int)deck->arrCards[i].suit)+std::to_string((int)deck->arrCards[i].name)+".bmp";
            deck->arrCards[i].setTexture(deck->arrCards[i].m_path);
            i--;
        }
    }
}
 void readBox(Box* a){
     FILE* box;
     box = fopen("C:\\Users\\Asus\\Desktop\\spider\\box.txt", "r");
     auto * temp = new NodeStack;
     temp->item.path = "C:/Users/Asus/Desktop/spider/resource/cards/card_back.bmp";
     for(int i = 0; i < 10; i++){
         int count;
         fscanf(box, "%d", &count);
         count--;
         Card c = c.emptyCard();
         fscanf(box, "%f", &c.posX);
         fscanf(box, "%f", &c.posY);
         a->box[i].Push(c);
         for(count; count > 0; count --){
             fscanf(box, "%d", &temp->item.value);
             fscanf(box, "%d",  &temp->item.name);
             fscanf(box, "%d", &temp->item.visible);
             fscanf(box, "%d", &temp->item.suit);
             temp->item.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card" + std::to_string((int)temp->item.suit)+std::to_string((int)temp->item.name)+".bmp";
             a->box[i].Push(temp->item);
         }
     }
     fclose(box);
 }

#endif //SPIDER_SPIDERLOGIC_H
