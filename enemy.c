#include "enemy.h"
#include <stdlib.h>

enemy imps[MAX_ENEMY];

void InitEnemies(enemy imps[], Texture2D impTexture, Camera *camera) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        imps[i].Maxhp = 35;
        imps[i].alive = true;
        imps[i].position = (Vector3){ GetRandomValue(-15, 15), 1.5f, GetRandomValue(-15, 15) };
    }
}

void DrawEnemies(Texture2D imp, Camera3D camera) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (imps[i].alive) {
            // Draw the alive enemy as a billboard (2D sprite in 3D world)
            
            DrawBillboard((Camera3D)camera, imp, imps[i].position, 2.0f, WHITE);
        }
    }
}
