#include "snakegame.h"

Snake_Game::Snake_Game()
{
    main_Window = NULL;
    main_Surface = NULL;
    main_Renderer = NULL;

    window_width = 1280;
    window_height = 720;

    Black.r = 0, Black.g = 0,   Black.b = 0,   Black.a = 255;
    White.r = 255, White.g = 255, White.b = 255, White.a = 255;
    Light_Green.r = 51, Light_Green.g = 255, Light_Green.b = 0, Light_Green.a = 255;
    Yellow.r = 237, Yellow.g = 249, Yellow.b = 62, Yellow.a = 255;

    R = 2;

    resolutions[0] = "800 x 600";
    resolutions[1] = "1024 x 576";
    resolutions[2] = "1280 x 720";
    resolutions[3] = "1360 x 768";
    resolutions[4] = "1920 x 1080";

    C = 2;

    colors[0].r = 0, colors[0].g = 102, colors[0].b = 255, colors[0].a = 255;
    colors[1].r = 255, colors[1].g = 17, colors[1].b = 0, colors[1].a = 255;
    colors[2] = Light_Green;

    current_score = 0;
    high_score = 0;
}

void Snake_Game::reset() {
    high_score = (current_score > high_score) ? current_score : high_score;
    current_score = 0;
}

bool Snake_Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { return false; }
    if (TTF_Init() < 0) { return false; }
    main_Window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (main_Window == NULL) { return false; }
    main_Surface = SDL_GetWindowSurface(main_Window);
    if (main_Surface == NULL) { return false; }
    main_Renderer = SDL_CreateRenderer(main_Window, -1, SDL_RENDERER_SOFTWARE);
    if (main_Renderer == NULL) { return false; }
    return true;
}

int Snake_Game::game_loop()
{
    MenuCodes next_menu = home_menu();
    while (next_menu != GAME)
    {
        if (next_menu == HOME) { next_menu = home_menu(); }
        if (next_menu == HTP) { next_menu = howtoplay_menu(); }
        if (next_menu == OPTIONS) { next_menu = option_menu(); }
        if (next_menu == QUIT) { return 0; }
    }
    if (pick_color() < 0) { return 0; }
    if (begin_game() < 0) { return 0; }
    return play_again();
}

bool Snake_Game::play_again()
{
    clear_screen();
    reset();
    string font = "Other/Pixel Countdown.ttf";
    TextBox title("Play Again?", window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10, font, Light_Green, Light_Green);
    TextBox yes("Yes", window_width * 3/10, window_height * 6/10, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    TextBox no("No", window_width * 6/10, window_height * 6/10, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    title.set_bold();
    yes.set_bold();
    no.set_bold();
    SDL_Event event;
    while (1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { return false; }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (yes.Detection()) { return true; }
                    else if (no.Detection()) { return false; }
                }
            }
        }
        Uint32 start = SDL_GetTicks();
        title.Render(main_Renderer, Black);
        yes.Render(main_Renderer, Black);
        no.Render(main_Renderer, Black);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 100);
    }
}

int Snake_Game::begin_game()
{
    string text[3] = {"", ""};
    string font = "Other/Pixel Countdown.ttf";
    text[0] = adjust_score_string(current_score);
    text[1] = adjust_score_string(high_score);
    clear_screen();
    TextBox title("Snake", window_width * 4/10, window_height * 1/20, window_width * 2/10, window_height * 1/10, font, Light_Green, Light_Green);
    TextBox score("Score:", window_width * 3/20, window_height * 3/20, window_width * 1/10, window_height * 1/20, font, White, White);
    TextBox highscore("High Score:", window_width * 14/20, window_height * 3/20, window_width * 3/20, window_height * 1/20, font, White, White);
    TextBox score_counter(text[0], window_width * 5/20, window_height * 3/20, window_width * 1/10, window_height * 1/20, font, White, White);
    TextBox hscore_counter(text[1], window_width * 17/20, window_height * 3/20, window_width * 1/10, window_height * 1/20, font, White, White);
    title.set_bold();
    score.set_bold();
    highscore.set_bold();
    score_counter.set_bold();
    hscore_counter.set_bold();
    TextBox game_border("", window_width * 1/40, window_height * 5/20, window_width * 38/40, window_height * 14/20, font, White, White);
    game_border.set_border(White);
    SDL_Rect game_rect = game_border.get_dimensions();
    GameBlock head(colors[C], window_width/2, window_height/2, window_width * 1/80, window_width * 1/80);
    SnakeBlock snake(head);
    GameBlock new_block = point_block(game_rect);
    SDL_Event event;
    Direction movement = NONE;
    while(1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { break; }
            if (event.type == SDL_KEYDOWN)
            {
                const Uint8 * keys = SDL_GetKeyboardState(NULL);
                if (keys[SDL_SCANCODE_UP] && movement != DOWN) { movement = UP; }
                else if (keys[SDL_SCANCODE_DOWN] && movement != UP && movement != NONE) { movement = DOWN; }
                else if (keys[SDL_SCANCODE_RIGHT] && movement != LEFT) { movement = RIGHT; }
                else if (keys[SDL_SCANCODE_LEFT] && movement != RIGHT) { movement = LEFT; }
            }
        }
        Uint32 start = SDL_GetTicks();
        if (snake.collision(game_rect)) { return 0; }
        if (snake.collect(new_block))
        {
            snake.add_block();
            new_block.clear_block(main_Renderer, Black);
            new_block = point_block(game_rect);
            current_score += 10;
            score_counter.set_name(adjust_score_string(current_score));
        }
        game_border.Render(main_Renderer, Black);
        new_block.Render(main_Renderer);
        title.Render(main_Renderer, Black);
        score.Render(main_Renderer, Black);
        highscore.Render(main_Renderer, Black);
        score_counter.Render(main_Renderer, Black);
        hscore_counter.Render(main_Renderer, Black);
        snake.Render(main_Renderer, Black, movement);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 15);
    }

    return -1;
}

int Snake_Game::pick_color()
{
    string font = "Other/Pixel Countdown.ttf";
    clear_screen();
    TextBox title("Snake", window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10, font, Light_Green, Light_Green);
    TextBox color_title("Pick Color",  window_width * 7/20, window_height * 4/10, window_width * 3/10, window_height * 1/10, font, White, White);
    TextBox lower("<", window_width * 7/20, window_height * 6/10, window_width * 1/40, window_height * 1/10, font, White, Yellow);
    TextBox raise(">", window_width * 25/40, window_height * 6/10, window_width * 1/40, window_height * 1/10, font, White, Yellow);
    TextBox cont("Continue", window_width * 4/10, window_height * 17/20, window_width * 2/10, window_height * 1/10, font, White, Yellow);
    title.set_bold();
    color_title.set_bold();
    lower.set_bold();
    raise.set_bold();
    cont.set_bold();
    GameBlock block(colors[C], window_width * 39/80, window_height * 12/20, window_width * 1/40, window_width * 1/40);
    SnakeBlock test_snake(block);
    SDL_Event event;
    Direction movement = NONE;
    while(1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { break; }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (lower.Detection())
                    {
                        if (C > 0) { C--; }
                        else if (C == 0) { C = 2; }
                        test_snake.set_color(colors[C]);
                    }

                    if (raise.Detection())
                    {
                        if (C < 2) { C++; }
                        else if (C == 2) { C = 0; }
                        test_snake.set_color(colors[C]);
                    }

                    if (cont.Detection()) { return 0; }
                }
            }
        }
        Uint32 start = SDL_GetTicks();
        title.Render(main_Renderer, Black);
        color_title.Render(main_Renderer, Black);
        lower.Render(main_Renderer, Black);
        raise.Render(main_Renderer, Black);
        cont.Render(main_Renderer, Black);
        test_snake.Render(main_Renderer, Black, movement);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 100);
    }
    return -1;
}

MenuCodes Snake_Game::home_menu()
{
    string font = "Other/Pixel Countdown.ttf";
    clear_screen();
    TextBox title("Snake", window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10, font, Light_Green, Light_Green);
    TextBox start_game("Start Game",  window_width * 7/20, window_height * 4/10, window_width * 3/10, window_height * 1/10, font, White, Yellow);
    TextBox how_to_play("How To Play", window_width * 13/40, window_height * 11/20, window_width * 14/40, window_height * 1/10, font, White, Yellow);
    TextBox options("Options", window_width * 4/10, window_height * 7/10, window_width * 2/10, window_height * 1/10, font, White, Yellow);
    TextBox quit("Quit", window_width * 9/20, window_height * 17/20, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    title.set_bold();
    start_game.set_bold();
    how_to_play.set_bold();
    options.set_bold();
    quit.set_bold();
    SDL_Event event;
    while(1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { break; }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (start_game.Detection()) { return GAME; }
                    else if (how_to_play.Detection()) { return HTP; }
                    else if (options.Detection()) { return OPTIONS; }
                    else if (quit.Detection()) { break; }
                }
            }
        }
        Uint32 start = SDL_GetTicks();
        title.Render(main_Renderer, Black);
        start_game.Render(main_Renderer, Black);
        how_to_play.Render(main_Renderer, Black);
        options.Render(main_Renderer, Black);
        quit.Render(main_Renderer, Black);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 100);
    }
    return QUIT;
}

MenuCodes Snake_Game::howtoplay_menu()
{
    string font = "Other/Pixel Countdown.ttf";
    clear_screen();
    TextBox title("How To Play", window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10, font, Light_Green, Light_Green);
    TextBox back("Back", window_width * 9/20, window_height * 17/20, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    title.set_bold();
    back.set_bold();
    SDL_Event event;
    while(1)
    {
        Uint32 start = SDL_GetTicks();
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { break; }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (back.Detection()) { return HOME; }
                }
            }
        }
        title.Render(main_Renderer, Black);
        back.Render(main_Renderer, Black);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 100);
    }
    return QUIT;
}

MenuCodes Snake_Game::option_menu()
{
    string font = "Other/Pixel Countdown.ttf";
    clear_screen();
    TextBox title("Options", window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10, font, Light_Green, Light_Green);
    TextBox resolution("Resolution", window_width * 2/10, window_height * 5/10, window_width * 5/20, window_height * 1/10, font, White, White);
    TextBox lower("<", window_width * 5/10, window_height * 5/10, window_width * 1/40, window_height * 1/10, font, White, Yellow);
    TextBox dimension(resolutions[R], window_width * 11/20, window_height * 5/10, window_width * 2/10, window_height * 1/10, font, White, White);
    TextBox raise(">", window_width * 31/40, window_height * 5/10, window_width * 1/40, window_height * 1/10, font, White, Yellow);
    TextBox apply("Apply", window_width * 3/10, window_height * 17/20, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    TextBox back("Back", window_width * 12/20, window_height * 17/20, window_width * 1/10, window_height * 1/10, font, White, Yellow);
    title.set_bold();
    resolution.set_bold();
    lower.set_bold();
    dimension.set_bold();
    raise.set_bold();
    apply.set_bold();
    back.set_bold();
    SDL_Event event;

    while (1)
    {
        if (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) { break; }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (apply.Detection())
                    {
                        change_resolution(dimension.get_name());
                        title.set_dimensions(window_width * 1/10, window_height * 1/20, window_width * 8/10, window_height * 3/10);
                        resolution.set_dimensions(window_width * 2/10, window_height * 5/10, window_width * 5/20, window_height * 1/10);
                        lower.set_dimensions(window_width * 5/10, window_height * 5/10, window_width * 1/40, window_height * 1/10);
                        dimension.set_dimensions(window_width * 11/20, window_height * 5/10, window_width * 2/10, window_height * 1/10);
                        raise.set_dimensions(window_width * 31/40, window_height * 5/10, window_width * 1/40, window_height * 1/10);
                        apply.set_dimensions(window_width * 3/10, window_height * 17/20, window_width * 1/10, window_height * 1/10);
                        back.set_dimensions(window_width * 12/20, window_height * 17/20, window_width * 1/10, window_height * 1/10);
                    }

                    if (back.Detection()) { return HOME; }
                    if (lower.Detection())
                    {
                        if (R > 0) { R--; }
                        dimension.set_name(resolutions[R]);
                    }

                    if (raise.Detection())
                    {
                        if (R < 4) { R++; }
                        dimension.set_name(resolutions[R]);
                    }
                }
            }
        }
        Uint32 start = SDL_GetTicks();
        title.Render(main_Renderer, Black);
        resolution.Render(main_Renderer, Black);
        lower.Render(main_Renderer, Black);
        dimension.Render(main_Renderer, Black);
        raise.Render(main_Renderer, Black);
        apply.Render(main_Renderer, Black);
        back.Render(main_Renderer, Black);
        SDL_RenderPresent(main_Renderer);
        Uint32 end = SDL_GetTicks();
        fps_cap(start, end, 100);
    }
    return QUIT;
}

void Snake_Game::exit()
{
    SDL_DestroyRenderer(main_Renderer);
    SDL_DestroyWindow(main_Window);
    SDL_Quit();
}

void Snake_Game::change_resolution(string new_resolution)
{
    if (new_resolution == resolutions[0])
    {
        window_width = 800;
        window_height = 600;
    }
    else if (new_resolution == resolutions[1])
    {
        window_width = 1024;
        window_height = 576;
    }
    else if (new_resolution == resolutions[2])
    {
        window_width = 1280;
        window_height = 720;
    }
    else if (new_resolution == resolutions[3])
    {
        window_width = 1360;
        window_height = 768;
    }
    else if (new_resolution == resolutions[4])
    {
        window_width = 1920;
        window_height = 1080;
    }
    SDL_DestroyWindow(main_Window);
    main_Window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_SHOWN);
    main_Surface = SDL_GetWindowSurface(main_Window);
    main_Renderer = SDL_CreateRenderer(main_Window, -1, SDL_RENDERER_SOFTWARE);
}

void Snake_Game::clear_screen()
{
    SDL_Rect temp;
    temp.x = 0, temp.y = 0, temp.w = window_width, temp.h = window_height;
    SDL_SetRenderDrawColor(main_Renderer, Black.r, Black.g, Black.b, Black.a);
    SDL_RenderFillRect(main_Renderer, &temp);
}

void Snake_Game::fps_cap(Uint32 start, Uint32 end, int frames)
{
    int delay = 1000/frames - (end - start);
    if (delay > 0) { SDL_Delay(delay); }
}

GameBlock Snake_Game::point_block(SDL_Rect & rectangle)
{
    srand((time(NULL)));
    int random_x = 0, random_y = 0;
    while (random_x < rectangle.x) { random_x = rand() % (rectangle.x + rectangle.w - window_width * 1/80); }
    while (random_y < rectangle.y) { random_y = rand() % (rectangle.y + rectangle.h - window_width * 1/80); }
    GameBlock new_block(Yellow, random_x, random_y, window_width * 1/80, window_width * 1/80);
    return new_block;
}

string Snake_Game::int_to_string(int num = 0)
{
    stringstream ss;
    ss << num;
    return ss.str();
}

string Snake_Game::adjust_score_string(int num = 0)
{
    if (num == 0) { return " 00000"; }
    int pow = 5;
    int extra = num;
    string add_text = " ";
    while (extra != 0) { extra /= 10; pow--; }
    while (pow > 0) { add_text += "0"; pow--; }
    add_text += int_to_string(num);
    return add_text;
}
