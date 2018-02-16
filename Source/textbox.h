#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <string>

using namespace std;

extern const SDL_Color Black;
extern const SDL_Color White;
extern const SDL_Color Red;
extern const SDL_Color Blue;
extern const SDL_Color Green;
extern const SDL_Color Orange;
extern const SDL_Color Yellow;
extern const SDL_Color Purple;
extern const SDL_Color Pink;
extern const SDL_Color Gray;


class TextBox
{
public:

    TextBox();

    TextBox(string optionName, int x, int y, int w, int h, string font, const SDL_Color & regular, const SDL_Color & highlight);

    bool Detection();

    void Render(SDL_Renderer * renderer, const SDL_Color & background);

    void set_name(string new_name);

    string get_name() const;

    void set_dimensions(int x, int y, int w, int h);

    void set_dimensions(const SDL_Rect & new_rect);

    SDL_Rect get_dimensions() const;

    void set_font(string new_font);

    string get_font() const;

    void set_regular(const SDL_Color & new_regular);

    SDL_Color get_regular() const;

    void set_highlight(const SDL_Color & new_highlight);

    SDL_Color get_highlight() const;

    void set_border(const SDL_Color & borderline);

    SDL_Color get_border() const;

    void disable_border();

    void set_bold();

    void disable_bold();

private:

    SDL_Rect rectangle;
    string name;
    string text_font;
    SDL_Color Regular;
    SDL_Color Highlight;
    bool Border;
    SDL_Color Borderline;
    bool Bold;
};



class ImageBox
{
public:

    ImageBox(string image_file, const SDL_Rect & rect);

    ImageBox(string image_file, int x, int y, int w, int h);

    void set_image(string image_file);

    string get_image() const;

    void set_dimensions(int x, int y, int w, int h);

    void set_dimensions(const SDL_Rect & rect);

    SDL_Rect get_dimensions() const;

    void Render(SDL_Renderer * renderer, const SDL_Color & background);

private:

    string image_name;
    SDL_Rect rectangle;
};
#endif // __TEXTBOX_H__
