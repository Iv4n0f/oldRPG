#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

class ObjetoJuego
{
public:
    ObjetoJuego(const std::string &filename)
    {
        if (!texture.loadFromFile(filename))
        {
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        escalar();
        if (!failTexture.loadFromFile("fallo.png"))
        {
            exit(EXIT_FAILURE);
        }
        failSprite.setTexture(failTexture);
    }

    virtual void renderizar(sf::RenderWindow &window, bool derecha)
    {
        float posX = derecha ? 750 : 50;
        sprite.setScale(derecha ? -1.0f : 1.0f, 1.0f);
        sprite.setPosition(posX, 100);
        window.draw(sprite);
    }

protected:
    void escalar()
    {
        sf::FloatRect bounds = sprite.getLocalBounds();
        int targetWidth = 300;
        int targetHeight = 300;
        float scaleX = targetWidth / bounds.width;
        float scaleY = targetHeight / bounds.height;
        sprite.setScale(scaleX, scaleY);
    }

    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture failTexture;
    sf::Sprite failSprite;
};

class Personaje : public ObjetoJuego
{
protected:
    int vida;
    int daño;

public:
    Personaje(const std::string &filename) : ObjetoJuego(filename){};
    int getVida() { return vida; }
    int getDaño() { return daño; }
    void setVida(int vida) { this->vida = vida; }
    void setDaño(int daño) { this->daño = daño; }
    virtual void ataque(Personaje &objetivo){
        int random = rand() % 100;
        if (random < 10){
            std::cout << "El ataque ha fallado" << std::endl;
        }
        objetivo.setVida(objetivo.getVida() - this->daño);
    }
};

class Guerrero : public Personaje
{
public:
    Guerrero(const std::string &filename) : Personaje(filename) {}
    
};

class Mago : public Personaje
{
public:
    Mago(const std::string &filename) : Personaje(filename) {}
};

class Arquero : public Personaje
{
public:
    Arquero(const std::string &filename) : Personaje(filename) {}
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Escalando y posicionando un Sprite");

    Guerrero guerrero("goblin_sprite.png");
    Mago mago("goblin_sprite.png");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        guerrero.renderizar(window, true);
        mago.renderizar(window, false);
        window.display();
    }

    return 0;
}
