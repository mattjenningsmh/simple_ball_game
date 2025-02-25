#include "balls.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
Enemy enemies[10];
// Function prototype declarations 
void update_circle_position(float *x, float *y, float radius, float speed);
void initialize_enemies(int enemies[][4], int max_enemies);
void update_enemies(int enemies[][4], int max_enemies);
void draw_enemies(int enemies[][4], int max_enemies);
bool check_collision(float playerX, float playerY, float playerRadius, int enemyX, int enemyY, float enemyRadius);
void play_again(bool *game_over);
void interval_spawn_enemies(int * enemy, float *radius);

//Global variables
const int screenWidth = 1920;
const int screenHeight = 1080;

int main(void) {
    // Initialization
    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);  // Set game to run at 60 frames-per-second
    srand(time(NULL)); // Seed random number generator

    // Player circle position
    float circleX = screenWidth / 2.0f;  // Initial X position
    float circleY = screenHeight / 2.0f; // Initial Y position
    float radius = 30.0f;               // Circle radius
    float speed = 8.0f;                 // Movement speed

    // Enemy variables
    const int max_enemies = 100;
    int enemies[max_enemies][4]; // Array for enemy positions and velocities
    initialize_enemies(enemies, max_enemies); // Initialize enemy data
    int enemy_trajectory = 0; // 0-3 Left Right Up Down 

    double start_time = GetTime(); // Timer to spawn new enemies
    const double interval = .2;  // Spawn interval in seconds
    
    //Condition to stop the game
    bool game_over = 0;

    // Main game loop
    while (!WindowShouldClose()) { // Detect window close button or ESC key
        // Update
        update_circle_position(&circleX, &circleY, radius, speed); // Player movement

        //The goal is to spawn something every $interval seconds
        double current_time = GetTime();
        if (current_time - start_time >= interval) {
            for (int i = 0; i < max_enemies; i++) {
                if (enemies[i][2] == 0) { // Every interval, find an enemy who has gone off screen 
                    interval_spawn_enemies(enemies[i], &radius); // see TODO on in function definition
                    break; // Spawn only one enemy at a time
                }
            }
            start_time = current_time; // Reset spawn timer
        }
        //for all inactive enemies, attributes have been set

        update_enemies(enemies, max_enemies); // Update enemy positions, with attributes 

        //check for collisions, if there is a collision, display game over and stop the game
        for (int i = 0; i < max_enemies; i++) {
            if (enemies[i][2] > 0) { // Only check active enemies
                if (check_collision(circleX, circleY, radius, enemies[i][0], enemies[i][1], 30.0f)) {
                    game_over = true; // Collision detected, stop the game
                    break;
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if(game_over){
            DrawText("GAME OVER", screenWidth/2, screenHeight/2, 30, RED);
            play_again(&game_over); 
        }
        else{
            DrawText("Enemies fly down the screen. Reused after going off-screen!", 10, 10, 20, DARKGRAY);
            DrawCircle(circleX, circleY, radius, BLUE); // Draw player circle
            draw_enemies(enemies, max_enemies);        // Draw enemies
        }
        
        EndDrawing();
    }

    CloseWindow(); // Close window and OpenGL context
    return 0;
}

// Update player movement
void update_circle_position(float *x, float *y, float radius, float speed) {
    if (IsKeyDown(KEY_A)) *x -= speed;
    if (IsKeyDown(KEY_D)) *x += speed;
    if (IsKeyDown(KEY_W)) *y -= speed;
    if (IsKeyDown(KEY_S)) *y += speed;

    // Keep player circle within screen boundaries
    if (*x - radius < 0) *x = radius;
    if (*x + radius >= screenWidth) *x = screenWidth - radius;
    if (*y - radius < 0) *y = radius;
    if (*y + radius >= screenHeight) *y = screenHeight - radius;
}

// Initialize enemies
void initialize_enemies(int enemies[][4], int max_enemies) {
    for (int i = 0; i < max_enemies; i++) {
        enemies[i][0] = 0; // X position
        enemies[i][1] = 0; // Y position
        enemies[i][2] = 0; // Speed (0 means inactive)
        enemies[i][3] = 0; // Enemy trajectory (Down)
    }
}

// Interval update enemy procedure
// TODO: spawn enemies in from all corners of the screen
void interval_spawn_enemies(int *enemy, float *radius){
    int screen_spawn_direction = rand() & 3; // Produce number 0-3
    switch (screen_spawn_direction) {
        case 0: // Top of the screen 
            enemy[0] = *radius + (rand() % (int)(screenWidth-*radius)); // X position
            enemy[1] = -10; // Start slightly above the screen
            enemy[3] = 0; // Down
            break; 

        case 1: // Bottom of screen
            enemy[0] = *radius + (rand() % (int)(screenWidth-*radius)); // X position
            enemy[1] = screenHeight+10; // Start slightly below the screen
            enemy[3] = 1; // Up
            break; 
        case 2: // Left
            enemy[0] = -10; 
            enemy[1] = *radius + (rand() % (int)(screenHeight-*radius));
            enemy[3] = 2; // Right 
            break;
        case 3: 
            enemy[0] = screenWidth + 10; //Off right side of screen 
            enemy[1] = *radius + (rand() % (int)(screenHeight-*radius));
            enemy[3] = 3; // Left
            break; 
    }
    enemy[2] = 5 + (rand() % 11); // Random speed 
}

// Update enemy positions
void update_enemies(int enemies[][4], int max_enemies) {
    for (int i = 0; i < max_enemies; i++){
        if(enemies[i][2] > 0) {
            switch (enemies[i][3]) {
                case 0:
                    enemies[i][1] += enemies[i][2]; // Move enemy down the screen
                    if (enemies[i][1] > screenHeight) { // If off-screen, deactivate
                        enemies[i][2] = 0; // Mark as inactive
                    }
                    break;
                case 1: 
                    enemies[i][1] -= enemies[i][2]; // Up 
                    if (enemies[i][1] < 0) { // If off-screen, deactivate
                        enemies[i][2] = 0; // Mark as inactive
                    }                                                   
                    break; 
                case 2: 
                    enemies[i][0] += enemies[i][2]; // Right 
                    if (enemies[i][0] > screenWidth) { // If off-screen, deactivate
                        enemies[i][2] = 0; // Mark as inactive
                    }                               
                    break; 
                case 3: 
                    enemies[i][0] -= enemies[i][2]; // Left 
                    if (enemies[i][0] < 0) {
                        enemies[i][2] = 0; 
                    } 
                    break;
            }
        }
    }
}

// Draw enemies
void draw_enemies(int enemies[][4], int max_enemies) {
    for (int i = 0; i < max_enemies; i++) {
        if (enemies[i][2] > 0) { // Only draw active enemies
            DrawCircle(enemies[i][0], enemies[i][1], 30.0f, RED);
        }
    }
}

//Check for collisions 
bool check_collision(float playerX, float playerY, float playerRadius, int enemyX, int enemyY, float enemyRadius) {
    float dx = playerX - enemyX;
    float dy = playerY - enemyY;
    float distance = sqrtf(dx * dx + dy * dy);

    return distance < (playerRadius + enemyRadius); // Collision if distance is less than sum of radii
}

//Play again
void play_again(bool *game_over){
    if(IsKeyPressed(KEY_SPACE)){*game_over = 0;}
}
