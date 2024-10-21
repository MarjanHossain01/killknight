#ifndef ENEMY_H_
#define ENEMY_H_

#include "raylib.h"

typedef struct enemy {
    Vector3 position;
    int Maxhp;
    bool alive;
} enemy;

#define MAX_ENEMY 15

void InitEnemies(enemy imps[], Texture2D impTexture, Camera *camera);
void DrawEnemies(Texture2D imp, Camera3D camera);

#endif
