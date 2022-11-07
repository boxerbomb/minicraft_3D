import sys
import math
import pygame
from pygame.locals import *
 
pygame.init()
 
fps = 60
fpsClock = pygame.time.Clock()

MAP_SIZE = 40
CHUNK_SIZE = 4
FOV_ANGLE = 45
BLOCK_SIZE = 16
SCREEN_WIDTH = MAP_SIZE*BLOCK_SIZE
SCREEN_HEIGHT = MAP_SIZE*BLOCK_SIZE
 

screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))




color_list_dark = [
(255,255,255),
(127,127,127)
]
color_list_light = [
(126,200,80),
(139,69,19)
]

SPEED = 3
REACH = 16
VIEW_DISTANCE = 320



player1X = 180
player1Y = 80
player1_theta = 0

world = [
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1],
    [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
    ]



def inTriangle(p1, p2, p3, test_point):
  alpha = ((p2[1] - p3[1])*(test_point[0] - p3[0]) + (p3[0] - p2[0])*(test_point[1] - p3[1])) / ((p2[1] - p3[1])*(p1[0] - p3[0]) + (p3[0] - p2[0])*(p1[1] - p3[1]))
  beta = ((p3[1] - p1[1])*(test_point[0] - p3[0]) + (p1[0] - p3[0])*(test_point[1] - p3[1])) / ((p2[1] - p3[1])*(p1[0] - p3[0]) + (p3[0] - p2[0])*(p1[1] - p3[1]))
  gamma = 1.0 - alpha - beta

  if alpha>0 and beta>0 and gamma>0:
    return True
  else:
    return False

def to_Radians(degrees):
  return math.radians(degrees)


# Game loop.
while True:
  screen.fill((0, 0, 0))
  
  for event in pygame.event.get():
    if event.type == QUIT:
      pygame.quit()
      sys.exit()
  

  reach_pos = (player1X+(REACH*math.cos(to_Radians(player1_theta)))  ,player1Y+(REACH*math.sin(to_Radians(player1_theta))))
  collide_pos = (player1X+(9*math.cos(to_Radians(player1_theta)))  ,player1Y+(9*math.sin(to_Radians(player1_theta))))

  active_rec = (math.floor(reach_pos[0]/BLOCK_SIZE),math.floor(reach_pos[1]/BLOCK_SIZE))

  move_collide_rect_pos = (math.floor(collide_pos[0]/BLOCK_SIZE),math.floor(collide_pos[1]/BLOCK_SIZE))
  collide_tile_index = world[move_collide_rect_pos[1]][move_collide_rect_pos[0]] 

  # Update.
  keys=pygame.key.get_pressed()
  if keys[K_LEFT]:
    player1_theta -= 3
    if player1_theta<0:
      player1_theta=360
  if keys[K_RIGHT]:
    player1_theta += 3
    if player1_theta>359:
      player1_theta=0
  if keys[K_UP]:
    if collide_tile_index==0:
      player1X += SPEED*math.cos(to_Radians(player1_theta))
      player1Y += SPEED*math.sin(to_Radians(player1_theta))
  if keys[K_DOWN]:
    player1X -= SPEED*math.cos(to_Radians(player1_theta))
    player1Y -= SPEED*math.sin(to_Radians(player1_theta))
  

  left_FOV = (player1X+(VIEW_DISTANCE*math.cos(to_Radians(player1_theta-(FOV_ANGLE/2))))  ,player1Y+(VIEW_DISTANCE*math.sin(to_Radians(player1_theta-(FOV_ANGLE/2)))))
  right_FOV = (player1X+(VIEW_DISTANCE*math.cos(to_Radians(player1_theta+(FOV_ANGLE/2))))  ,player1Y+(VIEW_DISTANCE*math.sin(to_Radians(player1_theta+(FOV_ANGLE/2)))))

  # Draw MAP
  #for y in range(0,MAP_SIZE):
  #  for x in range(0,MAP_SIZE):
  #    pygame.draw.rect(screen, color_list[world[y][x]], pygame.Rect(x*16, y*16, 16, 16))
  for y in range(0,int(MAP_SIZE/CHUNK_SIZE)):
    for x in range(0,int(MAP_SIZE/CHUNK_SIZE)):
      is_seen = False
      left_top_pos = (x*(BLOCK_SIZE*CHUNK_SIZE),y*(BLOCK_SIZE*CHUNK_SIZE))
      right_top_pos = ((x+1)*(BLOCK_SIZE*CHUNK_SIZE),y*(BLOCK_SIZE*CHUNK_SIZE))
      left_bottom_pos = (x*(BLOCK_SIZE*CHUNK_SIZE),(y+1)*(BLOCK_SIZE*CHUNK_SIZE))
      right_bottom_pos = ((x+1)*(BLOCK_SIZE*CHUNK_SIZE),(y+1)*(BLOCK_SIZE*CHUNK_SIZE))
      if inTriangle( (player1X,player1Y),left_FOV,right_FOV,left_top_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,right_top_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,left_bottom_pos) or inTriangle( (player1X,player1Y),left_FOV,right_FOV,right_bottom_pos):
        is_seen = True

      for ay in range(0,CHUNK_SIZE):
        for ax in range(0,CHUNK_SIZE):
          if is_seen:
            active_color = color_list_light[world[(y*CHUNK_SIZE)+ay][(x*CHUNK_SIZE)+ax]]
          else:
            active_color = color_list_dark[world[(y*CHUNK_SIZE)+ay][(x*CHUNK_SIZE)+ax]]
          pygame.draw.rect(screen, active_color, pygame.Rect(((x*CHUNK_SIZE)+ax)*BLOCK_SIZE, ((y*CHUNK_SIZE)+ay)*BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE))

  # Draw Chunk Boundaries
  for x in range(0,MAP_SIZE-1):
    pygame.draw.line(screen, (0,0,0), (x*BLOCK_SIZE,0),  ( x*BLOCK_SIZE, SCREEN_HEIGHT ))
  for y in range(0,MAP_SIZE-1):
    pygame.draw.line(screen, (0,0,0), (0,y*BLOCK_SIZE),  ( SCREEN_WIDTH, y*BLOCK_SIZE ))


  pygame.draw.rect(screen, (255,0,255), pygame.Rect(active_rec[0]*16,active_rec[1]*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE),2)
  pygame.draw.circle(screen, (200,12,67), (player1X,player1Y), 8)
  pygame.draw.line(screen, (0,0,255),(player1X,player1Y),reach_pos,4)

  pygame.draw.line(screen, (255,0,255), (player1X,player1Y), left_FOV)
  pygame.draw.line(screen, (255,0,255), (player1X,player1Y), right_FOV)
  pygame.draw.line(screen, (255,0,255), left_FOV, right_FOV)


    # Draw Chunk Boundaries
  for x in range(0,int((MAP_SIZE/CHUNK_SIZE))+1):
    for y in range(0,int((MAP_SIZE/CHUNK_SIZE))+1):
      marker_pos = (x*(BLOCK_SIZE*CHUNK_SIZE),y*(BLOCK_SIZE*CHUNK_SIZE))
      if inTriangle( (player1X,player1Y),left_FOV,right_FOV,marker_pos ):
        color_sel = (255,0,0)
      else:
        color_sel= (0,0,0)
      pygame.draw.circle(screen, color_sel, marker_pos,8)
  
  pygame.display.flip()
  fpsClock.tick(fps)
