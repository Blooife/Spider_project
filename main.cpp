#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "Menu.h"
#include "SpiderLogic.h"

#define cardWidth a.box[0].Check().m_texture.getSize().x
#define cardHeight a.box[0].Check().m_texture.getSize().y

using namespace sf;

bool startGame(RenderWindow &window){
    int menuNum =menu(window);//вызов меню
    Tile collected[8];
    float x;
    x = window.getSize().x - 920;
    for(int i=0; i<8; i++){
        collected[i].setPosition(x,window.getSize().y - 120);
        x += 20;
    }
    Deck deck;
    Box a;
    int score;
    int numCollected = 0;
    if(menuNum == 4){
        readBox(&a);
        readDeck(&deck);
        if(deck.size < 10){
            deck.m_path="C:/Users/Asus/Desktop/spider/resource/cards/empty_card.png";
            deck.setTexture(deck.m_path);
        }
        numCollected = readCollected(collected, &score);
    } else{
        deck.SetupCards(menuNum);
        deck.shuffle();
        deck.m_path = "C:/Users/Asus/Desktop/spider/resource/cards/card_back.bmp";
        deck.setTexture(deck.m_path);
        a.init(&deck, window.getSize().x % 10 + 50, cardWidth+20);
        score = 500;
    }


    deck.posX = window.getSize().x-150;
    deck.posY = window.getSize().y-120;
    deck.setPosition(deck.posX,deck.posY);

    StackList prev[10];
    for(int i = 0; i<10; i++){
        NodeStack* temp = a.box[i].bottom;
        while(temp){
            prev[i].Push(temp->item);
            temp = temp->prev;
        }
    }
    ////////////////////////////////////////////////////////////////////////////////
    Sound soundDealt, soundPlace, soundSlide;
    sf::SoundBuffer bufferDealt, bufferPlace, bufferSlide;
    Tile hintTile, back, exitTile, restartTile, m_background, volume, win;
    Text scoreText;
    Font font;
    font.loadFromFile("C:\\Users\\Asus\\Desktop\\spider\\resource\\lato-light.ttf");
    scoreText.setPosition(window.getSize().x-850, window.getSize().y-598);
    scoreText.setFont(font);
    scoreText.setFillColor(Color::Black);
    scoreText.setCharacterSize(23);
    scoreText.setStyle(Text::Bold);
    scoreText.setString("Score" + to_string(score));
    bufferDealt.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/dealt.ogg");
    soundDealt.setBuffer(bufferDealt);
    bufferPlace.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/cardPlace.ogg");
    soundPlace.setBuffer(bufferPlace);
    bufferSlide.loadFromFile("C:/Users/Asus/Desktop/spider/resource/sounds/cardSlide.ogg");
    soundSlide.setBuffer(bufferSlide);
    hintTile.m_path = "C:/Users/Asus/Desktop/spider/resource/hint.png";
    hintTile.setTexture(hintTile.m_path);
    hintTile.setPosition(window.getSize().x-50, window.getSize().y-300);
    restartTile.m_path = "C:/Users/Asus/Desktop/spider/resource/icons/refresh.png";
    restartTile.setTexture(restartTile.m_path);
    restartTile.setPosition(window.getSize().x-50, window.getSize().y-250);
    volume.setPosition(window.getSize().x-50, window.getSize().y-350);
    volume.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\icons\\volume.png";
    volume.setTexture(volume.m_path);
    back.m_path = "C:/Users/Asus/Desktop/spider/resource/icons/back.png";
    back.setTexture(back.m_path);
    back.setPosition(window.getSize().x-50, window.getSize().y-200);
    exitTile.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\icons\\exit.png";
    exitTile.setTexture(exitTile.m_path);
    exitTile.setPosition(window.getSize().x-50, window.getSize().y-590);
    m_background.setTexture("C:/Users/Asus/Desktop/spider/resource/backgrounds/back_img.jpg");
    sf::ConvexShape convex;
    convex.setPointCount(4);
    convex.setFillColor(sf::Color(0,0,0,0));
    convex.setOutlineThickness(0);
    convex.setOutlineColor(sf::Color(0,0,0,0));
    bool isMove = false;
    bool Chosen = false;
    int moveBack = 0;
    int chBox=-1;
    bool hintAnim=false;
    //////////////////////////////////////////////////////////////////////////////
    while (window.isOpen()) {
        sf::Event event;
        int dX;
        int dY;
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            } else{
                NodeStack* col;
                StackList hintNode;
                int hintTo;
                if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-45, window.getSize().y-250, 50,50))){
                    return true;
                }
                if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-45, window.getSize().y-590, 50,50))){
                    saveToFileBox(&a);
                    saveToFileDeck(&deck);
                    saveToFileCollected(collected, numCollected, score);
                    return false;
                }

                if (event.type == Event::MouseButtonPressed)//если нажата клавиша мыши
                    if (event.key.code == Mouse::Left) {
                        if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-45, window.getSize().y-350, 50,50))){
                            if(volume.m_path == "C:\\Users\\Asus\\Desktop\\spider\\resource\\icons\\volume_off.png"){
                                volume.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\icons\\volume.png";
                                volume.setTexture(volume.m_path);
                                soundSlide.setVolume(100);
                                soundPlace.setVolume(100);
                                soundDealt.setVolume(100);
                            }else{
                                volume.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\icons\\volume_off.png";
                                volume.setTexture(volume.m_path);
                                soundSlide.setVolume(0);
                                soundPlace.setVolume(0);
                                soundDealt.setVolume(0);
                            }
                            Chosen = false;
                            convex.setOutlineThickness(0);
                        }
                        if(deck.size == 0){
                            if(!hint(&a, &hintNode, hintTo)){
                                win.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\lost.png";
                                win.setTexture(win.m_path);
                                win.setPosition(100,150);
                                win.m_sprite.setColor(sf::Color(255,255,255,255));
                            }
                        }
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
                                        convex = setColorIfCardChosen(col, cardWidth, cardHeight);
                                        Chosen = true;
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
                                            moveBack  = 0;
                                        }
                                        //saveCurCond(a, prev);
                                        moveBack  = 0;
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
                                    moveBack = 1;
                                }
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
                                        window.draw(volume);
                                        window.draw(deck.m_sprite);
                                        window.draw(hintTile);
                                        window.draw(restartTile);
                                        window.draw(back);
                                        c.setTexture(c.m_path);
                                        c.setPosition(c.posX,c.posY);
                                        drawStart(window,&a,0);
                                        window.draw(c);
                                        window.draw(scoreText);
                                        window.draw(exitTile);
                                        drawCollected(window, collected);

                                        window.display();
                                    }
                                    a.box[i].Push(c);
                                    if(c.value == 1){
                                        if(checkPas(&a.box[i])){
                                            for(int k = 0; k<12; k++){
                                                a.box[i].Pop();
                                            }
                                            collected[numCollected].m_path = a.box[i].pTop->item.m_path;
                                            a.box[i].Pop();
                                            a.box[i].pTop->item.visible = true;
                                            numCollected++;
                                        }
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
                            hintAnim = hint(&a, &hintNode, hintTo);
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
                        if(clickInRange(event.mouseButton, sf::IntRect(window.getSize().x-45, window.getSize().y-200, 60,60))){
                            for(int j = 0; j<10; j++){
                                NodeStack* temp = a.box[j].bottom;
                                NodeStack* pr = prev[j].bottom;
                                if(a.box[j].Count() > prev[j].Count()){
                                    int k = a.box[j].Count() - prev[j].Count();
                                    while(k> 0){
                                        a.box[j].Pop();
                                        k--;
                                    }
                                }
                                while (pr){
                                    if(temp){
                                        temp->item = pr->item;
                                        temp = temp->prev;
                                    }else{
                                        a.box[j].Push(pr->item);
                                    }
                                    pr = pr->prev;

                                }
                            }
                            score--;
                            scoreText.setString("Score " + to_string(score));
                            win.m_sprite.setColor(sf::Color(0,0,0,0));

                            if(moveBack == 1){
                                deck.size += 10;
                            } else if(moveBack == 2){
                                numCollected--;
                                for(int i = 0; i<8; i++){
                                    if(collected[i].m_path.empty()){
                                        collected[i-1].m_path = "";
                                        break;
                                    }
                                }
                            }
                            Chosen = false;
                           convex.setOutlineThickness(0);
                        }
                    }
                if(hintAnim){
                    float borderX, borderY;
                    float To, sum;
                    To = 10;
                    if(a.box[hintTo].pTop->item.posX>hintNode.bottom->item.posX){
                        borderX = a.box[hintTo].pTop->item.posX-hintNode.bottom->item.posX;
                        borderX = borderX/To;
                    } else{
                        borderX = a.box[hintTo].pTop->item.posX - hintNode.bottom->item.posX;
                        borderX = borderX/To;
                    }
                    borderY = (a.box[hintTo].pTop->item.posY + 20 - hintNode.bottom->item.posY) / To;
                    int i;
                    for (i =0; i<(int)To; i++){
                        window.clear();
                        window.draw(m_background);
                        window.draw(deck.m_sprite);
                        window.draw(volume);
                        //  window.draw(convex);
                        window.draw(hintTile);
                        window.draw(restartTile);
                        window.draw(back);
                        window.draw(scoreText);
                        window.draw(exitTile);
                        drawStart(window,&a,0);
                        drawCollected(window, collected);
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
                                    if(a.box[i].pTop->item.value==1){
                                        if(checkPas(&a.box[i])){
                                            score +=100;
                                            scoreText.setString("Score " + to_string(score));
                                            moveBack = 2;
                                            for(int k = 0; k<12; k++){
                                                a.box[i].Pop();
                                            }
                                            collected[numCollected].m_path = a.box[i].pTop->item.m_path;
                                            a.box[i].Pop();
                                            a.box[i].pTop->item.visible = true;
                                            numCollected++;
                                            if(numCollected == 8){
                                                win.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\fire.png";
                                                win.setTexture(win.m_path);
                                                win.setPosition(250,100);
                                                win.m_sprite.setColor(sf::Color(255,255,255,255));
                                            }
                                        }
                                    }
                                    if(moved) {
                                        score--;
                                        scoreText.setString("Score " + to_string(score));
                                        if (!a.box[chBox].pTop->item.visible) {
                                            Card c = a.box[chBox].pTop->item;
                                            c.setPosition(c.posX, c.posY);
                                            c.setTexture(c.path);
                                            for (int i = 1; i < 3; i++) {
                                                c.m_sprite.setScale(1 - i * 0.5, 1);
                                                c.m_sprite.setPosition(c.m_sprite.getPosition().x + cardWidth * 0.25,
                                                                       c.m_sprite.getPosition().y);
                                                window.clear();
                                                window.draw(m_background);
                                                window.draw(volume);
                                                drawStart(window, &a, 0);
                                                window.draw(deck.m_sprite);
                                                window.draw(hintTile);
                                                window.draw(restartTile);
                                                window.draw(back);
                                                window.draw(scoreText);
                                                window.draw(c);
                                                window.draw(exitTile);
                                                drawCollected(window, collected);
                                                window.display();
                                            }
                                            c.visible = true;
                                            c.m_path = a.box[chBox].pTop->item.m_path;
                                            c.setTexture(c.m_path);
                                            for (int i = 1; i < 3; i++) {
                                                c.m_sprite.setScale(0 + i * 0.5, 1);
                                                c.m_sprite.setPosition(c.m_sprite.getPosition().x - cardWidth * 0.25,
                                                                       c.m_sprite.getPosition().y);
                                                window.clear();
                                                window.draw(m_background);
                                                drawStart(window, &a, 0);
                                                window.draw(volume);
                                                window.draw(deck.m_sprite);
                                                window.draw(hintTile);
                                                window.draw(restartTile);
                                                window.draw(back);
                                                window.draw(scoreText);
                                                window.draw(exitTile);
                                                window.draw(c.m_sprite);
                                                drawCollected(window, collected);
                                                window.display();
                                            }
                                            a.box[chBox].pTop->item.visible = true;
                                        }
                                    }
                                    break;
                                }
                            }
                            if(!contained)
                                for(int i = chBox+1; i<10; i++){
                                    contained = releasedInRange(pos, sf::IntRect(a.box[i].Check().posX-10, a.box[i].Check().posY-10, 100, 150));
                                    if (contained) {
                                        moved = move(col, &a.box[chBox],&a.box[i]);
                                        if(a.box[i].pTop->item.value==1){
                                            if(checkPas(&a.box[i])){
                                                score+=100;
                                                scoreText.setString("Score " + to_string(score));
                                                moveBack = 2;
                                                for(int k = 0; k<12; k++){
                                                    a.box[i].Pop();
                                                }
                                                collected[numCollected].m_path = a.box[i].pTop->item.m_path;
                                                a.box[i].Pop();
                                                a.box[i].pTop->item.visible = true;
                                                numCollected++;
                                                if(numCollected == 8){
                                                    win.m_path = "C:\\Users\\Asus\\Desktop\\spider\\resource\\fire.png";
                                                    win.setTexture(win.m_path);
                                                    win.setPosition(250,100);
                                                    win.m_sprite.setColor(sf::Color(255,255,255,255));
                                                }
                                            }
                                        }
                                        if(moved) {
                                            score--;
                                            scoreText.setString("Score " + to_string(score));
                                            if (!a.box[chBox].pTop->item.visible) {
                                                Card c = a.box[chBox].pTop->item;
                                                c.setPosition(c.posX, c.posY);
                                                c.setTexture(c.path);
                                                // if(a.box[i].pTop->item.value != 0)
                                                for (int i = 1; i < 3; i++) {
                                                    c.m_sprite.setScale(1 - i * 0.5, 1);
                                                    c.m_sprite.setPosition(
                                                            c.m_sprite.getPosition().x + cardWidth * 0.25,
                                                            c.m_sprite.getPosition().y);
                                                    window.clear();
                                                    window.draw(m_background);
                                                    window.draw(volume);
                                                    drawStart(window, &a, 0);
                                                    window.draw(deck.m_sprite);
                                                    window.draw(hintTile);
                                                    window.draw(restartTile);
                                                    window.draw(back);
                                                    window.draw(scoreText);
                                                    window.draw(c);
                                                    window.draw(exitTile);
                                                    drawCollected(window, collected);
                                                    window.display();
                                                }
                                                c.visible = true;
                                                c.m_path = a.box[chBox].pTop->item.m_path;
                                                c.setTexture(c.m_path);
                                                for (int i = 1; i < 3; i++) {
                                                    c.m_sprite.setScale(0 + i * 0.5, 1);
                                                    c.m_sprite.setPosition(
                                                            c.m_sprite.getPosition().x - cardWidth * 0.25,
                                                            c.m_sprite.getPosition().y);
                                                    window.clear();
                                                    window.draw(m_background);
                                                    window.draw(deck.m_sprite);
                                                    window.draw(volume);
                                                    window.draw(hintTile);
                                                    window.draw(restartTile);
                                                    window.draw(back);
                                                    window.draw(c.m_sprite);
                                                    window.draw(scoreText);
                                                    window.draw(exitTile);
                                                    drawCollected(window, collected);
                                                    drawStart(window, &a, 0);
                                                    window.display();
                                                }
                                                a.box[chBox].pTop->item.visible = true;
                                            }
                                        }
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
        window.draw(volume);
        window.draw(deck.m_sprite);
        window.draw(hintTile);
        window.draw(restartTile);
        drawCollected(window, collected);
        window.draw(back);
        window.draw(exitTile);
        window.draw(scoreText);
        drawStart(window, &a, chBox);
        window.draw(convex);
        window.draw(win);
        window.display();
    }
}

void gameRunning(RenderWindow &window){
    if(startGame(window)){
        gameRunning(window);
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 600), "Spider", Style::Close);
    sf::Image app_icon;
    //window.create(sf::VideoMode(1000, 600), "Spider", Style::Close);
    window.setVerticalSyncEnabled(true);
     window.setFramerateLimit(90);
    window.setKeyRepeatEnabled(false);
    app_icon.loadFromFile("C:/Users/Asus/Desktop/spider/resource/app_icon.bmp");
    window.setIcon(app_icon.getSize().x, app_icon.getSize().y, app_icon.getPixelsPtr());
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window.setPosition(sf::Vector2i(desktop.width / 2 - window.getSize().x/2, desktop.height/2 - window.getSize().y/2));


    gameRunning(window);
    return 0;
};




