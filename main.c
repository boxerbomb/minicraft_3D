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

Image hotbar_image;
Texture2D hotbar_texture;

struct PlayerData player_list[2];



// Globals for map data
u8 map[5][WORLD_SIZE*WORLD_SIZE];
u8 data[5][WORLD_SIZE*WORLD_SIZE];

// NUM_CHUNKS is actually NUM_CHUNKS in one direction
bool drawChunk[NUM_CHUNKS*NUM_CHUNKS];


bool FindPoint(int x1, int y1, int x2, int y2, int x, int y)
{
    if (x > x1 && x < x2 && y > y1 && y < y2)
        return true;
 
    return false;
}

bool inChunk(struct Position chunkPos, struct Position player_pos){
    int realX1 = chunkPos.x*(CHUNK_SIZE*CHUNK_SIZE);
    int realY1 = chunkPos.y*(CHUNK_SIZE*CHUNK_SIZE);
    int realX2 = (chunkPos.x+1)*(CHUNK_SIZE*CHUNK_SIZE);
    int realY2 = (chunkPos.y+1)*(CHUNK_SIZE*CHUNK_SIZE);


    if (player_pos.x > realX1 && player_pos.x < realX2 && player_pos.y > realY1 && player_pos.y < realY2){
        return true;
    }
 
    return false;

}

bool inTriangle(struct Position p1,struct Position p2,struct Position p3,struct Position test_point){
    float alpha = ((p2.y - p3.y)*(test_point.x - p3.x) + (p3.x - p2.x)*(test_point.y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
    float beta = ((p3.y - p1.y)*(test_point.x - p3.x) + (p1.x - p3.x)*(test_point.y - p3.y)) / ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
    float gamma = 1.0 - alpha - beta;

    if (alpha>0 && beta>0 && gamma>0){
        return true;
    }else{
        return false;
    }
}

float to_Radians(int degree){
    return degree * ( C_PI / 180.0 );  
}

bool renderChunk(struct Position chunk_pos){

    bool isSeen=false;

    struct Position left_top_pos = {chunk_pos.x*(BLOCK_SIZE*CHUNK_SIZE),chunk_pos.y*(BLOCK_SIZE*CHUNK_SIZE)};
    struct Position right_top_pos = {(chunk_pos.x+1)*(BLOCK_SIZE*CHUNK_SIZE),chunk_pos.y*(BLOCK_SIZE*CHUNK_SIZE)};
    struct Position left_bottom_pos = {chunk_pos.x*(BLOCK_SIZE*CHUNK_SIZE),(chunk_pos.y+1)*(BLOCK_SIZE*CHUNK_SIZE)};
    struct Position right_bottom_pos = {(chunk_pos.x+1)*(BLOCK_SIZE*CHUNK_SIZE),(chunk_pos.y+1)*(BLOCK_SIZE*CHUNK_SIZE)};
    //struct Position middle_pos = {chunk_pos.x*(BLOCK_SIZE*CHUNK_SIZE),chunk_pos.y*(BLOCK_SIZE*CHUNK_SIZE)};

    // left_top_pos = (x*(BLOCK_SIZE*CHUNK_SIZE),y*(BLOCK_SIZE*CHUNK_SIZE))
    // right_top_pos = ((x+1)*(BLOCK_SIZE*CHUNK_SIZE),y*(BLOCK_SIZE*CHUNK_SIZE))
    // left_bottom_pos = (x*(BLOCK_SIZE*CHUNK_SIZE),(y+1)*(BLOCK_SIZE*CHUNK_SIZE))
    // right_bottom_pos = ((x+1)*(BLOCK_SIZE*CHUNK_SIZE),(y+1)*(BLOCK_SIZE*CHUNK_SIZE))
    //      if inTriangle( (player1X,player1Y),left_FOV,right_FOV,left_top_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,right_top_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,left_bottom_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,right_bottom_pos):
    // is_seen = True


    //left_FOV = (player1X+(VIEW_DISTANCE*math.cos(to_Radians(player1_theta-(FOV_ANGLE/2))))  ,player1Y+(VIEW_DISTANCE*math.sin(to_Radians(player1_theta-(FOV_ANGLE/2)))))
    //right_FOV = (player1X+(VIEW_DISTANCE*math.cos(to_Radians(player1_theta+(FOV_ANGLE/2))))  ,player1Y+(VIEW_DISTANCE*math.sin(to_Radians(player1_theta+(FOV_ANGLE/2)))))

    for(int player_id=0; player_id<NUM_PLAYERS; player_id++){
        //struct Position player_pos = {player_list[player_id].cam.position.x - 64*sin(to_Radians(player_list[player_id].theta)), player_list[player_id].cam.position.z- 64*cos(to_Radians(player_list[player_id].theta))};
        struct Position player_pos = {player_list[player_id].cam.position.x, player_list[player_id].cam.position.z};
        struct Position left_FOV = {player_list[player_id].cam.position.x+(VIEW_DISTANCE* sin(to_Radians(player_list[player_id].theta - (CHUNK_LOAD_FOV/2))))  , player_list[player_id].cam.position.z+(VIEW_DISTANCE * cos(to_Radians(player_list[player_id].theta - (CHUNK_LOAD_FOV/2))))};
        struct Position right_FOV = {player_list[player_id].cam.position.x+(VIEW_DISTANCE* sin(to_Radians(player_list[player_id].theta + (CHUNK_LOAD_FOV/2)))) , player_list[player_id].cam.position.z+(VIEW_DISTANCE * cos(to_Radians(player_list[player_id].theta + (CHUNK_LOAD_FOV/2))))};


        DrawCube((Vector3){left_FOV.x,10,left_FOV.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, GREEN);
        DrawCube((Vector3){right_FOV.x,10,right_FOV.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, PURPLE);
        DrawCube((Vector3){player_pos.x,20,player_pos.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, ORANGE);


        if(inChunk(chunk_pos,player_pos) || inTriangle(player_pos, left_FOV, right_FOV, left_top_pos) || inTriangle(player_pos, left_FOV, right_FOV, right_top_pos) || inTriangle(player_pos, left_FOV, right_FOV, left_bottom_pos) || inTriangle(player_pos, left_FOV, right_FOV, right_bottom_pos)){
            isSeen = true;
            //DrawCube((Vector3){left_top_pos.x,20,left_top_pos.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, RED);
            //DrawCube((Vector3){right_top_pos.x,20,right_top_pos.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, RED);
            //DrawCube((Vector3){left_bottom_pos.x,20,left_bottom_pos.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, RED);
            //DrawCube((Vector3){right_bottom_pos.x,20,right_bottom_pos.y}, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, RED);
        }
    }

    return isSeen;
}

void checkVisibleChunks(){
    for(int y=0; y<NUM_CHUNKS; y++){
        for(int x=0; x<NUM_CHUNKS; x++){
            struct Position chunkPos = {x,y};
            drawChunk[(y*NUM_CHUNKS)+x] = renderChunk(chunkPos);
        }
    }
}


void drawHotbar(int player_id){
    DrawTexture(hotbar_texture, 0, 0, WHITE);
}

void drawInventory(int player_id){
    int gap = 16;
    int item_size = 64;
    int x_offset = 400;
    int y_offset = 400;

    for(int y=0; y<5; y++){
        for(int x=0; x<5; x++){
            DrawRectangle(x*(item_size+gap)+x_offset,y*(item_size+gap)+y_offset,item_size,item_size,(Color){156, 147, 128,255});
        }
    }

}


// Scene drawing
void DrawScene(RenderTexture *floor_texture)
{
    int count = 5;
    float spacing = 4;
    //wall_tex = LoadTextureFromImage(wall_image);


    // This implementation is a first proof-of-concept for a chunk loading system
    // Every chunk in the world will be checked EVERY FRAME, the next interation might check for chunks every 10 frames or so adding to a list and then only draw the selected ones here



    // Grid of cube trees on a plane to make a "world"
    //DrawPlane((Vector3){ SCREEN_WIDTH/2, -8.0f, SCREEN_HEIGHT/2 }, (Vector2){ WORLD_SIZE*16, WORLD_SIZE*16 }, BEIGE); // Simple world plane
    DrawCubeTexture(floor_texture->texture,(Vector3) { (WORLD_SIZE*BLOCK_SIZE)/2, -8.0f, (WORLD_SIZE*BLOCK_SIZE)/2 }, WORLD_SIZE*BLOCK_SIZE, 0, WORLD_SIZE*BLOCK_SIZE, WHITE);

        for(int y=0; y<NUM_CHUNKS; y++){
            for(int x=0; x<NUM_CHUNKS; x++){
                //struct Position chunkPos = {x,y};
                if(drawChunk[(y*NUM_CHUNKS)+x]==true){
                    for(int ay=0; ay<CHUNK_SIZE; ay++){
                        for(int ax=0; ax<CHUNK_SIZE; ax++){

                            int actual_y = ((y*CHUNK_SIZE)+ay);
                            int actual_x = ((x*CHUNK_SIZE)+ax);
                            u8 tile_index = map[1][(((y*CHUNK_SIZE)+ay)*WORLD_SIZE)+((x*CHUNK_SIZE)+ax)];
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
                                    DrawCubeTexture(tile_data[TILE_TREE_TRUNK].texture,(Vector3) { (actual_x*BLOCK_SIZE)+(BLOCK_SIZE/2), -1.0f, (actual_y*BLOCK_SIZE)+(BLOCK_SIZE/2) }, BLOCK_SIZE/2, BLOCK_SIZE, BLOCK_SIZE/2, WHITE);
                                    DrawCubeTexture(tile_data[TILE_TREE].texture,(Vector3) { (actual_x*BLOCK_SIZE)+(BLOCK_SIZE/2), 15.0f, (actual_y*BLOCK_SIZE)+(BLOCK_SIZE/2) }, 20, -20, 0, WHITE);
                                    DrawCubeTexture(tile_data[TILE_TREE].texture,(Vector3) { (actual_x*BLOCK_SIZE)+(BLOCK_SIZE/2), 15.0f, (actual_y*BLOCK_SIZE)+(BLOCK_SIZE/2) }, 0, -20, 20, WHITE);
                                }else if(tile_index==TILE_CACTUS){
                                    DrawCubeTexture(tile_data[TILE_CACTUS_TRUNK].texture,(Vector3) { (actual_x*BLOCK_SIZE)+(BLOCK_SIZE/2), -1.0f, (actual_y*BLOCK_SIZE)+(BLOCK_SIZE/2) }, 6, BLOCK_SIZE, 6, WHITE);
                                }else{
                                    DrawCubeTexture(tile_data[tile_index].texture,(Vector3) { (actual_x*BLOCK_SIZE)+(BLOCK_SIZE/2), 0.1f, (actual_y*BLOCK_SIZE)+(BLOCK_SIZE/2) }, BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, WHITE);
                                }
                            }

                        }
                    }
                }

            }
        }




    // Draw a cube at each player's position
    DrawCube(player_list[0].cam.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, RED);
    DrawCube(player_list[1].cam.position, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_WIDTH, BLUE);
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
        player_list[i].cam.fovy = PLAYER_FOV;
        player_list[i].cam.up.y = 1.0f;
        player_list[i].cam.target.y = 1.0f;
        player_list[i].cam.position.z = -3.0f;
        player_list[i].cam.position.y = 1.0f;
        player_list[i].inventoryMode = false;

        // This is the canvas that the players screen will be drawn on
        // These are screen sizes not screen position
        // 4 player will require different sized screens
        player_list[i].screen = LoadRenderTexture(SCREEN_WIDTH/2, SCREEN_HEIGHT);
        player_list[i].screenUI = LoadRenderTexture(SCREEN_WIDTH/2, SCREEN_HEIGHT);
        player_list[i].screenHotbar = LoadRenderTexture(SCREEN_WIDTH/2, SCREEN_HEIGHT/10);
        player_list[i].needsHotbarUpdate = true;

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


    float CHUNK_REFRESH_COUNT = 0.0;

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Minicraft 3D");


    RenderTexture floor_texture = LoadRenderTexture(WORLD_SIZE*BLOCK_SIZE, WORLD_SIZE*BLOCK_SIZE);


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

    hotbar_image = LoadImage("./res/hotbar.png");
    ImageResize(&hotbar_image,SCREEN_WIDTH/2,SCREEN_HEIGHT/10);
    hotbar_texture = LoadTextureFromImage(hotbar_image);
    UnloadImage(hotbar_image);


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
            u8 tile_index = map[1][((y)*WORLD_SIZE)+(x)];
            // Only Draw the texture to the floor, if it is a floor tile and not a 3D object
            if(tile_data[tile_index].draw_3d==false){
                DrawTexture(tile_textures[tile_index], x*BLOCK_SIZE, y*BLOCK_SIZE, WHITE);
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


        // Temporarily the 'I' key will open both inventories, just for testing
        if (IsKeyDown(KEY_I))
        {
            player_list[0].inventoryMode = true;
            player_list[1].inventoryMode = true;
        }



        // Move Player1 forward and backwards (no turning)
        if (IsKeyDown(KEY_W))
        {
            player_list[0].cam.position.z += PLAYER_SPEED*offsetThisFrame*cos(to_Radians(player_list[0].theta));
            player_list[0].cam.position.x += PLAYER_SPEED*offsetThisFrame*sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(to_Radians(player_list[0].theta));
        }
        else if (IsKeyDown(KEY_S))
        {
            player_list[0].cam.position.z -= PLAYER_SPEED*offsetThisFrame*cos(to_Radians(player_list[0].theta));
            player_list[0].cam.position.x -= PLAYER_SPEED*offsetThisFrame*sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(to_Radians(player_list[0].theta));
        }
        if (IsKeyDown(KEY_A)){
            player_list[0].theta += (offsetThisFrame*ROTATE_SPEED);
            // I don't really think that the degree # needs to be a float, but it helps with the smooth scrolling
            //printf("%.6f\n", player_list[0].theta);
            if(player_list[0].theta > 360){player_list[0].theta=0;}
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(to_Radians(player_list[0].theta));

        }else if (IsKeyDown(KEY_D))
        {

            player_list[0].theta -= (offsetThisFrame*ROTATE_SPEED);
            //printf("%.6f\n", player_list[0].theta);
            if(player_list[0].theta < 0){player_list[0].theta=360;}
            player_list[0].cam.target.x = player_list[0].cam.position.x + sin(to_Radians(player_list[0].theta));
            player_list[0].cam.target.z = player_list[0].cam.position.z + cos(to_Radians(player_list[0].theta));
        }



        // Move Player2 forward and backwards (no turning)
        if (IsKeyDown(KEY_UP))
        {
            player_list[1].cam.position.z += PLAYER_SPEED*offsetThisFrame*cos(to_Radians(player_list[1].theta));
            player_list[1].cam.position.x += PLAYER_SPEED*offsetThisFrame*sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(to_Radians(player_list[1].theta));
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            player_list[1].cam.position.z -= PLAYER_SPEED*offsetThisFrame*cos(to_Radians(player_list[1].theta));
            player_list[1].cam.position.x -= PLAYER_SPEED*offsetThisFrame*sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(to_Radians(player_list[1].theta));
        }
        if (IsKeyDown(KEY_LEFT))
        {
            player_list[1].theta += (offsetThisFrame*ROTATE_SPEED);
            if(player_list[1].theta > 360){player_list[1].theta=0;}
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(to_Radians(player_list[1].theta));

        }else if (IsKeyDown(KEY_RIGHT))
        {
            player_list[1].theta -= (offsetThisFrame*ROTATE_SPEED);
            if(player_list[1].theta < 0){player_list[1].theta=360;}
            player_list[1].cam.target.x = player_list[1].cam.position.x + sin(to_Radians(player_list[1].theta));
            player_list[1].cam.target.z = player_list[1].cam.position.z + cos(to_Radians(player_list[1].theta));
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


        // Draw to the relevant screens only if nessesary
        if(player_list[0].inventoryMode == true){
            BeginTextureMode(player_list[0].screenUI);
            drawInventory(0);
            EndTextureMode();
        }
        if(player_list[1].inventoryMode == true){
            BeginTextureMode(player_list[1].screenUI);
            drawInventory(1);
            EndTextureMode();
        }



        // This is the hotbar, it might be a bit graohically intensive so I would only change the texture when needed
        // if the person gains XP, takes damage, or changes weapon a HOTBAR UPDATE could go true;
        if(player_list[0].needsHotbarUpdate==true){
            BeginTextureMode(player_list[0].screenHotbar);
            drawHotbar(0);
            EndTextureMode();
            player_list[0].needsHotbarUpdate = false;
        }
        if(player_list[1].needsHotbarUpdate==true){
            BeginTextureMode(player_list[1].screenHotbar);
            drawHotbar(1);
            EndTextureMode();
            player_list[1].needsHotbarUpdate = false;
        }



        // Draw both views render textures to the screen side by side
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextureRec(player_list[0].screen.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);
            DrawTextureRec(player_list[1].screen.texture, splitScreenRect, (Vector2){ SCREEN_WIDTH/2.0f, 0 }, WHITE);

            // Draw the Inventory to the screen
            if(player_list[0].inventoryMode == true){DrawTextureRec(player_list[0].screenUI.texture, splitScreenRect, (Vector2){ 0, 0 }, WHITE);}
            if(player_list[1].inventoryMode == true){DrawTextureRec(player_list[1].screenUI.texture, splitScreenRect, (Vector2){ SCREEN_WIDTH/2.0f, 0 }, WHITE);}


            DrawTextureRec(player_list[0].screenHotbar.texture, splitScreenRect, (Vector2){ 0, SCREEN_HEIGHT-(SCREEN_HEIGHT/10) }, WHITE);
            DrawTextureRec(player_list[1].screenHotbar.texture, splitScreenRect, (Vector2){ SCREEN_WIDTH/2.0f, SCREEN_HEIGHT-(SCREEN_HEIGHT/10) }, WHITE);

        EndDrawing();



        // Do math for which chunks are visible every 1/CHUNK_REFRESH seconds?
        CHUNK_REFRESH_COUNT+=offsetThisFrame;
        if(CHUNK_REFRESH_COUNT > CHUNK_REFRESH_LIMIT){
            checkVisibleChunks();
            CHUNK_REFRESH_COUNT = 0.0;
        } 


    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}