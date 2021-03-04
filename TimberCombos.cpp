// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Include important libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
// sstream is a standard header for manipulatin strings
#include <sstream>  

// Make code easier to type with “using namespace”
using namespace sf;

enum class side
{
    LEFT,
    RIGHT,
    NONE
};

class Cloud // does not work correctly: shows only one cloud
{
public:
    Cloud() {}
    Cloud(Texture& textureCloud, float posX, float posY)
    {
        active = false;
        speed = 0.0f;
        spriteCloud.setTexture(textureCloud);
        spriteCloud.setPosition(posX, posY);
    }

    void process(int seed, Time& dt)
    {
        if (!active)
        {
            active = true;
            resetSpeedPosition(seed);
        }
        else
        {
            spriteCloud.setPosition(spriteCloud.getPosition().x + (speed * dt.asSeconds()), spriteCloud.getPosition().y);
            if (spriteCloud.getPosition().x > 1920)
            {
                // Set it up ready to be a whole new cloud next frame
                active = false;
                resetSpeedPosition(seed);
            }
        }
    }
    // Determine speed and position
    void resetSpeedPosition(int seed)
    {
        srand((int)time(0) * seed);
        speed = (rand() % 200);
        spriteCloud.setPosition(-200.0f, (rand() % 150));
    }

public:
    Sprite spriteCloud;
    bool active;
    float speed;
};

// Function declaration
void positionBranches();
side randomizePosition(int seed);
void updateBranches(int seed);
Text operationMessageText(std::string operatinMessage, Font f);
Text operationMessage;
int const NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
// Where is the player / branch?
// Left or right

side branchPositions[NUM_BRANCHES];
bool buttonIsPressed = false;
//float branchHight[NUM_BRANCHES]{ 0.0f, 150.0f, 300.0f, 450.0f, 600.0f, 750.0f};

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
    //Sprite spriteCloud1;
    //Sprite spriteCloud2;
    //Sprite spriteCloud3;
    //spriteCloud1.setTexture(textureCloud);
    //spriteCloud2.setTexture(textureCloud);
    //spriteCloud3.setTexture(textureCloud);
    //// Position the clouds on the left of the screen
    //// at different heights
    //spriteCloud1.setPosition(0, 0);
    //spriteCloud2.setPosition(50, 40);
    //spriteCloud3.setPosition(100, 70);
    //// Are the clouds currently on screen?
    //bool cloud1Active = false;
    //bool cloud2Active = false;
    //bool cloud3Active = false;
    //// How fast is each cloud?    
    //float cloud1Speed = 0.0f;
    //float cloud2Speed = 0.0f;
    //float cloud3Speed = 0.0f;
    // Clouds object initalization
    Cloud clouds[3];  // does not work
    clouds[0] = Cloud(textureCloud, 0.0f, 0.0f);
    clouds[1] = Cloud(textureCloud, 50.0f, 40.0f);
    clouds[2] = Cloud(textureCloud, 100.0f, 70.0f);
     //Variables to control time itself    
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
    
    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    // Player starts on the left
    side playerSide = side::LEFT;
    // Prepare the grave stone
    Texture textureRip;
    textureRip.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/rip.png");
    Sprite spriteRip;
    spriteRip.setTexture(textureRip);
    spriteRip.setPosition(600, 860);
    // Prepare axe
    Texture textureAxe;
    textureAxe.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    // Line the axe up with the tree
    // determine the horizontal position the axe will be drawn at, depends on player side
    const float AXE_POSITION_LEFT = 700;// 800-76 = 724 My calculation
    const float AXE_POSITION_RIGHT = 1075;// 800 + 300 - 76 = 1024
    spriteAxe.setPosition(2000, 830);
    // Prepare the flying log    
    Texture textureLog;
    textureLog.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720); // yLog = 720(playerY) + 120(logHeight); yLog = 840
    bool logActive = false;
    float logSpeedX = 1000.0f;
    float logSpeedY = -1500.0f;
    bool acceptInput = false;
    // Introduce playerDies
    bool playerDies = false;

    std::string ssOperationMessage = "operation:";
    
    // Idea to write operations in a game
    Event event;

    while (window.isOpen()) {

        /* ****************************************
        Handle the players input
        **************************************** */
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused)
            {
                acceptInput = true;
                spriteAxe.setPosition(2000.0f, spriteAxe.getPosition().y);
            }
        }

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
            score = 0;    
            /*    Initialize branch positions for branches 0 to 5*/
            for (int i = 1; i < NUM_BRANCHES; i++)
            {
                branchPositions[i] = side::NONE;
            }
            playerDies = false;
            spritePlayer.setPosition(580, 720);
        }
        if (acceptInput)
        {
            // Activate chopping of the log only when player is already positioned
            if (Keyboard::isKeyPressed(Keyboard::Key::Right))
            {
                // Idea: make player swing the axe once he has arived on the right side // same for left
                // Change position / chopp
                
                if (playerSide == side::RIGHT)
                {                    
                    logActive = true;
                    logSpeedX = -5000;
                    updateBranches(score);
                    spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                    score++;                    
                    // Add to the time
                }
                else
                {
                    playerSide = side::RIGHT;   
                    spriteAxe.setPosition(2000.0f, spriteAxe.getPosition().y);
                    // Anulate last branch if it is in the way of the player
                    if (branches[5].getPosition().y > spritePlayer.getPosition().y && playerSide == branchPositions[5])
                    {
                        branchPositions[5] = side::NONE;
                    }
                }
                
                spritePlayer.setPosition(1200.0f, spritePlayer.getPosition().y);
                spriteLog.setPosition(810, 720);                           
                acceptInput = false;
            }
            else if (Keyboard::isKeyPressed(Keyboard::Key::Left))
            {
                if (playerSide == side::LEFT)
                {
                    
                    logActive = true;
                    logSpeedX = 5000;
                    updateBranches(score);
                    score++;
                    spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                }
                // Swithching position from right to left
                else
                {
                    playerSide = side::LEFT;              
                    spriteAxe.setPosition(2000.0f, spriteAxe.getPosition().y);
                    // Anulate last branch if it is in the way of the player
                    if (branches[5].getPosition().y > spritePlayer.getPosition().y && playerSide == branchPositions[5])
                    {
                        branchPositions[5] = side::NONE;
                    }
                    
                }                
                spritePlayer.setPosition(580, spritePlayer.getPosition().y);
                spriteLog.setPosition(810, 720);                                  
                acceptInput = false;
            }            
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
            for (int i = 0; i < 3; i++)
            {
                clouds[i].process(score , dt);
            }

            //// Cloud 1
            //if (!cloud1Active)
            //{
            //    // How fast is the cloud?
            //    srand((int)time(0) * 10);
            //    cloud1Speed = (rand() % 200);
            //    // How high is the cloud?
            //    srand((int)time(0) * 10);
            //    float height = (rand() % 150);
            //    // Set position
            //    spriteCloud1.setPosition(-200, height);
            //    cloud1Active = true;
            //}
            //else
            //{
            //    // Move the cloud      
            //    spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
            //    // Has the cloud reached the right hand edge of the screen?
            //    if (spriteCloud1.getPosition().x > 1920)
            //    {
            //        // Set it up ready to be a whole new cloud next frame
            //        cloud1Active = false;
            //    }
            //}

            //// Cloud 2
            //if (!cloud2Active)
            //{
            //    // How fast is the cloud?
            //    srand((int)time(0) * 20);
            //    cloud2Speed = (rand() % 200);
            //    // How high is the cloud?
            //    srand((int)time(0) * 20);
            //    float height = (rand() % 300) - 150;
            //    // Set position
            //    spriteCloud2.setPosition(-200, height);
            //    cloud2Active = true;
            //}
            //else
            //{
            //    // Move the cloud      
            //    spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
            //    // Has the cloud reached the right hand edge of the screen?
            //    if (spriteCloud2.getPosition().x > 1920)
            //    {
            //        // Set it up ready to be a whole new cloud next frame
            //        cloud2Active = false;
            //    }
            //}

            //// Cloud 3
            //if (!cloud3Active)
            //{
            //    // How fast is the cloud?
            //    srand((int)time(0) * 30);
            //    cloud3Speed = (rand() % 200);
            //    // How high is the cloud?
            //    srand((int)time(0) * 30);
            //    float height = (rand() % 450) - 150;
            //    // Set position
            //    spriteCloud3.setPosition(-200, height);
            //    cloud3Active = true;
            //}
            //else
            //{
            //    // Move the cloud      
            //    spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
            //    // Has the cloud reached the right hand edge of the screen?
            //    if (spriteCloud3.getPosition().x > 1920)
            //    {
            //        // Set it up ready to be a whole new cloud next frame
            //        cloud3Active = false;
            //    }
            //}
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
            //// Make timer to change the height
            //timeLowerBranch -= dt.asSeconds();
            //if (timeLowerBranch <= 0.0f)
            //{
            //    for (int i = NUM_BRANCHES - 1; i >= 0; i--)
            //    {
            //        branchHight[i] += 150.0f;
            //        if (branchHight[i] >= 900)
            //        {
            //            branchHight[i] = 0.0f;
            //            branchPositions[i] = randomizePosition(12);
            //        }
            //    }                
            //    timeLowerBranch = 2.0f;
            //}       

            // Position all the branches
            for (int i = 0; i < NUM_BRANCHES; i++)
            {
                float hight = i * 150;
                if (branchPositions[i] == side::LEFT)
                {
                    // Move the sprite to the left side of the tree
                    branches[i].setPosition(610.0f, hight);
                    // Flip the side other way
                    branches[i].setRotation(180);
                }
                else if (branchPositions[i] == side::RIGHT)
                {
                    // Move the sprite to the right side
                    branches[i].setPosition(1330.0f, hight);
                    // Set the sprite rotation to normal
                    branches[i].setRotation(0);
                }
                else
                {
                    branches[i].setPosition(3000.0f, 3000.0f);
                }
            }         
            // Update the log
            if (logActive)
            {
                if (AXE_POSITION_LEFT)
                {
                    spriteLog.setPosition(Vector2f(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                        spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())));
                }
                else
                {
                    spriteLog.setPosition(Vector2f(spriteLog.getPosition().x - (logSpeedX * dt.asSeconds()),
                        spriteLog.getPosition().y + (logSpeedY * dt.asSeconds())));
                }
            }
            if (spriteLog.getPosition().y < 0.0f)
            {
                logActive = false;
                spriteLog.setPosition(810, 720);
            }
            // Handle players death
            if ((spritePlayer.getPosition().y <= branches[5].getPosition().y) && (playerSide == branchPositions[5]))
            {
                playerDies = true;
                spriteRip.setPosition(spritePlayer.getPosition().x, spritePlayer.getPosition().y);
                spritePlayer.setPosition(2000.0f, spritePlayer.getPosition().y);
                messageText.setString("Squished!!");
                // Reposition the text based on its new size
                FloatRect rectText = messageText.getLocalBounds();
                // Move origin of text to the center of the text
                messageText.setOrigin(rectText.left + (rectText.width / 2.0f),
                    (rectText.top + (rectText.height / 2.0f)));
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                paused = true;
            }

        }// End if(!paused)


        /* ****************************************
        Draw the scene
        **************************************** */

        // Clear everything from the last run frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        // Draw the clouds
        for (int i = 0; i < 3; i++)
        {
            window.draw(clouds[i].spriteCloud);
        }       

        // Draw branches
        for (int i = 0; i < NUM_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        // Draw the tree
        window.draw(spriteTree);
        // Draw the player
        window.draw(spritePlayer);
        // Draw the axe
        window.draw(spriteAxe);            
        // Draw the log
        window.draw(spriteLog);
        // Draw the rip
        if (playerDies)
        {
            window.draw(spriteRip);
        }
        // Draw the insect
        window.draw(spriteBee);
        // Draw the score
        window.draw(scoreText);
        // Draw frame rate
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

    std::cout << logSpeedX * timePerFrame.asSeconds(); // How many px per sec
    // what is the last y position of player and last branch
    std::cout << spritePlayer.getPosition().y << " : " << branches[5].getPosition().y << std::endl;

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

side randomizePosition(int seed)
{
    // Generate new branch position
    srand((int)time(0) * seed);
    int remainder = rand() % 3;
    switch (remainder)
    {
    case 0:
        return side::LEFT;
    case 1:
        return side::RIGHT;
    default:
        return side::NONE;        
    }
}

// Idea to write operations message - program execution
Text operationMessageText(std::string operationMessage, Font f)
{
    
    Text operationMessageText;
    // Set the font to our message  
    operationMessageText.setFont(f);
    operationMessageText.setString(operationMessage);
    operationMessageText.setCharacterSize(40);      
    operationMessageText.setFillColor(Color::Green);    
    // Position the text
    FloatRect textOpRect = operationMessageText.getLocalBounds();
    operationMessageText.setOrigin(textOpRect.left + textOpRect.width / 2.0f, textOpRect.top + textOpRect.height / 2.0f);
    operationMessageText.setPosition((192.0f / 2) , 980.0f);

    return operationMessageText;
}


