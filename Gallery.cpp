#include <vector>
#include <SDL.h>
#include "painter.cpp"

enum PictureID {
    PIC_CHERRY = 0, PIC_TEEMO,
    PIC_SNAKE_BODY_UP, PIC_SNAKE_BODY_DOWN, PIC_SNAKE_BODY_LEFT, PIC_SNAKE_BODY_RIGHT,
    PIC_SNAKE_HEAD_UP, PIC_SNAKE_HEAD_DOWN, PIC_SNAKE_HEAD_LEFT, PIC_SNAKE_HEAD_RIGHT,
    PIC_SNAKE_TAIL_UP, PIC_SNAKE_TAIL_DOWN, PIC_SNAKE_TAIL_LEFT, PIC_SNAKE_TAIL_RIGHT, 
    PIC_GALAXY, PIC_MENU1, PIC_MENU2,
    YOU_DIED,
    PLAY_AGAIN_RED, PLAY_AGAIN_GREY,
    QUIT_RED, QUIT_GREY
};

class Gallery
{
    std::vector<SDL_Texture*> pictures;
    Painter& painter;
public:
    Gallery(Painter& painter_) : painter(painter_)
    {
        loadGamePictures();
    }

    ~Gallery() 
    {
        for (SDL_Texture* texture : pictures)
            SDL_DestroyTexture(texture);
    }

    void loadGamePictures()
    {
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\cherry.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\Teemo.png")) ;
        
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\body_up.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\body_down.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\body_left.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\body_right.png"));

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\head_up.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\head_down.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\head_left.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\head_right.png"));

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\tail_up.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\tail_down.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\tail_left.png"));
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\tail_right.png"));

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\galaxy.jpg")) ;
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\menu1.jpg")) ;
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\menu2.jpg")) ;

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\you_died.jpg")) ;

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\playagainRed.png")) ;
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\playagainGrey.png")) ;

        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\quitRed.png")) ;
        pictures.push_back(painter.loadTexture("D:\\Code_library\\Gallery\\quitGrey.png")) ;

    }

    SDL_Texture* getImage(PictureID id) const 
    { 
        return pictures[id]; 
    }

};

