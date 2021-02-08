// Timber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

// Include important libraries here
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

// Make code easier to type with “using namespace”
using namespace sf;

// This is where our game starts from
int main()
{
    //Test VS git
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

    // Prepare the beez
    Texture textureBee;
    textureBee.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/bee.png");
    Sprite spriteBeez[5];
    // Is the bee currently moving?
    bool beezActive[5];
    float beezSpeed[5];
    int rankedSequence[5]{0, 0, 0, 0, 0}; 
    int arrayOrderedGroups[5]{};
    int arrayOrderdBeeNumber[5]{};
    int beeNumber[5]{ 0, 0, 0, 0, 0 };
    float spriteComponentY = 500.0f;   

    for (int i = 0; i < 5; i++)
    {
        // Make 5 beez
        spriteBeez[i].setTexture(textureBee);
        spriteBeez[i].setPosition( 0 , spriteComponentY + (i * 1) * 100);
        spriteComponentY += 100.0f;
        beezActive[i] = false;
        // How fast is the bee moving?
        beezSpeed[i] = 0.0f;
        // Make 5 clouds
    }    

/*    float beeVelocityY = 0.0f;   
    float beeVelocityX = 0.0f; */  

    // Prepare the clouds
    // make 5 cloud sprites from 1 texture
    Texture textureCloud;
    textureCloud.loadFromFile("D:/GameProgramming/VS Projects/Timber/graphics/cloud.png");
    Sprite spriteClouds[5];
    bool cloudsActive[5];
    float cloudsSpeed[5];
    float cloudPositionY = -40;
    for (size_t i = 0; i < 5; i++)
    {
        // 5 New sprites with the same texture
        spriteClouds[i].setTexture(textureCloud);
        // Position the clouds on the left of the screen
        // at different heights
        spriteClouds[i].setPosition(0, cloudPositionY);
        cloudPositionY += 40;
        // Are clouds active?
        cloudsActive[i] = false;
        // How fast is each cloud?
        cloudsSpeed[i] = 0.0f;
    }
        
    // Variables to control time itself
    bool pause = true;
    Clock clock;
    Time timePerFrame;

    // Make sstream object to put some String values in it
    //std::string strHello = "Hello";
    //std::string strWorlds = " Worlds!";
    //int num_worlds = 3;
    //std::stringstream ss;
    //ss << strHello << num_worlds << strWorlds;

    // Make font object for our strings
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");
    // Make score
    int scoreBeez[5];
    Text textScoreBeez[5];
    int num_bee = 1;
    std::stringstream ss_score[5];
    std::string strScore = "score bee ";
    float scorePositionX = 20;

    for (int i = 0; i < 5; i++)
    {
        scoreBeez[i] = 0;
        ss_score[i] << strScore << num_bee << ": " << scoreBeez[i];
        // Set string to text
        textScoreBeez[i].setString(ss_score[i].str());
        // assign a size
        textScoreBeez[i].setCharacterSize(20);
        // chooze a color
        textScoreBeez[i].setFillColor(Color::Green);
        // Set the font
        textScoreBeez[i].setFont(font);
        // set position
        textScoreBeez[i].setPosition( scorePositionX , 20  );
        scorePositionX += 300;
        num_bee++;
    }
    
    // Make text message
    Text textMessage;
    // Assign the actual message    
    textMessage.setString( "Press Enter to start the Race!" );    
    textMessage.setCharacterSize(50);    
    textMessage.setFillColor(Color::Green);        
    textMessage.setFont(font);    
    FloatRect textRect = textMessage.getLocalBounds();
    textMessage.setOrigin(textRect.left + (textRect.width / 2.0f), textRect.top + (textRect.height / 2.0f));    
    textMessage.setPosition(1920 / 2.0f, 1080 / 2.0f);
    // ?
    Time gameTimeTotal;
    // Construct the time bar
    RectangleShape timeBar;
    float timeBarWidth = 400;
    float timeBarHeight = 80;
    timeBar.setFillColor( Color::Green );
    timeBar.setSize( Vector2f( timeBarWidth, timeBarHeight ) );
    timeBar.setPosition(1920 / 2.0f - timeBarWidth / 2.0f, 980);
    RectangleShape randomRect;
    float rndRectWidth = 25;
    float rndRectHeight = 25;
    randomRect.setSize(Vector2f(rndRectWidth, rndRectHeight));
    randomRect.setFillColor(Color::Blue);
    srand((int)time(0) * 35);
    randomRect.setPosition(rand() % 1895, rand() % 1055);
    /*RectangleShape movableRect; // idea for movable rect
    float width = 30;
    float height = width;
    movableRect.setSize(Vector2f(width, height));
    movableRect.setFillColor(Color::Magenta);
    float move_y = rand() % 1890;
    float move_x = rand() % 1050;    
    float moveRangePx = 30.0f;*/    


    float timeRemaining = 10.0f; // time is arbitrary number
    float widthPerSecondFrame = timeBarWidth / timeRemaining;
    Text textCheerBee;
    textCheerBee.setFont(font);
    textCheerBee.setString("Goo bees!");
    textCheerBee.setFillColor(Color::Cyan);
    float positionCheerX = 1000;
    float positionCheerY = 50;
    textCheerBee.setPosition(positionCheerX, positionCheerY);
    int counterCheerBees = 0;

    while (window.isOpen()) {

        /* ****************************************
        Handle the players input
        **************************************** */
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
        {
            window.close();
        }
        if (Keyboard::isKeyPressed(Keyboard::Key::Enter))
        {
            pause = false;
            // Reset time and score for bee 1 and bee 2 
            timeRemaining = 10.0f;
            
            for (int i = 0; i < 5; i++)
            {                
                beezActive[i] = false;
                // Reset scores 
                scoreBeez[i] = 0;
            }
            
            // Change cheer bees position            
            // Set cheer position
            srand((int)time(0) * 46);
            positionCheerX = rand() % (1980 - 120); // make text cheer visible
            positionCheerY = rand() % (1080 - 60);
            textCheerBee.setPosition(positionCheerX, positionCheerY);
        }        

        /* ****************************************
        Update the scene
        **************************************** */
        // If game not paused update game
        if ( !pause )
        {
            // Measure time
            Time dt = clock.restart();
            timePerFrame = dt;
            timeRemaining -= dt.asSeconds();            
            timeBar.setSize(Vector2f(widthPerSecondFrame * timeRemaining, timeBarHeight));
            // Make cheer apear on different places
            while ( true )
            {                
                if (counterCheerBees > 20)
                {
                    break;
                }
                counterCheerBees++;
            }
            srand((int)time(0) * 33);
            positionCheerX = rand() % (1980 - 120);
            positionCheerY = rand() & (1080 - 60);
            textCheerBee.setPosition(positionCheerX, positionCheerY);
            counterCheerBees = 0;
            // For loop
            for (int delay = 0; delay < 22; delay++)
            {
                // delay execution
                if (delay == 21)
                {
                    // Make a blue square appear on different position
                    srand((int)time(0) * 63);
                    randomRect.setPosition(rand() % 1920, rand() % 1080);
                }
            }

            // Set up the beez            
            spriteComponentY = 500.0f;
            for (int i = 0; i < 5; i++)
            {
                if (!beezActive[i])
                {
                    //Set up the beez
                    // How fast is the bee?
                    srand((int)time(0) * (10 + (i+1)));
                    beezSpeed[i] = (rand() % 500) + 200; // 200px - 699px
                    //beeVelocityY = beeSpeed;
                    //beeVelocityX = beeSpeed;

                    // How high is the bee?
                    /*srand((int)time(0) * 10);*/
                    //float height = (rand() % 800) + 200; // 500px - 1849px --> y // The scope of height just in if statement                    
                    // Set position of beez; position depends on var. i
                    spriteBeez[i].setPosition(2000.0f, spriteComponentY + (i * 1) * 100);                    
                    beezActive[i] = true;
                }               
            }
            for (int i = 0; i < 5; i++)
            {                
                // Move the bee
                if (beezActive[i])
                {
                    float pos_x = spriteBeez[i].getPosition().x;
                    float pos_y = spriteBeez[i].getPosition().y;
                    // Change bee direction of travel
                    //if (pos_y > 600.0f || pos_y < 300.0f)
                    //{
                    //    beeVelocityY = -beeVelocityY;
                    //}
                    //if (pos_x < 0.0f)
                    //{
                    //    beeVelocityX = -beeVelocityX;
                    //}


                    spriteBeez[i].setPosition(pos_x - (beezSpeed[i] * dt.asSeconds()), pos_y);

                    if (pos_x < -100)
                    {
                        beezActive[i] = false;
                        scoreBeez[i]++;
                        // How fast is the bee?
                        srand((int)time(0)* (10 + (i + 1)));
                        beezSpeed[i] = (rand() % 500) + 200;
                        spriteBeez[i].setPosition(2000.0f, 500.0f + (i * 1) * 100.0f);                        
                    }
                }
                
            }
                        
            

            // Manage the clouds
            // Set up cluds
            for (int i = 0; i < 5; i++)
            {
                if (!cloudsActive[i])
                {
                    // How fast is the cloud?
                    srand((int)time(0) * (10 + i));                    
                    cloudsSpeed[i] = (rand() % 600) + 50;
                    // How high is the cloud?
                    srand((int)time(0) * (10 + i));                    
                    int height = (rand() % 150) + 50; // range: 50px to 199px
                    // Set position
                    spriteClouds[i].setPosition(-100, height);
                    cloudsActive[i] = true;
                }                
            }
            // Move the clouds
            for (int i = 0; i < 5; i++)
            {
                if (cloudsActive[i])
                {
                    /*float secFrame = dt.asSeconds();*/
                    float pix_Sec = cloudsSpeed[i] * dt.asSeconds();
                    float newPosX = spriteClouds[i].getPosition().x + pix_Sec;
                    spriteClouds[i].setPosition(newPosX, spriteClouds[i].getPosition().y);
                    if (spriteClouds[i].getPosition().x > 2150)
                    {
                        cloudsActive[i] = false;
                    }
                }

            }
              
            // Bee scoreboard
            std::stringstream ss[5];
            for (int i = 0; i < 5; i++)
            {
                ss[i] << "score bee "<< (i + 1) << ": " << scoreBeez[i];              
                textScoreBeez[i].setString(ss[i].str());
            }       
            
            if (timeRemaining <= 0.0f)
            {
                pause = true;                
                std::stringstream ss_beeName;                
                int first = 1;            
                int i_OrGr = 0;
                int maxScore = 0;
                int i_max = 0;
                for (int i = 0; i < 5; i++)
                {
                    maxScore = scoreBeez[i];
                    i_max = i;
                    for (int j = 0; j < 5; j++)
                    {
                        if ((scoreBeez[j] == maxScore) && (spriteBeez[j].getPosition().x < spriteBeez[i_max].getPosition().x))
                        {
                            maxScore = scoreBeez[j];
                            i_max = j;
                        } 
                        else if((scoreBeez[j] > maxScore))
                        {
                            maxScore = scoreBeez[j];
                            i_max = j;
                        }
                    }
                    scoreBeez[i_max] = -1;
                    arrayOrderedGroups[i_OrGr] = maxScore;
                    arrayOrderdBeeNumber[i_OrGr] = i_max + 1;                    
                    i_OrGr++;
                }
                            
                for (int i = 0; i < 5; i++)
                {
                    switch (i)
                    {
                    case 0:
                        ss_beeName << (0 + 1) << ".position:  Bee " << arrayOrderdBeeNumber[0] << " wins gold!!!\n";
                        break;
                    case 1:
                        ss_beeName << (1 + 1) << ".position:  Bee " << arrayOrderdBeeNumber[1] << " wins silver!!!\n";
                        break;  
                    case 2:
                        ss_beeName << (2 + 1) << ".position:  Bee " << arrayOrderdBeeNumber[2] << " wins bronze!!!\n";
                        break;        
                    case 3:
                        ss_beeName << (3 + 1) << ".position:  Bee " << arrayOrderdBeeNumber[3] << "\n";
                        break;    
                    case 4:
                        ss_beeName << (4 + 1) << ".position:  Bee " << arrayOrderdBeeNumber[4];
                        break;                        
                    default:
                        break;
                    }

                }
             
                textMessage.setString(ss_beeName.str());
                // Text message has changed so we need new bounds of text m. to reposition the origin of text m.
                //  to be positioned at the middle of text m.                
                textRect = textMessage.getLocalBounds();
                textMessage.setOrigin(textRect.left + (textRect.width / 2.0f), textRect.top + (textRect.height / 2.0f));
                textMessage.setPosition(1920 / 2.0f, 1080 / 2.0f); 
                // Set beez to not active
                for (int i = 0; i < 5; i++)
                {
                    beezActive[i] = false;
                }
            }
            
        }
        /* ****************************************
        Draw the scene
        **************************************** */

        // Clear everything from the last run frame
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        // Draw the clouds
        for (int i = 0; i < 5; i++)
        {
            window.draw(spriteClouds[i]);
        }                
        // Draw the tree
        window.draw(spriteTree);
        // Draw the insects and score
        for (int i = 0; i < 5; i++)
        {
            window.draw(spriteBeez[i]);
            window.draw(textScoreBeez[i]);
        }                        
        // Draw text message
        if (pause)
        {
            window.draw(textMessage);
        }
        if (!pause)
        {
            window.draw(textCheerBee);
            window.draw(randomRect);
        }
        window.draw(timeBar);
        // Show everything we just drew
        window.display();

    }
    
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
