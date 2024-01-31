#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

class ObjetoJuego
{
public:
    ObjetoJuego(const std::string &filename, sf::RenderWindow *windowPtr)
        : window(windowPtr)
    {
        if (!texture.loadFromFile(filename))
        {
            exit(EXIT_FAILURE);
        }
        sprite.setTexture(texture);
        if (!failTexture.loadFromFile("fallo.png"))
        {
            exit(EXIT_FAILURE);
        }
        failSprite.setTexture(failTexture);
    }

    void renderizar(bool derecha)
    {
        float posX = derecha ? 750 : 50;
        sprite.setPosition(posX, 100);

        float scaleX = derecha ? -1.0f : 1.0f;
        sprite.setScale(scaleX, 1.0f);

        window->draw(sprite);
    }
    void renderizarFallo()
    {
        std::cout << "El ataque ha fallado" << std::endl;
        failSprite.setPosition(200, 50);
        window->draw(failSprite);
        window->display();         // Mostrar la ventana
        sf::sleep(sf::seconds(1.5)); // Pausar la ejecución durante 2 segundos
    }

protected:
    sf::RenderWindow *window;
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
    Personaje(const std::string &filename, sf::RenderWindow *windowPtr)
        : ObjetoJuego(filename, windowPtr) {}
    int getVida() { return vida; }
    int getDaño() { return daño; }
    void setVida(int vida) { this->vida = vida; }
    void setDaño(int daño) { this->daño = daño; }

    void ataque(Personaje &objetivo)
    {
        int random = rand() % 100;
        if (random < 20)
        {
            renderizarFallo();
        }
        else
        {
            int daño = getDaño();
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
        }
    }
};

class Guerrero : public Personaje
{
public:
    Guerrero(const std::string &filename, sf::RenderWindow *windowPtr)
        : Personaje(filename, windowPtr)
    {
        vida = 130;
        daño = 10;
    }

    void berserker(Personaje &objetivo)
    {
        vida -= 15;

        int random = rand() % 100;
        if (random < 33)
        {
            renderizarFallo();
        }
        else
        {
            int daño = getDaño() * 2;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
        }
    }

    void curarse(Personaje &objetivo)
    {
        int random = rand() % 100;
        if (random < 33)
        {
            renderizarFallo();
        }
        else
        {
            vida += 15;
        }
    }
};

class Mago : public Personaje
{
public:
    Mago(const std::string &filename, sf::RenderWindow *windowPtr)
        : Personaje(filename, windowPtr)
    {
        vida = 70;
        daño = 18;
    }

    void robar(Personaje &objetivo)
    {
        int random = rand() % 100;
        if (random < 33)
        {
            renderizarFallo();
        }
        else if (random < 66)
        {
            // 33% robar 1 de daño del objetivo
            int daño_enemigo = objetivo.getDaño();
            objetivo.setDaño(daño_enemigo - 1);
            daño += 1;
        }
        else
        {
            // 33% robar 15 de vida del objetivo
            int vida_enemigo = objetivo.getVida();
            objetivo.setVida(vida_enemigo - 15);
            vida += 15;
        }
    }

    void _50_50(Personaje &objetivo)
    {
        int random = rand() % 100;
        if (random < 50)
        {
            renderizarFallo();
        }
        else
        {
            int daño = getDaño() * 2;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
        }
    }
};

class Arquero : public Personaje
{
public:
    Arquero(const std::string &filename, sf::RenderWindow *windowPtr)
        : Personaje(filename, windowPtr)
    {
        vida = 100;
        daño = 15;
    }

    void ataque_preciso(Personaje &objetivo)
    {
        vida -= 13;

        int daño = getDaño() * 2;
        int vida = objetivo.getVida();
        vida -= daño;
        objetivo.setVida(vida);
    }

    void critico(Personaje &objetivo)
    {
        int random = rand() % 100;
        if (random < 33)
        {
            int daño = getDaño() + 10;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
        }
        else{
            int daño = getDaño() - 5;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Escalando y posicionando un Sprite");

    Guerrero guerrero("guerrero.png", &window);
    Mago mago("mago.png", &window);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);

        guerrero.renderizar(true);
        mago.renderizar(false);

        mago.ataque(guerrero);
        std::cout << "VIDA GUERRERO: " << guerrero.getVida() << std::endl;
        if (guerrero.getVida() <= 0)
        {
            std::cout << "Guerrero murio";
        }

        window.display();
    }

    return 0;
}
