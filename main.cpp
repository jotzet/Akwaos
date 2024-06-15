#include "vclib/include/SDL.h"
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;

// max num of fish that can be inside the akwaos
const int MAX_FISH = 100;
// num of fish that ARE inside the akwaos
int totalFish;

const std::string PATH = "textures\\";

bool init();
bool loadMedia();
void close();
void spawnObjects(int num, std::vector<class swimmingObject> &swimmingObjects);
bool checkCollision(const swimmingObject &a, const swimmingObject &b);
void reproduce(const swimmingObject &parent, std::vector<swimmingObject> &newFish);
void spawn(int num, std::vector<swimmingObject> &spawnGroup, std::string name);
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

struct Texture
{
    SDL_Texture *texture;
    std::string path;
};

std::map<std::string, Texture> textures = {
    {"water", {nullptr, PATH + "water.bmp"}},
    {"swimmingObject", {nullptr, PATH + "swimmingObject.bmp"}},
    {"basicFish", {nullptr, PATH + "basicFish.bmp"}},
    {"goldFish", {nullptr, PATH + "goldFish.bmp"}},
    {"crazyFish", {nullptr, PATH + "crazyFish.bmp"}},
    {"dumbFish", {nullptr, PATH + "dumbFish.bmp"}},
    {"aggressiveFish", {nullptr, PATH + "aggressiveFish.bmp"}},
    {"goldfishEater", {nullptr, PATH + "goldfishEater.bmp"}},
    {"disgustingFish", {nullptr, PATH + "disgustingFish.bmp"}}};

class swimmingObject
{
public:
    std::string name;
    int posX, posY;
    int velocityX, velocityY;
    int width, height;
    int speed;
    int randomnessX;
    int randomnessY;
    bool isAggressive;
    bool isEdible;
    double reproductionRate;
    SDL_Texture *texture;

    swimmingObject()
        : velocityX((std::rand() % 2) * 2 - 1),
          velocityY((std::rand() % 2) * 2 - 1),
          width(34),
          height(14),
          speed(1),
          randomnessX(1),
          randomnessY(1),
          reproductionRate(10),
          isEdible(true),
          isAggressive(false)

    {
        posX = std::rand() % (SCREEN_WIDTH - width);
        posY = std::rand() % (SCREEN_HEIGHT - height);
    }

    bool isTouching(const swimmingObject &other) const
    {
        double centerX_A = posX + width / 2.0;
        double centerY_A = posY + height / 2.0;
        double centerX_B = other.posX + other.width / 2.0;
        double centerY_B = other.posY + other.height / 2.0;
        double distance = std::sqrt(std::pow(centerX_A - centerX_B, 2) + std::pow(centerY_A - centerY_B, 2));
        return distance <= 10;
    }

    bool canEat(const swimmingObject &other) const
    {
        // aggressive fish eat fish that are smaller and edible
        return isAggressive && (height * width) > (other.height * other.width) && other.isEdible;
    }

    bool canReproduce(const swimmingObject &other) const
    {
        return (name == other.name && (std::rand() % 100) < reproductionRate);
    }

    void updatePosition(std::vector<swimmingObject> &swimmingObjects)
    {
        posX += velocityX * speed;
        posY += velocityY * speed;

        // Random behaviour
        if ((std::rand() % 100) < randomnessX)
        {
            velocityX = -velocityX;
        }
        if ((std::rand() % 100) < randomnessY)
        {
            velocityY = -velocityY;
        }

        // Collision w/ screen edges
        if (posX < 0)
        {
            velocityX = std::abs(velocityX);
            posX = 0;
        }
        if (posY < 0)
        {
            velocityY = std::abs(velocityY);
            posY = 0;
        }
        if (posX + width > SCREEN_WIDTH)
        {
            velocityX = -std::abs(velocityX);
            posX = SCREEN_WIDTH - width;
        }
        if (posY + height > SCREEN_HEIGHT)
        {
            velocityY = -std::abs(velocityY);
            posY = SCREEN_HEIGHT - height;
        }

        for (size_t i = 0; i < swimmingObjects.size(); ++i)
        {
            if (&swimmingObjects[i] != this && isTouching(swimmingObjects[i]))
            {
                if (canReproduce(swimmingObjects[i]))
                {
                    spawn(1, swimmingObjects, name);
                }

                else if (canEat(swimmingObjects[i]))
                {
                    swimmingObjects.erase(swimmingObjects.begin() + i);
                    totalFish--;
                }
            }
        }
    }

    void render() const
    {
        SDL_Rect objectRect = {posX, posY, width, height};
        SDL_RendererFlip flip = (velocityX > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(gRenderer, texture, NULL, &objectRect, 0, NULL, flip);
    }
};

class basicFish : public swimmingObject
{
public:
    basicFish() : swimmingObject()
    {
        texture = textures["basicFish"].texture;
        name = "basicFish";
    }
};

class goldFish : public swimmingObject
{
public:
    goldFish() : swimmingObject()
    {
        reproductionRate = 50;
        width = 16;
        height = 12;
        texture = textures["goldFish"].texture;
        speed = 2;
        name = "goldFish";
    }
};

class aggressiveFish : public swimmingObject
{
public:
    aggressiveFish() : swimmingObject()
    {
        reproductionRate = 5;
        randomnessX = 1;
        randomnessY = 1;
        width = 200;
        height = 77;
        texture = textures["aggressiveFish"].texture;
        speed = 1;
        name = "aggressiveFish";
        isAggressive = true;
    }
};

class crazyFish : public swimmingObject
{
public:
    crazyFish() : swimmingObject()
    {
        reproductionRate = 20;
        randomnessX = 5;
        randomnessY = 10;
        width = 54;
        height = 23;
        texture = textures["crazyFish"].texture;
        speed = 3;
        name = "crazyFish";
    }
};

class goldfishEater : public swimmingObject
{
public:
    goldfishEater() : swimmingObject()
    {
        reproductionRate = 5;
        randomnessX = 2;
        randomnessY = 2;
        width = 33;
        height = 12;
        texture = textures["goldfishEater"].texture;
        speed = 4;
        name = "goldfishEater";
        isAggressive = true;
    }
};

class dumbFish : public swimmingObject
{
public:
    dumbFish() : swimmingObject()
    {
        reproductionRate = 50;
        randomnessX = 2;
        randomnessY = 2;
        width = 54;
        height = 23;
        texture = textures["dumbFish"].texture;
        speed = 1;
        name = "dumbFish";
    }
};

class disgustingFish : public swimmingObject
{
public:
    disgustingFish() : swimmingObject()
    {
        width = 54;
        height = 23;
        texture = textures["disgustingFish"].texture;
        name = "disgustingFish";
        isEdible = false;
    }
};

void spawn(int num, std::vector<swimmingObject> &spawnGroup, std::string name)
{
    if (totalFish < MAX_FISH)
    {
        for (int i = 0; i < num; ++i)
        {
            if (name == "basicFish")
            {
                spawnGroup.push_back(basicFish());
            }
            else if (name == "goldFish")
            {
                spawnGroup.push_back(goldFish());
            }
            else if (name == "crazyFish")
            {
                spawnGroup.push_back(crazyFish());
            }
            else if (name == "dumbFish")
            {
                spawnGroup.push_back(dumbFish());
            }
            else if (name == "aggressiveFish")
            {
                spawnGroup.push_back(aggressiveFish());
            }
            else if (name == "disgustingFish")
            {
                spawnGroup.push_back(disgustingFish());
            }
            else if (name == "goldfishEater")
            {
                spawnGroup.push_back(goldfishEater());
            }
            else
            {
                spawnGroup.push_back(swimmingObject());
            }
            totalFish++;
        }
    }
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("AKWAOS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                std::srand(static_cast<unsigned int>(std::time(0)));
            }
        }
    }

    return success;
}

bool loadSpecificMedia(const char *path, SDL_Texture **specificTexture)
{
    bool success = true;
    SDL_Surface *loadedSurface = SDL_LoadBMP(path);
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL Error: %s\n", path, SDL_GetError());
        success = false;
    }
    else
    {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
        *specificTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (*specificTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            success = false;
        }
    }
    return success;
}

bool loadMedia()
{
    bool success = true;

    for (auto &pair : textures)
    {
        const char *path = pair.second.path.c_str();
        if (!loadSpecificMedia(path, &pair.second.texture))
        {
            success = false;
        }
    }
    return success;
}

void close()
{
    for (auto &pair : textures)
    {
        SDL_DestroyTexture(pair.second.texture);
        pair.second.texture = NULL;
    }
    textures.clear();

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    SDL_Quit();
}

int main(int argc, char *args[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            std::vector<swimmingObject> spawnGroup;
            spawn(4, spawnGroup, "basicFish");
            spawn(6, spawnGroup, "goldFish");
            spawn(2, spawnGroup, "crazyFish");
            spawn(2, spawnGroup, "dumbFish");
            spawn(2, spawnGroup, "aggressiveFish");
            spawn(2, spawnGroup, "disgustingFish");
            spawn(3, spawnGroup, "goldfishEater");

            bool quit = false;
            SDL_Event e;

            while (!quit)
            {
                while (SDL_PollEvent(&e) != 0)
                {
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                }

                for (auto &obj : spawnGroup)
                {
                    obj.updatePosition(spawnGroup);
                }

                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, textures["water"].texture, NULL, NULL);

                for (const auto &fish : spawnGroup)
                {
                    fish.render();
                }

                SDL_RenderPresent(gRenderer);

                SDL_Delay(10);
            }
        }
    }

    close();

    return 0;
}