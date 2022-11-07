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
Image tile_images[NUM_TILES];
Texture2D tile_textures[NUM_TILES];
struct TileData tile_data[NUM_TILES];

struct PlayerData player_list[2];



// Globals for map data
u8 map[5][WORLD_SIZE*WORLD_SIZE];
u8 data[5][WORLD_SIZE*WORLD_SIZE];

u8 scroll_offset_x = 0;
u8 scroll_offset_y = 0;


// Scene drawing
void DrawScene(RenderTexture *floor_texture)
{
    int count = 5;
    float spacing = 4;
    //wall_tex = LoadTextureFromImage(wall_image);


    // Grid of cube trees on a plane to make a "world"
    //DrawPlane((Vector3){ SCREEN_WIDTH/2, -8.0f, SCREEN_HEIGHT/2 }, (Vector2){ WORLD_SIZE*16, WORLD_SIZE*16 }, BEIGE); // Simple world plane
    DrawCubeTexture(floor_texture->texture,(Vector3) { (WORLD_SIZE*16)/2, -8.0f, (WORLD_SIZE*16)/2 }, WORLD_SIZE*16, 0, WORLD_SIZE*16, WHITE);


        for(int y=0; y<WORLD_SIZE; y++){
            for(int x=0; x<WORLD_SIZE; x++){       
                u8 tile_index = map[1][((y+scroll_offset_y)*WORLD_SIZE)+(x+scroll_offset_x)];
                //DrawTexture(tile_textures[tile_index], x*16, y*16, WHITE);

                /*
                if(tile_index==TILE_TREE){
                    DrawCubeTexture(tile_textures[TILE_TREE_TRUNK],(Vector3) { (x*16)+(16/2), -1.0f, (y*16)+(16/2) }, 8, 16, 8, WHITE);
                    // Drawing these criss-cross trees drops the FPS a lot.
                    DrawCubeTexture(tile_textures[TILE_TREE],(Vector3) { (x*16)+(16/2), 15.0f, (y*16)+(16/2) }, 20, 20, 0, WHITE);
                    DrawCubeTexture(tile_textures[TILE_TREE],(Vector3) { (x*16)+(16/2), 15.0f, (y*16)+(16/2) }, 0, 20, 20, WHITE);
                }else if(tile_index==TILE_ROCK){
                	DrawCubeTexture(tile_textures[tile_index],(Vector3) { (x*16)+(16/2), -1.0f, (y*16)+(16/2) }, 16, 16, 16, WHITE);
                }
                */  

                // If the tile has been identified as one that is 3d (not a floor tile), draw it to screen
                // There are special cases like the tree that are not cubes though
                if(tile_data[tile_index].draw_3d == true){
                    if(tile_index==TILE_TREE){
                        DrawCubeTexture(tile_data[TILE_TREE_TRUNK].texture,(Vector3) { (x*16)+(16/2), -1.0f, (y*16)+(16/2) }, 8, 16, 8, WHITE);
                        DrawCubeTexture(tile_data[TILE_TREE].texture,(Vector3) { (x*16)+(16/2), 15.0f, (y*16)+(16/2) }, 20, -20, 0, WHITE);
                        DrawCubeTexture(tile_data[TILE_TREE].texture,(Vector3) { (x*16)+(16/2), 15.0f, (y*16)+(16/2) }, 0, -20, 20, WHITE);
                    }else if(tile_index==TILE_CACTUS){
                        DrawCubeTexture(tile_data[TILE_CACTUS_TRUNK].texture,(Vector3) { (x*16)+(16/2), -1.0f, (y*16)+(16/2) }, 6, 16, 6, WHITE);
                    }else{
                        DrawCubeTexture(tile_data[tile_index].texture,(Vector3) { (x*16)+(16/2), 0.1f, (y*16)+(16/2) }, 16, 16, 16, WHITE);
                    }
                }



                
            }
        }

    // Draw a cube at each player's position
    DrawCube(player_list[0].cam.position, 16, 16, 16, RED);
    DrawCube(player_list[1].cam.position, 16, 16, 16, BLUE);
}

void initNewMap() {
    newSeed();
    createAndValidateSkyMap(WORLD_SIZE, WORLD_SIZE, map[0], data[0]);
    createAndValidateTopMap(WORLD_SIZE, WORLD_SIZE, map[1], data[1]);
    createAndValidateUndergroundMap(WORLD_SIZE, WORLD_SIZE, 1, map[2], data[2]);
    createAndValidateUndergroundMap(WORLD_SIZE, WORLD_SIZE, 2, map[3], data[3]);
    createAndValidateUndergroundMap(WORLD_SIZE, WORLD_SIZE, 3, map[4], data[4]);
}


// Unfortunetly it seems to be easier to use the player cameras position as the player position
void setupPlayers(){
    for(int i=0; i<NUM_PLAYERS; i++){

        //Init Camera
        //player_list[i].cam = { 0 };
        // Camera starting position
        player_list[i].cam.fovy = 45.0f;
        player_list[i].cam.up.y = 1.0f;
        player_list[i].cam.target.y = 1.0f;
        player_list[i].cam.position.z = -3.0f;
        player_list[i].cam.position.y = 1.0f;

        // This is the canvas that the players screen will be drawn on
        // These are screen sizes not screen position
        // 4 player will require different sized screens
        player_list[i].screen = LoadRenderTexture(SCREEN_WIDTH/2, SCREEN_HEIGHT);

        player_list[i].theta = 0;
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minicraft 3D");


    RenderTexture floor_texture = LoadRenderTexture(WORLD_SIZE*16, WORLD_SIZE*16);


    setupPlayers();

    // This is the Rectangle representing the position and size of our 2-player split screen, this will need to be changed for 4-players
	Rectangle splitScreenRect = { 0.0f, 0.0f, (float)player_list[0].screen.texture.width, (float)-player_list[0].screen.texture.height };




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
    tile_images[TILE_TREE_TRUNK] = LoadImage("./res/tree_trunk.png");
    tile_images[TILE_CACTUS_TRUNK] = LoadImage("./res/cactus_trunk.png");


    for(int i=0; i<NUM_TILES; i++){
        struct TileData new_tile_data;
        new_tile_data.tile_index = i;
        new_tile_data.minimap_color = 0;

        // Needed to prevent Raylib from drawing textures upseide down
        ImageFlipVertical(&tile_images[i]);
        new_tile_data.texture = LoadTextureFromImage(tile_images[i]);

        if(i==TILE_GRASS){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_TREE){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_ROCK){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_DIRT){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_SAND){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_WATER){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_LAVA){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_CACTUS){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_FLOWER){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_IRONORE){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_GOLDORE){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_GEMORE){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_FARM){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_WHEAT){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_SAPLING_TREE){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_SAPLING_CACTUS){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_STAIRS_DOWN){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_CLOUD){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_HARDROCK){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_CLOUDCACTUS){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_HOLE){
            new_tile_data.draw_3d = false;
        }else if(i==TILE_TREE_TRUNK){
            new_tile_data.draw_3d = true;
        }else if(i==TILE_CACTUS_TRUNK){
            new_tile_data.draw_3d = true;
        }else{
            printf("ERROR\n");
        }

        tile_data[i] = new_tile_data;

        tile_textures[i] = LoadTextureFromImage(tile_images[i]);
        UnloadImage(tile_images[i]);
    }

    //SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    initNewMap();

    /*    
    //Uncomment to print maps to console
    for(int level=0; level<5; level++){

        for(int y=0; y<WORLD_SIZE; y++){
            for(int x=0; x<WORLD_SIZE; x++){
                u8 tile_index = map[level][(y*WORLD_SIZE)+x];
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
    

    // Draw the floor texture, this will be used by both screens
    // For now, only the ground level, but later each level floor texture will need to be saved
    BeginTextureMode(floor_texture);
    BeginDrawing();
    ClearBackground((Color){77,37,28,255});
    for(int y=0; y<WORLD_SIZE; y++){
        for(int x=0; x<WORLD_SIZE; x++){
            u8 tile_index = map[1][((y+scroll_offset_y)*WORLD_SIZE)+(x+scroll_offset_x)];

            // Only Draw the texture to the floor, if it is a floor tile and not a 3D object
            if(tile_data[tile_index].draw_3d==false){
                DrawTexture(tile_textures[tile_index], x*16, y*16, WHITE);
            }
            
        }
    }
    EndDrawing();
    EndTextureMode();


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
float offsetThisFrame = 10.0f*GetFrameTime();

        // Move Player1 forward and backwards (no turning)
        if (IsKeyDown(KEY_W))
        {
            player_list[0].cam.position.z += PLAYER_SPEED*offsetThisFrame*cos(player_list[0].theta);
            player_list[0].cam.position.x += PLAYER_SPEED*offsetThisFrame*sin(player_list[0].theta);
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(player_list[0].theta);
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(player_list[0].theta);
        }
        else if (IsKeyDown(KEY_S))
        {
            player_list[0].cam.position.z -= PLAYER_SPEED*offsetThisFrame*cos(player_list[0].theta);
            player_list[0].cam.position.x -= PLAYER_SPEED*offsetThisFrame*sin(player_list[0].theta);
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(player_list[0].theta);
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(player_list[0].theta);
        }
        if (IsKeyDown(KEY_A)){
            player_list[0].theta += (offsetThisFrame*0.2);
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(player_list[0].theta);
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(player_list[0].theta);

        }else if (IsKeyDown(KEY_D))
        {

            player_list[0].theta -= (offsetThisFrame*0.2);
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(player_list[0].theta);
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(player_list[0].theta);
        }



        // Move Player2 forward and backwards (no turning)
        if (IsKeyDown(KEY_UP))
        {
            player_list[1].cam.position.z += PLAYER_SPEED*offsetThisFrame*cos(player_list[1].theta);
            player_list[1].cam.position.x += PLAYER_SPEED*offsetThisFrame*sin(player_list[1].theta);
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(player_list[1].theta);
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(player_list[1].theta);
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            player_list[1].cam.position.z -= PLAYER_SPEED*offsetThisFrame*cos(player_list[1].theta);
            player_list[1].cam.position.x -= PLAYER_SPEED*offsetThisFrame*sin(player_list[1].theta);
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(player_list[1].theta);
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(player_list[1].theta);
        }
        if (IsKeyDown(KEY_LEFT))
        {
            player_list[1].theta += (offsetThisFrame*0.2);
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(player_list[1].theta);
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(player_list[1].theta);

        }else if (IsKeyDown(KEY_RIGHT))
        {
            player_list[1].theta -= (offsetThisFrame*0.2);
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(player_list[1].theta);
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(player_list[1].theta);
        }
        //----------------------------------------------------------------------------------



        // Draw Player1 view to the render texture
        BeginTextureMode(player_list[0].screen);
            ClearBackground(SKYBLUE);
            BeginMode3D(player_list[0].cam);
                DrawScene(&floor_texture);
            EndMode3D();
            //DrawText("PLAYER1 W/S to move", 10, 10, 20, RED);
            DrawFPS(10, 10);  
        EndTextureMode();

        // Draw Player2 view to the render texture
        BeginTextureMode(player_list[1].screen);
            ClearBackground(SKYBLUE);
            BeginMode3D(player_list[1].cam);
                DrawScene(&floor_texture);
            EndMode3D();
            //DrawText("PLAYER2 UP/DOWN to move", 10, 10, 20, BLUE);
        EndTextureMode();

        // Draw both views render textures to the screen side by side
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(player_list[0].screen.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
            DrawTextureRec(player_list[1].screen.texture, splitScreenRect, (Vector2){ SCREEN_WIDTH/2.0f, 0 }, WHITE);
        EndDrawing();


    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}