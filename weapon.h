#define weapon_h_
#include "raylib.h"

void pistol(Texture2D pistolAtlas, Rectangle *frameRec, float *frameCounter, int *currentFrame, bool *isShooting, Sound pistolSFX);
void shotgun(Texture2D shotgunAtlas, Rectangle *frameRec, float *frameCounter, int *currentFrame, bool *isShooting, Sound shotgunSFX, float *recoilOffsetX, float *recoilOffsetY, Camera3D camera);
int switchWeapon(int weaponWheel, bool isPunching, bool isShooting);
void punch(Texture2D handAtlas, Rectangle *punchRec, float *frameCounter, int *currentFrame, bool *isPunching, Sound punchSFX);