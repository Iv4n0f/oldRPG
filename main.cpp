#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

void renderizarAtaque(sf::RenderWindow *window, sf::Sprite sprite_ataque, bool derecha) // 100x100
{
    if (derecha == true)
    {
        sprite_ataque.setPosition(700, 50);
        sprite_ataque.setScale(-1.0f, 1.0f);
    }
    else
    {
        sprite_ataque.setPosition(100, 50);
        sprite_ataque.setScale(1.0f, 1.0f);
    }
    window->draw(sprite_ataque);
    window->display();
    sf::sleep(sf::seconds(1));
}

void renderizarCentro(sf::RenderWindow *window, sf::Sprite sprite_centro) // 400x400
{
    sprite_centro.setPosition(200, 50);
    window->draw(sprite_centro);
    window->display(); // Mostrar la ventana
    sf::sleep(sf::seconds(1));
}

void renderizarHUD(sf::RenderWindow *window, sf::Sprite sprite_hud) // 800x200
{
    sprite_hud.setPosition(0, 400);
    window->draw(sprite_hud);
    window->display(); // Mostrar la ventana
    sf::sleep(sf::seconds(1));
}
class ObjetoJuego
{
public:
    ObjetoJuego(const std::string &filename, sf::RenderWindow *windowPtr, bool derecha)
        : window(windowPtr)
    {
        this->derecha = derecha;
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

    void renderizar()
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
        renderizarCentro(window, failSprite);
    }

    bool getDerecha() { return derecha; }

protected:
    sf::RenderWindow *window;
    sf::Texture texture;
    sf::Sprite sprite;
    sf::Texture failTexture;
    sf::Sprite failSprite;
    bool derecha;
};

class Personaje : public ObjetoJuego
{
protected:
    int vida;
    int daño;
    sf::Texture textura_ataque;
    sf::Sprite sprite_ataque;
    sf::Texture textura_hud;
    sf::Sprite sprite_hud;

public:
    Personaje(const std::string &filename, sf::RenderWindow *windowPtr, bool derecha)
        : ObjetoJuego(filename, windowPtr, derecha)
    {
        textura_ataque.loadFromFile("ataque.png");
        sprite_ataque.setTexture(textura_ataque);
    }
    int getVida() { return vida; }
    int getDaño() { return daño; }
    void setVida(int vida) { this->vida = vida; }
    void setDaño(int daño) { this->daño = daño; }

    virtual void ataque(Personaje &objetivo)
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

            renderizarAtaque(window, sprite_ataque, objetivo.getDerecha());
        }
    }

    sf::Sprite getHUD() { return sprite_hud; }
};

class Guerrero : public Personaje
{
protected:
    sf::Texture textura_berserker;
    sf::Sprite sprite_berserker;
    sf::Texture textura_curarse;
    sf::Sprite sprite_curarse;

public:
    Guerrero(const std::string &filename, sf::RenderWindow *windowPtr, bool derecha)
        : Personaje(filename, windowPtr, derecha)
    {
        vida = 130;
        daño = 10;
        textura_berserker.loadFromFile("berserker.png");
        sprite_berserker.setTexture(textura_berserker);
        textura_curarse.loadFromFile("curarse.png");
        sprite_curarse.setTexture(textura_curarse);
        textura_hud.loadFromFile("hud_guerrero.png");
        sprite_hud.setTexture(textura_hud);
    }

    void primera(Personaje &objetivo) // BERSERKER
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
            renderizarAtaque(window, sprite_berserker, objetivo.getDerecha());
        }
    }

    void segunda(Personaje &objetivo) // CURAR
    {
        int random = rand() % 100;
        if (random < 33)
        {
            renderizarFallo();
        }
        else
        {
            vida += 15;
            renderizarAtaque(window, sprite_curarse, derecha);
        }
    }
};

class Mago : public Personaje
{
    sf::Texture textura_robar;
    sf::Sprite sprite_robar;
    sf::Texture textura_50_50;
    sf::Sprite sprite_50_50;

public:
    Mago(const std::string &filename, sf::RenderWindow *windowPtr, bool derecha)
        : Personaje(filename, windowPtr, derecha)
    {
        vida = 70;
        daño = 18;
        textura_robar.loadFromFile("robar.png");
        sprite_robar.setTexture(textura_robar);
        textura_50_50.loadFromFile("50_50.png");
        sprite_50_50.setTexture(textura_50_50);
        textura_hud.loadFromFile("hud_mago.png");
        sprite_hud.setTexture(textura_hud);
    }

    void primera(Personaje &objetivo) // ROBAR
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
            renderizarCentro(window, sprite_robar);
        }
        else
        {
            // 33% robar 15 de vida del objetivo
            int vida_enemigo = objetivo.getVida();
            objetivo.setVida(vida_enemigo - 15);
            vida += 15;
            renderizarCentro(window, sprite_robar);
        }
    }

    void segunda(Personaje &objetivo) // 50/50
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
            renderizarAtaque(window, sprite_50_50, objetivo.getDerecha());
        }
    }
};

class Arquero : public Personaje
{
    sf::Texture textura_critico;
    sf::Sprite sprite_critico;

public:
    Arquero(const std::string &filename, sf::RenderWindow *windowPtr, bool derecha)
        : Personaje(filename, windowPtr, derecha)
    {
        vida = 100;
        daño = 15;
        textura_critico.loadFromFile("critico.png");
        sprite_critico.setTexture(textura_critico);
        textura_hud.loadFromFile("hud_arquero.png");
        sprite_hud.setTexture(textura_hud);
    }

    void ataque(Personaje &objetivo) // Redefinir ataque para que sea más preciso
    {
        int random = rand() % 100;
        if (random < 10) // Menos probabilidad de fallar
        {
            renderizarFallo();
        }
        else
        {
            int daño = getDaño();
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
            renderizarAtaque(window, sprite_ataque, objetivo.getDerecha());
        }
    }

    void primera(Personaje &objetivo) // PRECISION
    {
        vida -= 13;

        int daño = getDaño() * 2;
        int vida = objetivo.getVida();
        vida -= daño;
        objetivo.setVida(vida);
        renderizarAtaque(window, sprite_critico, objetivo.getDerecha());
    }

    void segunda(Personaje &objetivo) // CRITICO
    {
        int random = rand() % 100;
        if (random < 33)
        {
            int daño = getDaño() + 10;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
            renderizarAtaque(window, sprite_ataque, objetivo.getDerecha());
        }
        else
        {
            int daño = getDaño() - 5;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
            renderizarAtaque(window, sprite_critico, objetivo.getDerecha());
        }
    }
};

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(800, 600), "JUEGO");

    // FUENTE
    sf::Font font;
    if (!font.loadFromFile("Pixeled.ttf"))
    {
        std::cout << "Error cargando la fuente" << std::endl;
        return EXIT_FAILURE;
    }

    // Crear personajes
    std ::cout << "Personaje izquierda: " << std::endl;
    std ::cout << "1. Guerrero" << std::endl;
    std ::cout << "2. Mago" << std::endl;
    std ::cout << "3. Arquero" << std::endl;
    int opcion;
    std ::cin >> opcion;
    Personaje *personaje_izquierda;
    if (opcion == 1)
    {
        personaje_izquierda = new Guerrero("guerrero.png", &window, false);
    }
    else if (opcion == 2)
    {
        personaje_izquierda = new Mago("mago.png", &window, false);
    }
    else if (opcion == 3)
    {
        personaje_izquierda = new Arquero("arquero.png", &window, false);
    }
    std::cout << "Personaje derecha: " << std::endl;
    std::cout << "1. Guerrero" << std::endl;
    std::cout << "2. Mago" << std::endl;
    std::cout << "3. Arquero" << std::endl;
    int opcion2;
    std::cin >> opcion2;
    Personaje *personaje_derecha;
    if (opcion2 == 1)
    {
        personaje_derecha = new Guerrero("guerrero.png", &window, true);
    }
    else if (opcion2 == 2)
    {
        personaje_derecha = new Mago("mago.png", &window, true);
    }
    else if (opcion2 == 3)
    {
        personaje_derecha = new Arquero("arquero.png", &window, true);
    }

    // BUCLE JUEGO
    bool juego_terminado = false;                  // Variable para controlar el fin de juego
    Personaje *turno_actual = personaje_izquierda; // Empezamos con el jugador izquierda
    while (window.isOpen() && !juego_terminado)
    {
        // Otros procesamientos del bucle principal

        // Verificación del fin de juego
        if (turno_actual->getVida() <= 0)
        {
            std::cout << "Juego terminado. El jugador " << (turno_actual == personaje_izquierda ? "derecha" : "izquierda") << " ha perdido." << std::endl;
            juego_terminado = true;
            continue;
        }

        // Mostrar HUD correspondiente al turno actual
        if (turno_actual == personaje_izquierda)
        {
            renderizarHUD(&window, personaje_izquierda->getHUD());
        }
        else
        {
            renderizarHUD(&window, personaje_derecha->getHUD());
        }

        // Esperar entrada del jugador
        // Procesamiento de la entrada del jugador
        // Resolución del ataque
        // Renderizado del ataque

        // Cambiar el turno
        turno_actual = (turno_actual == personaje_izquierda) ? personaje_derecha : personaje_izquierda;
    }

    return 0;
}
