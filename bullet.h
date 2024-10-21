#ifndef BULLET_H_ // Include guard to prevent redefinition
#define BULLET_H_

#include "raylib.h"



// Use extern to declare these variables
extern const int screenWidth;
extern const int screenHeight;
extern int aSB;
extern int aPB;

typedef struct Bullet {
    Vector3 position;
    Vector3 direction;
    float speed;
    bool active;
} Bullet;


// Function prototypes
void InitBullet();
void shotgunReload();
void pistolReload();
void shotgunShoot();
void pistolShoot();

#endif // BULLET_H_
