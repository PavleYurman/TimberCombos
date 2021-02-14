// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Include important libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
// sstream is a standard header for manipulatin strings
#include <sstream>  

// Make code easier to type with “using namespace”
using namespace sf;

// Function declaration
void positionBranches();
void updateBranches(int seed);
int const NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player / branch?
// Left or right
enum class side
{
    LEFT,
    RIGHT,
    NONE
};
side branchPositions[NUM_BRANCHES];
bool buttonIsPressed = false;
float branchHight = 0;

// This is where our game starts from
int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Trimber!!!", Style::Fullscreen);
    // Create a texture to hold a graphic on the GPU
    Texture textureBackground;
    // Load a graphic into the texture
    textureBackground.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/background.png");
    // Create a sprite
    Sprite spriteBackground;
    // Attach the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    // Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);

    // Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(820, 0);

    // Prepare the bee
    Texture textureBee;
    textureBee.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    // Is the bee currently moving?
    bool beeActive = false;
    // How fast is the bee moving?
    float beeSpeed = 0.0f;

    // make 3 cloud sprites from 1 texture
    Texture textureCloud;
    textureCloud.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/cloud.png");
    // 3 New sprites with the same texture
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    // Position the clouds on the left of the screen
    // at different heights
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(50, 40);
    spriteCloud3.setPosition(100, 70);
    // Are the clouds currently on screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    // How fast is each cloud?    
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    // Variables to control time itself    
    Clock clock;
    Time timePerFrame;
    // Time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - (timeBarStartWidth / 2), 980);
    Time gameTimeTotal;
    float timeRemaining = 50.0f;
    float timeLowerBranch = 0.7f;
    // That many pixels will the bar shrink per second; used for resizing timeBar each frame
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining; // How did he got seconds ?

    // Track whether the game is running
    bool paused = true;

    // Draw some text
    int score = 0;
    Text messageText;  // for indicating the state of the game
    Text scoreText; // indicate score for player
    // Chooze a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Set the font to our message
    messageText.setFont(font);
    scoreText.setFont(font);
    // Assighn the actual message
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");
    // Make it realy big
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    // Chooze a color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    // Position the text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20.0f, 20.0f);
    // Prepare 6 branches
    // Texture branch
    Texture textureBranch;
    textureBranch.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/branch.png");       
    // Set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(0, 0);
        // Set sprites origin to dead centre  
        branches[i].setOrigin(220, 20); // The picture is 440 x 80 in dimensions ? Shoud this be 220, 40 enstead ?
    }
    
    // Initialize branch positions for branches 0 to 5
    //for (int i = 0; i < NUM_BRANCHES; i++)
    //{
    //    srand((int)time(0) + i);
    //    int r = rand() % 5;
    //    switch (r)
    //    {
    //    case 0:
    //        branchPositions[i] = side::LEFT;
    //        break;
    //    case 1:
    //        branchPositions[i] = side::RIGHT;
    //        break;
    //    default:
    //        branchPositions[i] = side::NONE;
    //        break;
    //    }
    //}    
    /*updateBranches(1);*/
    /*updateBranches(2);
    updateBranches(3);
    updateBranches(4);
    updateBranches(5);*/
    while (window.isOpen()) {

        /* ****************************************
        Handle the players input
        **************************************** */
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        // Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return))
        {
            paused = false;
            // Reset the time and the score
            timeRemaining = 50.0f;
            timeLowerBranch = 0.7f;
            branchHight = 0;
            score = 0;               
        }
        
       
        /* ****************************************
        Update the scene
        **************************************** */
        if (!paused)
        {
            // Measure time
            Time dt = clock.restart();
            // Substract from the amount of time remaining
            timeRemaining -= dt.asSeconds();
            // Size up the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));
            // Continue: detect what happens if time runs out ...
            if (timeRemaining <= 0.0f)
            {
                // Pause the game
                paused = true;
                // Change the message shown to the player               
                messageText.setString("Out of time!!");
                // Reposition the text based on its new size
                FloatRect rectText = messageText.getLocalBounds();
                // Move origin of text to the center of the text
                messageText.setOrigin(rectText.left + (rectText.width / 2.0f),
                    (rectText.top + (rectText.height / 2.0f)));
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
            }
            timePerFrame = dt;

            // Set up the bee
            if (!beeActive)
            {
                // How fast is the bee?
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200; // 200px - 399px
                // How high is the bee?
                srand((int)time(0) * 10);
                float height = (rand() % 1350) + 500; // 500px - 1849px --> y // The scope of height just in if statement
                // Set position
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else
                // Move the bee
            {
                spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()), spriteBee.getPosition().y);
                if (spriteBee.getPosition().x < -100)
                {
                    beeActive = false;
                }
            }
            // Manage the clouds
            // Cloud 1
            if (!cloud1Active)
            {
                // How fast is the cloud?
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);
                // How high is the cloud?
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                // Set position
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                // Move the cloud      
                spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud1.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {
                // How fast is the cloud?
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);
                // How high is the cloud?
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                // Set position
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {
                // Move the cloud      
                spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud2.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud2Active = false;
                }
            }

            // Cloud 3
            if (!cloud3Active)
            {
                // How fast is the cloud?
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);
                // How high is the cloud?
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                // Set position
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                // Move the cloud      
                spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
                // Has the cloud reached the right hand edge of the screen?
                if (spriteCloud3.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud next frame
                    cloud3Active = false;
                }
            }
            // Update the score text
            std::stringstream ss;
            ss << "Score " << score;
            scoreText.setString(ss.str());
                     
            // Position branches
            //for (int i = 0; i < NUM_BRANCHES; i++)
            //{
            //    float hight = i * 150;
            //    if (branchPositions[i] == side::LEFT)
            //    {
            //        // Move the sprite to the left side of the tree
            //        branches[i].setPosition(610.0f, hight);
            //        // Flip the side other way
            //        branches[i].setRotation(180);
            //    }
            //    else if (branchPositions[i] == side::RIGHT)
            //    {
            //        // Move the sprite to the right side
            //        branches[i].setPosition(1330.0f, hight);
            //        // Set the sprite rotation to normal
            //        branches[i].setRotation(0);
            //    }
            //    else
            //    {
            //        branches[i].setPosition(3000.0f, hight);
            //    }
            //}
            // Position one branch            
            // Move the sprite to the left side of the tree
            // Make timer to change the height
            timeLowerBranch -= dt.asSeconds();
            if (timeLowerBranch <= 0.0f)
            {
                branchHight += 150.0f;
                timeLowerBranch = 2.0f;
            }
            if (branchHight >= 900)
            {
                branchHight = 0.0f;
            }
            branches[0].setPosition(610.0f, branchHight);
            // Flip the side other way
            branches[0].setRotation(180);

        }// End if(!paused)


        /* ****************************************
        Draw the scene
        **************************************** */

        // Clear everything from the last run frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        // Draw the clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);
        // Draw branches
        //for (int i = 0; i < NUM_BRANCHES; i++)
        //{
            window.draw(branches[0]);
   /*     }*/
        // Draw the tree
        window.draw(spriteTree);
        // Draw the insect
        window.draw(spriteBee);
        // Draw the score
        window.draw(scoreText);
        // Draw time bar
        window.draw(timeBar);
        // Show everything we just drew
        if (paused)
        {
            // Draw our message
            window.draw(messageText);
        }
        window.display();

    }

    // Test: Call 2 succecive time funcions
    //int varTime0 = (int)time(0);
    //int varTime1 = (int)time(0);
    //std::cout << varTime0 << " : " << varTime1 << std::endl;

    // Random number calculation test for two different seeds
    //srand((int)time(0));
    //int randNum = rand();
    //srand((int)time(0)*10);
    //int randNum1 = rand();
    //std::cout << randNum << " : " << randNum1 << std::endl; // OK: we get two different numbers

    // Random number calculation
    //srand((int)time(0)); // seed for the random number generator - rand()
    //int randNum = rand();
    //int remNum = (randNum % 100);
    //std::cout << randNum << '\n';
    //std::cout << remNum << '\n';

    // Logical expressions
    //bool a = true;
    //bool b = true;
    //bool c = true;
    //bool expression = ( a && b && c );
    //std::cout << "( a && b && c ) == " << expression << std::endl;

    // Get current position of bee
    float currentPosition = spriteBee.getPosition().x;
    std::cout << "current bee x komponent == " << currentPosition << std::endl;
    // what is the frame rate for this laptop? - how many frames in a second
    std::cout << "time in ms per frame = " << timePerFrame.asMilliseconds();
    std::cout << "time in seconds per frame = " << timePerFrame.asSeconds() << std::endl;
    //// See random values
    //srand((int)time(0) * 4);
    //for (int i = 0; i < 10; i++)
    //{
    //    std::cout << rand() % 10 << std::endl;
    //}
    // See branch positions
    for (int i = 0; i < NUM_BRANCHES; i++)
    {
        std::cout <<(int) branchPositions[i] << std::endl;
    }

    return 0;
}

void updateBranches(int seed)
{
    // Move all branches down one place
    for (int j = NUM_BRANCHES - 1; j > 0; j--)
    {
        branchPositions[j] = branchPositions[j - 1];
    }
    // Generate new branch position for 0 element
    srand((int)time(0) * seed);
    int remainder = rand() % 3;
    switch (remainder)
    {
    case 0:
        branchPositions[0] = side::LEFT;
        break;
    case 1:
        branchPositions[0] = side::RIGHT;
        break;
    case 2:
        branchPositions[0] = side::NONE;
        break;
    default:
        break;
    }
    // Set branch positions for branches 0 to 5
    //for (int i = 0; i < NUM_BRANCHES; i++)
    //{
    //    srand((int)time(0) + seed);
    //    int r = rand() % 5;
    //    switch (r)
    //    {
    //    case 0:
    //        branchPositions[i] = side::LEFT;
    //        break;
    //    case 1:
    //        branchPositions[i] = side::RIGHT;
    //        break;
    //    default:
    //        branchPositions[i] = side::NONE;
    //        break;
    //    }
    //}
}





