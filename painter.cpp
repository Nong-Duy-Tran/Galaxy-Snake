#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;


const SDL_Color CYAN_COLOR = {0, 255, 255};
const SDL_Color BLUE_COLOR = {0, 0, 255};
const SDL_Color ORANGE_COLOR = {255, 165, 0};
const SDL_Color YELLOW_COLOR = {255, 255, 0};
const SDL_Color LIME_COLOR = {0, 255, 0};
const SDL_Color PURPLE_COLOR = {128, 0, 128};
const SDL_Color RED_COLOR = {255, 0, 0};
const SDL_Color WHITE_COLOR = {255, 255, 255};
const SDL_Color BLACK_COLOR = {0, 0, 0};
const SDL_Color GREEN_COLOR = {0, 128, 0};
const SDL_Color GREY_COLOR = {105, 105, 105} ;

const SDL_Color DEFAULT_COLOR = BLACK_COLOR;

class Painter
{
    float x;
    float y;
    float angle;
    int width;
    int height;
    SDL_Color color;
    SDL_Renderer* renderer;
public:
    Painter(SDL_Window* window, SDL_Renderer* renderer) : renderer(renderer)
    {
        setPosition(width/2, height/2);
        setAngle(0);
        setColor(WHITE_COLOR);
        clearWithBgColor(BLACK_COLOR);
    }

    void setPosition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    
    float getX() const { 
        return x; 
    }

    float getY() const { 
        return y; 
    }

    void setAngle(float angle)
    {
        this->angle = angle - floor(angle/360)*360;
    }

    float getAngle() const { 
        return angle; 
    }

    int getWidth() const { 
        return width; 
    }

    int getHeight() const { 
        return height; 
    }

    void setColor(SDL_Color color)
    {
        this->color = color;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
    }

    SDL_Color getColor() const { 
        return color; 
    }

    SDL_Renderer* getRenderer() const { 
        return renderer; 
    }

    void clearWithBgColor(SDL_Color bgColor)
    {
        SDL_Color curColor = color;
        setColor(bgColor);
        SDL_RenderClear(renderer);
        setColor(curColor);
    }

    void moveForward(float length)
    {
        float prevX = x, prevY = y;
        jumpForward(length);
        SDL_RenderDrawLine(renderer, (int)prevX, (int)prevY, (int)x, (int)y);
    }

    void jumpForward(float length)
    {
        float rad = (angle / 180) * M_PI;
        x += (cos(rad) * length);
        y -= (sin(rad) * length);
    }

    void moveBackward(float length)
    {
        moveForward(-length);
    }

    void jumpBackward(float length)
    {
        jumpForward(-length);
    }

    void turnLeft(float angle)
    {
        setAngle(this->angle+angle);
    }

    void turnRight(float angle)
    {
        turnLeft(-angle);
    }

    void setRandomColor()
    {
        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        SDL_Color color = {r, g, b};
        setColor(color);
    }

    void createCircle(float radius)
    {
        double rad = (angle / 180) * M_PI;
        int centerX = x + cos(rad) * radius;
        int centerY = y - sin(rad) * radius;

        int dx = radius;
        int dy = 0;
        int err = 0;

        while (dx >= dy)
        {
            SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            SDL_RenderDrawPoint(renderer, centerX + dy, centerY + dx);
            SDL_RenderDrawPoint(renderer, centerX - dy, centerY + dx);
            SDL_RenderDrawPoint(renderer, centerX - dx, centerY + dy);
            SDL_RenderDrawPoint(renderer, centerX - dx, centerY - dy);
            SDL_RenderDrawPoint(renderer, centerX - dy, centerY - dx);
            SDL_RenderDrawPoint(renderer, centerX + dy, centerY - dx);
            SDL_RenderDrawPoint(renderer, centerX + dx, centerY - dy);

            if (err <= 0)
            {
                dy += 1;
                err += 2*dy + 1;
            }
            if (err > 0)
            {
                dx -= 1;
                err -= 2*dx + 1;
            }
        }
    }

    void createSquare(float size)
    {
        for (int i = 0; i < 4; ++i) {
            moveForward(size);
            turnLeft(90);
        }
    }

    void createParallelogram(float size)
    {
        for (int i = 0; i < 2; ++i) {
            moveForward(size);
            turnLeft(60);
            moveForward(size);
            turnLeft(120);
        }
    }

    SDL_Texture* loadTexture( string path )
    {
        SDL_Texture* newTexture = NULL;
        SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
        if ( loadedSurface == NULL )
            cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
        else {
            newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
            if( newTexture == NULL )
                cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
            SDL_FreeSurface( loadedSurface );
        }
        return newTexture;
    }


    bool createImage( SDL_Texture* texture, SDL_Rect* srcrect = nullptr, SDL_Rect* dstrect = nullptr )
    {
        if( texture == NULL ) return false;
        SDL_RenderCopy( renderer, texture, srcrect, dstrect );
        return true;
    }

};











