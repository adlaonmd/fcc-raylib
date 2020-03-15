#include "raylib.h"
#include "stdlib.h"

#define MAX_INPUT_CHARS     2

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum GameScreen { LOGO, MENU, PLAY, HELP, ABOUT, GAMEPLAY, ENDING } GameScreen;

//----------------------------------------------------------------------------------
// Main entry point
//----------------------------------------------------------------------------------
int main(void)
{
    // Initialization (Note windowMENU is unused on Android)
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;
    
    InitWindow(screenWidth, screenHeight, "Raylib - FCC");

    GameScreen currentScreen = LOGO;

    // Initialize all required variables and load all required data here!
    //LOGO
    int logoPositionX = screenWidth/2 - 128;
    int logoPositionY = screenHeight/2 - 128;
    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;
    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;
    int state = 0;
    float alpha = 1.0f;
    
    //INPUT FIELD
    int framesCounter = 0;
    int lettersCount = 0;
    char inputVertices[MAX_INPUT_CHARS + 1] = "\0";
    char inputEdges[MAX_INPUT_CHARS + 1] = "\0";
    int vertices = 20;
    int edges = 0;
    int extraEdges = 0;
    int verticesCount = 0;
    int edgesCount = 0;
    bool invalidVertices = false;
    bool invalidEdges = false;

    //MENU
    Rectangle playButton = { 245, 100, 335, 50 };
    Rectangle helpButton = { 245, 170, 335, 50 };
    Rectangle aboutButton = { 245, 240, 335, 50 };
    Rectangle quitButton = { 245, 310, 335, 50 };
    Rectangle backButton = { 300, 370, 225, 50 };
    bool mouseOnPlayButton = false;
    bool mouseOnHelpButton = false;
    bool mouseOnAboutButton = false;
    bool mouseOnQuitButton = false;
    bool mouseOnBackButton = false;
    int menuOption = 0;
    
    //PLAY
    Rectangle verticesBox = { 285, 105, 225, 50 };
    Rectangle edgesBox = { 285, 190, 225, 50 };
    Rectangle startButton = { 285, 255, 225, 50 };
    Rectangle returnButton = { 285, 320, 225, 50 };
    bool mouseOnVertices = false;
    bool mouseOnEdges = false;
    bool mouseOnStartButton = false;
    bool mouseOnReturnButton = false;
    int playOption = 0;
    
    //SQUARE
    Color colors[5] = { DARKGRAY, RED, GREEN, BLUE, YELLOW };
    Color vertexColor[vertices];
    int color[vertices] = { 0 };
    Rectangle square[vertices];
    Vector2 squarePosition[vertices];
    bool squareClicked[vertices] = { false };
    bool mouseOnSquare[vertices] = { false };
    
    //GAME
    Rectangle submitButton = { 650, 397, 140, 40 };
    bool mouseOnSubmitButton = false;
    bool squareFilled = false;
    bool squareValid = false;
    bool invalidPuzzle = false;
    int timer = 0;
    bool success = false;
    
    //MISC
    int randomX[15];
    int randomSpeed[15];
    for (int i = 0; i < 15; i++)
    {
        randomX[i] = GetRandomValue(0, 750);
        randomSpeed[i] = GetRandomValue(2, 5);
    }

    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        switch(currentScreen) 
        {
            case LOGO: 
            {
                if (state == 0)
                {
                    framesCounter++;

                    if (framesCounter == 120)
                    {
                        state = 1;
                        framesCounter = 0;
                    }
                }
                else if (state == 1)
                {
                    topSideRecWidth += 4;
                    leftSideRecHeight += 4;

                    if (topSideRecWidth == 256) state = 2;
                }
                else if (state == 2)
                {
                    bottomSideRecWidth += 4;
                    rightSideRecHeight += 4;

                    if (bottomSideRecWidth == 256) state = 3;
                }
                else if (state == 3)
                {
                    framesCounter++;

                    if (framesCounter/12)
                    {
                        lettersCount++;
                        framesCounter = 0;
                    }

                    if (lettersCount >= 10)
                    {
                        alpha -= 0.02f;

                        if (alpha <= 0.0f)
                        {
                            alpha = 0.0f;
                            state = 4;
                        }
                    }
                }
                else if (state == 4)
                {
                    currentScreen = MENU;
                }
            } break;
            case MENU: 
            {
                if (CheckCollisionPointRec(GetMousePosition(), playButton)) 
                {
                    mouseOnPlayButton = true;
                    mouseOnHelpButton = false;
                    mouseOnAboutButton = false;
                    mouseOnQuitButton = false;
                    menuOption = 0;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), helpButton)) 
                {
                    mouseOnPlayButton = false;
                    mouseOnHelpButton = true;
                    mouseOnAboutButton = false;
                    mouseOnQuitButton = false;
                    menuOption = 0;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), aboutButton)) 
                {
                    mouseOnPlayButton = false;
                    mouseOnHelpButton = false;
                    mouseOnAboutButton = true;
                    mouseOnQuitButton = false;
                    menuOption = 0;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), quitButton)) 
                {
                    mouseOnPlayButton = false;
                    mouseOnHelpButton = false;
                    mouseOnAboutButton = false;
                    mouseOnQuitButton = true;
                    menuOption = 0;
                }
                else
                {
                    mouseOnPlayButton = false;
                    mouseOnHelpButton = false;
                    mouseOnAboutButton = false;
                    mouseOnQuitButton = false;
                }
                
                switch(menuOption)
                {
                    case 1:
                    {
                        mouseOnPlayButton = true;
                        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) menuOption += 1;
                        if (IsKeyPressed(KEY_ENTER)) currentScreen = PLAY;
                    } break;
                    case 2:
                    {
                        mouseOnHelpButton = true;
                        if (IsKeyPressed(KEY_UP)) menuOption -= 1;
                        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) menuOption += 1;
                        if (IsKeyPressed(KEY_ENTER)) currentScreen = HELP;
                    } break;
                    case 3:
                    {
                        mouseOnAboutButton = true;
                        if (IsKeyPressed(KEY_UP)) menuOption -= 1;
                        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) menuOption += 1;
                        if (IsKeyPressed(KEY_ENTER)) currentScreen = ABOUT;
                    } break;
                    case 4:
                    {
                        mouseOnQuitButton = true;
                        if (IsKeyPressed(KEY_UP)) menuOption -= 1;
                        else if (IsKeyPressed(KEY_TAB)) menuOption = 1;
                        if (IsKeyPressed(KEY_ENTER)) CloseWindow();
                    } break;
                    default: 
                    {
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) menuOption = 1;
                    } break;
                }
                
                if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    mouseOnPlayButton = false;
                    currentScreen = PLAY;
                }
                if (CheckCollisionPointRec(GetMousePosition(), helpButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    mouseOnHelpButton = false;
                    currentScreen = HELP;
                }
                if (CheckCollisionPointRec(GetMousePosition(), aboutButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    mouseOnAboutButton = false;
                    currentScreen = ABOUT;
                }
                if (CheckCollisionPointRec(GetMousePosition(), quitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    mouseOnQuitButton = false;
                    CloseWindow();
                    return 0;
                }
            } break;
            case HELP:
            {
                if (CheckCollisionPointRec(GetMousePosition(), backButton)) mouseOnBackButton = true;
                else mouseOnBackButton = false;
                
                if ((mouseOnBackButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER))
                {
                    mouseOnBackButton = false;
                    currentScreen = MENU;
                }
            } break;
            case ABOUT:
            {
                if (CheckCollisionPointRec(GetMousePosition(), backButton)) mouseOnBackButton = true;
                else mouseOnBackButton = false;
                
                framesCounter++;
                if (framesCounter == 260)
                {
                    for (int i = 0; i < 15; i++)
                    {
                        randomX[i] = GetRandomValue(0, 750);
                        randomSpeed[i] = GetRandomValue(2, 5);
                    }
                    framesCounter = 0;
                }
                
                if ((mouseOnBackButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_ENTER))
                {
                    for (int i = 0; i < 15; i++)
                    {
                        randomX[i] = GetRandomValue(0, 750);
                        randomSpeed[i] = GetRandomValue(2, 5);
                    }
                    framesCounter = 0;
                    mouseOnBackButton = false;
                    currentScreen = MENU;
                }
            } break;
            case PLAY:
            {
                if (CheckCollisionPointRec(GetMousePosition(), verticesBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                {
                    mouseOnVertices = true;
                    mouseOnEdges = false;
                    playOption = 1;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), edgesBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
                {
                    mouseOnVertices = false;
                    mouseOnEdges = true;
                    playOption = 2;
                }
                else if (!CheckCollisionPointRec(GetMousePosition(), verticesBox) && !CheckCollisionPointRec(GetMousePosition(), edgesBox) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    mouseOnVertices = false;
                    mouseOnEdges = false;
                    playOption = 0;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), startButton)) 
                {
                    mouseOnStartButton = true;
                    mouseOnReturnButton = false;
                    playOption = 0;
                }
                else if (CheckCollisionPointRec(GetMousePosition(), returnButton))
                {
                    mouseOnStartButton = false;
                    mouseOnReturnButton = true;
                    playOption = 0;
                }
                else
                {
                    mouseOnStartButton = false;
                    mouseOnReturnButton = false;
                }
                
                switch(playOption)
                {
                    case 1:
                    {
                        mouseOnVertices = true;
                        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_ENTER))
                        {
                            mouseOnVertices = false;
                            playOption += 1;
                        }
                    } break;
                    case 2:
                    {
                        mouseOnEdges = true;
                        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_ENTER))
                        {
                            mouseOnEdges = false;
                            playOption += 1;
                        }
                        else if (IsKeyPressed(KEY_UP)) 
                        {
                            mouseOnEdges = false;
                            playOption -= 1;
                        }
                    } break;
                    case 3:
                    {
                        mouseOnStartButton = true;
                        if (IsKeyPressed(KEY_TAB) || IsKeyPressed(KEY_DOWN))
                        {
                            mouseOnStartButton = false;
                            playOption += 1;
                        }
                        else if (IsKeyPressed(KEY_UP)) playOption -= 1;
                    } break;
                    case 4:
                    {
                        mouseOnReturnButton = true;
                        if (IsKeyPressed(KEY_UP))
                        {
                            mouseOnReturnButton = false;
                            playOption -= 1;
                        }
                        else if (IsKeyPressed(KEY_TAB)) 
                        {
                            mouseOnReturnButton = false;
                            playOption = 1;
                        }
                    } break;
                    default:
                    {
                        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_TAB)) playOption = 1;
                    } break;
                }
                
                if (mouseOnVertices)
                {
                    int key = GetKeyPressed();
                    if ((key >= 48) && (key <= 57) && (verticesCount < MAX_INPUT_CHARS))
                    {
                        inputVertices[verticesCount] = (char)key;
                        verticesCount++;
                    }
                    
                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        verticesCount--;
                        inputVertices[verticesCount] = '\0';
                        if (verticesCount < 0) verticesCount = 0;
                    }
                }

                if (mouseOnEdges)
                {
                    int key = GetKeyPressed();
                    if ((key >= 48) && (key <= 57) && (edgesCount < MAX_INPUT_CHARS))
                    {
                        inputEdges[edgesCount] = (char)key;
                        edgesCount++;
                    }
                    
                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        edgesCount--;
                        inputEdges[edgesCount] = '\0';
                        
                        if (edgesCount < 0) edgesCount = 0;
                    }
                }
                
                if (mouseOnVertices || mouseOnEdges) framesCounter++;
                else framesCounter = 0;
                
                if ((CheckCollisionPointRec(GetMousePosition(), startButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (mouseOnStartButton && IsKeyPressed(KEY_ENTER)))
                {
                    vertices = atoi(inputVertices);
                    edges = atoi(inputEdges);
                    if (vertices > 3 && vertices <= 20) 
                    {
                        invalidVertices = false;
                        if (edges >= vertices && edges <= (vertices * 2) - 2)
                        {
                            if (edges <= 10) timer = 30;
                            else if (edges <= 20) timer = 60;
                            else timer = 120;
                            
                            invalidEdges = false;
                            extraEdges = edges - vertices;
                            edges -= extraEdges;
                            for (int i = 0; i < vertices; i++)
                            {   
                                vertexColor[i] = colors[0];
                                
                                square[i].x = GetRandomValue(40, 720);
                                square[i].y = GetRandomValue(40, 330);
                                for (int j = 0; j < i; j++)
                                {
                                    while (CheckCollisionRecs(square[i], square[j]))
                                    {
                                        square[i].x = GetRandomValue(20, 720);
                                        square[i].y = GetRandomValue(20, 330);
                                    }
                                }
                                square[i].width = 50;
                                square[i].height = 50;
                                
                                squarePosition[i].x = square[i].x + 25;
                                squarePosition[i].y = square[i].y + 25;
                            }
                            alpha = 0.0f;
                            mouseOnStartButton = false;
                            currentScreen = GAMEPLAY;
                        }
                        else
                        {
                            invalidEdges = true;
                        }
                    }
                    else
                    {
                        invalidVertices = true;
                    }
                }
                
                if ((CheckCollisionPointRec(GetMousePosition(), returnButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || (mouseOnReturnButton && IsKeyPressed(KEY_ENTER)))
                {
                    mouseOnReturnButton = false;
                    for (int i = 0; i < verticesCount; i++) inputVertices[i] = '\0';
                    for (int i = 0; i < edgesCount; i++) inputEdges[i] = '\0';
                    verticesCount = 0;
                    edgesCount = 0;
                    invalidVertices = false;
                    invalidEdges = false;
                    currentScreen = MENU;
                }
            } break;
            case GAMEPLAY:
            {
                if (CheckCollisionPointRec(GetMousePosition(), submitButton)) mouseOnSubmitButton = true;
                else mouseOnSubmitButton = false;
                
                //Reposition square if it collides with another square
                for (int i = 0; i < vertices; i++)
                {
                    for (int j = 0; j < i; j++)
                    {
                        if(CheckCollisionRecs(square[i], square[j]))
                        {
                            square[i].x = GetRandomValue(40, 720);
                            square[i].y = GetRandomValue(40, 330);
                            
                            squarePosition[i].x = square[i].x + 25;
                            squarePosition[i].y = square[i].y + 25;
                        }
                    }
                }
                
                //Clickable square
                for (int i = 0; i < vertices; i++)
                {
                    if (CheckCollisionPointRec(GetMousePosition(), square[i])) mouseOnSquare[i] = true;
                    else mouseOnSquare[i] = false;
                    
                    if (mouseOnSquare[i] && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                    {
                        squareClicked[i] = true;
                    }
                }
                
                //Timer
                framesCounter++;
                if (((framesCounter/60)%2) == 1)
                {
                    timer -= 1;
                    invalidPuzzle = false;
                    framesCounter = 0;
                }
                
                if (timer <= 10)
                {
                    if (alpha <= 0.5f)
                    {
                        alpha += 0.005f;
                    }
                }
                
                if ((mouseOnSubmitButton && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) || IsKeyPressed(KEY_ENTER))
                {
                    //Checks if square is square is filled out
                    for (int i = 0 ; i < vertices; i++)
                    {
                        if (ColorToInt(vertexColor[i]) != ColorToInt(colors[0])) squareFilled = true;
                        else 
                        {
                            invalidPuzzle = true;
                            squareFilled = false;
                            break;
                        }
                    }
                    
                    //Checks if square has adjacent color
                    for (int i = 1; i < vertices; i++)
                    {
                        if (ColorToInt(vertexColor[0]) != ColorToInt(vertexColor[1]) && ColorToInt(vertexColor[0]) != ColorToInt(vertexColor[vertices - 1])) squareValid = true;
                        else
                        {
                            invalidPuzzle = true;
                            squareValid = false;
                            break;
                        }
                        
                        if (ColorToInt(vertexColor[i]) != ColorToInt(vertexColor[i + 1]) && ColorToInt(vertexColor[i]) != ColorToInt(vertexColor[i - 1])) squareValid = true;
                        else
                        {
                            invalidPuzzle = true;
                            squareValid = false;
                            break;
                        }
                        
                        for (int j = 0; j < extraEdges; j++)
                        {
                            if (ColorToInt(vertexColor[j]) != ColorToInt(vertexColor[j + 2])) squareValid = true;
                            else 
                            {
                                invalidPuzzle = true;
                                squareValid = false;
                                break;
                            }
                        }
                    }
                }
                
                if (timer == 0)
                {
                    framesCounter = 0;
                    success = false;
                    currentScreen = ENDING;
                }
                
                if (squareFilled && squareValid)
                {
                    framesCounter = 0;
                    success = true;
                    currentScreen = ENDING;
                }
                
            } break;
            case ENDING: 
            {
                framesCounter++;
                if (framesCounter == 260)
                {
                    for (int i = 0; i < 15; i++)
                    {
                        randomX[i] = GetRandomValue(0, 750);
                        randomSpeed[i] = GetRandomValue(2, 5);
                    }
                    framesCounter = 0;
                }
                
                //Resets the game
                for (int i = 0; i < verticesCount; i++) inputVertices[i] = '\0';
                for (int i = 0; i < edgesCount; i++) inputEdges[i] = '\0';
                for (int i = 0; i < vertices; i++) 
                {
                    vertexColor[i] = colors[0];
                    color[i] = 0;
                    squareClicked[i] = false;
                    mouseOnSquare[i] = false;
                }
                verticesCount = 0;
                edgesCount = 0;
                vertices = 0;
                edges = 0;
                extraEdges = 0;
                timer = 0;
                squareFilled = false;
                squareValid = false;
                invalidPuzzle = false;
                
                if (IsKeyPressed(KEY_ENTER)) currentScreen = MENU;
            } break;
            default: break;
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        
            ClearBackground(RAYWHITE);
            
            switch(currentScreen) 
            {
                case LOGO: 
                {
                    if (state == 0)
                    {
                        if ((framesCounter/15)%2) DrawRectangle(logoPositionX, logoPositionY, 16, 16, BLACK);
                    }
                    else if (state == 1)
                    {
                        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
                    }
                    else if (state == 2)
                    {
                        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
                        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);

                        DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
                        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
                    }
                    else if (state == 3)
                    {
                        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
                        DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));

                        DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
                        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));

                        DrawRectangle(screenWidth/2 - 112, screenHeight/2 - 112, 224, 224, Fade(RAYWHITE, alpha));

                        DrawText(TextSubtext("raylib", 0, lettersCount), screenWidth/2 - 44, screenHeight/2 + 48, 50, Fade(BLACK, alpha));
                    }
                } break;
                case MENU:
                {
                    DrawText("FOUR COLOR CONJECTURE", 130, 30, 40, MAROON);
                    
                    if (mouseOnPlayButton)
                    {
                        DrawRectangleRec(playButton, MAROON);
                        DrawText("PLAY", 360, 107, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(playButton, LIGHTGRAY);
                        DrawText("PLAY", 360, 107, 40, GRAY);
                    }
                    if (mouseOnHelpButton)
                    {
                        DrawRectangleRec(helpButton, MAROON);
                        DrawText("INSTRUCTIONS", 253, 176, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(helpButton, LIGHTGRAY);
                        DrawText("INSTRUCTIONS", 253, 176, 40, GRAY);
                    }
                    if (mouseOnAboutButton)
                    {
                        DrawRectangleRec(aboutButton, MAROON);
                        DrawText("ABOUT", 345, 247, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(aboutButton, LIGHTGRAY);
                        DrawText("ABOUT", 345, 247, 40, GRAY);
                    }
                    if (mouseOnQuitButton)
                    {
                        DrawRectangleRec(quitButton, MAROON);
                        DrawText("QUIT", 360, 317, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(quitButton, LIGHTGRAY);
                        DrawText("QUIT", 360, 317, 40, GRAY);
                    }
                } break;
                case HELP:
                {
                    DrawText("INSTRUCTIONS", 253, 30, 40, MAROON);
                    
                    //Select/switch colors by clicking the square
                    //Color the squares such that no two adjacent squares share the same color
                    //The time you have is based on the number of edges you entered
                    DrawText("-Select/switch colors by clicking the square", 180, 150, 20, DARKGRAY);
                    DrawText("-Color the squares such that no two adjacent", 180, 190, 20, DARKGRAY);
                    DrawText("squares share the same color", 190, 210, 20, DARKGRAY);
                    DrawText("-The time you have is based on the number", 180, 250, 20, DARKGRAY);
                    DrawText("of edges you entered", 190, 270, 20, DARKGRAY);
                    
                    if (mouseOnBackButton)
                    {
                        DrawRectangleRec(backButton, MAROON);
                        DrawText("BACK", 360, 377, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(backButton, LIGHTGRAY);
                        DrawText("BACK", 360, 377, 40, GRAY);
                    }
                } break;
                case ABOUT:
                {
                    for (int i = 0; i < 15; i++)
                    {
                        DrawRectangle(randomX[i], (framesCounter * randomSpeed[i]) - 50, 50, 50, DARKGRAY);
                    }
                    
                    DrawText("ABOUT", 345, 30, 40, MAROON);
                    
                    DrawText("Game made by: ", 300, 120, 20, MAROON);
                    DrawText("MD Adlaon", 300, 160, 20, DARKGRAY);
                    DrawText("Made with C++ and raylib.", 300, 200, 20, DARKGRAY);
                    
                    if (mouseOnBackButton)
                    {
                        DrawRectangleRec(backButton, MAROON);
                        DrawText("BACK", 360, 377, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(backButton, LIGHTGRAY);
                        DrawText("BACK", 360, 377, 40, GRAY);
                    }
                } break;
                case PLAY: 
                {
                    DrawText("PLAY", 345, 30, 40, MAROON);
                    
                    DrawText("Vertices:", 285, 82, 20, GRAY);
                    DrawRectangleRec(verticesBox, LIGHTGRAY);
                    if (mouseOnVertices || CheckCollisionPointRec(GetMousePosition(), verticesBox)) DrawRectangleLines(verticesBox.x, verticesBox.y, verticesBox.width, verticesBox.height, RED);
                    else DrawRectangleLines(verticesBox.x, verticesBox.y, verticesBox.width, verticesBox.height, DARKGRAY);
                    DrawText(inputVertices, verticesBox.x + 5, verticesBox.y + 8, 40, MAROON);
                    if (mouseOnVertices)
                    {
                        if (verticesCount < MAX_INPUT_CHARS)
                        {
                            // Draw blinking underscore char
                            if (((framesCounter/20)%2) == 0) DrawText("_", verticesBox.x + 8 + MeasureText(inputVertices, 40), verticesBox.y + 12, 40, MAROON);
                        }
                    }
                    
                    DrawText("Edges:", 285, 167, 20, GRAY);
                    DrawRectangleRec(edgesBox, LIGHTGRAY);
                    if (mouseOnEdges || CheckCollisionPointRec(GetMousePosition(), edgesBox)) DrawRectangleLines(edgesBox.x, edgesBox.y, edgesBox.width, edgesBox.height, RED);
                    else DrawRectangleLines(edgesBox.x, edgesBox.y, edgesBox.width, edgesBox.height, DARKGRAY);
                    DrawText(inputEdges, edgesBox.x + 5, edgesBox.y + 8, 40, MAROON);
                    if (mouseOnEdges)
                    {
                        if (edgesCount < MAX_INPUT_CHARS)
                        {
                            // Draw blinking underscore char
                            if (((framesCounter/20)%2) == 0) DrawText("_", edgesBox.x + 8 + MeasureText(inputEdges, 40), edgesBox.y + 12, 40, MAROON);
                        }
                    }
                    
                    if (mouseOnStartButton)
                    {
                        DrawRectangleRec(startButton, MAROON);
                        DrawText("START", 330, 262, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(startButton, LIGHTGRAY);
                        DrawText("START", 330, 262, 40, GRAY);
                    }
                    if (mouseOnReturnButton)
                    {
                        DrawRectangleRec(returnButton, MAROON);
                        DrawText("BACK", 343, 327, 40, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(returnButton, LIGHTGRAY);
                        DrawText("BACK", 343, 327, 40, GRAY);
                    }
                    
                    if (invalidVertices)
                    {
                        DrawText("Vertices cannot be less than 4 and", 210, 390, 20, MAROON);
                        DrawText("cannot be greater than 20", 260, 410, 20, MAROON);
                    }
                    else if (invalidEdges)
                    {
                        DrawText("Edges cannot be less than the vertices", 190, 390, 20, MAROON);
                        DrawText("and cannot be greater than (vertices x 2) - 2", 155, 410, 20, MAROON);
                    }
                } break;
                case GAMEPLAY:
                {   
                    //Darkens screen
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(DARKGRAY, alpha));
                
                    //Draws the edges on the screen
                    for (int i = 0; i < edges; i++)
                    {
                        if (i == (vertices - 1))
                        {
                            DrawLineEx(squarePosition[i], squarePosition[(i - vertices) + 1], 2, DARKGRAY);
                        }
                        else if (i <= 8)
                        {
                            DrawLineEx(squarePosition[i], squarePosition[i + 1], 2, DARKGRAY);
                        }
                        else 
                        {
                            DrawLineBezier(squarePosition[i], squarePosition[i + 1], 2, DARKGRAY);
                        }
                    }
                    
                    //Draws the extra edges on the screen
                    for (int i = 0; i < extraEdges; i++)
                    {
                        DrawLineBezier(squarePosition[i], squarePosition[i + 2], 2, DARKGRAY);
                    }
                    
                    //Draws the square on the screen
                    for (int i = 0; i < vertices; i++)
                    {
                        if (squareClicked[i])
                        {   
                            color[i]++;
                            if (color[i] > 4) color[i] = 0;
                            vertexColor[i] = colors[color[i]];
                            squareClicked[i] = false;
                        }
                        DrawRectangleRec(square[i], vertexColor[i]);
                    }
                    
                    //Margin
                    DrawRectangle(0, 385, screenWidth, screenHeight, LIGHTGRAY);
                    
                    //Puzzle text
                    if (invalidPuzzle)
                    {
                        DrawText("Invalid Puzzle", 300, 403, 30, MAROON);
                    }
                    
                    //Submit
                    if (mouseOnSubmitButton)
                    {
                        DrawRectangleRec(submitButton, MAROON);
                        DrawText("SUBMIT", 660, 403, 30, LIGHTGRAY);
                    }
                    else
                    {
                        DrawRectangleRec(submitButton, GRAY);
                        DrawText("SUBMIT", 660, 403, 30, DARKGRAY);
                    }
                    
                    //Timer
                    DrawText(FormatText("%i", timer), 20, 400, 40, MAROON);
                } break;
                case ENDING: 
                {
                    //END SCREEN
                    if (success)
                    {
                        ClearBackground(RAYWHITE);
                        
                        for (int i = 0; i < 15; i++)
                        {
                            DrawRectangle(randomX[i], (framesCounter * randomSpeed[i]) - 50, 50, 50, DARKGRAY);
                        }
                        
                        DrawText("CONGRATULATIONS!", 80, 140, 60, MAROON);
                        DrawText("You completed the puzzle", 130, 200, 40, DARKGRAY);
                        DrawText("PRESS [ENTER] TO CONTINUE", 230, 250, 20, GRAY);
                    }
                    else
                    {
                        ClearBackground(DARKGRAY);
                        
                        for (int i = 0; i < 15; i++)
                        {
                            DrawRectangle(randomX[i], (framesCounter * randomSpeed[i]) - 50, 50, 50, MAROON);
                        }
                        
                        DrawText("GAME OVER!", 210, 140, 60, MAROON);
                        DrawText("You ran out of time", 198, 200, 40, LIGHTGRAY);
                        DrawText("PRESS [ENTER] TO CONTINUE", 230, 250, 20, GRAY);
                    }
                } break;
                default: break;
            }
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

bool IsAnyKeyPressed()
{
    bool keyPressed = false;
    int key = GetKeyPressed();

    if ((key >= 48) && (key <= 57)) keyPressed = true;

    return keyPressed;
}