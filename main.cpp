// 8244 4702 ---- 0162, цсв 711:  MP2906869
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <complex>

#define cmp std::complex<double>
#define plr std::polar<double>

using namespace std;

const int W = 1366;
const int H = 768;

class ShapeComplex {
private:
    cmp pos, tform;
    sf::Shape *shape;
    
public:
    ShapeComplex() {};
    ShapeComplex(cmp pos, cmp tform, sf::Shape *shape)
        : pos(pos), tform(tform), shape(shape) {};
    
    void draw(sf::RenderWindow &wnd) {
        const int draw_scale = 200;
        double scale = abs(tform);
        double angle = arg<double>(tform) * 180 / M_PI;
        shape->setScale(scale, scale);
        shape->setRotation(angle);
        shape->setPosition(W/2 + pos.real()*draw_scale - 25*scale, H/2 - pos.imag()*draw_scale - 25*scale);
        wnd.draw(*shape);        
    }
    
    void move(double dt) {
        pos = pos * plr(1+dt/3.2,dt);
        tform = tform * plr(1+dt/2.6,-dt);
    }
    
    double getSize() {
        return abs(tform);
    }
};

int main() {
    sf::Music music;
    if (!music.openFromFile("bg.ogg"))
        throw std::exception();
    music.setLoop(true);
    music.play();
    // 8244 4702 ---- 0162, цсв 711
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow App(sf::VideoMode(W, H, 32), "Complex Koshi", sf::Style::None, settings);
    sf::Clock clock;
    
    sf::CircleShape koshi(50);
    
    sf::Texture koshi_texture;
    if (!koshi_texture.loadFromFile("koshi.png"))
        throw std::exception();
    koshi_texture.setSmooth(true);
    
    koshi.setTexture(&koshi_texture);
    ShapeComplex k(cmp(0.02, 0.02), plr(0.01, 0), &koshi);
    
    std::vector<ShapeComplex> vk;
    vk.push_back(ShapeComplex(k));
    
    bool pause = false;
    
    while (App.isOpen()) {
        sf::Event Event;
        
        
        while (App.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed)
                App.close();
            if (Event.type == sf::Event::KeyPressed) {
                switch (Event.key.code) {
                    case sf::Keyboard::Escape: App.close();
                    case sf::Keyboard::Space:  pause = !pause;
                }
            }
                
        }
        
        if (!pause) {
            App.clear(sf::Color(100,100,100));
            float dt = clock.restart().asSeconds();

            double min_size = 1;
            for(auto i = vk.begin(); i!=vk.end(); ++i) {
                i->move(dt);
                i->draw(App);
                
                double size = i->getSize();
                if (size < min_size)
                    min_size = size;
                
                if (size > 7) 
                    vk.erase(i);
            }

            if (min_size > 0.015)
                vk.push_back( ShapeComplex(k) );
        }
        
        App.display();
    }
    
    return 0;
}
