//
// Created by Asus on 22.11.2022.
//

#ifndef SPIDER_MENU_H
#define SPIDER_MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Tile.h"
//using namespace sf;
int menu(sf::RenderWindow & window) {
    sf::Texture menuTexture1, menuTexture2,menuTexture3;

    menuTexture1.loadFromFile("C:/Users/Asus/Desktop/spider/resource/easy.png");
    menuTexture2.loadFromFile("C:/Users/Asus/Desktop/spider/resource/medium.png");
    menuTexture3.loadFromFile("C:/Users/Asus/Desktop/spider/resource/hard.png");
    sf::Sprite menu1(menuTexture1), menu2(menuTexture2),menu3(menuTexture3);
    bool isMenu = 1;
    int menuNum = 0;
    unsigned  int x1 = window.getSize().x /2 - menuTexture1.getSize().x /2;
    unsigned  int x2 = window.getSize().x /2 - menuTexture2.getSize().x /2;
    unsigned  int x3 = window.getSize().x /2 - menuTexture3.getSize().x /2;
    menu1.setPosition((float)x1, 120);
    menu2.setPosition((float)x2, 220);
    menu3.setPosition((float)x3, 320);

    //////////////////////////////МЕНЮ///////////////////
    while (isMenu)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();}
        menu1.setColor(sf::Color::White);
        menu2.setColor(sf::Color::White);
        menu3.setColor(sf::Color::White);
        menuNum = 0;
        window.clear(sf::Color(0, 49, 83));

        if (sf::IntRect(x1, 120, 300, 70).contains(sf::Mouse::getPosition(window))) {
            menu1.setColor(sf::Color::Black);
            menuNum = 1; } else
        if (sf::IntRect(x2, 220, 300, 70).contains(sf::Mouse::getPosition(window))) {
            menu2.setColor(sf::Color::Black);
            menuNum = 2; } else
        if (sf::IntRect(x3, 320, 300, 70).contains(sf::Mouse::getPosition(window))) {
            menu3.setColor(sf::Color::Black);
            menuNum = 3; }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            if(menuNum==1 || menuNum == 2 || menuNum == 3)
            isMenu = false;//если нажали первую кнопку, то выходим из меню
        }
        window.draw(menu1);
        window.draw(menu2);
        window.draw(menu3);
        window.display();
    }
    return menuNum;
    ////////////////////////////////////////////////////
}
#endif //SPIDER_MENU_H

