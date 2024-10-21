#include "weapon.h"
#include "raylib.h"
#include "bullet.h"

void pistol(Texture2D pistolAtlas, Rectangle *frameRec, float *frameCounter, int *currentFrame, bool *isShooting, Sound pistolSFX)
{   
    DrawText(TextFormat("%d/10", aPB), 400, screenHeight/2, 18, RED);
    if(IsKeyPressed(KEY_R))
    {
        pistolReload();
    }
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !(*isShooting) && aPB > 0)
    {
        PlaySound(pistolSFX);
        *isShooting = true; //Raise animation flag
        pistolShoot();        
    }

    if (*isShooting)
    {
        if (*frameCounter < 1.5f)
        {
            *currentFrame = 1; // Set to frame 2
        }
        else if (*frameCounter < 3.0f)
        {
            *currentFrame = 2; // Set to frame 3
        }
        else
        {
            *frameCounter = 0;   // Reset after animation
            *currentFrame = 0;   // Go back to idle
            *isShooting = false; // Stop the animation
        }

        *frameCounter += 0.3f; // Control speed
    }

    
    frameRec->x = *currentFrame * frameRec->width;

    // Define the source rectangle using the updated frameRec
    Rectangle sourceRec = { frameRec->x, frameRec->y, frameRec->width, frameRec->height };

    // Set the on-screen position for the pistol (e.g., center of the screen)
    float scaleFactor = 2.0f; // Change this value to control the size
    Vector2 position = { screenWidth / 2 + (frameRec->width * scaleFactor) / 2, screenHeight - frameRec->height * scaleFactor };

    // Destination rectangle for drawing the scaled pistol
    Rectangle destRec = { position.x, position.y, frameRec->width * scaleFactor, frameRec->height * scaleFactor };
    
    bool isMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);

    // Apply sway only if the player is moving
    if (isMoving)
    {
        float swayAmount = 5.0f; // Adjusted sway amount to make it more noticeable
        float swaySpeed = 5.0f; 
        float swayX = sin(GetTime() * swaySpeed) * swayAmount;
        float swayY = cos(GetTime() * swaySpeed) * swayAmount;

        // Use swayX and swayY to offset the weapon's position
        destRec.x += swayX;
        destRec.y += swayY;
    }
    
    // Center of rotation (you can leave this at 0,0 if you're not rotating the texture)
    Vector2 origin = { 0, 0 };

    // Draw the scaled pistol
    DrawTexturePro(pistolAtlas, sourceRec, destRec, origin, 0.0f, WHITE);
}

void shotgun(Texture2D shotgunAtlas, Rectangle *frameRec, float *frameCounter, int *currentFrame, bool *isShooting, Sound shotgunSFX, float *recoilOffsetX, float *recoilOffsetY, Camera3D camera)
{   
    DrawText(TextFormat("%d/5", aSB), 400, screenHeight/2, 18, RED);
    if(IsKeyPressed(KEY_R))
    {
        shotgunReload();
    }
    
    // Play sound and start animation when left mouse button is pressed
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !(*isShooting) && aSB > 0)
    {
        PlaySound(shotgunSFX);
        *isShooting = true;  //Raise animation flag
        *recoilOffsetX = 20.0f; // Add an initial recoil effect on X-axis (rightward)
        *recoilOffsetY = 15.0f; // Add an initial recoil effect on Y-axis (upward)
        shotgunShoot();
    }

    // Control animation frames
    if (*isShooting)
    {
        if (*frameCounter < 1.5f)
        {
            *currentFrame = 1; // Set to frame 2
        }
        else if (*frameCounter < 3.0f)
        {
            *currentFrame = 2; // Set to frame 3
        }
        else
        {
            *frameCounter = 0;   // Reset after animation
            *currentFrame = 0;   // Go back to idle
            *isShooting = false; // Stop the animation
        }

        *frameCounter += 0.1f; // Control speed
    }

    // Slowly decrease recoil offset over time (simulate return to normal)
    *recoilOffsetX *= 0.9f;
    *recoilOffsetY *= 0.9f;

    frameRec->x = *currentFrame * frameRec->width;

    // Define Source Rectangle
    Rectangle sourceRec = { frameRec->x, frameRec->y, frameRec->width, frameRec->height };

    // Set position and Scale of shotgun
    float scaleFactor = 0.7f; // Change this value to control the size
    Vector2 position = { screenWidth / 2 + 100, screenHeight / 2 };
    
    bool isMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);
    
    if (isMoving)
    {
        float swayAmount = 3.0f; 
        float swaySpeed = 2.0f; 
        float swayX = sin(GetTime() * swaySpeed) * swayAmount;
        float swayY = cos(GetTime() * swaySpeed) * swayAmount;
        
        position.x += swayX;
        position.y += swayY;
    }

    // Recoil
    position.x -= *recoilOffsetX; 
    position.y -= *recoilOffsetY; 

    // Destination rectangle (koi bohaite chas)
    Rectangle destRec = { position.x, position.y, frameRec->width * scaleFactor, frameRec->height * scaleFactor };
    // Center of rotation
    Vector2 origin = { 0, 0 };
    
    DrawTexturePro(shotgunAtlas, sourceRec, destRec, origin, 0.0f, WHITE);
}




int switchWeapon(int weaponWheel, bool isPunching, bool isShooting)
{
    if (IsKeyPressed(KEY_ONE) && !isPunching && !isShooting)
    {
        return 1;  // Set to weapon 1
    }
    else if (IsKeyPressed(KEY_TWO) && !isPunching && !isShooting)
    {
        return 2;  // Set to weapon 2
    }
    else if (IsKeyPressed(KEY_THREE) && !isPunching && !isShooting)
    {
        return 3;  // Set to weapon 3 (shotgun in this case)
    }
    else if (IsKeyPressed(KEY_FOUR) && !isPunching && !isShooting)
    {
        return 4;  // Set to weapon 4
    }

    return weaponWheel;  // No change, return current weapon
}

void punch(Texture2D handAtlas, Rectangle *punchRec, float *frameCounter, int *currentFrame, bool *isPunching, Sound punchSFX)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !(*isPunching))
    {
        PlaySound(punchSFX);
        *isPunching = true;  // Start the animation
    }

    if (*isPunching)
    {
        if (*frameCounter < 1.5f)
        {
            *currentFrame = 1; // Set to frame 2
        }
        else if (*frameCounter < 3.0f)
        {
            *currentFrame = 2; // Set to frame 3
        }
        else
        {
            *frameCounter = 0;   // Reset after animation
            *currentFrame = 0;   // Go back to idle
            *isPunching = false; // Stop the animation
        }

        *frameCounter += 0.1f; // Control speed
    }

    punchRec->x = *currentFrame * punchRec->width;

    // Define the source rectangle using the updated punchRec
    Rectangle sourceRec = { punchRec->x, punchRec->y, punchRec->width, punchRec->height };

    // Set the position based on the current frame
    float scaleFactor = 3.5f;  // Scale factor for the punch texture
    Vector2 position;

    if (*currentFrame == 0)  // Idle position (right side)
    {
        position = (Vector2){ screenWidth/2 + 100, screenHeight - punchRec->height * scaleFactor };
    }
    else  // Other frames (left side)
    {
        position = (Vector2){ screenWidth/2 - 600, screenHeight - punchRec->height * scaleFactor };
    }

    // Destination rectangle for drawing the scaled punch texture
    Rectangle destRec = { position.x, position.y, punchRec->width * scaleFactor, punchRec->height * scaleFactor };
    
    bool isMoving = IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D);

    // Apply sway only if the player is moving
    if (isMoving)
    {
        float swayAmount = 5.0f; // Adjusted sway amount to make it more noticeable
        float swaySpeed = 5.0f; 
        float swayX = sin(GetTime() * swaySpeed) * swayAmount;
        float swayY = cos(GetTime() * swaySpeed) * swayAmount;

        // Use swayX and swayY to offset the weapon's position
        destRec.x += swayX;
        destRec.y += swayY;
    }
    
    // Draw the punch animation
    DrawTexturePro(handAtlas, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
}