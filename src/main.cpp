#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include <iostream>
#include <array>

#define countTiles 11

int main(int argc, char *argv[])
{
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
        dt = 1.f / (float)refreshRate, // delta time, Zeit zwischen den Frames in Sekunden
        x = 50.f,
        y = 50.f,
        xv = 15.f,
        yv = 10.f;

    ///  Zusätzlich zu den Globals / Class variables

    constexpr int
        TileSize = 16,
        SizeX = 11,
        SizeY = 11,
        RealSizeX = 60,
        RealSizeY = 60,
        TileSheetHeight = 2,
        TileSheetWidth = 6;

    // Die Karte / das Level
    std::array<std::array<std::array<int, SizeX>, SizeY>, 6> map;

    double temp = 0;

    // Positionen innerhalb des Tilesets
    constexpr const SDL_Point fieldStates[countTiles]{
        {.x = 0, .y = 0}, // player
        {.x = 1, .y = 0}, // wall
        {.x = 2, .y = 0}, // stone
        {.x = 0, .y = 1}, // diamond
        {.x = 1, .y = 1}, // dirt
        {.x = 2, .y = 1}, // space
        {.x = 3, .y = 0}, // firefly
        {.x = 4, .y = 0}, // butterfly
        {.x = 3, .y = 1}, // amoeba
        {.x = 5, .y = 0}, // hard wall (never randomly selected)
        {.x = 5, .y = 1}, // hard wall (never randomly selected)
    };

    ///  Zusätzlich zum Startup

    // Laden externer shared Libraries und initialisierung des SDL Image Subsystems
    IMG_Init(IMG_INIT_JPG |
             IMG_INIT_PNG |
             IMG_INIT_TIF |
             IMG_INIT_WEBP);

    // Bild (hier unser Tileset) in den Hauptspeicher laden
    SDL_Surface *surface = IMG_Load("res/AllTestTyleWaterChangeWall.png");

    // Bild vom Hauptspeicher in den Grafikkartenspeicher laden/übertragen
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Kein Bedarf mehr das Bild im Hauptspeicher zu halten, freigeben
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
                map[i][y][x] = 10;
            }
        }
    }

    int points = 0;
    int test_number = 0;
    int feld_typ = 5;
    int player_temp = 0;
    int player_state = 0; // 0 nichts / 1 links / 2 rechts / 3 Oben / 4 unten
    int side = 0;
    int selectAnimation = 0;

    const SDL_Rect desBackground{
        .x = 0,
        .y = RealSizeY,
        .w = SizeX * RealSizeX,
        .h = RealSizeY * SizeY};
    while (true)
    {

        // ein Frame

        // SDL_PumpEvents() nur nutzen wenn kein SDL_PollEvent() verwendet wird

        SDL_Event event;
        while (SDL_PollEvent(&event))
            if (event.type == SDL_QUIT)
            {
                SDL_DestroyTexture(texture);
                IMG_Quit();
                return 0;
            }

        // Auswahl des feld types
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

        int x, y;
        Uint32 buttons;

        ////SDL_PumpEvents();  brauch nicht weil oben PollEvent // make sure we have the latest mouse state.

        buttons = SDL_GetMouseState(&x, &y);

        // SDL_Log("Mouse cursor is at %d, %d", x, y);
        if ((buttons & SDL_BUTTON_LMASK) != 0)
        {
            // SDL_Log("Mouse Button 1 (left) is pressed.");
            int zelle_x = x / (RealSizeX);
            int zelle_y = y / (RealSizeY)-1; // Eines weg nehmen wegen Title
                                             // std::cout << "zelle_x: " << zelle_x << " zelle_y: " << zelle_y << "\n";

            if (zelle_y < SizeY && zelle_x < SizeX)
            {
                map[side][zelle_y][zelle_x] = feld_typ;
                // std::cout << "Hallo \n";
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
            // std::cout << "zelle_x: " << zelle_x << " zelle_y: " << zelle_y << "\n";
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

                // Anhand des Status-Index wird die Position innerhalb des Tileset bestimmt
                const SDL_Point &selectedState = fieldStates[currentField];

                // Quell-Rechteck innerhalb unseres Tilesets
                const SDL_Rect srcrect{
                    .x = selectedState.x * TileSize,
                    .y = selectedState.y * TileSize,
                    .w = TileSize,
                    .h = TileSize};

                // std::cout << srcrect.x << " "<< srcrect.y << " " << TileSize << "\n";

                // Ziel-Rechteck innerhalb unseres Windows/Fensters
                const SDL_Rect dstrect{
                    .x = x * RealSizeX,
                    .y = y * RealSizeY + RealSizeY, // Die oberste Zeile wird später für das HUD benötigt, deswegen wird alles um einen Tile nach unten geschoben
                    .w = RealSizeX,
                    .h = RealSizeY};

                // Hier wird von der Texture in den Renderer kopiert (hardware-beschleunigt)
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
                    .y = y * RealSizeY + RealSizeY, // Die oberste Zeile wird später für das HUD benötigt, deswegen wird alles um einen Tile nach unten geschoben
                    .w = RealSizeX,
                    .h = RealSizeY};

                // Hier wird von der Texture in den Renderer kopiert (hardware-beschleunigt)
                SDL_RenderDrawRect(renderer, &dstrect);
            }
        }
        SDL_RenderPresent(renderer);
    }
}