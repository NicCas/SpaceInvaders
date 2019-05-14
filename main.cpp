/*
 * finish bullet
 * finish comet
 * create library
*/


// Final game

#include <SFML/Graphics.hpp>
#include <time.h>

// sf = std for this library

// globals dimension for window
int num_vertBox = 30, num_horiBox = 50;
int size = 16; // num of pixels
int w = size * num_horiBox; // pixel in width
int h = size * num_vertBox; // pixel in height

int direction;
bool shoot = false;

// Building the ship

struct spaceship
{
    int x, y = h - size;

}ship;

// moving the ship
void move ()
{

    // If user presses left
    if (direction == 1)
    {
        ship.x -= size;
    }


    // If user presses right
    if (direction == 2)
    {
        ship.x += size;
    }


    // Boundary Checking ship as hits screen end, cannot pass
    if (ship.x + (4* size) > w)
        ship.x = w - (3 * size);
    if (ship.x < 0)
        ship.x = 0;

}

// Building the comet

struct comet
{
    int x,y;

}obstacle;

void newComet()
{
    int random = rand() % num_horiBox;
    random *= size;

    obstacle.x = random;
    obstacle.y = 0;
}

// Building the bullet

struct gun
{
    int x, y;

}projectile;

// Set the initial bullet position to ship's x and y
void bulletFired ()
{
    projectile.x = ship.x + size;
    projectile.y = ship.y - size;

}


int continueBullet ()
{
    projectile.y -= 16;

    if ((projectile.x == obstacle.x) && (projectile.y == obstacle.y)) // If comet gets hit
    {
        newComet();
        return 1;

    } else if ((projectile.y < 0)){ // if projectile reaches the end of the screen
        return 2;

    } else { // if nothing happens to to projectile
        return 3;
    }
}

int main ()
{
    //Setting pseudorandom time, TODO:discuss true random vs pseudorandom
    srand(time(0));

    // Window we can play
    sf::RenderWindow window (sf::VideoMode(w,h), "Space Invadors");

    // Textures load image into the GPU
    sf::Texture t1, t2, t3, t4;
    t1.loadFromFile("images/blue.jpg");
    t2.loadFromFile("images/red.png");
    t3.loadFromFile("images/green.png");
    t4.loadFromFile("images/white.png");

    // Sprite has physical dimensions
    sf::Sprite background(t1);
    sf::Sprite icon(t2);
    sf::Sprite comet(t3);
    sf::Sprite bullet(t4);

    //***NEW*** initially place food somewhere on screen
    newComet();

    sf::Clock clock;
    float timer = 0.0f, delay = 0.1f;

    int updateBullet;

    // The amount of times it takes to go through the while loop if your frame rate
    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        sf::Event e;

        // check when the window is closed
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Control for ship by user
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && shoot == false)
        {
            shoot = true;
            bulletFired();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) direction = 1;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) direction = 2;

        if (timer > delay)
        {
            timer = 0; // reset time
            move(); // move snake one forward


            if (shoot == true)
            {
                updateBullet = continueBullet();

                // If the bullet encounters something, can shoot again
                if (updateBullet == 1 || updateBullet == 2)
                {
                    shoot = false;
                }
            }
        }

        // Draw in window
        window.clear(); // get rid of all frame

        // Draw background
        background.setPosition(0,0);
        window.draw(background);


        //draw ship
        icon.setPosition (ship.x, ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + size, ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + (2 * size), ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + size , ship.y - size);
        window.draw(icon);


        // Draw bullet
        shoot = continueBullet();
        if (shoot == true)
        {

            bullet.setPosition (projectile.x, projectile.y);
            window.draw(bullet);
        }

        /*

        //***NEW*** 3rd: Draw Fruit
        comet.setPosition(food.x*size, food.y*size);
        window.draw(comet);

        */

        window.display();


    }

}