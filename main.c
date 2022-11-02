/*******************************************************************************************
*
*   raylib [core] example - Keyboard input
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <stddef.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "mapgen.h"

// Holds the tile images
Image tile_images[22];
Texture2D tile_textures[22];

Camera cameraPlayer1 = { 0 };
float player1_theta = 0.0;
Camera cameraPlayer2 = { 0 };
float player2_theta = 0.0;


// Globals for map data
u8 map[5][128*128];
u8 data[5][128*128];

u8 scroll_offset_x = 0;
u8 scroll_offset_y = 0;


// Scene drawing
void DrawScene(RenderTexture *floor_texture)
{
    int count = 5;
    float spacing = 4;
    //wall_tex = LoadTextureFromImage(wall_image);


    // Grid of cube trees on a plane to make a "world"
    //DrawPlane((Vector3){ SCREEN_WIDTH/2, -8.0f, SCREEN_HEIGHT/2 }, (Vector2){ 128*16, 128*16 }, BEIGE); // Simple world plane
    DrawCubeTexture(floor_texture->texture,(Vector3) { (128*16)/2, -8.0f, (128*16)/2 }, 128*16, 0.01f, 128*16, WHITE);

        for(int y=0; y<127; y++){
            for(int x=0; x<127; x++){       
                u8 tile_index = map[1][((y+scroll_offset_y)*128)+(x+scroll_offset_x)];
                //DrawTexture(tile_textures[tile_index], x*16, y*16, WHITE);
                if(tile_index==TILE_TREE || tile_index==TILE_ROCK){
                	DrawCubeTexture(tile_textures[tile_index],(Vector3) { (x*16)+(16/2), 1.0f, (y*16)+(16/2) }, 16, 16, 16, WHITE);
                }
                
            }
        }

    // Draw a cube at each player's position
    DrawCube(cameraPlayer1.position, 1, 1, 1, RED);
    DrawCube(cameraPlayer2.position, 1, 1, 1, BLUE);
}

void initNewMap() {
    newSeed();
    createAndValidateSkyMap(128, 128, map[0], data[0]);
    createAndValidateTopMap(128, 128, map[1], data[1]);
    createAndValidateUndergroundMap(128, 128, 1, map[2], data[2]);
    createAndValidateUndergroundMap(128, 128, 2, map[3], data[3]);
    createAndValidateUndergroundMap(128, 128, 3, map[4], data[4]);
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - keyboard input");


    RenderTexture floor_texture = LoadRenderTexture(128*16, 128*16);

	// Setup player 1 camera and screen
	cameraPlayer1.fovy = 45.0f;
	cameraPlayer1.up.y = 1.0f;
	cameraPlayer1.target.y = 1.0f;
	cameraPlayer1.position.z = -3.0f;
	cameraPlayer1.position.y = 1.0f;
	RenderTexture screenPlayer1 = LoadRenderTexture(SCREEN_WIDTH/2, SCREEN_HEIGHT);
	// Setup player two camera and screen
	cameraPlayer2.fovy = 45.0f;
	cameraPlayer2.up.y = 1.0f;
	cameraPlayer2.target.y = 1.0f;
	cameraPlayer2.position.x = -3.0f;
	cameraPlayer2.position.y = 1.0f;
	RenderTexture screenPlayer2 = LoadRenderTexture(SCREEN_WIDTH / 2, SCREEN_HEIGHT);

	Rectangle splitScreenRect = { 0.0f, 0.0f, (float)screenPlayer1.texture.width, (float)-screenPlayer1.texture.height };






    tile_images[TILE_GRASS] = LoadImage("./res/grass.png");
    tile_images[TILE_TREE] = LoadImage("./res/tree.png");
    tile_images[TILE_ROCK] = LoadImage("./res/rock.png");
    tile_images[TILE_DIRT] = LoadImage("./res/dirt.png");
    tile_images[TILE_SAND] = LoadImage("./res/sand.png");
    tile_images[TILE_WATER] = LoadImage("./res/water.png");
    tile_images[TILE_LAVA] = LoadImage("./res/lava.png");
    tile_images[TILE_CACTUS] = LoadImage("./res/cactus.png");
    tile_images[TILE_FLOWER] = LoadImage("./res/flower.png");
    tile_images[TILE_IRONORE] = LoadImage("./res/ironore.png");
    tile_images[TILE_GOLDORE] = LoadImage("./res/goldore.png");
    tile_images[TILE_GEMORE] = LoadImage("./res/gemore.png");
    tile_images[TILE_FARM] = LoadImage("./res/farm.png");
    tile_images[TILE_WHEAT] = LoadImage("./res/wheat.png");
    tile_images[TILE_SAPLING_TREE] = LoadImage("./res/sapling_tree.png");
    tile_images[TILE_SAPLING_CACTUS] = LoadImage("./res/sapling_cactus.png");
    tile_images[TILE_STAIRS_DOWN] = LoadImage("./res/stairs_down.png");
    tile_images[TILE_STAIRS_UP] = LoadImage("./res/stairs_up.png");
    tile_images[TILE_CLOUD] = LoadImage("./res/cloud.png");
    tile_images[TILE_HARDROCK] = LoadImage("./res/hardrock.png");
    tile_images[TILE_CLOUDCACTUS] = LoadImage("./res/cloudcactus.png");
    tile_images[TILE_HOLE] = LoadImage("./res/hole.png");

    for(int i=0; i<22; i++){
        tile_textures[i] = LoadTextureFromImage(tile_images[i]);
        UnloadImage(tile_images[i]);
    }

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initNewMap();

    /*
    //Uncomment to print maps to console
    for(int level=0; level<5; level++){

        for(int y=0; y<127; y++){
            for(int x=0; x<127; x++){
                u8 tile_index = map[level][(y*128)+x];
                if(tile_index == TILE_GRASS){
                    printf("#");
                }else if(tile_index == TILE_ROCK){
                    printf("o");
                }else if(tile_index == TILE_TREE){
                    printf("^");
                }else if(tile_index == TILE_WATER){
                    printf("~");
                }else{
                    printf(" ");
                }
            }
            printf("\n");
        }
        printf("\n---------------------------------------------------------------------------------------------------------------------\n");
    }
    */

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
float offsetThisFrame = 10.0f*GetFrameTime();

        // Move Player1 forward and backwards (no turning)
        if (IsKeyDown(KEY_W))
        {
            cameraPlayer1.position.z += PLAYER_SPEED*offsetThisFrame*cos(player1_theta);
            cameraPlayer1.position.x += PLAYER_SPEED*offsetThisFrame*sin(player1_theta);
            cameraPlayer1.target.x = cameraPlayer1.position.x + sin(player1_theta);
            cameraPlayer1.target.z = cameraPlayer1.position.z + cos(player1_theta);
        }
        else if (IsKeyDown(KEY_S))
        {
            cameraPlayer1.position.z -= PLAYER_SPEED*offsetThisFrame*cos(player1_theta);
            cameraPlayer1.position.x -= PLAYER_SPEED*offsetThisFrame*sin(player1_theta);
            cameraPlayer1.target.x = cameraPlayer1.position.x + sin(player1_theta);
            cameraPlayer1.target.z = cameraPlayer1.position.z + cos(player1_theta);
        }else if (IsKeyDown(KEY_A))
        {
            player1_theta += (offsetThisFrame*0.2);
            cameraPlayer1.target.x = cameraPlayer1.position.x + sin(player1_theta);
            cameraPlayer1.target.z = cameraPlayer1.position.z + cos(player1_theta);

        }else if (IsKeyDown(KEY_D))
        {
            player1_theta -= (offsetThisFrame*0.2);
            cameraPlayer1.target.x = cameraPlayer1.position.x + sin(player1_theta);
            cameraPlayer1.target.z = cameraPlayer1.position.z + cos(player1_theta);
        }

        // Move Player2 forward and backwards (no turning)
        if (IsKeyDown(KEY_UP))
        {
            cameraPlayer2.position.z += PLAYER_SPEED*offsetThisFrame*cos(player2_theta);
            cameraPlayer2.position.x += PLAYER_SPEED*offsetThisFrame*sin(player2_theta);
            cameraPlayer2.target.x = cameraPlayer2.position.x + sin(player2_theta);
            cameraPlayer2.target.z = cameraPlayer2.position.z + cos(player2_theta);
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            cameraPlayer2.position.z -= PLAYER_SPEED*offsetThisFrame*cos(player2_theta);
            cameraPlayer2.position.x -= PLAYER_SPEED*offsetThisFrame*sin(player2_theta);
            cameraPlayer2.target.x = cameraPlayer2.position.x + sin(player2_theta);
            cameraPlayer2.target.z = cameraPlayer2.position.z + cos(player2_theta);
        }else if (IsKeyDown(KEY_LEFT))
        {
            player2_theta += (offsetThisFrame*0.2);
            cameraPlayer2.target.x = cameraPlayer2.position.x + sin(player2_theta);
            cameraPlayer2.target.z = cameraPlayer2.position.z + cos(player2_theta);

        }else if (IsKeyDown(KEY_RIGHT))
        {
            player2_theta -= (offsetThisFrame*0.2);
            cameraPlayer2.target.x = cameraPlayer2.position.x + sin(player2_theta);
            cameraPlayer2.target.z = cameraPlayer2.position.z + cos(player2_theta);
        }
        //----------------------------------------------------------------------------------


        BeginTextureMode(floor_texture);
        BeginDrawing();
        ClearBackground((Color){77,37,28});
        for(int y=0; y<127; y++){
            for(int x=0; x<127; x++){
                u8 tile_index = map[1][((y+scroll_offset_y)*128)+(x+scroll_offset_x)];
                DrawTexture(tile_textures[tile_index], x*16, y*16, WHITE);
            }
        }
        EndDrawing();
        EndTextureMode();



        // Draw Player1 view to the render texture
        BeginTextureMode(screenPlayer1);
            ClearBackground(SKYBLUE);
            BeginMode3D(cameraPlayer1);
                DrawScene(&floor_texture);
            EndMode3D();
            DrawText("PLAYER1 W/S to move", 10, 10, 20, RED);
        EndTextureMode();

        // Draw Player2 view to the render texture
        BeginTextureMode(screenPlayer2);
            ClearBackground(SKYBLUE);
            BeginMode3D(cameraPlayer2);
                DrawScene(&floor_texture);
            EndMode3D();
            DrawText("PLAYER2 UP/DOWN to move", 10, 10, 20, BLUE);
        EndTextureMode();

        // Draw both views render textures to the screen side by side
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(screenPlayer1.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
            DrawTextureRec(screenPlayer2.texture, splitScreenRect, (Vector2){ SCREEN_WIDTH/2.0f, 0 }, WHITE);
        EndDrawing();


    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}