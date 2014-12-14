#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include <cmath>
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
        
        // собственно, преобразование из спирали вочтохошь
//        cmp p = pos;
        cmp p = (pos+cmp(1,0)) / (pos-cmp(1,0));
        
        double scale = 1 - abs( (pos+cmp(1,0)) / (pos-cmp(1,0)) );
        double angle = arg<double>(tform) * 180 / M_PI;
        shape->setScale(scale, scale);
        //shape->setRotation(M_PI_2 - angle);
        
        shape->setPosition(W/2 + p.real()*draw_scale - 25*scale, H/2 - p.imag()*draw_scale - 25*scale);
        wnd.draw(*shape);      
    }
    
    void move(double dt) {
        float k = 0.8;
        float a = M_PI/4;
        //pos = pos * pow( plr(k, a) , dt );
        pos = pos * pow(plr(k,-a), dt);
        //tform = tform * pow(plr(1,-a), dt);
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
    //music.play();
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
    //ShapeComplex k(cmp(0.02, 0.02), plr(0.01, 0), &koshi);
    ShapeComplex k(cmp(1000*M_PI, 0), plr(1, 0), &koshi);
    
    std::vector<ShapeComplex> vk;
    vk.push_back(ShapeComplex(k));
    
    bool pause = false;
    
    float timeStamp = 0;
    float clr = 0;
    
    while (App.isOpen()) {
        sf::Event Event;
        
        while (App.pollEvent(Event)) {
            if (Event.type == sf::Event::Closed)
                App.close();
            if (Event.type == sf::Event::KeyPressed) {
                switch (Event.key.code) {
                    case sf::Keyboard::Escape: App.close();
                }
            }
                
        }
        
        if (!pause) {
            float dt = clock.restart().asSeconds();
            timeStamp += dt;

            clr += dt;
            int r = sin(clr)*127 + 127;
            int g = sin(clr*2)*127 + 127;
            int b = sin(clr*3)*127 + 127;
            
            App.clear( sf::Color(r,g,b) );
            
            double min_size = 1;
            for(auto i = vk.begin(); i!=vk.end(); ++i) {
                i->move(dt*2);
                i->draw(App);
                
                double size = i->getSize();
                if (size < min_size)
                    min_size = size;
                
                if (size > 8 || size < 0.01) 
                    vk.erase(i);
            }
            if (timeStamp >= 0.5) {
                vk.push_back( ShapeComplex(k) );
                timeStamp = 0;
            }
            /*
            if (min_size > 0.011) {
               vk.push_back( ShapeComplex(k) );
            }*/
        }
        
        App.display();
    }
    
    return 0;
}
