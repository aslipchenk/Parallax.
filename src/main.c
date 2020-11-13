#include "header.h"

void load_menu(GameState *game)
{
    SDL_Surface *surface = NULL;
    game->menu_status = 0;
    //меню_________________________________________________________________________
    surface = IMG_Load("./resource/images/menu/menu2.png");
    game->menu = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    if (surface == NULL)
    {
        printf("Cannot find menu.png");
        SDL_Quit();
        exit(1);
    }
}

void menu_events(GameState *gameState)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_b:
        case SDLK_1:
            gameState->menu_status = 1;

            break;
        case SDLK_r:
        case SDLK_2:
            gameState->menu_status = 2;

            break;
            //            case SDLK_q:
        case SDLK_ESCAPE:
            gameState->menu_status = 3;

            break;
        default:
            gameState->menu_status = 0;
            break;
        }
    }
}

int doRender_menu1(SDL_Renderer *renderer, GameState *game)
{
    int menu_status = 0;
    SDL_Rect menu = {0, 0, 1280, 720};
    SDL_RenderCopy(renderer, game->menu, NULL, &menu);
    SDL_RenderPresent(renderer); //_________________________________________menu
                                 //  game->musicChannel = Mix_PlayChannel(-1, game->menuMusic, -1);
    return menu_status;
}

int random_sign(int mult, int step)
{
    step = step * mult * pow(-1, (random() % 2));
    return step;
}

// void initStars(GameState *game)
// {
//   for(int i = 0; i < NUM_STARS; i++)
//   {
//         game->stars[i].baseX = game->ledges[i].x - random_sign(1, 1) * random() % 120;
//         game->stars[i].baseY = game->ledges[i].y - random() % 120;
//         game->stars[i].mode = random() % 2;
//         game->stars[i].phase = 2 * 3.14 * (random() % 360) / 360.0f;
//   }
// }

void doRender(SDL_Renderer *renderer, GameState *game)
{
    if (game->statusState == STATUS_STATE_LIVES)
    {
        draw_status_lives(game);
    }
    else if (game->statusState == STATUS_STATE_GAMEOVER)
    {
        draw_game_over(game);
    }
    else if (game->statusState == STATUS_STATE_WIN)
    {
        draw_game_win(game);
    }
    else if (game->statusState == STATUS_STATE_GAME)
    {
        //set the drawing color to blue
        //SDL_SetRenderDrawColor(renderer, 128, 128, 255, 255);

        //Clear the screen (to blue)
        SDL_RenderClear(renderer);

        //set the drawing color to white
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        SDL_RenderCopy(renderer, game->back[0], NULL, NULL);
        SDL_RenderCopy(renderer, game->back[1], NULL, NULL);
        for (long long i = 0; i < 100000; i += 1280) {
        SDL_Rect mountRect = {game->scrollMount + game->mount.x + i, game->mount.y, 1280, 720};
        SDL_RenderCopy(renderer, game->mountain, NULL, &mountRect);
        SDL_Rect villageRect = {game->scrollVillage + game->village.x + i, game->village.y, 1280, 720};
        SDL_RenderCopy(renderer, game->houses, NULL, &villageRect);
        SDL_Rect bridgeRect = {game->scrollBridge + game->bridges.x + i, game->bridges.y, 1280, 720};
        SDL_RenderCopy(renderer, game->bridge, NULL, &bridgeRect);
        }
        // SDL_Rect mountRect = {game->scrollMount + game->mount.x + 5000, game->mount.y, 1280, 720};
        // SDL_RenderCopy(renderer, game->mountain, NULL, &mountRect);
        SDL_Rect houseRect = {game->scrollX + game->house.x, game->house.y - 300, 1400, 800};
        SDL_RenderCopy(renderer, game->home, NULL, &houseRect);
        

        //floor render
        // for(int i = 0; i < 100; i++)
        // {
        //   SDL_Rect backRect = { game->scrollX+game->back[i].x, game->back[i].y, game->back[i].w, game->back[i].h };
        //   SDL_RenderCopy(renderer, game->back[6], NULL, &backRect);
        // }

        for (int i = 0; i < NUM_LEDGES; i++)
        {
            SDL_Rect ledgeRect = {game->scrollX + game->ledges[i].x, game->ledges[i].y, game->ledges[i].w, game->ledges[i].h};
            SDL_RenderCopy(renderer, game->brick, NULL, &ledgeRect);
        }

        //draw the star images
        for (int i = 0; i < NUM_STARS; i++)
        {
            SDL_Rect starRect = {game->scrollX + game->stars[i].x, game->stars[i].y, 60, 60};
            SDL_RenderCopy(renderer, game->star, NULL, &starRect);
        }

        //draw a rectangle at man's position
        SDL_Rect rect = {game->scrollX + game->man.x, game->man.y, 100, 100};
        SDL_RenderCopyEx(renderer, game->manFrames[game->man.animFrame],
                         NULL, &rect, 0, NULL, (game->man.facingLeft == 1));

        if (game->multiplayer == true)
        {
            SDL_Rect scdPlr = {game->scrollX + game->secondPlayer.x, game->secondPlayer.y, 100, 100};
            SDL_RenderCopyEx(renderer, game->secFrames[game->secondPlayer.animFrame], NULL, &scdPlr, 0, NULL, (game->secondPlayer.facingLeft == 1));

            // SDL_RenderCopy(renderer, game->secFrames[1], NULL, &rect1);
        }

        if (game->man.isDead)
        {
            SDL_Rect rect = {game->scrollX + game->man.x + 25, game->man.y - 5, 38, 83};
            SDL_RenderCopyEx(renderer, game->fire,
                             NULL, &rect, 0, NULL, (game->time % 20 < 10));
            if (game->multiplayer) {
                SDL_Rect rect1 = {game->scrollX + game->secondPlayer.x + 25, game->secondPlayer.y - 5, 38, 83};
                SDL_RenderCopyEx(renderer, game->fire,
                             NULL, &rect1, 0, NULL, (game->time % 20 < 10));     
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int processEvents(SDL_Window *window, GameState *game)
{
    SDL_Event event;
    int done = 0;

    //joystick

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_WINDOWEVENT_CLOSE:
        {
            if (window)
            {
                SDL_DestroyWindow(window);
                window = NULL;
                done = 1;
            }
        }
        break;
        case SDL_KEYDOWN:
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                game->menu_status = 0;
                Mix_FreeChunk(game->bgMusic);
                //done = 1;
                break;
            case SDLK_UP:
                if (game->man.onLedge)
                {
                    game->man.dy = -8;
                    game->man.onLedge = 0;
                    Mix_PlayChannel(-1, game->jumpSound, 0);
                }
                break;
            case SDLK_w:
                if (game->secondPlayer.onLedge)
                {
                    game->secondPlayer.dy = -8;
                    game->secondPlayer.onLedge = 0;
                    Mix_PlayChannel(-1, game->jumpSound, 0);
                }
                break;
            case SDLK_m:
                //less annoying mode
                Mix_VolumeChunk(game->dieSound, 0);
                Mix_VolumeChunk(game->bgMusic, 0);
                break;
                // case SDLK_RETURN: //alt+enter fullscreen toggle
                // {
                //     if(event.key.keysym.mod & KMOD_ALT)
                // {
                //     if(!(SDL_GetWindowFlags(game->window) & SDL_WINDOW_FULLSCREEN_DESKTOP))
                //     SDL_SetWindowFullscreen(game->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                // else
                // SDL_SetWindowFullscreen(game->window, 0);
                // }

                break;
            }
        }
        break;
        case SDL_QUIT:
            done = 1;
            break;
        }
    }

    //joystick button sound

    //More jumping
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_UP])
    {
        game->man.dy -= 0.2f;
    }

    //Walking
    if (state[SDL_SCANCODE_LEFT])
    {
        game->man.dx -= 0.5;
        if (game->man.dx < -6)
        {
            game->man.dx = -6;
        }
        game->man.facingLeft = 1;
        game->man.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_RIGHT])
    {
        game->man.dx += 0.5;
        if (game->man.dx > 6)
        {
            game->man.dx = 6;
        }
        game->man.facingLeft = 0;
        game->man.slowingDown = 0;
    }
    else
    {
        game->man.animFrame = 0;
        game->man.dx *= 0.8f;
        game->man.slowingDown = 1;
        if (fabsf(game->man.dx) < 0.1f)
        {
            game->man.dx = 0;
        }
    }


    if (state[SDL_SCANCODE_W])
    {
        game->secondPlayer.dy -= 0.2f;
    }

    if (state[SDL_SCANCODE_A])
    {
        game->secondPlayer.dx -= 0.5;
        if (game->secondPlayer.dx < -6)
        {
            game->secondPlayer.dx = -6;
        }
        game->secondPlayer.facingLeft = 1;
        game->secondPlayer.slowingDown = 0;
    }
    else if (state[SDL_SCANCODE_D])
    {
        game->secondPlayer.dx += 0.5;
        if (game->secondPlayer.dx > 6)
        {
            game->secondPlayer.dx = 6;
        }
        game->secondPlayer.facingLeft = 0;
        game->secondPlayer.slowingDown = 0;
    }
    else
    {
        game->secondPlayer.animFrame = 0;
        game->secondPlayer.dx *= 0.8f;
        game->secondPlayer.slowingDown = 1;
        if (fabsf(game->secondPlayer.dx) < 0.1f)
        {
            game->secondPlayer.dx = 0;
        }
    }

    // if (state [SDL_SCANCODE_UP] )
    // {
    //     game->man.y -= 10;
    // }
    // if (state [SDL_SCANCODE_DOWN] )
    // {
    //     game->man.y += 10;
    // }
    return done;
}

void process(GameState *game)
{
    //add time
    game->time++;

    if (game->statusState == STATUS_STATE_LIVES)
    {
        if (game->time > 120)
        {
            shutdown_status_lives(game);
            game->statusState = STATUS_STATE_GAME;
            game->musicChannel = Mix_PlayChannel(-1, game->bgMusic, -1);
        }
    }

    else if (game->statusState == STATUS_STATE_GAMEOVER)
    {
        if (game->time > 190)
        {
            // SDL_Quit();
            // exit(0);
            game->menu_status = 0;
        }
    }

    else if (game->statusState == STATUS_STATE_GAME)
    {
        //Mix_HaltChannel(game->musicChannel);
        if (!game->man.isDead)
        {
            //man movement
            Man *man = &game->man;
            man->x += man->dx;
            man->y += man->dy;

            if (man->dx != 0 && man->onLedge && !man->slowingDown)
            {
                if (game->time % 3 == 0)
                {
                    if (man->animFrame < 11)
                    {
                        man->animFrame++;
                    }
                    else
                    {
                        man->animFrame = 0;
                    }
                }
            }

            if (man->x > 38320)
            {
                init_game_win(game);
                game->statusState = STATUS_STATE_WIN;
            }

            man->dy += GRAVITY;
        }
        if (game->multiplayer) 
        {
            if (!game->man.isDead)
            {
            //man movement
            Man *secondPlayer = &game->secondPlayer;
            secondPlayer->x += secondPlayer->dx;
            secondPlayer->y += secondPlayer->dy;

            if (secondPlayer->dx != 0 && secondPlayer->onLedge && !secondPlayer->slowingDown)
            {
                if (game->time % 3 == 0)
                {
                    if (secondPlayer->animFrame < 11)
                    {
                        secondPlayer->animFrame++;
                    }
                    else
                    {
                        secondPlayer->animFrame = 0;
                    }
                }
            }

                if (secondPlayer->x > 38320)
                {
                    init_game_win(game);
                    game->statusState = STATUS_STATE_WIN;
                }

                secondPlayer->dy += GRAVITY;
            }
        }
        for (int i = 0; i < NUM_STARS; i++)
        {
            game->stars[i].x = game->stars[i].baseX;
            game->stars[i].y = game->stars[i].baseY;

            if (game->stars[i].mode == 0)
            {
                game->stars[i].x = game->stars[i].baseX + sinf(game->stars[i].phase + game->time * 0.06f) * 75;
            }
            else
            {
                game->stars[i].y = game->stars[i].baseY + cosf(game->stars[i].phase + game->time * 0.06f) * 75;
            }
        }

        if (game->man.isDead && game->deathCountdown < 0)
        {
            game->deathCountdown = 120;
        }
        if (game->deathCountdown >= 0)
        {
            game->deathCountdown--;
            if (game->deathCountdown < 0)
            {
                //init_game_over(game);
                //game->statusState = STATUS_STATE_GAMEOVER;

                game->man.lives--;

                if (game->man.lives >= 0)
                {
                    init_status_lives(game);
                    game->statusState = STATUS_STATE_LIVES;
                    game->time = 0;
                    game->man.isDead = 0;
                    game->man.x = 100;
                    game->man.y = 240 - 40;
                    game->man.dx = 0;
                    game->man.dy = 0;
                    game->man.onLedge = 0;
                    game->time = 0;
                    game->secondPlayer.isDead = 0;
                    game->secondPlayer.x = 50;
                    game->secondPlayer.y = 240 - 40;
                    game->secondPlayer.dx = 0;
                    game->secondPlayer.dy = 0;
                    game->secondPlayer.onLedge = 0;
                    // initStars(game);
                }
                else
                {
                    init_game_over(game);
                    game->statusState = STATUS_STATE_GAMEOVER;
                    game->time = 0;
                }
            }
        }
    }
    if (!game->multiplayer) {
    game->scrollX = -game->man.x + 320;
    game->scrollMount = -game->man.x/4 + 80;
    game->scrollVillage = -game->man.x/3 + 100;
    game->scrollBridge = -game->man.x/2 + 160;
    if (game->scrollX > 0)
        game->scrollX = 0;
    if (game->scrollMount > 0)
        game->scrollMount = 0;
    if (game->scrollVillage > 0)
        game->scrollVillage = 0;
    if (game->scrollBridge > 0)
        game->scrollBridge = 0;
    if (game->scrollX < -38000 + 320)
        game->scrollX = -38000 + 320;
    }
    if (game->multiplayer)
    {
        if (game->time >= 200) {
            game->scrollX -= 3;
            game->scrollMount -= 1;
            game->scrollVillage -= 1.5;
            game->scrollBridge -= 2;
        }
            if (game->statusState == STATUS_STATE_LIVES) {
                game->scrollX = 0;
            }
    }

    // PARALLAX ----------------------------------------------------------
    // if (game->man.x > game->house.x + 600) {
    //     game->house.x = game->man.x + 1280;
    // }
    
    if (game->scrollX == 0) {
        game->house.x = game->house.baseX;
    }
    if (sqrt(game->scrollX * game->scrollX) - 800 > sqrt(game->house.x * game->house.x)) {
        game->house.x = game->house.x + 2300;
    }
}

int collide2d(float x1, float y1, float x2, float y2, float wt1, float ht1, float wt2, float ht2)
{
    return (!((x1 > (x2 + wt2)) || (x2 > (x1 + wt1)) || (y1 > (y2 + ht2)) || (y2 > (y1 + ht1))));
}

void collisionDetect(GameState *game)
{
    for (int i = 0; i < NUM_STARS; i++)
    {
        if (collide2d(game->man.x, game->man.y, game->stars[i].x, game->stars[i].y, 53, 53, 35, 35))
        {
            if (!game->man.isDead)
            {
                game->man.isDead = 1;
                Mix_HaltChannel(game->musicChannel);
                Mix_PlayChannel(-1, game->dieSound, 0);
            }
            break;
        }
    }

    if (game->man.y > 670)
    {
        if (!game->man.isDead)
        {
            game->man.isDead = 1;
            Mix_HaltChannel(game->musicChannel);
            Mix_PlayChannel(-1, game->dieSound, 0);
        }
    }

    for (int i = 0; i < NUM_LEDGES; i++)
    {
        float mw = 60, mh = 60;
        float mx = game->man.x, my = game->man.y;
        float bx = game->ledges[i].x , by = game->ledges[i].y , bw = game->ledges[i].w - 45, bh = game->ledges[i].h;

        if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
        {
            //are we bumping our head?
            if (my < by + bh && my > by && game->man.dy < 0)
            {
                //correct y
                game->man.y = by + bh;
                my = by + bh;

                //bumped our head, stop any jump velocity
                game->man.dy = 0;
                game->man.onLedge = 1;
            }
        }
        if (mx + mw > bx && mx < bx + bw)
        {
            //are we landing on the ledge
            if (my + mh > by && my < by && game->man.dy > 0)
            {
                //correct y
                game->man.y = by - mh;
                my = by - mh;

                //landed on this ledge, stop any jump velocity
                game->man.dy = 0;
                if (!game->man.onLedge)
                {
                    Mix_PlayChannel(-1, game->landSound, 0);
                    game->man.onLedge = 1;
                }
            }
        }

        if (my + mh > by && my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw && mx + mw > bx + bw && game->man.dx < 0)
            {
                //correct x
                game->man.x = bx + bw;
                mx = bx + bw;

                game->man.dx = 0;
            }
            //rubbing against left edge
            else if (mx + mw > bx && mx < bx && game->man.dx > 0)
            {
                //correct x
                game->man.x = bx - mw;
                mx = bx - mw;

                game->man.dx = 0;
            }
        }
    }
    //////////////////////////////// MULTIPLAYER /////////////////////////////////////////////
    for (int i = 0; i < NUM_STARS; i++)
    {
        if (collide2d(game->secondPlayer.x, game->secondPlayer.y, game->stars[i].x, game->stars[i].y, 53, 53, 35, 35))
        {
            if (!game->man.isDead)
            {
                game->man.isDead = 1;
                Mix_HaltChannel(game->musicChannel);
                Mix_PlayChannel(-1, game->dieSound, 0);
            }
            break;
        }
    }

    if (game->secondPlayer.y > 670)
    {
        if (!game->man.isDead)
        {
            game->man.isDead = 1;
            Mix_HaltChannel(game->musicChannel);
            Mix_PlayChannel(-1, game->dieSound, 0);
        }
    }

    for (int i = 0; i < NUM_LEDGES; i++)
    {
        float mw = 60, mh = 60;
        float mx = game->secondPlayer.x, my = game->secondPlayer.y;
        float bx = game->ledges[i].x + 10, by = game->ledges[i].y, bw = game->ledges[i].w - 45, bh = game->ledges[i].h;

        if (mx + mw / 2 > bx && mx + mw / 2 < bx + bw)
        {
            //are we bumping our head?
            if (my < by + bh && my > by && game->secondPlayer.dy < 0)
            {
                //correct y
                game->secondPlayer.y = by + bh;
                my = by + bh;

                //bumped our head, stop any jump velocity
                game->secondPlayer.dy = 0;
                game->secondPlayer.onLedge = 1;
            }
        }
        if (mx + mw > bx && mx < bx + bw)
        {
            //are we landing on the ledge
            if (my + mh > by && my < by && game->secondPlayer.dy > 0)
            {
                //correct y
                game->secondPlayer.y = by - mh;
                my = by - mh;

                //landed on this ledge, stop any jump velocity
                game->secondPlayer.dy = 0;
                if (!game->secondPlayer.onLedge)
                {
                    Mix_PlayChannel(-1, game->landSound, 0);
                    game->secondPlayer.onLedge = 1;
                }
            }
        }

        if (my + mh > by && my < by + bh)
        {
            //rubbing against right edge
            if (mx < bx + bw && mx + mw > bx + bw && game->secondPlayer.dx < 0)
            {
                //correct x
                game->secondPlayer.x = bx + bw;
                mx = bx + bw;

                game->secondPlayer.dx = 0;
            }
            //rubbing against left edge
            else if (mx + mw > bx && mx < bx && game->secondPlayer.dx > 0)
            {
                //correct x
                game->secondPlayer.x = bx - mw;
                mx = bx - mw;

                game->secondPlayer.dx = 0;
            }
        }
    }
}

void loadGame(GameState *game)
{
    SDL_Surface *surface = NULL; //Declare a renderer

    /// PARLLLAX////////////////////////////////
    surface = IMG_Load("resource/images/back/1_game_background/layers/6.png");
    if (surface == NULL)
    {
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->home = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);


    surface = IMG_Load("resource/images/back/1_game_background/layers/4.png");
    if (surface == NULL)
    {
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->houses = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/5.png");
    if (surface == NULL)
    {
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->bridge = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);


    surface = IMG_Load("resource/images/back/1_game_background/layers/3.png");
    if (surface == NULL)
    {
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }
    game->mountain = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/enemies/pumkin1.png");
    if (surface == NULL)
    {
        printf("Cannot find star.png!\n\n");
        SDL_Quit();
        exit(1);
    }

    game->star = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    //load Background
    surface = IMG_Load("resource/images/back/1_game_background/layers/1.png");
    game->back[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/2.png");
    game->back[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/3.png");
    game->back[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/4.png");
    game->back[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/5.png");
    game->back[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/6.png");
    game->back[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/back/1_game_background/layers/7.png");
    game->back[6] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_000.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)0.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_001.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)1.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_002.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)2.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_003.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)3.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_004.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)4.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_005.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)5.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_006.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)6.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[6] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_007.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)7.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[7] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_008.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)8.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[8] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_009.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)9.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[9] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_010.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)10.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[10] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/main_hero/run/Wraith_01_Moving Forward_011.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)11.png");
        SDL_Quit();
        exit(1);
    }
    game->manFrames[11] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    /////////////
    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_000.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)0.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_001.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)1.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_002.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)2.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_003.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)3.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[3] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_004.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)4.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[4] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_005.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)5.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[5] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_006.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)6.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[6] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_007.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)7.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[7] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_008.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)8.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[8] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_009.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)9.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[9] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_010.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)10.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[10] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("./resource/images/second_player/run/Wraith_03_Moving Forward_011.png");
    if (surface == NULL)
    {
        printf("Cannot find V_g(rn)11.png");
        SDL_Quit();
        exit(1);
    }
    game->secFrames[11] = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    ////////////

    surface = IMG_Load("resource/images/main_hero/death/fire.png");
    game->fire = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("resource/images/terrain/block2.png");
    game->brick = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);

    //load fonts
    game->font = TTF_OpenFont("resource/text/Fonts/crazy-pixel.ttf", 60);
    if (!game->font)
    {
        printf("Cannot find font file!\n\n");
        SDL_Quit();
        exit(1);
    }

    //load sounds
    game->menuMusic = Mix_LoadWAV("resource/sounds/Monster_In_The_Field.mp3"); //???
    if (game->menuMusic != NULL)
    {
        Mix_VolumeChunk(game->menuMusic, 120);
    }

    game->bgMusic = Mix_LoadWAV("resource/sounds/Evil_Rising.mp3"); //???
    if (game->bgMusic != NULL)
    {
        Mix_VolumeChunk(game->bgMusic, 120);
    }

    game->dieSound = Mix_LoadWAV("resource/sounds/short-scared-scream.wav");
    if (game->dieSound != NULL)
    {
        Mix_VolumeChunk(game->dieSound, 10);
    }

    game->jumpSound = Mix_LoadWAV("resource/sounds/jump.wav");
    if (game->jumpSound != NULL)
    {
        Mix_VolumeChunk(game->jumpSound, 10);
    }
    //game->landSound = Mix_LoadWAV("resource/sounds/some-steps.wav");

    game->label = NULL;
    game->label2 = NULL;

    game->man.x = 10;
    game->man.y = 50;
    game->man.dx = 0;
    game->man.dy = 0;
    game->man.onLedge = 0;
    game->man.animFrame = 0;
    game->man.facingLeft = 1;
    game->man.slowingDown = 0;
    game->man.lives = 3;
    game->man.isDead = 0;
    game->statusState = STATUS_STATE_LIVES;

    if (game->multiplayer == true)
    {
        game->secondPlayer.x = 100;
        game->secondPlayer.y = 100;
        game->secondPlayer.dx = 0;
        game->secondPlayer.dy = 0;
        game->secondPlayer.onLedge = 0;
        game->secondPlayer.animFrame = 0;
        game->secondPlayer.facingLeft = 1;
        game->secondPlayer.slowingDown = 0;
        game->secondPlayer.lives = 3;
        game->secondPlayer.isDead = 0;
        game->statusState = STATUS_STATE_LIVES;
    }

    init_status_lives(game);

    game->time = 0;
    game->scrollX = 0;
    game->scrollMount = 0;
    game->scrollBridge = 0;
    game->scrollVillage = 0;
    game->deathCountdown = -1;



     // PARALLAX -----------------------------------------
     game->house.x = 300;
     game->house.y = 300;
     game->house.baseX = game->house.x;
     game->mount.x = 0;
     game->mount.y = 0;
     game->mount.baseX = game->house.x;
     game->village.x = 0;
     game->village.y = 0;
     game->village.baseX = game->house.x;
    game->bridges.x = 0;
     game->bridges.y = 0;
     game->bridges.baseX = game->house.x;

    //init stars
    // initStars(game);
    // for(int i = 0; i < NUM_STARS; i++)
    // {
    //     game->stars[i].x = random()%50000;
    //     game->stars[i].y = random()%720;
    // }

    //init floor
    // for(int i = 0; i < 100; i++)
    // {
    //     game->floor[i].x = 256;
    //     game->floor[i].y = 64;
    //     game->floor[i].x = i*256;
    //     game->floor[i].y = 400;
    // }

    //init ledges

    int bulb = 700;
    for (int i = 0; i < 100; i++)
    {
        game->ledges[i].w = 240;
        game->ledges[i].h = 80;
        game->ledges[i].x = i * 300;
        game->ledges[i].y = bulb - random_sign(3, 40);
        while (game->ledges[i].y >= 700)
            game->ledges[i].y -= 50;
        while (game->ledges[i].y <= 100)
            game->ledges[i].y += 50;
        bulb = game->ledges[i].y;

        //init stars
        game->stars[i].baseX = game->ledges[i].x - random_sign(1, 1) * random() % 120;
        game->stars[i].baseY = game->ledges[i].y - random() % 120;
        game->stars[i].mode = random() % 2;
        game->stars[i].phase = 2 * 3.14 * (random() % 360) / 360.0f;

        //  game->stars[i + 100].x = 320 +random()%38400;
        //  game->stars[i + 100].y = random()%720;
    }
    game->ledges[99].x = 00;
    game->ledges[99].y = 300;
}

int main()
{
    GameState gameState;
    SDL_Window *window = NULL; //Declare a window
    SDL_Renderer *renderer = NULL;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO); //Initialize SDL2

    //gameState.joystick = SDL_JoystickOpen(0);

    srandom((int)time(NULL));

    //Create an application window  with the following settings:
    window = SDL_CreateWindow("Game Window",           //window title
                              SDL_WINDOWPOS_UNDEFINED, //initial x position
                              SDL_WINDOWPOS_UNDEFINED, //initial y position
                              1280,                    //width, in pixels
                              720,                     //height, in pixels
                              0                        //flags
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    gameState.renderer = renderer;

    SDL_RenderSetLogicalSize(renderer, 1280, 720);

    TTF_Init();

    Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    loadGame(&gameState);

    //The windiw is open: enter program loop (see SDL_PollEVENT)
    int done = 0;

    load_menu(&gameState);
    while (!done)
    {
        //Check for events
        done = processEvents(window, &gameState);
        //Check for events
        switch (gameState.menu_status)
        {
        //menu
        case 0:
            loadGame(&gameState);
            menu_events(&gameState);
            doRender_menu1(renderer, &gameState);
            break;
            //battle game
        case 1:
            gameState.multiplayer = false;
            //Check for events
            process(&gameState);
            collisionDetect(&gameState);
            doRender(renderer, &gameState);
            processEvents(window, &gameState);
            break;
            //multiplayer
        case 2:
            gameState.multiplayer = true;
            process(&gameState);
            collisionDetect(&gameState);
            doRender(renderer, &gameState);
            processEvents(window, &gameState);
            break;
        case 3:
            done = 1;
            break;
            //Pause
        default:
            gameState.menu_status = 0;
            break;
        }
        //Wait a few seconds before quitting
        //SDL_Delay(10);
    }

    SDL_DestroyTexture(gameState.star);
    // SDL_DestroyTexture(gameState.manFrames[0]);
    // SDL_DestroyTexture(gameState.manFrames[1]);
    for (int i = 0; i < 11; i++)
    {
        SDL_DestroyTexture(gameState.manFrames[i]);
    }

    SDL_DestroyTexture(gameState.brick);
    if (gameState.label != NULL)
        SDL_DestroyTexture(gameState.label);
    if (gameState.label2 != NULL)
        SDL_DestroyTexture(gameState.label2);

    for (int i = 0; i < 7; i++)
    {
        SDL_DestroyTexture(gameState.back[i]);
    }

    TTF_CloseFont(gameState.font);

    Mix_FreeChunk(gameState.menuMusic);
    Mix_FreeChunk(gameState.bgMusic);
    Mix_FreeChunk(gameState.dieSound);
    Mix_FreeChunk(gameState.jumpSound);
    Mix_FreeChunk(gameState.landSound);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    TTF_Quit();

    SDL_Quit();
    return 0;
}
