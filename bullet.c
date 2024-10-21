#include "bullet.h"
#define SHOTGUN_AMMO 5
#define PISTOL_AMMO 10


// Define variables here
const int screenWidth = 1024;
const int screenHeight = 512;


int aSB = SHOTGUN_AMMO;
int aPB = PISTOL_AMMO;

Bullet ShotgunBull[SHOTGUN_AMMO];
Bullet PistolBull[PISTOL_AMMO];

void InitBullet(){
    for(int i = 1; i<=SHOTGUN_AMMO; i++){
        ShotgunBull[i].active = true;
    }
    for(int i = 1; i<=PISTOL_AMMO; i++){
        PistolBull[i].active = true;
    }
}

void shotgunReload()
{
    for(int i = 1; i<=SHOTGUN_AMMO; i++){
        ShotgunBull[i].active = true;
        aSB = 5;
    }
}

void pistolReload()
{
    for(int i = 1; i<=PISTOL_AMMO; i++){
        PistolBull[i].active = true;
    }
    aPB = 10;
}

void shotgunShoot(){
    //Collision Logic Here
    ShotgunBull[aSB].active = false;
    aSB--;
}

void pistolShoot(){
    //Collision Logic Here
    PistolBull[aPB].active = false;
    aPB--;
}