#include <math.h>
#include <stdio.h>
#include "raylib.h"
#include "rcamera.h"
#include "raymath.h"

#include "bullet.c"
#include "weapon.c"
#include "enemy.c"

#define GRAVITY 9.81
#define MAX_COLUMNS 20
#define JUMP_VELOCITY 3.0f

void dead(bool *isDead, Camera3D *camera, int *cameraMode){
    if(IsKeyPressed(KEY_F)){
        *isDead = true;
    }
    if(*isDead==true){
        *cameraMode=CAMERA_ORBITAL;
        camera->position.z -= 1;
    }
}
void lean(Camera3D *camera) {
    static float leanAngle = 0.0f;
    const float maxLeanAngle = 0.15f;  // Maximum lean angle in radians
    const float leanSpeed = 0.00005f;      // Speed of leaning
    const float returnSpeed = 0.05f;   // Speed of returning to normal

    if (IsKeyDown(KEY_A)) {
        // Lean left, but don't exceed the maxLeanAngle
        if(leanAngle > 0.0f) leanAngle = 0.0f;
        if (leanAngle > -maxLeanAngle)
            leanAngle -= leanSpeed;
    } 
    if (IsKeyDown(KEY_D)) {
        if(leanAngle < 0.0f) leanAngle = 0.0f;
        if (leanAngle < maxLeanAngle) 
            leanAngle += leanSpeed;
    }
    else {
        // Gradually return to the neutral position
        if (leanAngle > 0.0f) 
            leanAngle -= returnSpeed * GetFrameTime();
        else if (leanAngle < 0.0f) 
            leanAngle += returnSpeed * GetFrameTime();

        // Clamp to zero if close enough
        if (fabs(leanAngle) < 0.001f)
            leanAngle = 0.0f;
    }

    printf("%f\n",leanAngle);
    CameraRoll(camera, leanAngle);
}


int main(void)
{
    
    int weaponWheel=1;
    InitAudioDevice();
    InitWindow(screenWidth, screenHeight, "Doom");
    InitBullet();
    DisableCursor();
    SetTargetFPS(60);
    
    Image image = LoadImage("map.png");
    Texture2D cubicmap = LoadTextureFromImage(image);
    Mesh mesh = GenMeshCubicmap(image, (Vector3){ 1.0f, 10.0f, 1.0f });
    Model model = LoadModelFromMesh(mesh);
    
    //Load and set texture. By default each cube is mapped to one part of texture atlas
    Texture2D texture = LoadTexture("cubicmap_atlas.png");
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    
    Sound punchSFX = LoadSound("SFX/punch.wav");
    Music walkSFX = LoadMusicStream("SFX/walk.mp3");
    Music ost = LoadMusicStream("SFX/ost.mp3");
    Sound pistolSFX = LoadSound("SFX/pistol.mp3");
    Sound shotgunSFX = LoadSound("SFX/shotgun.mp3");
    
    Texture2D crosshair = LoadTexture("Sprites/Crosshair.png");
    
    Texture2D imp = LoadTexture("Sprites/imp.png");
    
    Texture2D handAtlas = LoadTexture("Sprites/hand.png");
    Texture2D pistolAtlas = LoadTexture("Sprites/pistol.png");
    Texture2D shotgunAtlas = LoadTexture("Sprites/shotgun.png");
    float recoilOffsetX;
    float recoilOffsetY;
    
    Rectangle punchRec = { 0.0f, 0.0f, (float)handAtlas.width / 4, (float)handAtlas.height};
    Rectangle frameRec = { 0.0f, 0.0f, (float)pistolAtlas.width / 3, (float)pistolAtlas.height / 2};
    Rectangle shotgunFrameRec = { 0.0f, 0.0f, (float)shotgunAtlas.width / 3, (float)shotgunAtlas.height / 2};
    

    Camera camera = { 0 };
    camera.position = (Vector3){ 2.0f, -10.0f, 4.0f }; 
    camera.target = (Vector3){ 0.185f, 0.4f, 0.0f }; 
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };       
    camera.fovy = 100.0f;                             
    camera.projection = CAMERA_PERSPECTIVE;          
    Vector3 position = { 0.0f, 0.0f, 0.0f };   
    int cameraMode = CAMERA_FIRST_PERSON;
    bool isDead=false;    

    Vector3 oldCam; 
    
    float velocityY = 0.0f;
    Vector3 playerCube;

    // Variables for weapon animation
    float frameCounter = 0;
    int currentFrame = 0;
    bool isPunching = false;
    bool isShooting = false;
    
    Vector3 mapPosition = (Vector3){0.0f, 1.4f, 0.0f};
    
    Color *mapColor = LoadImageColors(image);
    
    while (!WindowShouldClose())
    {
        dead(&isDead, &camera, &cameraMode);
        weaponWheel = switchWeapon(weaponWheel, isPunching, isShooting);
        
        PlayMusicStream(ost);
        oldCam = camera.position;
        UpdateCamera(&camera, cameraMode);
        UpdateMusicStream(walkSFX);
        UpdateMusicStream(ost);
        playerCube = (Vector3){camera.position.x, camera.position.y+12, camera.position.z};
        lean(&camera);
        
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_D) || IsKeyDown(KEY_S)) && velocityY == 0.0f) {
        // Player is on the ground and moving
            if (!IsMusicStreamPlaying(walkSFX)) {
                PlayMusicStream(walkSFX);
            }
            walkSFX.looping = true;
        } else {
            // Player is either not moving or is in the air
            StopMusicStream(walkSFX);
        }

        
        Vector2 playerPos = { camera.position.x, camera.position.z };
        float playerRadius = 0.1f;
        int playerCellX = (int)(playerPos.x - mapPosition.x + 0.5f);
        int playerCellY = (int)(playerPos.y - mapPosition.z + 0.5f);
        
        if (playerCellX < 0) 
            playerCellX = 0;
        else if (playerCellX >= cubicmap.width) 
            playerCellX = cubicmap.width - 1;

        if (playerCellY < 0) 
            playerCellY = 0;
        else if (playerCellY >= cubicmap.height) 
            playerCellY = cubicmap.height - 1;

        // Check map collisions using image data and player position
        // TODO: Improvement: Just check player surrounding cells for collision
        for (int y = 0; y < cubicmap.height; y++)
        {
            for (int x = 0; x < cubicmap.width; x++)
            {
                
                if ((mapColor[y*cubicmap.width + x].r == 255) &&       // Collision: white pixel, only check R channel
                    (CheckCollisionCircleRec(playerPos, playerRadius, (Rectangle){ mapPosition.x - 0.5f + x*1.0f, mapPosition.z - 0.5f + y*1.0f, 1.0f, 1.0f })))
                {
                    // Collision detected, reset camera position
                    camera.position = oldCam;
                }
            }
        }
        
        if (camera.position.y > 2.0f)
        {
            velocityY -= GRAVITY * GetFrameTime();
        }
        else
        {
            velocityY = 0.0f;
            camera.position.y = 2.0f;
        }

        if (camera.position.y <= 2.0f && IsKeyDown(KEY_SPACE))
        {
            velocityY = JUMP_VELOCITY;
        }

        camera.position.y += velocityY * GetFrameTime();

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
            
           BeginMode3D(camera);
            
            DrawModel(model, mapPosition, 1.0f, WHITE);
            DrawCube(playerCube, 1.0f, 1.0f, 1.0f, RED);
            
           EndMode3D();
        
        DrawText(TextFormat("- Target: (%06.3f, %06.3f, %06.3f)", camera.target.x, camera.target.y, camera.target.z), 610, 75, 24, BLUE);
        
        if(isDead==false){
            DrawTextureEx(crosshair, (Vector2){screenWidth/2 - 36, screenHeight/2 - 36}, 0.0f, 0.5f, WHITE);
            if(weaponWheel==1){
                punch(handAtlas, &punchRec, &frameCounter, &currentFrame, &isPunching, punchSFX);
            }
            if(weaponWheel==2){
                pistol(pistolAtlas, &frameRec, &frameCounter, &currentFrame, &isShooting, pistolSFX);
            }
            if (weaponWheel == 3) {
                shotgun(shotgunAtlas, &shotgunFrameRec, &frameCounter, &currentFrame, &isShooting, shotgunSFX, &recoilOffsetX, &recoilOffsetY, camera);
            }
        }
            EndDrawing();
        
    }

    CloseWindow();
    return 0;
}