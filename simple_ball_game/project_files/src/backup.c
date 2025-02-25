#include "balls.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);  // Set game to run at 60 frames-per-second
    srand(time(NULL)); // Seed random number generator

    // Player circle position
    float circleX = SCREEN_WIDTH / 2.0f;  // Initial X position
    float circleY = SCREEN_HEIGHT / 2.0f; // Initial Y position
    float radius = 30.0f;               // Circle radius
    float speed = 8.0f;                 // Movement speed

    // Enemy variables
    const int max_enemies = 100;
    Enemy enemies[max_enemies]; 
    initialize_enemies(enemies, max_enemies); // Initialize enemy data

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
                if (enemies[i].velocity.x == 0 && enemies[i].velocity.y == 0) { // Every interval, find an enemy who has gone off screen 
                    interval_spawn_enemies(&enemies[i], &radius); // see TODO on in function definition
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
            DrawText("GAME OVER", SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 30, RED);
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
    if (*x + radius >= SCREEN_WIDTH) *x = SCREEN_WIDTH - radius;
    if (*y - radius < 0) *y = radius;
    if (*y + radius >= SCREEN_HEIGHT) *y = SCREEN_HEIGHT - radius;
}

// Initialize enemies
void initialize_enemies(Enemy *enemies, int max_enemies) {
    for (int i = 0; i < max_enemies; i++) {
        enemies[i].position.x = 0; 
        enemies[i].position.y = 0; 
        enemies[i].velocity.x = 0; 
        enemies[i].velocity.y = 0; 
        enemies[i].health = 100;
    }
}

// Interval update enemy procedure
// TODO: spawn enemies in from all corners of the screen
void interval_spawn_enemies(Enemy *enemy, float *radius){
    int screen_spawn_direction = rand() & 3; // Produce number 0-3
    switch (screen_spawn_direction) {
        case 0: // Top of the screen
            enemy->position.x = *radius + (rand() % (int)(SCREEN_WIDTH-*radius)); // X position
            enemy->position.y = -10; // Start slightly above the screen
            break;

        case 1: // Bottom of screen
            enemy->position.x = *radius + (rand() % (int)(SCREEN_WIDTH-*radius)); // X position
            enemy->position.y = SCREEN_HEIGHT+10; // Start slightly below the screen
            break; 
        case 2: // Left
            enemy->position.x = -10; 
            enemy->position.y = *radius + (rand() % (int)(SCREEN_HEIGHT-*radius));
            break;
        case 3: 
            enemy->position.x = SCREEN_WIDTH + 10; //Off right side of screen 
            enemy->position.y = *radius + (rand() % (int)(SCREEN_HEIGHT-*radius));
            break; 
    }
}

// Update enemy positions
void update_enemies(Enemy *enemies, int max_enemies) {
    for (int i = 0; i < max_enemies; i++){
        if(enemies[i].velocity.x != 0 || enemies[i].velocity.y != 0) {
            //TODO: Move towards the player position
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
