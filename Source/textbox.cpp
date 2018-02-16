#include "textbox.h"

const SDL_Color Black = {0, 0, 0, 0};
const SDL_Color White = {255, 255, 255, 255};
const SDL_Color Red = {255, 0, 0, 255};
const SDL_Color Blue = {0, 0, 255, 255};
const SDL_Color Green = {0, 128, 0, 255};
const SDL_Color Orange = {255, 87, 51, 255};
const SDL_Color Yellow = {255, 255, 0, 255};
const SDL_Color Purple = {128, 0, 128, 255};
const SDL_Color Pink = {255, 0, 255, 255};
const SDL_Color Gray = {94, 94, 94, 255};


TextBox::TextBox(string optionName, int x, int y, int w, int h, string font, const SDL_Color & regular, const SDL_Color & highlight)
{
    name = optionName;

    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;

    text_font = font;

    Regular = regular;
    Highlight = highlight;

    Border = false;
    Borderline.r = 0, Borderline.g = 0, Borderline.b = 0, Borderline.a = 0;
    Bold = false;
}

bool TextBox::Detection()
{
    int x = 0, y = 0;

    SDL_GetMouseState(&x, &y);

    if (rectangle.x > x || x > rectangle.x + rectangle.w) { return false; }

    if (rectangle.y > y || y > rectangle.y + rectangle.h) { return false; }

    return true;
}

void TextBox::Render(SDL_Renderer * renderer, const SDL_Color & background)
{
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRect(renderer, &rectangle);

    TTF_Font * font = TTF_OpenFont(text_font.c_str(), 64);

    if (Bold) { TTF_SetFontStyle(font, TTF_STYLE_BOLD); }

    SDL_Surface * text_surface = NULL;
    SDL_Texture * text_texture = NULL;

    if (Detection()) { text_surface = TTF_RenderText_Solid(font, name.c_str(), Highlight); }

    else { text_surface = TTF_RenderText_Solid(font, name.c_str(), Regular); }

    text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

    SDL_RenderCopy(renderer, text_texture, NULL, &rectangle);

    if (Border)
    {
        SDL_SetRenderDrawColor(renderer, Borderline.r, Borderline.g, Borderline.b, Borderline.a);
        SDL_RenderDrawRect(renderer, &rectangle);
    }

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);

    text_surface = NULL;
    text_texture = NULL;

    if (Bold) { TTF_SetFontStyle(font, TTF_STYLE_NORMAL); }

    TTF_CloseFont(font);

    font = NULL;
}

void TextBox::set_name(string new_name) { name = new_name; }

string TextBox::get_name() const { return name; }

void TextBox::set_dimensions(int x, int y, int w, int h)
{
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
}

void TextBox::set_dimensions(const SDL_Rect & new_rect) { rectangle = new_rect; }

SDL_Rect TextBox::get_dimensions() const { return rectangle; }

void TextBox::set_font(string new_font) { text_font = new_font; }

string TextBox::get_font() const { return text_font; }

void TextBox::set_regular(const SDL_Color & new_regular) { Regular = new_regular; }

SDL_Color TextBox::get_regular() const { return Regular; }

void TextBox::set_highlight(const SDL_Color & new_highlight) { Highlight = new_highlight; }

SDL_Color TextBox::get_highlight() const { return Highlight; }

void TextBox::set_border(const SDL_Color & borderline)
{
    Border = true;
    Borderline = borderline;
}

SDL_Color TextBox::get_border() const { return Borderline; }

void TextBox::disable_border() { Border = false; }

void TextBox::set_bold() { Bold = true; }

void TextBox::disable_bold() { Bold = false; }


ImageBox::ImageBox(string image_file, const SDL_Rect & rect)
{
    image_name = image_file;
    rectangle = rect;
}

ImageBox::ImageBox(string image_file, int x, int y, int w, int h)
{
    image_name = image_file;
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
}

void ImageBox::set_image(string image_file) { image_name = image_file; }

string ImageBox::get_image() const { return image_name; }

void ImageBox::set_dimensions(int x, int y, int w, int h)
{
    rectangle.x = x;
    rectangle.y = y;
    rectangle.w = w;
    rectangle.h = h;
}

void ImageBox::set_dimensions(const SDL_Rect & rect) { rectangle = rect; }

SDL_Rect ImageBox::get_dimensions() const { return rectangle; }

void ImageBox::Render(SDL_Renderer * renderer, const SDL_Color & background)
{
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_RenderFillRect(renderer, &rectangle);

    SDL_Surface * image_surface = IMG_Load(image_name.c_str());
    SDL_Texture * image_texture = SDL_CreateTextureFromSurface(renderer, image_surface);

    SDL_RenderCopy(renderer, image_texture, NULL, &rectangle);

    SDL_FreeSurface(image_surface);
    SDL_DestroyTexture(image_texture);

    image_surface = NULL;
    image_texture = NULL;
}
