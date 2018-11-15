#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;


const Color
    LIGHT(1,1, 1),
    DARK(0, 0, 0);

View
    view(sf::FloatRect(0, 0, 400, 400));

const int 
    scaleW      = 500,
    scaleH      = 320,

    size        = 2,

    dim[]       = {size*scaleW, size*scaleH};

int 
    // r1 = rand() % 255,
    // g1 = rand() % 255,
    // b1 = rand() % 255,

    // r2 = rand() % 255,
    // g2 = rand() % 255,
    // b2 = rand() % 255;

    r1          = 190,
    g1          = 0,
    b1          = 240,

    r2          = 255,
    g2          = 28,
    b2          = 0;


const char *
    dirs        = "LRUD";



int 
    cam         = 0;


struct screen{
    Color tile;
}Screen[scaleH][scaleW];

class Ant{
    private:

    int x, y;
    char dir;
    Color ant_color;

    void swapTile()
    {
        if (Screen[this->y][this->x].tile == DARK)
            Screen[this->y][this->x].tile = this->ant_color;
        else
            Screen[this->y][this->x].tile = DARK;
    }

    // ant possible moves
    void moveDown()
    {
        swapTile();

        if (this->y < scaleH-1)
            this->y++;
        else
            this->y = 0;
    }
    void moveUp()
    {
        swapTile();

        if (this->y > 0)
            this->y--;
        else
            this->y = scaleH-1;
    }
    void moveLeft(){
        swapTile();

        if(this->x > 0)
            this->x--;
        else
            this->x = scaleW-1;
    }
    void moveRight()
    {
        swapTile();
        if(this->x < scaleW-1)
            this->x++;
        else
            this->x = 0;
    }

    public:
    // new ant
    Ant(int r, int g, int b)
    {
        // set ant to the center of screen
        // this->x = scaleW/2;
        // this->y = scaleH/2;

        

        // "random" position
        this->x = rand() % scaleW;
        this->y = rand() % scaleH;

        // debugging position
        // std::cout << this->x << "-" << this->y << std::endl;

        // ant color
        this->ant_color = Color(r, g, b);

        this->dir = dirs[rand() % 4];
        
    }
    void Move()
    {
        bool light = 
            Screen[this->y][this->x].tile == Color(r1, g1, b1) 
        ||  Screen[this->y][this->x].tile == Color(r2, g2, b2)
        ||  Screen[this->y][this->x].tile == LIGHT;

        // On Light Turn Right
        switch(this->dir)
        {
            case 'U':
                if (light)
                {
                    this->moveLeft();
                    this->dir = 'R';
                }else
                {
                    this->moveRight();
                    this->dir = 'L';
                }
                break;
            case 'D':
                if (light)
                {
                    this->moveRight();
                    this->dir = 'L';
                }
                else
                {
                    this->moveLeft();
                    this->dir = 'R';
                }
                break;
            case 'L':
                if (light)
                {
                    this->moveDown();
                    this->dir = 'U';
                }
                else
                {
                    this->moveUp();
                    this->dir = 'D';
                }
                break;

            case 'R':
                if (light)
                {
                    this->moveUp();
                    this->dir = 'D';
                }
                else
                {
                    this->moveDown();
                    this->dir = 'U';
                }
                break;
        }
    }const
    int *pos()
    {
        int *pos = (int *) malloc(2);
        *(pos)      = this->x;
        *(pos+1)    = this->y;
        return pos;
    }

};

void moveCam()
{
    int i = 20;
    Vector2f pos;
    switch(cam)
    {
        case 1:
            pos = (view.getCenter()-Vector2f(i, 0));
            break;
        case 2:
            pos = (view.getCenter()+Vector2f(i, 0));
            break;
        case 3:
            pos = (view.getCenter()-Vector2f(0, i));
            break;
        case 4:
            pos = (view.getCenter()+Vector2f(0, i));
            break;
        default:
            break;
    };
    view.setCenter(pos);
}

void init_components()
{
    srand(time(0));
    int i = 0, j = 0;

    //screen presets
    for (i = 0; i < scaleW; i++){
            for (j = 0; j < scaleH; j++){
                Screen[j][i].tile = LIGHT;
            }
    }
        
}

int main()
{
    RenderWindow win(VideoMode(dim[0], dim[1]), "Langton's Ant");
    win.setFramerateLimit(100);

    // Tiles
    RectangleShape tile(Vector2f(size, size));

    // Ant
    Ant *ant1 = new Ant(r1, g1, b1);
    Ant *ant2 = new Ant(r2, g2, b2);


    init_components();

    // tile.setOutlineThickness(1);
    // tile.setOutlineColor(Color(50, 50, 50));


    Event e;

    // Draw
    for (int i = 0; i < scaleW; i++)
        for (int j = 0; j < scaleH; j++){
            tile.setFillColor(Screen[j][i].tile);
            tile.setPosition(i*size, j*size);
            win.draw(tile);
        }
    win.clear();
    while(win.isOpen())
    {

        // Handling events
        while(win.pollEvent(e))
        {
            if(e.type == Event::Closed)
                win.close();
        }
        // cam = 0;
        // if(Keyboard::isKeyPressed(Keyboard::Left))
        //     cam = 1;
        // if(Keyboard::isKeyPressed(Keyboard::Right))
        //     cam = 2;
        // if(Keyboard::isKeyPressed(Keyboard::Up))
        //     cam = 3;
        // if(Keyboard::isKeyPressed(Keyboard::Down))
        //     cam = 4;
        // if (cam == 0)
        //     view.setCenter(ant1->pos()[0]*size, ant1->pos()[1]*size);
        // else
        //     moveCam();

        

        //win.setView(view);
        
        ant1->Move();
        tile.setFillColor(Screen[ant1->pos()[1]][ant1->pos()[0]].tile);
        tile.setPosition(ant1->pos()[0] * size, ant1->pos()[1] * size);
        win.draw(tile);

        ant2->Move();
        tile.setFillColor(Screen[ant2->pos()[1]][ant2->pos()[0]].tile);
        tile.setPosition(ant2->pos()[0] * size, ant2->pos()[1] * size);
        win.draw(tile);
        

        win.display();
    }
}