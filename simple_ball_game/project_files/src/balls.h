#ifndef BALLS_H 
#define BALLS_H

#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

//structs
typedef struct Enemy{
    Vector2 position;
    Vector2 velocity; 
    int health;
}Enemy;

//function prototypes 
void update_circle_position(float *x, float *y, float radius, float speed);
void initialize_enemies(Enemy *enemies, int max_enemies);
void update_enemies(Enemy *enemies, int max_enemies);
void draw_enemies(int enemies[][4], int max_enemies);
bool check_collision(float playerX, float playerY, float playerRadius, int enemyX, int enemyY, float enemyRadius);
void play_again(bool *game_over);
void interval_spawn_enemies(Enemy *enemy, float *radius);



#endif // BALLS_H
