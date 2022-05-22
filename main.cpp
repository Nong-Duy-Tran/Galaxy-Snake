#include "PlayGround.cpp"
#include "Gallery.cpp"

using namespace std;

const int BOARD_WIDTH = 24;
const int BOARD_HEIGHT = 14;
const int CELL_SIZE = 50;


double STEP_DELAY = 0.2;
#define CLOCK_NOW chrono::system_clock::now
typedef chrono::duration<double> ElapsedTime;

SDL_Texture* loadTexture (string path, SDL_Renderer* renderer) ;
void renderMenuScreen(SDL_Renderer* renderer, Painter painter);
void renderGamePlay(Painter& painter, const PlayGround& playGround);
void renderGameOver(Painter& Painter,  PlayGround& playGround, bool& playAgain);
UserInput interpretEvent(SDL_Event e);

Gallery* gallery = nullptr; 

int main(int argc, char* argv[])
{
    srand(time(0));
    SDL_Window* window;
    SDL_Renderer* renderer;
    initSDL(window, renderer);
    Painter painter(window, renderer);
    gallery = new Gallery(painter);

    renderMenuScreen(renderer, painter);
    SDL_RenderClear(renderer) ;
    

    SDL_Event e;
    bool playAgain = true ;
    auto start = CLOCK_NOW();
    int musicGamePlay = loadMusic((char*)"D:\\Code_library\\Audio\\You Say Jet Set Run.mp3") ;
    do 
    {   
        //playMusic(musicGamePlay) ;
        setVolume(50) ;
        PlayGround playGround(BOARD_WIDTH, BOARD_HEIGHT);
        while (playGround.isGameRunning()) {
            while (SDL_PollEvent(&e)) {
                UserInput input = interpretEvent(e);
                playGround.processUserInput(input);
            }

            auto now = CLOCK_NOW();
            ElapsedTime elapsed = now-start;
            if (elapsed.count() > STEP_DELAY) {
                playGround.nextStep();
                renderGamePlay(painter, playGround);
                start = now;
            }
            
        }
        painter.clearWithBgColor(GREEN_COLOR) ;
        renderGameOver(painter, playGround, playAgain);

    } while (playAgain) ;

    delete gallery;
    quitSDL(window, renderer);
    return 0;
}



void renderMenuScreen(SDL_Renderer* renderer, Painter painter)
{   
    int ButtonSound = loadSound((char*)"D:\\Code_library\\Audio\\ButtonSound.wav") ;
    int GameClickSound = loadSound((char*)"D:\\Code_library\\Audio\\GameClick.wav") ;
    int menuItemNumber = 2 ;
    SDL_Event e ;
    
	SDL_Rect pos[menuItemNumber] ;

	bool selected[menuItemNumber] = {1, 0} ;

	SDL_Texture* menu1 = gallery->getImage(PIC_MENU1);
	SDL_Texture* menu2 = gallery->getImage(PIC_MENU2);
	painter.createImage(menu1) ;
	

	while (true) {
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				exit(0);
			}

			if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				SDL_RenderClear(painter.getRenderer()) ;
                playSound(ButtonSound) ;
				painter.createImage(menu2) ;
				selected[1] = true;
				selected[0] = false ;
			}

			if (e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				SDL_RenderClear(painter.getRenderer()) ;
                playSound(ButtonSound) ;
				painter.createImage(menu1) ;
				selected[0] = true;
				selected[1] = false ;
			}


			if ((e.key.keysym.scancode == SDL_SCANCODE_SPACE || e.key.keysym.scancode == SDL_SCANCODE_RETURN) && selected[0] == true) {
				playSound(GameClickSound) ;
                SDL_Delay(100) ;
                return;
			}

			if ((e.key.keysym.scancode == SDL_SCANCODE_SPACE || e.key.keysym.scancode == SDL_SCANCODE_RETURN) && selected[1] == true) {
				exit(0) ;
			}
		}

	}
}

void drawSnake(Painter& painter, int left, int top, vector<Position> pos, const PlayGround& playGround)
{
    HeadDirection headDirection = playGround.getHeadDirection() ;
    
    for (int i = 0; i < pos.size(); i++) {
        SDL_Rect dst = { left+pos[i].x*CELL_SIZE+1, top+pos[i].y*CELL_SIZE+1, CELL_SIZE-2, CELL_SIZE-2 };
        SDL_Texture* texture = NULL;
        if (i > 0 && i < pos.size() - 1) {
            if (pos[i].y == pos[i-1].y) {
                texture = gallery->getImage(PIC_SNAKE_BODY_LEFT); 
            }
            else {
                texture = gallery->getImage(PIC_SNAKE_BODY_UP);
            }
        } 

        else if (i == 0) { 
            switch (headDirection)
            {
                case HEAD_UP:
                    texture = gallery->getImage(PIC_SNAKE_HEAD_UP);
                    break;

                case HEAD_DOWN:
                    texture = gallery->getImage(PIC_SNAKE_HEAD_DOWN);
                    break;
                
                case HEAD_LEFT:
                    texture = gallery->getImage(PIC_SNAKE_HEAD_LEFT);
                    break;
                
                case HEAD_RIGHT:
                    texture = gallery->getImage(PIC_SNAKE_HEAD_RIGHT);
                    break;
            }
        }
        
        else {
            if (pos[i].y == pos[i-1].y) {
                switch (headDirection) {
                    case HEAD_LEFT:
                        texture = gallery->getImage(PIC_SNAKE_TAIL_LEFT); 
                        break;
                    case HEAD_RIGHT:
                        texture = gallery->getImage(PIC_SNAKE_TAIL_RIGHT); 
                        break;
                }

            }

            else {
                switch (headDirection) {
                    case HEAD_UP:
                        texture = gallery->getImage(PIC_SNAKE_TAIL_UP); 
                        break;
                    case HEAD_DOWN:
                        texture = gallery->getImage(PIC_SNAKE_TAIL_DOWN); 
                        break;
                }
            }
        }

        painter.createImage(texture, NULL, &dst);
    }
}

void drawCherry(Painter& painter, int left, int top)
{
    SDL_Rect dst = { left+5, top+5, CELL_SIZE-10, CELL_SIZE-10 };
    painter.createImage(gallery->getImage(PIC_CHERRY), NULL, &dst);
}

void drawTeemo(Painter& painter, int left, int top) {
    SDL_Rect dst = { left+5, top+5, CELL_SIZE-10, CELL_SIZE-10 };
    painter.createImage(gallery->getImage(PIC_TEEMO), NULL, &dst);
}

void drawVerticalLine(Painter& painter, int left, int top, int cells)
{
    painter.setColor(BLACK_COLOR);
    painter.setAngle(-90);
    painter.setPosition(left, top);
    painter.moveForward(cells * CELL_SIZE);
}

void drawHorizontalLine(Painter& painter, int left, int top, int cells)
{
    painter.setColor(BLACK_COLOR);
    painter.setAngle(0);
    painter.setPosition(left, top);
    painter.moveForward(cells * CELL_SIZE);
}

void renderGamePlay(Painter& painter, const PlayGround& playGround)
{
    int top = 0, left = 0;
    int width = playGround.width();
    int height = playGround.height();

    SDL_Texture* backGround = gallery->getImage(PIC_GALAXY) ;
    painter.createImage(backGround) ;

    for (int i = 0; i <= width; i++)
        drawVerticalLine(painter, left+i*CELL_SIZE, top+0, height);

    for (int i = 0; i <= height; i++)
        drawHorizontalLine(painter, left+0, top+i * CELL_SIZE, width);

    const Board& board = playGround.getBoard();
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (board[i][j] == CELL_TEEMO)
                drawTeemo(painter, left+j*CELL_SIZE, top+i*CELL_SIZE);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
            if (board[i][j] == CELL_CHERRY)
                drawCherry(painter, left+j*CELL_SIZE, top+i*CELL_SIZE);


    vector<Position> snakePositions = playGround.getSnakePositions();
    drawSnake(painter, left, top, snakePositions, playGround);

    SDL_RenderPresent(painter.getRenderer());
}

void renderFinishedGameScreen(SDL_Rect& playAgainRect, SDL_Rect& quitRect, SDL_Rect& scoreRect) {
	playAgainRect.w = 340;
	playAgainRect.h = 40;
	playAgainRect.x = 200;
	playAgainRect.y = 550; 
	
    quitRect.w = 340;
	quitRect.h = 40;
	quitRect.x = 700 ;
	quitRect.y = 550 ;
	
    scoreRect.w = 200;
	scoreRect.h = 60;
	scoreRect.x = (SCREEN_WIDTH / 2) - (scoreRect.w / 2);
	scoreRect.y = 0;
}

void renderGameOver(Painter& painter, PlayGround& playGround, bool& playAgain)
{   
    int ButtonSound = loadSound((char*)"D:\\Code_library\\Audio\\ButtonSound.wav") ;
    SDL_Event event ;
    string path = "arial.ttf" ;
	TTF_Font* font = TTF_OpenFont((char*)"D:\\Code_library\\Font\\arial.ttf", 10);
	
    if (font == NULL) 
	{
		cout << "Font loading error" << endl;
		
	}

    bool selected[2] = {0, 0} ;

    SDL_Surface* score = TTF_RenderText_Solid(font, (string("Score: ") + to_string(playGround.getScore())).c_str(),RED_COLOR);
    SDL_Texture* scoreMessage = SDL_CreateTextureFromSurface(painter.getRenderer(), score);

	SDL_Rect playAgainRect, quitRect, scoreRect;

	renderFinishedGameScreen(playAgainRect, quitRect, scoreRect) ;

    painter.createImage(gallery->getImage(YOU_DIED)) ;
    painter.createImage(gallery->getImage(PLAY_AGAIN_GREY), NULL, &playAgainRect) ;
    painter.createImage(gallery->getImage(QUIT_GREY), NULL, &quitRect) ;
    painter.createImage(scoreMessage, NULL, &scoreRect) ;
    

	TTF_CloseFont(font);
    bool running = true ;
	while (running) {
		SDL_RenderPresent(painter.getRenderer());

		if (SDL_PollEvent(&event)) {

            if (event.type == SDL_QUIT) {
                exit(0) ;
            }

			if (event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                SDL_RenderClear(painter.getRenderer()) ;
                playSound(ButtonSound) ;
                painter.createImage(gallery->getImage(YOU_DIED)) ;
                painter.createImage(gallery->getImage(PLAY_AGAIN_RED), NULL, &playAgainRect) ;
                painter.createImage(gallery->getImage(QUIT_GREY), NULL, &quitRect) ;
                painter.createImage(scoreMessage, NULL, &scoreRect) ;
                selected[0] = true ;
                selected[1] = false ;
			}

            if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                SDL_RenderClear(painter.getRenderer()) ;
                playSound(ButtonSound) ;
                painter.createImage(gallery->getImage(YOU_DIED)) ;
                painter.createImage(gallery->getImage(PLAY_AGAIN_GREY), NULL, &playAgainRect) ;
                painter.createImage(gallery->getImage(QUIT_RED), NULL, &quitRect) ;
                painter.createImage(scoreMessage, NULL, &scoreRect) ;
                selected[1] = true ;
                selected[0] = false ;
			}

			if ((event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_RETURN) && selected[0] == true) {
				playAgain = true ;
                running = false ;
			}

            if ((event.key.keysym.scancode == SDL_SCANCODE_SPACE || event.key.keysym.scancode == SDL_SCANCODE_RETURN) && selected[1] == true) {
                playAgain = false ;
                running = false ;
            }

		}

	}

}

UserInput interpretEvent(SDL_Event e)
{
    if (e.type == SDL_KEYUP) {
        switch (e.key.keysym.scancode)
	    {
            case SDL_SCANCODE_UP:
                return KEY_UP ;
                break;
            
            case SDL_SCANCODE_DOWN:
                return KEY_DOWN ;
                break;
            
            case SDL_SCANCODE_LEFT:
                return KEY_LEFT ;
                break;

            case SDL_SCANCODE_RIGHT:
                return KEY_RIGHT ;
                break;

	    }
    }
    if (e.type == SDL_QUIT)  exit(0) ;
    
    return NO_INPUT;
}


SDL_Texture* loadTexture (string path, SDL_Renderer* renderer) {
        SDL_Texture* newTexture = NULL ;
        SDL_Surface* loadedSurface = IMG_Load(path.c_str()) ;
        if (loadedSurface == NULL) 
        {
            cout << " Unable to load image " << path << " SDL_image Error: " ;
        }
        else 
        {
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface) ;
            if (newTexture == NULL) 
            {
                cout << "Unable to create texture from " << path << " SDL_Error: " << SDL_GetError() << endl;             
            }
            SDL_FreeSurface(loadedSurface) ;
        }
        return newTexture ;
}