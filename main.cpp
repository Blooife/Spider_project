#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "SpiderLogic.h"

#define cardWidth a.box[0].Check().m_texture.getSize().x
#define cardHeight a.box[0].Check().m_texture.getSize().y

using namespace sf;

bool startGame(RenderWindow &window, int menuNum){

    Deck deck;
    deck.SetupCards(menuNum);
    deck.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card_back.bmp";
    deck.setTexture(deck.m_path);
    deck.posX = window.getSize().x-150;
    deck.posY = window.getSize().y-150;
    deck.setPosition(deck.posX,deck.posY);
    deck.setTexture(deck.m_path);
    Sound soundDealt;
    Sound soundPlace;
    Sound soundSlide;
    sf::SoundBuffer bufferDealt;
    bufferDealt.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/dealt.ogg");
    soundDealt.setBuffer(bufferDealt);
    sf::SoundBuffer bufferPlace;
    bufferPlace.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/cardPlace.ogg");
    soundPlace.setBuffer(bufferPlace);
    sf::SoundBuffer bufferSlide;
    bufferSlide.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/cardSlide.ogg");
    soundSlide.setBuffer(bufferSlide);
    Tile hintTile;
    hintTile.m_path = "C:/Users/Asus/Desktop/spider/resource/hint.png";
    hintTile.setTexture(hintTile.m_path);
    hintTile.setPosition(window.getSize().x-50, window.getSize().y-300);
    Tile restartTile;
    restartTile.m_path = "C:/Users/Asus/Desktop/spider/resource/icons/refresh1.bmp";
    restartTile.setTexture(restartTile.m_path);
    restartTile.setPosition(window.getSize().x-45, window.getSize().y-250);
    deck.shuffle();
    Box a;
    a.init(&deck, window.getSize().x % 10 + 50, cardWidth+20);
    Tile m_background;
    m_background.setTexture("C:/Users/Asus/Desktop/spider/resource/backgrounds/back_img.jpg");
    sf::ConvexShape convex;
    convex.setPointCount(4);
    convex.setFillColor(sf::Color(0,0,0,0));
    convex.setOutlineThickness(0);
    convex.setOutlineColor(sf::Color(0,0,0,0));
    bool isMove = false;
    bool Chosen = false;
    int chBox=-1;
    bool hintAnim=false;
    while (window.isOpen()) {
        sf::Event event;
        int dX;
        int dY;
        //soundDealt.play();
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            } else{
                NodeStack* col;
                StackList hintNode;
                int hintTo;
                if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-45, window.getSize().y-250, 60,60))){
                    return true;
                }
                if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
                    if (event.key.code == Mouse::Left) {
                        sf::Vector2i clickPos = sf::Mouse::getPosition(window);
                        int i = getColomn(clickPos,window.getSize().x % 10 + 50, cardWidth+20);
                        if(i> -1) {
                            col = a.box[i].pTop;
                            if(col->item.value == 0){
                                int k = 0;
                            }
                            while (col && col->item.visible){
                                if (clickInRange(event.mouseButton,
                                                 sf::IntRect(col->item.posX, col->item.posY, 75,
                                                             115))) {
                                    if(canDrag(*col)){
                                        //setColorIfCardChosen(&chTile, col);
                                        convex = setColorIfCardChosen(col, cardWidth, cardHeight);
                                        Chosen = true;
                                        soundSlide.play();
                                        chBox = i;
                                        dX = col->item.posX;
                                        dY = col->item.posY;
                                        isMove = true;
                                        break;
                                    } else {
                                        Chosen = false;
                                        convex.setOutlineThickness(0);
                                    }

                                } else {
                                    Chosen = false;
                                    convex.setOutlineThickness(0);
                                }
                                col = col->next;
                            }
                        } else chBox = -1;
                        if  (clickInRange(event.mouseButton, sf::IntRect(deck.posX, deck.posY, 75, 115))) {
                            if(deck.size>0){
                                soundDealt.play();
                                for(int i = 0;i<10; i++){
                                    Card c = (deck).pop();
                                    c.posX = deck.posX;
                                    c.posY = deck.posY;
                                    float borderX, borderY , To;
                                    To = 2;
                                    borderX = c.posX - a.box[i].pTop->item.posX;
                                    if(c.posX > a.box[i].pTop->item.posX){
                                        borderX = -borderX/To;
                                    } else{
                                        borderX = borderX/To;
                                    }
                                    borderY = (a.box[i].pTop->item.posY+20 - c.posY) / To;
                                    for(int j = 0; j < To; j++){
                                        c.posX += borderX;
                                        c.posY +=borderY;
                                        window.clear();
                                        window.draw(m_background);
                                        window.draw(deck.m_sprite);
                                        window.draw(hintTile);
                                        window.draw(restartTile);
                                        drawStart(window,&a,0);
                                        c.setTexture(c.m_path);
                                        c.setPosition(c.posX,c.posY);
                                        window.draw(c);
                                        window.display();
                                    }
                                    a.box[i].Push(c);
                                    if(c.value == 1){
                                        checkPas(&a.box[i]);
                                    }
                                }
                                if((deck).size<10){
                                    deck.m_path="C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
                                    deck.setTexture(deck.m_path);
                                }

                            }
                        }
                        if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-70, window.getSize().y-310,
                                                                       60, 60))){
                            hintTile.setColor(Color(255,255,255,120));
                            hintAnim = hint(&a, window,&hintNode, hintTo);
                            if(!hintAnim){
                                if(deck.size>0){
                                    float startx = deck.posX, starty = deck.posY;
                                    convex.setOutlineThickness(3.f);
                                    convex.setPoint(0, sf::Vector2f(startx, starty));
                                    convex.setPoint(1, sf::Vector2f(startx+cardWidth, starty));
                                    convex.setPoint(2, sf::Vector2f(startx+cardWidth, starty+cardHeight));
                                    convex.setPoint(3, sf::Vector2f(startx, starty+cardHeight));
                                    convex.setFillColor(sf::Color(0,0,0,0));
                                    convex.setOutlineColor(sf::Color(255,255,0,100));
                                    Chosen = false;
                                }
                            }
                        }
                    }
                if(hintAnim){
                    float borderX, borderY;
                    float To, sum;
                    To = 10;
                    if(a.box[hintTo].pTop->item.posX>hintNode.bottom->item.posX){
                        borderX = a.box[hintTo].pTop->item.posX-hintNode.bottom->item.posX;
                        //To = borderX/20;
                        borderX = borderX/To;
                    } else{
                        borderX = a.box[hintTo].pTop->item.posX - hintNode.bottom->item.posX;
                        //To = -borderX/20;
                        borderX = borderX/To;
                    }
                    borderY = (a.box[hintTo].pTop->item.posY + 20 - hintNode.bottom->item.posY) / To;
                    int i;
                    for (i =0; i<(int)To; i++){
                        window.clear();
                        window.draw(m_background);
                        window.draw(deck.m_sprite);
                        //  window.draw(convex);
                        window.draw(hintTile);
                        window.draw(restartTile);
                        drawStart(window,&a,0);
                        hintNode.bottom->item.posX +=  borderX;
                        hintNode.bottom->item.posY += borderY;
                        hintNode.bottom->item.setPosition(hintNode.bottom->item.posX,hintNode.bottom->item.posY);
                        hintNode.bottom->item.m_sprite.setColor(Color(255,255,255,200));
                        window.draw(hintNode.bottom->item.m_sprite);
                        float y = hintNode.bottom->item.posY;
                        NodeStack* temp = hintNode.bottom;
                        temp = temp->prev;
                        while (temp){
                            temp->item.posX = hintNode.bottom->item.posX;
                            temp->item.posY =temp->next->item.posY+20;
                            temp->item.setPosition(temp->item.posX,temp->item.posY);
                            temp->item.m_sprite.setColor(Color(255,255,255,200));
                            window.draw(temp->item.m_sprite);
                            temp = temp->prev;
                        }
                        window.display();
                    }
                    sleep(milliseconds(100));
                    hintTile.setColor(Color(255,255,255,255));
                    hintAnim = false;
                    Chosen = false;
                } else{
                    hintTile.setColor(Color(255,255,255,255));
                }
                if(Chosen)
                    if (event.type == Event::MouseButtonReleased){
                        bool moved = false;
                        bool contained = false;
                        if (event.key.code == Mouse::Left){
                            Vector2i pos = Mouse::getPosition(window);
                            for(int i = 0; i<chBox; i++){
                                contained = releasedInRange(pos, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 100, 150));
                                if (contained) {
                                    moved = move(col,&a.box[chBox], &a.box[i]);
                                    if(moved && !a.box[chBox].pTop->item.visible){
                                        Card c = a.box[chBox].pTop->item;
                                        c.setPosition(c.posX, c.posY);
                                        c.setTexture(c.path);
                                        for(int i = 1; i<5; i++){
                                            c.m_sprite.setScale(1-i*0.25, 1);
                                            c.m_sprite.setPosition(c.m_sprite.getPosition().x + cardWidth*0.1,c.m_sprite.getPosition().y);
                                            window.clear();
                                            window.draw(m_background);
                                            drawStart(window,&a, 0);
                                            window.draw(deck.m_sprite);
                                            window.draw(hintTile);
                                            window.draw(restartTile);
                                            window.draw(c);
                                            window.display();
                                        }
                                        c.visible = true;
                                        c.m_path = a.box[chBox].pTop->item.m_path;
                                        c.setTexture(c.m_path);
                                        for(int i = 1; i<5; i++){
                                            c.m_sprite.setScale(0 +i*0.25, 1);
                                            c.m_sprite.setPosition(c.m_sprite.getPosition().x - cardWidth*0.1,c.m_sprite.getPosition().y);
                                            window.clear();
                                            window.draw(m_background);
                                            drawStart(window,&a, 0);
                                            window.draw(deck.m_sprite);
                                            window.draw(hintTile);
                                            window.draw(restartTile);
                                            window.draw(c.m_sprite);
                                            window.display();
                                        }
                                        a.box[chBox].pTop->item.visible = true;
                                    }
                                    break;
                                }
                            }
                            if(!contained)
                                for(int i = chBox+1; i<10; i++){
                                    contained = releasedInRange(pos, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 100, 150));
                                    if (contained) {
                                        moved = move(col, &a.box[chBox],&a.box[i]);
                                        if(moved && !a.box[chBox].pTop->item.visible){
                                            Card c = a.box[chBox].pTop->item;
                                            c.setPosition(c.posX, c.posY);
                                            c.setTexture(c.path);
                                            for(int i = 1; i<5; i++){
                                                c.m_sprite.setScale(1-i*0.25, 1);
                                                c.m_sprite.setPosition(c.m_sprite.getPosition().x + cardWidth*0.1,c.m_sprite.getPosition().y);
                                                window.clear();
                                                window.draw(m_background);
                                                drawStart(window,&a, 0);
                                                window.draw(deck.m_sprite);
                                                window.draw(hintTile);
                                                window.draw(restartTile);
                                                window.draw(c);
                                                window.display();
                                            }
                                            c.visible = true;
                                            c.m_path = a.box[chBox].pTop->item.m_path;
                                            c.setTexture(c.m_path);
                                            for(int i = 1; i<5; i++){
                                                c.m_sprite.setScale(0 +i*0.25, 1);
                                                c.m_sprite.setPosition(c.m_sprite.getPosition().x - cardWidth*0.1,c.m_sprite.getPosition().y);
                                                window.clear();
                                                window.draw(m_background);
                                                drawStart(window,&a, 0);
                                                window.draw(deck.m_sprite);
                                                window.draw(hintTile);
                                                window.draw(restartTile);
                                                window.draw(c.m_sprite);
                                                window.display();
                                            }
                                            a.box[chBox].pTop->item.visible = true;
                                        }
                                        break;
                                    }
                                }
                            if(!moved){
                                NodeStack* temp = col;
                                float y = dY;
                                while(temp){
                                    temp->item.posX = dX;
                                    temp->item.posY = y;
                                    y+=20;
                                    temp = temp->prev;
                                }
                                convex = setColorIfCardChosen(col, cardWidth,cardHeight);
                                Chosen = false;
                            } else {
                                convex.setOutlineThickness(0);
                                soundPlace.play();
                            }
                            isMove = false;
                        }
                    }
                if (isMove) {
                    Vector2i  pixelPos = Mouse::getPosition(window);
                    NodeStack* temp = col;

                    float y = pixelPos.y-45;
                    while(temp){
                        temp->item.posX = pixelPos.x-35;
                        temp->item.posY = y;
                        y+=20;
                        temp = temp->prev;
                    }
                    convex = setColorIfCardChosen(col,cardWidth,cardHeight);
                }
            }
        }
        window.clear();
        window.draw(m_background);
        drawStart(window, &a, chBox);
        window.draw(deck.m_sprite);
        window.draw(convex);
        window.draw(hintTile);
        window.draw(restartTile);
        window.display();
        // sf::sleep(sf::milliseconds(20));
    }
}

void gameRunning(RenderWindow &window, int menuNum){
    if(startGame(window, menuNum)){
        gameRunning(window, menuNum);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Spider", Style::Close);
    sf::Image app_icon;
    //window.create(sf::VideoMode(1000, 600), "Spider", Style::Close);
    window.setVerticalSyncEnabled(true);
    // window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    app_icon.loadFromFile("C:/Users/Asus/Desktop/spider/resource/app_icon.bmp");
    window.setIcon(app_icon.getSize().x, app_icon.getSize().y, app_icon.getPixelsPtr());
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(desktop.width / 2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));

    int menuNum =menu(window);//вызов меню
    gameRunning(window, menuNum);
    return 0;
};




