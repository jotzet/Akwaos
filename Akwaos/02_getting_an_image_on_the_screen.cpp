#include <SDL.h>
#include <stdio.h>
#include <vector>
#include <ctime>
#include <cstdlib>

const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;

bool init();
bool loadMedia();
void close();
void spawnBasicFish(int numOfFish, std::vector<class basicFish>& basicFishGroup);

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
SDL_Texture* gWater = NULL;
SDL_Texture* gBasicFish = NULL;

bool checkCollision(const basicFish& a, const basicFish& b);

class basicFish {
public:
    int posX, posY;
    int velocityX, velocityY;
    int width = 34, height = 14;
    int speed = 1;

    basicFish()
        : posX(std::rand() % (SCREEN_WIDTH - width)),
        posY(std::rand() % (SCREEN_HEIGHT - height)),
        velocityX((std::rand() % 2 == 0) ? speed : -speed),
        velocityY((std::rand() % 2 == 0) ? speed : -speed) {}

    basicFish(int x, int y, int velX, int velY) : posX(x), posY(y), velocityX(velX), velocityY(velY) {}

    void updatePosition(std::vector<basicFish>& basicFishGroup) {
        posX += velocityX * speed;
        posY += velocityY * speed;

        //Random behaviour
        if (std::rand() % 100 == 0) {
            velocityX = -velocityX;
        }
        if (std::rand() % 200 == 0) {
            velocityY = -velocityY;
        }

        //Colision w/ screen edges
        if ((posX < 0) || (posX + width > SCREEN_WIDTH)) {
            velocityX = -velocityX;
        }
        if ((posY < 0) || (posY + height > SCREEN_HEIGHT)) {
            velocityY = -velocityY;
        }

        //Colision w/ other fish
        for (size_t i = 0; i < basicFishGroup.size(); ++i) {
            if (&basicFishGroup[i] != this && checkCollision(*this, basicFishGroup[i])) {
                velocityX = -velocityX;
                velocityY = -velocityY;
            }
        }
    }

    void render() const {
        SDL_Rect bsicFishRect = { posX, posY, width, height };
        SDL_RendererFlip flip = (velocityX > 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        SDL_RenderCopyEx(gRenderer, gBasicFish, NULL, &bsicFishRect, 0, NULL, flip);
    }
};

bool checkCollision(const basicFish& a, const basicFish& b) {
    int leftA = a.posX;
    int rightA = a.posX + a.width;
    int topA = a.posY;
    int bottomA = a.posY + a.height;

    int leftB = b.posX;
    int rightB = b.posX + b.width;
    int topB = b.posY;
    int bottomB = b.posY + b.height;

    if (bottomA <= topB) {
        return false;
    }

    if (topA >= bottomB) {
        return false;
    }

    if (rightA <= leftB) {
        return false;
    }

    if (leftA >= rightB) {
        return false;
    }

    return true;
}



bool init() {
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                std::srand(static_cast<unsigned int>(std::time(0))); // Seed the random number generator here
            }
        }
    }

    return success;
}

bool loadMedia() {
    bool success = true;

    SDL_Surface* loadedSurface = SDL_LoadBMP("C:\\Users\\Jakub\\source\\repos\\Akwaos\\Akwaos\\x64\\Debug\\water.bmp");
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", "water.bmp", SDL_GetError());
        success = false;
    }
    else {
        gWater = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (gWater == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", "water.bmp", SDL_GetError());
            success = false;
        }
    }

    loadedSurface = SDL_LoadBMP("C:\\Users\\Jakub\\source\\repos\\Akwaos\\Akwaos\\x64\\Debug\\basic.bmp");
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL Error: %s\n", "basic.bmp", SDL_GetError());
        success = false;
    }
    else {
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 255, 255));
        gBasicFish = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        SDL_FreeSurface(loadedSurface);
        if (gBasicFish == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", "basic.bmp", SDL_GetError());
            success = false;
        }
    }

    return success;
}

void close() {
    SDL_DestroyTexture(gWater);
    gWater = NULL;
    SDL_DestroyTexture(gBasicFish);
    gBasicFish = NULL;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    SDL_Quit();
}

void spawnBasicFish(int numOfFish, std::vector<basicFish>& basicFishGroup) {
    for (int i = 0; i < numOfFish; ++i) {
        basicFish newFish;
        basicFishGroup.push_back(newFish);
    }
}

int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {
            std::vector<basicFish> fishCollection;
            spawnBasicFish(10, fishCollection);

            bool quit = false;
            SDL_Event e;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                for (auto& fish : fishCollection) {
                    fish.updatePosition(fishCollection);
                }

                SDL_RenderClear(gRenderer);
                SDL_RenderCopy(gRenderer, gWater, NULL, NULL);

                for (const auto& fish : fishCollection) {
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