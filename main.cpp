#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Menu.h"
//#include "Cards.cpp"
#include "SpiderLogic.h"

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
    a.init(&deck, window.getSize().x % 10 + 50);

    bool isMove = false;
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
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    for(int i = 0; i<10; i++){
                        if (clickInRange(event.mouseButton, sf::IntRect(a.box[i].Check().posX, a.box[i].Check().posY, 75, 115))) {
                            chTile = chosen(window,&a.box[i].pTop->item);
                            chBox = i;
                            dX = a.box[i].Check().posX;
                            dY = a.box[i].Check().posY;
                            isMove = true;
                            break;
                        } else chTile.m_sprite.setColor(sf::Color(0,0,0,0));
                    }
                    if (clickInRange(event.mouseButton, sf::IntRect(deck.posX, deck.posY, 75, 115))) {
                        dealt(&a, &deck);
                    }
                }
                if (event.type == Event::MouseButtonReleased){
                    bool moved;
                    if (event.key.code == Mouse::Left){
                        for(int i = 0; i<10; i++){
                            if (clickInRange(event.mouseButton, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 90, 130))) {
                                moved = move(&a.box[chBox], &a.box[i]);
                                break;
                            }
                        }
                        if(!moved){
                            a.box[chBox].pTop->item.posX = dX;
                            a.box[chBox].pTop->item.posY = dY;
                        }
                        isMove = false;
                    }

                }
                if (isMove) {
                    Vector2i  pixelPos = Mouse::getPosition(window);
                    a.box[chBox].pTop->item.posX = pixelPos.x-35;
                    a.box[chBox].pTop->item.posY =pixelPos.y-45;
                   }
                }
        }
     //   a.box[1].Check().m_sprite.
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




