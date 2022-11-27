#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Menu.h"
//#include "Cards.cpp"
#include "SpiderLogic.h"

#define cardWidth a.box[0].Check().m_texture.getSize().x

using namespace sf;

int main() {
    sf::RenderWindow window;
    sf::Image app_icon;
    window.create(sf::VideoMode(1000, 600), "Spider");
    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(false);
    app_icon.loadFromFile("C:/Users/Asus/Desktop/spider/resource/app_icon.bmp");
    window.setIcon(app_icon.getSize().x, app_icon.getSize().y, app_icon.getPixelsPtr());
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(desktop.width / 2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));

    //int menuNum =menu(window);//вызов меню
    Deck deck;
   // deck.SetupCards(menuNum);
    deck.SetupCards(1);
    deck.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card_back.bmp";
    deck.setTexture(deck.m_path);
    deck.posX = window.getSize().x-150;
    deck.posY = window.getSize().y-150;
    deck.setPosition(deck.posX,deck.posY);


    std::cout << std::endl;
    deck.shuffle();
    deck.PrintAll();

    Box a;
    a.init(&deck, window.getSize().x % 10 + 50, cardWidth+20);

    bool isMove = false;
    bool Chosen = false;
    bool contained = true;
    Tile m_background;
    m_background.setTexture("C:/Users/Asus/Desktop/spider/resource/backgrounds/back_img.jpg");
    Tile chTile;
    while (window.isOpen()) {

        sf::Event event;
        int dX;
        int dY;

        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            } else{
                int chBox;
                NodeStack* col;
                if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
                    if (event.key.code == Mouse::Left) {
                        sf::Vector2i clickPos = sf::Mouse::getPosition(window);
                        int i = getColomn(clickPos,window.getSize().x % 10 + 50, cardWidth+20);
                        if(i> -1) {
                            col = a.box[i].pTop;
                            while (col->item.visible){
                                if (clickInRange(event.mouseButton,
                                                 sf::IntRect(col->item.posX, col->item.posY, 75,
                                                             115))) {
                                    if(canDrag(col)){
                                        chTile = chosen(window,&col->item);
                                        Chosen = true;
                                        chBox = i;
                                        dX = col->item.posX;
                                        dY = col->item.posY;
                                        isMove = true;
                                        break;
                                    }

                                } else {
                                    chTile.m_sprite.setColor(sf::Color(0,0,0,0));
                                    Chosen = false;
                                }
                                col = col->next;
                            }
                        }
                        if  (clickInRange(event.mouseButton, sf::IntRect(deck.posX, deck.posY, 75, 115))) {
                        dealt(&a, &deck);
                    }
                }
                if(Chosen)
                if (event.type == Event::MouseButtonReleased){
                    bool moved = false;
                    if (event.key.code == Mouse::Left){
                        Vector2i pos = Mouse::getPosition(window);
                        for(int i = 0; i<chBox; i++){
                            contained = releasedInRange(pos, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 100, 150));
                            if (contained) {
                                moved = move(col,&a.box[chBox], &a.box[i]);
                                break;
                            }
                        }
                        if(!contained)
                        for(int i = chBox+1; i<10; i++){
                            contained = releasedInRange(pos, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 100, 150));
                            if (contained) {
                                moved = move(col, &a.box[chBox],&a.box[i]);
                                break;
                            }
                        }
                        if(!moved){
                            col->item.posX = dX;
                            col->item.posY = dY;
                            Chosen = false;
                        }
                        isMove = false;

                    }

                }
                if (isMove) {
                    Vector2i  pixelPos = Mouse::getPosition(window);
                    col->item.posX = pixelPos.x-35;
                    col->item.posY = pixelPos.y-45;
                   // window.draw(a.box[chBox].pTop->item.m_sprite);
                   }
                }
        }

      //  drawStart(window, a);
        window.draw(m_background);
        drawStart(window, a);
        deck.setTexture(deck.m_path);
        window.draw(deck.m_sprite);
        window.draw(chTile);
        window.display();
        sf::sleep(sf::milliseconds(20));
    }
    return 0;
};




