#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;


const Color
    LIGHT(255,255, 255),
    DARK(0, 0, 0);

View
    view(sf::FloatRect(0, 0, 200, 200));

const int 
    w_scale     = 200,
    h_scale     = 120,

    size        = 4,

    width       = size*w_scale,
    height      = size*h_scale;



const char *
    dirs        = "LRUD";



int 
    cam         = 0,
    cam_posX    = width/2,
    cam_posY    = height/2;

struct screen{
    Color tile;
}Screen[h_scale][w_scale];

class Ant{
    private:

    int x, y;
    char dir;

    void swapTile()
    {
        if (Screen[this->y][this->x].tile == DARK)
            Screen[this->y][this->x].tile = LIGHT;
        else
            Screen[this->y][this->x].tile = DARK;
    }

    // ant possible moves
    void moveDown()
    {
        swapTile();

        if (this->y < h_scale-1)
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
            this->y = h_scale-1;
    }
    void moveLeft(){
        swapTile();

        if(this->x > 0)
            this->x--;
        else
            this->x = w_scale-1;
    }
    void moveRight()
    {
        swapTile();
        if(this->x < w_scale-1)
            this->x++;
        else
            this->x = 0;
    }

    public:
    // new ant
    Ant()
    {
        // set ant to the center of screen

        this->x = w_scale/2;
        this->y = h_scale/2;
        // std::cout << this->x << "-" << this->y << std::endl;
        this->dir = dirs[rand() % 4];
        
    }
    void Move()
    {
        bool light = Screen[this->y][this->x].tile == LIGHT;
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

void moveCam(int x)
{
    int i = 0;
    i+=3;
    Vector2f pos;
    switch(x)
    {
        case 1:
            pos = (view.getCenter()-Vector2f(i, 0));
            break;
        case 2:
            pos = (view.getCenter())+Vector2f(i, 0);
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
    for (i = 0; i < w_scale; i++){
            for (j = 0; j < h_scale; j++){
                Screen[j][i].tile = LIGHT;
            }
    }
        
}

int main()
{
    RenderWindow win(VideoMode(width, height), "Langton's Ant");


    // Tiles
    RectangleShape tile(Vector2f(size, size));

    // Ant
    Ant *ant1 = new Ant();

    init_components();

    // tile.setOutlineThickness(1);
    // tile.setOutlineColor(Color(50, 50, 50));


    Event e;

    // Draw
    while(win.isOpen())
    {

        // Handling events
        while(win.pollEvent(e))
        {
            if(e.type == Event::Closed)
                win.close();
        }
        
        if(Keyboard::isKeyPressed(Keyboard::Left))
            cam = 1;
        else
        if(Keyboard::isKeyPressed(Keyboard::Right))
            cam = 2;
        else
        if(Keyboard::isKeyPressed(Keyboard::Up))
            cam = 3;
        else
        if(Keyboard::isKeyPressed(Keyboard::Down))
            cam = 4;

        if (cam == 0)
            view.setCenter(ant1->pos()[0]*size, ant1->pos()[1]*size);
        else
          moveCam(cam);
        

        win.setView(view);
        
        ant1->Move();
        
        for (int i = 0; i < w_scale; i++)
            for (int j = 0; j < h_scale; j++){
                tile.setFillColor(Screen[j][i].tile);
                tile.setPosition(i*size, j*size);
                win.draw(tile);
            }
        

        win.display();
        win.clear();
    }
}