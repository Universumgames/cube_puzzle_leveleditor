
#if defined(_WIN32) || defined(Macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#include <SDL.h>
#include <SDL_image.h>
#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <cstdlib>
#include <iostream>
#include <array>
#include <fstream>
#include <string>

#define countTiles 27

constexpr int
    TileSize = 16,
    SizeX = 11,
    SizeY = 11,
    RealSizeX = 60,
    RealSizeY = 60,
    TileSheetHeight = 6,
    TileSheetWidth = 6;

std::array<std::array<std::array<int, SizeX>, SizeY>, 6> map;
std::string level_name = "Test";
int id_index = 1;
double temp = 0;

constexpr const SDL_Point fieldStates[countTiles]{
    {.x = 0, .y = 0}, // Empty
    {.x = 0, .y = 1}, // Flag
    {.x = 1, .y = 0}, // Wall 1
    {.x = 2, .y = 0}, // Wall 2
    {.x = 1, .y = 1}, // Pressure Plate
    {.x = 2, .y = 1}, // Slider bigX
    {.x = 3, .y = 1}, // Slider smallX
    {.x = 5, .y = 1}, // Slider bigY
    {.x = 4, .y = 1}, // Slider smallY
    {.x = 3, .y = 0}, // Magnet
    {.x = 4, .y = 0}, // Stone
    {.x = 5, .y = 0}, // Object Blocker
    // ###############  green
    {.x = 0, .y = 2}, // Pressure Plate
    {.x = 1, .y = 2}, // Slider bigX
    {.x = 2, .y = 2}, // Slider smallX
    {.x = 4, .y = 2}, // Slider bigY
    {.x = 3, .y = 2}, // Slider smallY
    // ############### yellow
    {.x = 0, .y = 3}, // Pressure Plate
    {.x = 1, .y = 3}, // Slider bigX
    {.x = 2, .y = 3}, // Slider smallX
    {.x = 4, .y = 3}, // Slider bigY
    {.x = 3, .y = 3}, // Slider smallY
    // ############### blue
    {.x = 0, .y = 4}, // Pressure Plate
    {.x = 1, .y = 4}, // Slider bigX
    {.x = 2, .y = 4}, // Slider smallX
    {.x = 4, .y = 4}, // Slider bigY
    {.x = 3, .y = 4}, // Slider smallY
};

std::string input;
std::string output;

void writemap(std::string filename)
{
    std::ofstream os(filename);
    if (!os)
    {
        return;
    }
    os << level_name << " " << id_index << "\n";
    for (int s = 0; s < 6; s++)
    {
        os << SizeX << " " << SizeY << "\n";
        for (int i = 0; i < SizeY; ++i)
        {
            for (int j = 0; j < SizeX; ++j)
            {
                os << map[s][i][j] << " ";
            }
            os << "\n";
        }
    }
    os.close();
}

void readmap(std::string filename)
{

    std::ifstream is(filename);
    is >> level_name;
    is >> id_index;
    for (int s = 0; s < 6; s++)
    {
        int rSideX = 0;
        is >> rSideX;
        int rSideY = 0;
        is >> rSideY;

        if (rSideX != SizeX || rSideY != SizeY)
            return;

        for (int x = 0; x < SizeX; x++)
        {
            for (int y = 0; y < SizeY; y++)
            {
                is >> map[s][x][y];
            }
        }
    }
    is.close();
}

void clearmap()
{
    for (int s = 0; s < 6; s++)
    {
        for (int x = 0; x < SizeX; x++)
        {
            for (int y = 0; y < SizeY; y++)
            {
                map[s][x][y] = 0;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    std::cout << argc << "\n";
    if (argc >= 3)
    {
        input = argv[1];
        output = argv[2];
    }
    else if (argc == 2)
    {
        input = "level/out.txt";
        output = argv[1];
    }
    else
    {
        input = "level/out.txt";
        output = "level/out.txt";
    }

    if (argc == 5)
    {
        level_name = argv[3];
        id_index = std::stoi(argv[4]);
    }

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
        "Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1250,
        800,
        SDL_WINDOW_OPENGL);

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_PRESENTVSYNC);

    const int displayIndex = SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode displayMode;
    const int refreshRate = (SDL_GetDesktopDisplayMode(displayIndex, &displayMode) == 0 && displayMode.refresh_rate > 0)
                                ? displayMode.refresh_rate
                                : 60;

    float
        dt = 1.f / (float)refreshRate,
        x = 50.f,
        y = 50.f,
        xv = 15.f,
        yv = 10.f;

    IMG_Init(IMG_INIT_JPG |
             IMG_INIT_PNG |
             IMG_INIT_TIF |
             IMG_INIT_WEBP);

    SDL_Surface *surface = IMG_Load("res/AllTestTyle.png");

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube1.png");
    SDL_Texture *Cube1 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube2.png");
    SDL_Texture *Cube2 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube3.png");
    SDL_Texture *Cube3 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube4.png");
    SDL_Texture *Cube4 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube5.png");
    SDL_Texture *Cube5 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("res/Cube6.png");
    SDL_Texture *Cube6 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    for (int i = 0; i < 6; i++)
    {
        for (int y = 0; y < SizeY; ++y)
        {
            for (int x = 0; x < SizeX; ++x)
            {
                map[i][y][x] = 0;
            }
        }
    }

    int points = 0;
    int test_number = 0;
    int feld_typ = 5;
    int player_temp = 0;
    int player_state = 0;
    int side = 0;
    int selectAnimation = 0;

    const SDL_Rect desBackground{
        .x = 0,
        .y = RealSizeY,
        .w = SizeX * RealSizeX,
        .h = RealSizeY * SizeY};
    while (true)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyTexture(texture);
                IMG_Quit();
                return 0;
            }

        const Uint8 *state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_1])
        {
            side = 0;
        }
        if (state[SDL_SCANCODE_2])
        {
            side = 1;
        }
        if (state[SDL_SCANCODE_3])
        {
            side = 2;
        }
        if (state[SDL_SCANCODE_4])
        {
            side = 3;
        }
        if (state[SDL_SCANCODE_5])
        {
            side = 4;
        }
        if (state[SDL_SCANCODE_6])
        {
            side = 5;
        }
        if (state[SDL_SCANCODE_S])
        {

            std::cout << "export" << output << "\n";
            writemap(output);
            SDL_Delay(500);
        }

        if (state[SDL_SCANCODE_R])
        {
            std::cout << "reload/imported" << input << "\n";
            readmap(input);
            SDL_Delay(500);
        }

        if (state[SDL_SCANCODE_C])
        {
            std::cout << "clean" << output << "\n";
            clearmap();
            SDL_Delay(500);
        }

        int x,
            y;
        Uint32 buttons;

        buttons = SDL_GetMouseState(&x, &y);

        if ((buttons & SDL_BUTTON_LMASK) != 0)
        {
            int zelle_x = x / (RealSizeX);
            int zelle_y = y / (RealSizeY)-1;

            if (zelle_y < SizeY && zelle_x < SizeX)
            {
                map[side][zelle_y][zelle_x] = feld_typ;
            }

            if (zelle_x > SizeX)
            {
                zelle_x -= SizeX + 1;
                for (int i = 0; i < countTiles; i++)
                {
                    if (zelle_x == fieldStates[i].x && zelle_y == fieldStates[i].y)
                    {
                        feld_typ = i;
                        break;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        const SDL_Rect dstrect3{
            .x = 0,
            .y = 0,
            .w = RealSizeX,
            .h = RealSizeY};

        switch (side)
        {
        case 0:

            SDL_RenderCopy(renderer, Cube1, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube1, nullptr, &dstrect3);
            break;
        case 1:
            SDL_RenderCopy(renderer, Cube2, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube2, nullptr, &dstrect3);
            break;
        case 2:
            SDL_RenderCopy(renderer, Cube3, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube3, nullptr, &dstrect3);
            break;
        case 3:
            SDL_RenderCopy(renderer, Cube4, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube4, nullptr, &dstrect3);
            break;
        case 4:
            SDL_RenderCopy(renderer, Cube5, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube5, nullptr, &dstrect3);
            break;
        case 5:
            SDL_RenderCopy(renderer, Cube6, nullptr, &desBackground);
            SDL_RenderCopy(renderer, Cube6, nullptr, &dstrect3);
            break;

        default:
            break;
        }

        for (int y = 0; y < SizeY; ++y)
        {
            for (int x = 0; x < SizeX; ++x)
            {
                const int currentField = map[side][y][x];

                const SDL_Point &selectedState = fieldStates[currentField];

                const SDL_Rect srcrect{
                    .x = selectedState.x * TileSize,
                    .y = selectedState.y * TileSize,
                    .w = TileSize,
                    .h = TileSize};

                const SDL_Rect dstrect{
                    .x = x * RealSizeX,
                    .y = y * RealSizeY + RealSizeY,
                    .w = RealSizeX,
                    .h = RealSizeY};

                SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
            }
        }
        const SDL_Rect dstrect{
            .x = RealSizeX * SizeX + RealSizeX,
            .y = RealSizeY,
            .w = RealSizeX * TileSheetWidth,
            .h = RealSizeY * TileSheetHeight};
        SDL_RenderCopy(renderer, texture, nullptr, &dstrect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &dstrect);

        if (selectAnimation == 0)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            selectAnimation++;
        }
        else if (selectAnimation == 1)
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            selectAnimation++;
        }
        else
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            selectAnimation = 0;
        }
        const SDL_Rect dstrect2{
            .x = RealSizeX * SizeX + RealSizeX + fieldStates[feld_typ].x * RealSizeX,
            .y = RealSizeY + fieldStates[feld_typ].y * RealSizeY,
            .w = RealSizeX,
            .h = RealSizeY};
        SDL_RenderDrawRect(renderer, &dstrect2);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int y = 0; y < SizeY; ++y)
        {
            for (int x = 0; x < SizeX; ++x)
            {
                const int currentField = map[side][y][x];

                const SDL_Rect dstrect{
                    .x = x * RealSizeX,
                    .y = y * RealSizeY + RealSizeY,
                    .w = RealSizeX,
                    .h = RealSizeY};

                SDL_RenderDrawRect(renderer, &dstrect);
            }
        }
        SDL_RenderPresent(renderer);
    }
}