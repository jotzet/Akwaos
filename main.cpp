#include "vclib/include/SDL.h"
#include <stdio.h>
#include <map>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 1000;

// max num of fish that can be inside the akwaos
const int MAX_FISH = 100;

// here define how many fish of different species to spawn
const std::vector<std::pair<int, std::string>> spawn_requests = {
    {4, "basicFish"},
    {4, "goldFish"},
    {3, "crazyFish"},
    {3, "dumbFish"},
    {2, "aggressiveFish"},
    {3, "disgustingFish"},
    {3, "canibalFish"},
    {1, "zombieFish"},
    {1, "freezingFish"},
    {1, "healingFish"},
    {3, "goldfishEater"}};

// void spawnObjects(int num, std::vector<class swimmingObject> &swimmingObjects);
void spawn(int num, std::string name, int posX, int posY);
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
std::vector<class swimmingObject> spawnGroup;

int totalFish;

struct Texture
{
    SDL_Texture *texture;
    std::string path;
};

std::map<std::string, Texture> textures = {
    {"water", {nullptr, "textures\\water.bmp"}},
    {"swimmingObject", {nullptr, "textures\\swimmingObject.bmp"}},
    {"basicFish", {nullptr, "textures\\basicFish.bmp"}},
    {"goldFish", {nullptr, "textures\\goldFish.bmp"}},
    {"crazyFish", {nullptr, "textures\\crazyFish.bmp"}},
    {"dumbFish", {nullptr, "textures\\dumbFish.bmp"}},
    {"aggressiveFish", {nullptr, "textures\\aggressiveFish.bmp"}},
    {"canibalFish", {nullptr, "textures\\canibalFish.bmp"}},
    {"goldfishEater", {nullptr, "textures\\goldfishEater.bmp"}},
    {"zombieFish", {nullptr, "textures\\zombieFish.bmp"}},
    {"freezingFish", {nullptr, "textures\\freezingFish.bmp"}},
    {"healingFish", {nullptr, "textures\\healingFish.bmp"}},
    {"disgustingFish", {nullptr, "textures\\disgustingFish.bmp"}}};

class swimmingObject
{
public:
    std::string name;
    int spawnCooldown;
    int posX, posY;
    int velocityX, velocityY;
    int width, height;
    int speed;
    int randomnessX;
    int randomnessY;
    bool isAggressive;
    bool isEdible;
    bool isCanibal;
    bool isTurningZombie;
    bool isFreezing;
    bool isZombie;
    bool isFrozen;
    bool isHealing;
    double reproductionRate;
    SDL_Texture *texture;

    swimmingObject(int iniX, int iniY)
        : name("SwimmingObject"),
          spawnCooldown(0),
          posX(0),
          posY(0),
          velocityX((std::rand() % 2) * 2 - 1),
          velocityY((std::rand() % 2) * 2 - 1),
          width(34),
          height(14),
          speed(1),
          randomnessX(1),
          randomnessY(1),
          isAggressive(false),
          isEdible(true),
          isCanibal(false),
          isTurningZombie(false),
          isFreezing(false),
          isZombie(false),
          isFrozen(false),
          isHealing(false),
          reproductionRate(10),
          texture(nullptr)
    {
        if (iniX == 0 && iniY == 0)
        {
            posX = std::rand() % (SCREEN_WIDTH - width);
            posY = std::rand() % (SCREEN_HEIGHT - height);
        }
        else
        {
            posX = iniX;
            posY = iniY;
        }
    }

    int getDistance(const swimmingObject &other) const
    {
        double centerX_A = posX + width / 2.0;
        double centerY_A = posY + height / 2.0;
        double centerX_B = other.posX + other.width / 2.0;
        double centerY_B = other.posY + other.height / 2.0;
        double distance = std::sqrt(std::pow(centerX_A - centerX_B, 2) + std::pow(centerY_A - centerY_B, 2));
        return distance;
    }

    bool isNotSpecial(const swimmingObject &other) const
    {
        return !isHealing && !isTurningZombie && !isFreezing;
    }

    bool canEat(const swimmingObject &other) const
    {

        if (isZombie)
        {
            return getDistance(other) < 10 && isNotSpecial(other) && (height * width) > ((other.height * other.width) - 100);
        }
        else
        {
            return !isFrozen && isNotSpecial(other) && isAggressive && ((isCanibal && other.isCanibal && (std::rand() % 100) < 50) || (height * width) > (other.height * other.width)) && other.isEdible && getDistance(other) < 10;
        }
    }

    bool canReproduce(const swimmingObject &other) const
    {
        return (!isZombie && !isFrozen && spawnCooldown == 0 && name == other.name && getDistance(other) < 10 && (std::rand() % 100) < reproductionRate);
    }

    bool canHeal(const swimmingObject &other) const
    {
        return isHealing && (other.isZombie || other.isFrozen) && getDistance(other) < 15;
    }
    bool canFreeze(const swimmingObject &other) const
    {
        return isFreezing && isNotSpecial(other) && getDistance(other) < 15;
    }

    bool canTurnZombie(const swimmingObject &other) const
    {
        return isTurningZombie && isNotSpecial(other) && getDistance(other) < 15;
    }

    void eat(swimmingObject &other)
    {
        auto it = spawnGroup.begin();
        while (it != spawnGroup.end())
        {
            if (&(*it) == &other)
            {
                it = spawnGroup.erase(it);
                totalFish--;
                break;
            }
            else
            {
                ++it;
            }
        }
    }

    void updatePosition()
    {
        if (spawnCooldown > 0)
        {
            spawnCooldown--;
        }

        if (!isFrozen)
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

        for (size_t i = 0; i < spawnGroup.size(); ++i)
        {
            if (&spawnGroup[i] != this)
            {
                if (canReproduce(spawnGroup[i]))
                {

                    int spawnX = posX - 2 * width;
                    int spawnY = posY - 2 * height;

                    if (spawnX < 0)
                    {
                        spawnX = 0;
                    }
                    else if (spawnX + width > SCREEN_WIDTH)
                    {
                        spawnX = SCREEN_WIDTH - width;
                    }

                    if (spawnY < 0)
                    {
                        spawnY = 0;
                    }
                    else if (spawnY + height > SCREEN_HEIGHT)
                    {
                        spawnY = SCREEN_HEIGHT - height;
                    }

                    spawn(1, name, spawnX, spawnY);
                    spawnCooldown = 1000;
                }

                else if (canEat(spawnGroup[i]))
                {

                    eat(spawnGroup[i]);
                }
                else if (canFreeze(spawnGroup[i]))
                {
                    spawnGroup[i].isFrozen = true;
                }
                else if (canTurnZombie(spawnGroup[i]))
                {
                    spawnGroup[i].isZombie = true;
                }
                else if (canHeal(spawnGroup[i]))
                {
                    spawnGroup[i].isZombie = false;
                    spawnGroup[i].isFrozen = false;
                }
            }
        }
    }

    void render() const
    {
        SDL_Rect objectRect = {posX, posY, width, height};
        SDL_RendererFlip flip = (velocityX > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        if (isZombie && !isFrozen)
        {
            SDL_SetTextureColorMod(texture, 0, 255, 0);
        }
        else if (isFrozen)
        {
            SDL_SetTextureColorMod(texture, 102, 255, 255);
        }
        else
        {
            SDL_SetTextureColorMod(texture, 255, 255, 255);
        }

        SDL_RenderCopyEx(gRenderer, texture, NULL, &objectRect, 0, NULL, flip);
    }
};

class basicFish : public swimmingObject
{
public:
    basicFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        texture = textures["basicFish"].texture;
        name = "basicFish";
    }
};

class freezingFish : public swimmingObject
{
public:
    freezingFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        reproductionRate = 0;
        randomnessX = 5;
        randomnessY = 5;
        width = 54;
        height = 23;
        texture = textures["freezingFish"].texture;
        speed = 1;
        name = "freezingFish";
        isEdible = false;
        isFreezing = true;
    }
};

class canibalFish : public swimmingObject
{
public:
    canibalFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        texture = textures["canibalFish"].texture;
        name = "canibalFish";
        width = 54;
        height = 23;
        reproductionRate = 50;
        isAggressive = true;
        speed = 2;
        isCanibal = true;
    }
};
class healingFish : public swimmingObject
{
public:
    healingFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        texture = textures["healingFish"].texture;
        name = "healingFish";
        width = 91;
        height = 31;
        reproductionRate = 0;
        speed = 2;
        isEdible = false;
        isHealing = true;
    }
};
class goldFish : public swimmingObject
{
public:
    goldFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
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
    aggressiveFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        reproductionRate = 1;
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
    crazyFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
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
    goldfishEater(int iniX, int iniY) : swimmingObject(iniX, iniY)
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
    dumbFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
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
    disgustingFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        width = 54;
        height = 23;
        texture = textures["disgustingFish"].texture;
        name = "disgustingFish";
        isEdible = false;
    }
};

class zombieFish : public swimmingObject
{
public:
    zombieFish(int iniX, int iniY) : swimmingObject(iniX, iniY)
    {
        width = 54;
        height = 23;
        reproductionRate = 0;
        texture = textures["zombieFish"].texture;
        name = "disgustingFish";
        isEdible = false;
        isTurningZombie = true;
        speed = 1;
    }
};

void spawn(int num, std::string name, int posX, int posY)
{

    if (totalFish < MAX_FISH)
    {
        for (int i = 0; i < num; ++i)
        {
            if (name == "basicFish")
            {
                spawnGroup.push_back(basicFish(posX, posY));
            }
            else if (name == "goldFish")
            {
                spawnGroup.push_back(goldFish(posX, posY));
            }
            else if (name == "crazyFish")
            {
                spawnGroup.push_back(crazyFish(posX, posY));
            }
            else if (name == "freezingFish")
            {
                spawnGroup.push_back(freezingFish(posX, posY));
            }
            else if (name == "zombieFish")
            {
                spawnGroup.push_back(zombieFish(posX, posY));
            }
            else if (name == "healingFish")
            {
                spawnGroup.push_back(healingFish(posX, posY));
            }
            else if (name == "dumbFish")
            {
                spawnGroup.push_back(dumbFish(posX, posY));
            }
            else if (name == "aggressiveFish")
            {
                spawnGroup.push_back(aggressiveFish(posX, posY));
            }
            else if (name == "canibalFish")
            {
                spawnGroup.push_back(canibalFish(posX, posY));
            }
            else if (name == "disgustingFish")
            {
                spawnGroup.push_back(disgustingFish(posX, posY));
            }
            else if (name == "goldfishEater")
            {
                spawnGroup.push_back(goldfishEater(posX, posY));
            }
            else
            {
                spawnGroup.push_back(swimmingObject(posX, posY));
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

void spawnAll()
{
    for (const auto &request : spawn_requests)
    {
        spawn(request.first, request.second, 0, 0);
    }
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

            spawnAll();
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
                    else if (e.type == SDL_KEYDOWN)
                    {
                        if (e.key.keysym.sym == SDLK_r)
                        {
                            spawnGroup.clear();
                            totalFish = 0;
                            spawnAll();
                        }
                    }
                }

                for (auto &obj : spawnGroup)
                {
                    obj.updatePosition();
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