#include "SDL_utils.cpp" 
using namespace std;


enum HeadDirection {
    HEAD_UP, HEAD_DOWN, HEAD_LEFT, HEAD_RIGHT
};

enum Direction {
    UP = 0, DOWN, LEFT, RIGHT
};

enum UserInput {
    NO_INPUT = 0, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

enum CellType {
    CELL_EMPTY = 0, CELL_SNAKE, CELL_CHERRY, CELL_TEEMO
};

enum GameStatus {
    GAME_RUNNING = 1,
    GAME_STOP = 2,
    GAME_WON = 4 | GAME_STOP,
    GAME_LOST = 8 | GAME_STOP,
};

typedef vector<CellType> Row ; 
typedef vector<Row> Board ;  


struct Position
{
    int x;
    int y;

    Position(int x_, int y_) : x(x_), y(y_) {}

    Position move(Direction direction) {
        switch(direction) {
            case UP:
                return Position{x,y-1} ;

            case DOWN:
                return Position{x,y+1} ;

            case LEFT:
                return Position{x-1,y} ;

            case RIGHT:
                return Position{x+1,y} ;
        }
        return Position{x,y} ;
    }

    bool isInsideBox(int left, int top, int width, int height) {
        return x >= left && x < left+width && y >= top && y < top+height;
    }

    bool operator==(Position p) {
        return x == p.x && y == p.y;
    }
};



class PlayGround ;


struct SnakeNode
{
    Position position;
    SnakeNode* next;

    SnakeNode(Position p, SnakeNode* n = nullptr) : position(p), next(n) {}

    static SnakeNode* insertHead(SnakeNode* head, Position p)
    {
        SnakeNode* newHead = new SnakeNode(p, head);
        return newHead;
    }
};



class Snake
{
    HeadDirection headDirection ;
    SnakeNode* head;
    PlayGround* playGround;
    Direction direction;
    queue<UserInput> inputQueue;

    int cherry;
public:
    Snake(PlayGround* playGround);
    ~Snake() ;
    void processUserInput(UserInput input);
    void nextStep();
    bool checkPosition(Position pos);
    SnakeNode* getHead() const { return head; }
    void changeHeadDirection(Direction direction) ;
    Direction changeDirection(UserInput input);
    void changePlayGroundState(CellType type);
    HeadDirection getHeadDirection() const ;
};


class PlayGround
{
    Board board;
    Snake snake;
    GameStatus status;
    int score, teemoSpawn;
    bool playAgain ;
public:
    PlayGround(int width, int height): board(height, Row(width, CELL_EMPTY)), snake(this)
    , status(GAME_RUNNING), score(0), playAgain(true), teemoSpawn(3) 
    {
        addCherry();
    }

    ~PlayGround()  {
        
    }

    void dontPlayAgain()  {
        playAgain = false ;
    }

    bool getPlayAgain() {
        return playAgain ;
    }

    bool isGameRunning() const 
    {
         return status == GAME_RUNNING; 
    }

    void processUserInput(UserInput input)
    {
        snake.processUserInput(input);
    }

    void nextStep() 
    {
        snake.nextStep();
    }

    void changeCellState(Position pos, CellType type) 
    {
        board[pos.y][pos.x] = type;
    }
    
    void addCherry() {
        do {
            Position pos (rand()%width() , rand()%height()) ;
            if (getCellState(pos) == CELL_EMPTY) 
            {
                changeCellState(pos, CELL_CHERRY) ;
                teemoSpawn-- ;
                break;
            }
        } while (true);
    }

    void addTeemo() {
        if (teemoSpawn == 0) {
            do {
                Position pos (rand()%width() , rand()%height()) ;
                if (getCellState(pos) == CELL_EMPTY) 
                {
                    changeCellState(pos, CELL_TEEMO) ;
                    teemoSpawn = 3 ;
                    break;
                }
            } while (true);
        }
    }

    const Board& getBoard() const 
    { 
        return board; 
    }


    CellType getCellState(Position p) const 
    { 
        return board[p.y][p.x]; 
    }

    void setGameStatus(GameStatus status) {
        this->status = status;
    }
    

    vector<Position> getSnakePositions() const 
    {
        vector<Position> res;
        for (SnakeNode* p = snake.getHead(); p != nullptr; p = p->next)
        {
            res.push_back(p->position);
        }
        return res;
    }

    int getScore() const {
        return score ;
    }

    void plusScore() {
        score += 10 ;
    }

    void minusScore() {
        score -= 15 ;
    }

    int width() const 
    { 
        return board[0].size(); 
    }

    int height() const 
    { 
        return board.size(); 
    }

    HeadDirection getHeadDirection() const {
        return snake.getHeadDirection() ;
    }
};





Snake::Snake(PlayGround* playGround): 
    head( new SnakeNode(Position(0, 2)) ), playGround(playGround) , cherry(0) , direction(RIGHT), headDirection(HEAD_RIGHT)
{
    changePlayGroundState(CELL_SNAKE);
}

Snake::~Snake () {

}

void Snake::processUserInput(UserInput input)
{
    inputQueue.push(input);
}

Direction Snake::changeDirection(UserInput input)
{
    switch (input) {
    case KEY_UP:    return direction != DOWN ? UP : direction;
    case KEY_DOWN:  return direction != UP ? DOWN : direction;
    case KEY_LEFT:  return direction != RIGHT ? LEFT : direction;
    case KEY_RIGHT: return direction != LEFT ? RIGHT : direction;
    default:        return direction;
    }
}

HeadDirection Snake:: getHeadDirection() const {
    return headDirection ;
}

void Snake:: changeHeadDirection(Direction direction) {
    switch(direction) {
        case UP :
            headDirection = HEAD_UP;
            break;
        
        case DOWN :
            headDirection = HEAD_DOWN;
            break;
        
        case LEFT :
            headDirection = HEAD_LEFT;
            break;
        
        case RIGHT :
            headDirection = HEAD_RIGHT;
            break;
        
    }
}

void Snake::nextStep()
{
    int eatingSound = loadSound((char*)"D:\\Code_library\\Audio\\EatingSound.wav") ;
    int yuckSound = loadSound((char*)"D:\\Code_library\\Audio\\YUCK.wav") ;

    while (!inputQueue.empty()) {
        UserInput input = inputQueue.front();
        inputQueue.pop();
        Direction newDirection = changeDirection(input);
        if (newDirection != direction) {
            direction = newDirection;
            break;
        }
    }

    changeHeadDirection(direction) ;

    Position newPosition = head->position.move(direction);
    
    if (!checkPosition(newPosition)) {
        playGround->setGameStatus(GAME_LOST);
        return;
    }

    CellType type = playGround->getCellState(newPosition);

    changePlayGroundState(CELL_EMPTY);

    if (cherry > 0) {
        cherry--;
        head = new SnakeNode(newPosition, head);
    } 

    else {
        for (SnakeNode* p = head; p != nullptr; p = p->next) {
            swap(p->position, newPosition);
        }
    }

    changePlayGroundState(CELL_SNAKE);

    if (type == CELL_CHERRY) {
        playSound(eatingSound) ;
        cherry++;
        playGround->plusScore() ;
        playGround->addCherry();
        playGround->addTeemo() ;
    }

    if (type == CELL_TEEMO) {
        playSound(yuckSound) ;
        playGround->minusScore() ;
    }
}

bool Snake::checkPosition(Position pos)
{   
    int gameOverSound = loadSound((char*)"D:\\Code_library\\Audio\\GameOver.wav") ;

    if ( !pos.isInsideBox(0,0,playGround->width(),playGround->height()) ) {
        playSound(gameOverSound) ;
        return false;
    }

    for (SnakeNode* p = head; p->next != nullptr; p = p->next) {
        if (p->position == pos) {
            playSound(gameOverSound) ;
            return false;
        }
    }
    return true;
}

void Snake::changePlayGroundState(CellType type)
{
    for (SnakeNode* p = head; p != nullptr; p = p->next) {
        playGround->changeCellState(p->position, type);
    }
}

