#include "gameblocks.h"

GameBlock::GameBlock() { movement = NONE; }

GameBlock::GameBlock(SDL_Color & color, int x, int y, int w, int h)
{
    Color = color;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    movement = NONE;
}

void GameBlock::Render(SDL_Renderer * renderer)
{
    SDL_SetRenderDrawColor(renderer, Color.r, Color.g, Color.b, Color.a);
    SDL_RenderFillRect(renderer, &rectangle);
}

void GameBlock::set_color(SDL_Color & new_color) { Color = new_color; }

SDL_Color GameBlock::get_color() { return Color; }

void GameBlock::set_dimensions(int x, int y, int w, int h)
{
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
}

SDL_Rect GameBlock::get_dimensions() { return rectangle; }

Direction GameBlock::get_direction() { return movement; }

void GameBlock::set_direction(Direction & new_movement) { movement = new_movement; }


SnakeBlock::SnakeBlock(GameBlock & head_block)
{
    SDL_Color temp_color = head_block.get_color();
    SDL_Rect temp;
    temp = head_block.get_dimensions();
    temp.y = temp.y + temp.h;
    GameBlock add_on1(temp_color, temp.x, temp.y, temp.w, temp.h);
    temp.y = temp.y + temp.h;
    GameBlock add_on2(temp_color, temp.x, temp.y, temp.w, temp.h);
    blocks.insert(head_block);
    blocks.insert(add_on1);
    blocks.insert(add_on2);
}

void GameBlock::clear_block(SDL_Renderer * renderer, SDL_Color & background)
{
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRect(renderer, &rectangle);
}

void SnakeBlock::Render(SDL_Renderer * renderer, SDL_Color & background, Direction & movement)
{
    clear_snake(renderer, background);
    Node<GameBlock> * current = blocks.get_head();
    move(movement);
    while (current != NULL)
    {
        current->data.Render(renderer);
        current = current->next;
    }
}

void SnakeBlock::add_block()
{
    Node<GameBlock> * current = blocks.get_head();
    while (current->next != NULL) { current = current->next; }
    SDL_Color temp_color = current->data.get_color();
    SDL_Rect temp = current->data.get_dimensions();
    Direction last_block_movement = current->data.get_direction();
    if (last_block_movement == UP) { temp.y += temp.h; }
    else if (last_block_movement == DOWN) { temp.y -= temp.h; }
    else if (last_block_movement == RIGHT) { temp.x -= temp.w; }
    else if (last_block_movement == LEFT) { temp.x += temp.w; }
    else { temp.y += temp.h / 2; }
    GameBlock newBlock(temp_color, temp.x, temp.y, temp.w, temp.h);
    blocks.insert(newBlock);
}

void SnakeBlock::move(Direction & movement)
{
    if (movement != NONE)
    {
        Node<GameBlock> * current = blocks.get_head();
        SDL_Rect p_dim = current->data.get_dimensions();
        SDL_Rect change = p_dim;
        if (movement == UP) { change.y -= change.h; }
        else if (movement == DOWN) { change.y += change.h; }
        else if (movement == RIGHT) { change.x += change.w; }
        else if (movement == LEFT) { change.x -= change.w; }
        current->data.set_dimensions(change.x, change.y, change.w, change.h);
        current->data.set_direction(movement);
        current = current->next;
        while (current != NULL)
        {
            SDL_Rect c_dim = current->data.get_dimensions();
            SDL_Rect d_dim = { p_dim.x - c_dim.x, p_dim.y - c_dim.y, p_dim.w - c_dim.w, p_dim.h - p_dim.h};
            Direction new_movement;
            if (d_dim.x < 0) { new_movement = LEFT; }
            if (d_dim.x > 0) { new_movement = RIGHT; }
            if (d_dim.y > 0) { new_movement = DOWN; }
            if (d_dim.y < 0) { new_movement = UP; }
            else { new_movement = NONE; }
            current->data.set_direction(new_movement);
            current->data.set_dimensions(c_dim.x + d_dim.x, c_dim.y + d_dim.y, c_dim.w + d_dim.w, c_dim.h + d_dim.h);
            p_dim = c_dim;
            current = current->next;
        }
    }
}

void SnakeBlock::set_color(SDL_Color & new_color)
{
    Node<GameBlock> * current = blocks.get_head();
    while (current != NULL)
    {
        current->data.set_color(new_color);
        current = current->next;
    }
}

SDL_Color SnakeBlock::get_color()
{
    GameBlock temp = blocks.get_head()->data;
    return temp.get_color();
}

void SnakeBlock::clear_snake(SDL_Renderer * renderer, SDL_Color & background)
{
    Node<GameBlock> * current = blocks.get_head();
    while (current != NULL)
    {
        current->data.clear_block(renderer, background);
        current = current->next;
    }
}

bool SnakeBlock::collect(GameBlock & point_block)
{
    SDL_Rect a = blocks.get_head()->data.get_dimensions();
    SDL_Rect b = point_block.get_dimensions();
    SDL_Rect c;
    if (SDL_IntersectRect(&a, &b, &c)) { return true; }
    return false;
}

bool SnakeBlock::collision(SDL_Rect & border)
{
    SDL_Rect head = blocks.get_head()->data.get_dimensions();
    if (head.x <= border.x || head.x + head.w >= border.x + border.w) { return true; }
    else if (head.y <= border.y || head.y + head.h >= border.y + border.h) { return true; }
    Node<GameBlock> * current = blocks.get_head();
    current = current->next;

    while (current != NULL)
    {
        SDL_Rect body = current->data.get_dimensions();
        SDL_Rect temp;
        if (SDL_IntersectRect(&head, & body, &temp)) { return true; }
        current = current->next;
    }
    return false;
}
