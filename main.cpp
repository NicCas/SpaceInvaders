#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc32-c"
// Final game

#include <SFML/Graphics.hpp>
#include <time.h>
#include<iostream>
#include <string>


// globals dimension for window
int num_vertBox = 30, num_horiBox = 40;
int pixelSize = 16; // num of pixels
int w = pixelSize * num_horiBox; // pixel in width
int h = pixelSize * num_vertBox; // pixel in height

int score = 0; // Global score counter

int direction; // of ship
bool shoot = false; // is the a bullet being drawn right now?

// Defining the ship
struct spaceship
{
    // ship can only move on x axis, y is fixed
    int x = 0, y = h - pixelSize;

}ship;

// moving the ship
void moveShip ()
{

    // If user presses left, goes left
    if (direction == 1)
    {
        ship.x -= pixelSize;
    }

    // If user presses right, goes right
    if (direction == 2)
    {
        ship.x += pixelSize;
    }


    // Boundary Checking ship. If it hits the edge of the window, it cannot pass
    // Because the ship is not one 't' shaped object but rather the same texture repeated to appear as a 't' shape
    // the dimensions for checking have to be modified to take into account that what appears to be the right side of
    // the ship is not the same as ship.x
    if (ship.x + (4 * pixelSize) > w)
    {
        ship.x = w - (3 * pixelSize);
    }
    if (ship.x < 0)
    {
        ship.x = 0;
    }
}

// Defining the comet
struct comet
{
    int x,y;

}obstacle;

void newComet()
{
    // randomly generates an x value in a [1, 39] range, catching the two columns the ship can't shoot at because of
    // its wings
    int random = rand() % (num_horiBox - 1) + 1;
    random *= pixelSize;

    obstacle.x = random;
    obstacle.y = 0;
}

void moveComet ()
{
    // Commet moves at 1/4 the speed of a bullet
    obstacle.y +=8;

    // If comet reaches the bottom reduce the score and reset the comet
    if (obstacle.y > h)
    {
        score -= 1;
        newComet();
    }
}

// Defining the bullet
struct gun
{
    int x, y;

}projectile;

// Set the initial bullet position to ship's at-the-moment x and y
void bulletFired ()
{
    // Want to set the bullet to the ship's 'nose' which is 16 pixels right and 16 pixels up of ship's x and y
    projectile.x = ship.x + pixelSize;
    projectile.y = ship.y - pixelSize;

}

int moveBullet ()
{
    // Bullet moves faster than the ship and the comet
    projectile.y -= 32;

    // Bullet and comet are moving at different speeds, so need to adjust for the x's only appearing to be equal due
    // to the computer's refresh rate by factoring in a range for the y value
    if (projectile.x == obstacle.x) // Check if bullet hit comet 1. Do they have the same x value
    {
        // 2. Make sure to reset comet only when the bullet has actually hit it, therefore check for when the y's equal
        // or are within 16 pixels of each other
        if ((projectile.y == obstacle.y) || ( (projectile.y - pixelSize <= obstacle.y) && (obstacle.y <= projectile.y + pixelSize ) ))
        {
            score += 1;
            newComet(); // Reset comet
            return 1;

        } else { // If not in that range yet continue drawing bullet and comet

            return 3;
        }
    } else if ((projectile.y < 0)){ // if bullet reaches the end of the screen

        return 2;

    } else { // if nothing happens continue to draw bullet and comet
        return 3;
    }
}

int main ()
{
    //Setting pseudorandom time,
    srand(time(0));

    // Window we can play
    sf::RenderWindow window (sf::VideoMode(w,h), "Space Invaders");

    // Textures load image into the GPU
    sf::Texture t1, t2, t3, t4;
    t1.loadFromFile("images/blue.jpg");
    t2.loadFromFile("images/red.png");
    t3.loadFromFile("images/brown.png");
    t4.loadFromFile("images/white.png");

    // Sprite has physical dimensions
    sf::Sprite background(t1);
    sf::Sprite icon(t2);
    sf::Sprite comet(t3);
    sf::Sprite bullet(t4);

    //Declare a Font
    sf::Font MarkerFelt;

    //Load and check the availability of the font file
    if(!MarkerFelt.loadFromFile("MarkerFelt.ttc"))
    {
        std::cout << "Error: can't load font" << std::endl;
    }

    // Change int score to a string
    std::string str = std::to_string(score);

    // Create score board
    sf::Text scoreBoard("Score: " + str, MarkerFelt);
    scoreBoard.setCharacterSize(50);
    scoreBoard.setFillColor(sf::Color::White);

    // Create first comet
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !shoot) // Press up to shoot
        {
            shoot = true;
            bulletFired();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) direction = 1; // move ship left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) direction = 2; // move ship right

        if (timer > delay)
        {
            timer = 0; // reset time
            moveShip(); // move ship 16 pixels forward or backward

            if (shoot) // if the user pressed shoot
            {

                updateBullet = moveBullet(); // See if the bullet hit the edge of a comet

                // If the bullet encounters something, can shoot again
                if (updateBullet == 1 || updateBullet == 2)
                {
                    shoot = false;
                }
            }

            moveComet(); // move comet down 16 pixels
        }

        // Draw in window
        window.clear(); // get rid of all frame

        // Draw background
        background.setPosition(0,0);
        window.draw(background);

        //draw ship's four boxes to make up 't'shape
        icon.setPosition (ship.x, ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + pixelSize, ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + (2 * pixelSize), ship.y);
        window.draw(icon);

        icon.setPosition (ship.x + pixelSize , ship.y - pixelSize);
        window.draw(icon);

        // Draw bullet
        if (shoot)
        {
            bullet.setPosition (projectile.x, projectile.y);
            window.draw(bullet);
        }

       // Draw comet
        comet.setPosition(obstacle.x, obstacle.y);
        window.draw(comet);

        // Update score board
        std::string str = std::to_string(score);
        scoreBoard.setString("Score: " + str);

        // Draw score board
        window.draw(scoreBoard);

        // Display it all
        window.display();


    }

}