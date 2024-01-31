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

    virtual void primera(Personaje &objetivo) {}
    virtual void segunda(Personaje &objetivo) {}

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
        setVida(getVida() - 15);

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
            setVida(getVida() + 15);
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
            setDaño(getDaño() + 1);
            renderizarCentro(window, sprite_robar);
        }
        else
        {
            // 33% robar 15 de vida del objetivo
            int vida_enemigo = objetivo.getVida();
            objetivo.setVida(vida_enemigo - 15);
            setVida(getVida() + 15);
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
        daño = 13;
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
        setVida(getVida() - 15);

        int daño = getDaño() + 10;
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
            int daño = getDaño() + 15;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
            renderizarAtaque(window, sprite_ataque, objetivo.getDerecha());
        }
        else
        {
            int daño = getDaño() - 6;
            int vida = objetivo.getVida();
            vida -= daño;
            objetivo.setVida(vida);
            renderizarAtaque(window, sprite_critico, objetivo.getDerecha());
        }
    }
};

void renderizarEstadoDelJuego(sf::RenderWindow *window, Personaje *personaje_izquierda, Personaje *personaje_derecha, sf::Sprite sprite_fondo, sf::Sprite sprite_vida, sf::Sprite sprite_daño, sf::Font font)
{
    sprite_fondo.setPosition(0, 0);
    window->draw(sprite_fondo);

    sf::Text text_vida_izquierda;
    text_vida_izquierda.setFont(font);
    text_vida_izquierda.setCharacterSize(16);
    text_vida_izquierda.setFillColor(sf::Color::Red);
    text_vida_izquierda.setPosition(40, 10);
    text_vida_izquierda.setString(std::to_string(personaje_izquierda->getVida()));
    sprite_vida.setPosition(10, 10);
    window->draw(sprite_vida);
    window->draw(text_vida_izquierda);

    sf::Text text_vida_derecha;
    text_vida_derecha.setFont(font);
    text_vida_derecha.setCharacterSize(16);
    text_vida_derecha.setFillColor(sf::Color::Red);
    text_vida_derecha.setPosition(window->getSize().x - 80, 10);
    text_vida_derecha.setString(std::to_string(personaje_derecha->getVida()));
    sprite_vida.setPosition(window->getSize().x - 40, 10);
    window->draw(sprite_vida);
    window->draw(text_vida_derecha);

    sf::Color cafe(200, 150, 0);
    sf::Text text_daño_izquierda;
    text_daño_izquierda.setFont(font);
    text_daño_izquierda.setCharacterSize(16);
    text_daño_izquierda.setFillColor(cafe);
    text_daño_izquierda.setPosition(40, 36);
    text_daño_izquierda.setString(std::to_string(personaje_izquierda->getDaño()));
    sprite_daño.setPosition(10, 30);
    window->draw(sprite_daño);
    window->draw(text_daño_izquierda);

    sf::Text text_daño_derecha;
    text_daño_derecha.setFont(font);
    text_daño_derecha.setCharacterSize(16);
    text_daño_derecha.setFillColor(cafe);
    text_daño_derecha.setPosition(window->getSize().x - 80, 36);
    text_daño_derecha.setString(std::to_string(personaje_derecha->getDaño()));
    sprite_daño.setPosition(window->getSize().x - 40, 30);
    window->draw(sprite_daño);
    window->draw(text_daño_derecha);

    personaje_izquierda->renderizar();
    personaje_derecha->renderizar();
}

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

    // TEXTURAS
    sf::Texture textura_fondo;
    textura_fondo.loadFromFile("fondo.png");
    sf::Sprite sprite_fondo;
    sprite_fondo.setTexture(textura_fondo);

    sf::Texture textura_gameover;
    textura_gameover.loadFromFile("gameover.png");
    sf::Sprite sprite_gameover;
    sprite_gameover.setTexture(textura_gameover);

    sf::Texture indicador_turno;
    indicador_turno.loadFromFile("flecha.png");
    sf::Sprite sprite_indicador_turno;
    sprite_indicador_turno.setTexture(indicador_turno);

    sf::Texture textura_vida;
    textura_vida.loadFromFile("vida.png");
    sf::Sprite sprite_vida;
    sprite_vida.setTexture(textura_vida);

    sf::Texture textura_daño;
    textura_daño.loadFromFile("daño.png");
    sf::Sprite sprite_daño;
    sprite_daño.setTexture(textura_daño);

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

    /* Personaje *personaje_izquierda;
    Personaje *personaje_derecha;

    personaje_izquierda = new Guerrero("guerrero.png", &window, false);
    personaje_derecha = new Mago("mago.png", &window, true); */

    // BUCLE JUEGO
    bool juego_terminado = false;                  // Variable para controlar el fin de juego
    Personaje *turno_actual = personaje_izquierda; // Empezamos con el jugador izquierda
    Personaje *enemigo_actual = personaje_derecha; // Empezamos con el jugador derecha
    renderizarEstadoDelJuego(&window, personaje_izquierda, personaje_derecha, sprite_fondo, sprite_vida, sprite_daño, font);

    while (window.isOpen() && !juego_terminado)
    {
        // Verificación del fin de juego
        if (turno_actual->getVida() <= 0)
        {
            std::cout << "Juego terminado. El jugador " << (turno_actual == personaje_izquierda ? "derecha" : "izquierda") << " ha ganado." << std::endl;
            juego_terminado = true;
            renderizarEstadoDelJuego(&window, personaje_izquierda, personaje_derecha, sprite_fondo, sprite_vida, sprite_daño, font);
            sprite_gameover.setPosition(100, 50);
            window.draw(sprite_gameover);
            window.display();
            sf::sleep(sf::seconds(3));
        }

        // Mostrar HUD correspondiente al turno actual
        if (turno_actual == personaje_izquierda)
        {
            sprite_indicador_turno.setPosition(100, 20);
            window.draw(sprite_indicador_turno);
            renderizarHUD(&window, personaje_izquierda->getHUD());
        }
        else
        {
            sprite_indicador_turno.setPosition(window.getSize().x - 200, 20);
            window.draw(sprite_indicador_turno);
            renderizarHUD(&window, personaje_derecha->getHUD());
        }

        window.display();
        sf::sleep(sf::milliseconds(100));

        // Manejar eventos del teclado para esperar la entrada del jugador
        sf::Event event;
        bool jugada_realizada = false;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                // Procesar la tecla presionada
                if (event.key.code == sf::Keyboard::Z)
                {
                    turno_actual->ataque(*enemigo_actual);
                    jugada_realizada = true;
                }
                else if (event.key.code == sf::Keyboard::X)
                {
                    turno_actual->primera(*enemigo_actual);
                    jugada_realizada = true;
                }
                else if (event.key.code == sf::Keyboard::C)
                {
                    turno_actual->segunda(*enemigo_actual);
                    jugada_realizada = true;
                }
            }
        }

        // Cambiar el turno solo si se realizó una jugada en este turno
        if (jugada_realizada)
        {
            // Renderizar el estado actual del juego despues del ataque
            window.clear();
            renderizarEstadoDelJuego(&window, personaje_izquierda, personaje_derecha, sprite_fondo, sprite_vida, sprite_daño, font);
            window.display();
            sf::sleep(sf::milliseconds(100));
            turno_actual = (turno_actual == personaje_izquierda) ? personaje_derecha : personaje_izquierda;
            enemigo_actual = (enemigo_actual == personaje_izquierda) ? personaje_derecha : personaje_izquierda;
        }
        sf::sleep(sf::milliseconds(500));
    }

    return 0;
}
