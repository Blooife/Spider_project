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

    int menuNum =menu(window);//вызов меню
    Deck deck;
    deck.SetupCards(menuNum);
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


    Tile m_background;
    m_background.setTexture("C:/Users/Asus/Desktop/spider/resource/backgrounds/back_img.jpg");
    Tile chTile;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            } else{
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
               // chTile.m_sprite.setColor(sf::Color(0,0,0,0));

                } //else
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    for(int i = 0; i<10; i++){
                        if (clickInRange(event.mouseButton, sf::IntRect(a.box[i].Check().posX, a.box[i].Check().posY, 75, 115))) {
                            chTile = chosen(window,&a.box[i].pTop->item);
                            cout<<a.box[i].pTop->item.value;
                            break;
                        } else chTile.m_sprite.setColor(sf::Color(0,0,0,0));
                    }
                    if (clickInRange(event.mouseButton, sf::IntRect(deck.posX, deck.posY, 75, 115))) {
                        dealt(&a, &deck);
                    }
                }

            }
        }
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




