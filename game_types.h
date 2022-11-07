#include <stdint.h>
#include "raylib.h"

struct TileData{
	int tile_index;
	bool draw_3d;
	int minimap_color;
	Texture2D texture;
};

struct PlayerData{
	Camera cam;
	Color playerColor;
	float theta;
	RenderTexture screen;
};

typedef uint8_t u8;

#define NUM_PLAYERS 2
#define PLAYER_SPEED 40

#define SCREEN_HEIGHT 800
#define SCREEN_WIDTH 1600

#define WORLD_SIZE 128

#define MENU_NONE 0
#define MENU_TITLE 1
#define MENU_TUTORIAL 2
#define MENU_ABOUT 3
#define MENU_SETTINGS 4
#define MENU_INVENTORY 5
#define MENU_CRAFTING 6
#define MENU_CONTAINER 7
#define MENU_WIN 8
#define MENU_LOSE 9
#define MENU_PAUSED 10
#define MENU_LOADGAME 11
#define MENU_SETTINGS_REBIND 12
#define MENU_SETTINGS_TP 13

#define TILE_NULL 255
#define TILE_GRASS 0
#define TILE_TREE 1
#define TILE_ROCK 2
#define TILE_DIRT 3
#define TILE_SAND 4
#define TILE_WATER 5
#define TILE_LAVA 6
#define TILE_CACTUS 7
#define TILE_FLOWER 8
#define TILE_IRONORE 9
#define TILE_GOLDORE 10
#define TILE_GEMORE 11
#define TILE_FARM 12
#define TILE_WHEAT 13
#define TILE_SAPLING_TREE 14
#define TILE_SAPLING_CACTUS 15
#define TILE_STAIRS_DOWN 16
#define TILE_STAIRS_UP 17
#define TILE_CLOUD 18
#define TILE_HARDROCK 19
#define TILE_CLOUDCACTUS 20
#define TILE_HOLE 21
#define TILE_TREE_TRUNK 22
#define TILE_CACTUS_TRUNK 23
#define NUM_TILES 24