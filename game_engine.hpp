/*******************************************************************************
 Name:      William Santagata
 Course:    Artificial Intelligence I
 Section:   Graduate
 Semester:  Spring 2018
 Program:   AI Checkers
 *******************************************************************************/

// GAME_ENGINE.HPP //

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <tuple>
using namespace std;

const int BOARD_SIZE = 6;
enum COLOR{RED, BLACK, EMPTY};
enum DIRECTION{LEFT, RIGHT, BOTH, NONE};
enum NEIGHBOR{UL, UR, LL, LR};


// CLASS DECLARATIONS //
class Checker;
class Square;
class Board;


// CLASS DEFINITIONS //
class Checker{
public:
    // ~~ Constructors ~~ //
    Checker(COLOR Color, Square* Position);
    
    // ~~ Destructor ~~ //
    ~Checker();
    
    // ~~ Accessors ~~ //
    COLOR get_color();
    Square* get_position();
    DIRECTION get_jumps();
    DIRECTION get_moves();
    
    // ~~ Mutators ~~ //
    void set_color(COLOR Color);
    void set_position(Square* Position);
    void set_jumps(DIRECTION CanJump);
    void set_moves(DIRECTION CanMove);
    
    // ~~ Member Variables ~~ //
private:
    COLOR color;
    Square* position;
    DIRECTION jumps;
    DIRECTION moves;
};

class Square{
public:
    // ~~ Constructors ~~ //
    Square();
    Square(COLOR Color, int X, int Y);
    
    // ~~ Destructor ~~ //
    ~Square();
    
    // ~~ Accessors ~~ //
    COLOR get_color();
    Checker* get_occupant();
    int get_x_coordinate();
    int get_y_coordinate();
    Square* get_neighbor(NEIGHBOR n);
    
    // ~~ Mutators ~~ //
    void set_color(COLOR Color);
    void set_occupant(Checker* Occupant);
    void set_coordinates(int X, int Y);
    void set_neighbors(Square* UL, Square* UR, Square* LL, Square* LR);
    
    // ~~ Member Variables ~~ //
private:
    COLOR color;
    Checker* occupant;
    int x_coordinate, y_coordinate;
    Square *neighbors[4];
};

class Board{
public:
    // ~~ Constructor ~~ //
    Board();
    
    // ~~ Destructor ~~ //
    ~Board();
    
    // ~~ Accessors ~~ //
    int get_numRed();
    int get_numBlack();
    COLOR get_whoseTurn();
    bool get_jumpPossible();
    bool get_movePossible();
    bool get_gameEnd();
    Square get_square(int x, int y);
    
    // ~~ Mutators ~~ //
    void set_numRed(int NumRed);
    void set_numBlack(int NumBlack);
    void set_whoseTurn(COLOR Player);
    void set_jumpPossible(bool JumpPossible);
    void set_movePossible(bool MovePossible);
    void set_winner(COLOR Winner);
    
    // ~~ Member Functions ~~ //
    void initializeBoard();
    void printBoard();
    void countPieces();
    void resetCanJumpMove();
    void determineJumpMovePossible(); // also sets canJump & canMove variables of Checker objects
    
    bool validMove(int x, int y, DIRECTION dir);
    void move(int x, int y, DIRECTION dir);
    void changeMove();
    void inputMove(int &X, int &Y, DIRECTION &dir);
    void printWinner();
    void generate_PossibleMoves();
    
    vector<tuple<int, int, DIRECTION>> possible_moves;
    
    // ~~ Member Variables ~~ //
    Square checkerboard[BOARD_SIZE+2][BOARD_SIZE+2];
    int numRed, numBlack;
    COLOR whoseTurn;
    bool jumpPossible;
    bool movePossible;
    bool gameEnd;
    COLOR winner;
    int noMovesCounter;
};


// GLOBAL FUNCTIONS //
void printBanner();

void alpha_beta_search(Board state, int& X, int& Y, DIRECTION& Dir);
int max_value(Board state, int& alpha, int& beta);
int min_value(Board state, int& alpha, int& beta);
int utility(Board state);


///////////////////////////////////////

// FUNCTION DEFINITIONS //

///// Checker ///////

// ~~ Constructors ~~ //
Checker::Checker(COLOR Color, Square* Position){
    color = Color;
    position = Position;
    jumps = NONE;
    moves = NONE;
}

// ~~ Destructor ~~ //
Checker::~Checker(){}

// ~~ Accessors ~~ //
COLOR Checker::get_color(){return color;}
Square* Checker::get_position(){return position;}
DIRECTION Checker::get_jumps(){return jumps;}
DIRECTION Checker::get_moves(){return moves;}

// ~~ Mutators ~~ //
void Checker::set_color(COLOR Color){color = Color; return;}
void Checker::set_position(Square* Position){position = Position; return;}
void Checker::set_jumps(DIRECTION Jumps){jumps = Jumps; return;}
void Checker::set_moves(DIRECTION Moves){moves = Moves; return;}


///// Square ///////

// ~~ Constructors ~~ //
Square::Square(){
    color = RED;
    x_coordinate = 0;
    y_coordinate = 0;
    occupant = nullptr;
    neighbors[0] = nullptr;
    neighbors[1] = nullptr;
    neighbors[2] = nullptr;
    neighbors[3] = nullptr;
}

Square::Square(COLOR Color, int X, int Y){
    color = Color;
    x_coordinate = X;
    y_coordinate = Y;
    occupant = nullptr;
    
    neighbors[0] = nullptr;
    neighbors[1] = nullptr;
    neighbors[2] = nullptr;
    neighbors[3] = nullptr;
}

// ~~ Destructor ~~ //
Square::~Square(){}

// ~~ Accessors ~~ //
COLOR Square::get_color(){return color;}
Checker* Square::get_occupant(){return occupant;}
int Square::get_x_coordinate(){return x_coordinate;}
int Square::get_y_coordinate(){return y_coordinate;}
Square* Square::get_neighbor(NEIGHBOR n){return neighbors[n];}

// ~~ Mutators ~~ //
void Square::set_color(COLOR Color){color = Color; return;}
void Square::set_occupant(Checker* Occupant){occupant = Occupant; return;}
void Square::set_coordinates(int X, int Y){x_coordinate = X; y_coordinate = Y; return;}

void Square::set_neighbors(Square *UL, Square *UR, Square *LL, Square *LR){
    neighbors[0] = UL;
    neighbors[1] = UR;
    neighbors[2] = LL;
    neighbors[3] = LR;
    return;
}


///// Board ///////

// ~~ Constructor ~~ //
Board::Board(){
    numRed = 0;
    numBlack = 0;
    whoseTurn = BLACK;
    gameEnd = false;
    winner = EMPTY;
    noMovesCounter = 0;
    initializeBoard();
}

// ~~ Destructor ~~ //
Board::~Board(){}

// ~~ Accessors ~~ //
int Board::get_numRed(){return numRed;}
int Board::get_numBlack(){return numBlack;}
COLOR Board::get_whoseTurn(){return whoseTurn;}
bool Board::get_jumpPossible(){return jumpPossible;}
bool Board::get_movePossible(){return movePossible;}
bool Board::get_gameEnd(){return gameEnd;}
Square Board::get_square(int x, int y){return checkerboard[x][y];}

// ~~ Mutators ~~ //
void Board::set_numRed(int NumRed){numRed = NumRed; return;}
void Board::set_numBlack(int NumBlack){numBlack = NumBlack; return;}
void Board::set_whoseTurn(COLOR Player){whoseTurn = Player; return;}
void Board::set_jumpPossible(bool JumpPossible){jumpPossible = JumpPossible; return;}
void Board::set_movePossible(bool MovePossible){movePossible = MovePossible; return;}
void Board::set_winner(COLOR Winner){winner = Winner; return;}

// ~~ Member Functions ~~ //
void Board::printBoard(){
    
    int row(1);
    
    cout << endl << endl;
    
    cout << "                                                                       Black: " << get_numBlack();
    if(get_numBlack() > get_numRed()) cout << " *";
    cout << endl;
    cout << "                                                                       Red:   " << get_numRed();
    if(get_numRed() > get_numBlack()) cout << " *";
    cout << endl;
    
    for(char i = 65; i < 65+BOARD_SIZE; i++)
        cout << "     " << i << "     ";
    cout << endl;
    
    for(int j = 1; j < BOARD_SIZE+1; j++){
        
        // Print Row 1 of chars //
        for(int i = 1; i < BOARD_SIZE+1; i++)
            cout << "*---------*";
        cout << endl;
        
        // Print Row 2 of chars //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            if(checkerboard[i][j].get_color()==RED)
                cout << "|*********|";
            else
                cout << "|         |";
        }
        cout << endl;
        
        
        // Print Row 3 of chars //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            if(checkerboard[i][j].get_color()==RED)
                cout << "|*********|";
            else if(checkerboard[i][j].get_color()==BLACK && checkerboard[i][j].get_occupant() != nullptr){
                if(checkerboard[i][j].get_occupant()->get_color()==RED)
                    cout << "|   (R)   |";
                if(checkerboard[i][j].get_occupant()->get_color()==BLACK)
                    cout << "|   (B)   |";
            }
            else
                cout << "|         |";
        }
        
        
        // Print Row Numbers //
        cout << "  " << row;
        cout << endl;
        
        // Print Row 4 of chars //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            if(checkerboard[i][j].get_color()==RED)
                cout << "|*********|";
            else
                cout << "|         |";
        }
        cout << endl;
        
        // Print Row 5 of chars //
        for(int i = 1; i < BOARD_SIZE+1; i++)
            cout << "*---------*";
        cout << endl;
        
        row++;
    }
    cout << endl << endl;
    
    return;
}

void Board::initializeBoard(){
    
    for(int i = 0; i < BOARD_SIZE+2; i++){
        for(int j = 0; j < BOARD_SIZE+2; j++)
            checkerboard[i][j].set_coordinates(i,j);
    }
    
    for(int i = 0; i < BOARD_SIZE+2; i++){
        checkerboard[i][0].set_color(EMPTY); checkerboard[i][0].set_occupant(new Checker(EMPTY, &checkerboard[i][0]));}
    for(int i = 0; i < BOARD_SIZE+2; i++){
        checkerboard[i][BOARD_SIZE+1].set_color(EMPTY); checkerboard[i][BOARD_SIZE+1].set_occupant(new Checker(EMPTY, &checkerboard[i][BOARD_SIZE+1]));}
    for(int i = 0; i < BOARD_SIZE+2; i++){
        checkerboard[0][i].set_color(EMPTY); checkerboard[0][i].set_occupant(new Checker(EMPTY, &checkerboard[0][i]));}
    for(int i = 0; i < BOARD_SIZE+2; i++){
        checkerboard[BOARD_SIZE+1][i].set_color(EMPTY); checkerboard[BOARD_SIZE+1][i].set_occupant(new Checker(EMPTY, &checkerboard[BOARD_SIZE+1][i]));}
    
    for(int i = 1; i < BOARD_SIZE+1; i++){
        for(int j = 1; j < BOARD_SIZE+1; j++){
            if( (i % 2 == 0 && j % 2 == 0) || (i % 2 !=0 && j % 2 != 0) )
                checkerboard[i][j].set_color(RED);
            else
                checkerboard[i][j].set_color(BLACK);
        }
    }
    
    checkerboard[1][2].set_occupant(new Checker(RED, &checkerboard[1][2]));
    checkerboard[2][1].set_occupant(new Checker(RED, &checkerboard[2][1]));
    checkerboard[3][2].set_occupant(new Checker(RED, &checkerboard[3][2]));
    checkerboard[4][1].set_occupant(new Checker(RED, &checkerboard[4][1]));
    checkerboard[5][2].set_occupant(new Checker(RED, &checkerboard[5][2]));
    checkerboard[6][1].set_occupant(new Checker(RED, &checkerboard[6][1]));
    
    checkerboard[1][6].set_occupant(new Checker(BLACK, &checkerboard[1][6]));
    checkerboard[2][5].set_occupant(new Checker(BLACK, &checkerboard[2][5]));
    checkerboard[3][6].set_occupant(new Checker(BLACK, &checkerboard[3][6]));
    checkerboard[5][6].set_occupant(new Checker(BLACK, &checkerboard[5][6]));
    checkerboard[4][5].set_occupant(new Checker(BLACK, &checkerboard[4][5]));
    checkerboard[6][5].set_occupant(new Checker(BLACK, &checkerboard[6][5]));
    
    checkerboard[1][2].set_neighbors(&checkerboard[1][0], &checkerboard[2][1], &checkerboard[3][0], &checkerboard[2][3]);
    checkerboard[1][4].set_neighbors(&checkerboard[0][3], &checkerboard[2][3], &checkerboard[0][5], &checkerboard[2][5]);
    checkerboard[1][6].set_neighbors(&checkerboard[5][0], &checkerboard[2][5], &checkerboard[7][0], &checkerboard[7][2]);
    checkerboard[2][1].set_neighbors(&checkerboard[1][0], &checkerboard[3][0], &checkerboard[1][2], &checkerboard[3][2]);
    checkerboard[2][3].set_neighbors(&checkerboard[1][2], &checkerboard[3][2], &checkerboard[1][4], &checkerboard[3][4]);
    checkerboard[2][5].set_neighbors(&checkerboard[1][4], &checkerboard[3][4], &checkerboard[1][6], &checkerboard[3][6]);
    checkerboard[3][2].set_neighbors(&checkerboard[2][1], &checkerboard[4][1], &checkerboard[2][3], &checkerboard[4][3]);
    checkerboard[3][4].set_neighbors(&checkerboard[2][3], &checkerboard[4][3], &checkerboard[2][5], &checkerboard[4][5]);
    checkerboard[3][6].set_neighbors(&checkerboard[2][5], &checkerboard[4][5], &checkerboard[7][2], &checkerboard[7][4]);
    checkerboard[4][1].set_neighbors(&checkerboard[3][0], &checkerboard[5][0], &checkerboard[3][2], &checkerboard[5][2]);
    checkerboard[4][3].set_neighbors(&checkerboard[3][2], &checkerboard[5][2], &checkerboard[3][4], &checkerboard[5][4]);
    checkerboard[4][5].set_neighbors(&checkerboard[3][4], &checkerboard[5][4], &checkerboard[3][6], &checkerboard[5][6]);
    checkerboard[5][2].set_neighbors(&checkerboard[4][1], &checkerboard[6][1], &checkerboard[4][3], &checkerboard[6][3]);
    checkerboard[5][4].set_neighbors(&checkerboard[4][3], &checkerboard[6][3], &checkerboard[4][5], &checkerboard[6][5]);
    checkerboard[5][6].set_neighbors(&checkerboard[4][5], &checkerboard[6][5], &checkerboard[7][4], &checkerboard[7][6]);
    checkerboard[6][1].set_neighbors(&checkerboard[5][0], &checkerboard[7][0], &checkerboard[5][2], &checkerboard[7][2]);
    checkerboard[6][3].set_neighbors(&checkerboard[5][2], &checkerboard[7][2], &checkerboard[5][4], &checkerboard[7][4]);
    checkerboard[6][5].set_neighbors(&checkerboard[5][4], &checkerboard[7][4], &checkerboard[5][6], &checkerboard[7][6]);
    
    set_whoseTurn(BLACK);
    countPieces();
    determineJumpMovePossible();
    
    return;
}

void Board::inputMove(int &X, int &Y, DIRECTION &dir){
    char X_entry(',');
    int Y_entry(0);
    string direction("");
    bool moveValid(false);
    
    if(get_whoseTurn() == BLACK)
        cout << "It's BLACK's turn to move." << endl;
    else
        cout << "It's RED's turn to move." << endl;
    
    determineJumpMovePossible();
    
    if(jumpPossible == false && movePossible == false){
        changeMove();
        return;
    }
    
    while(moveValid == false){
        
        cout << "Column: "; cin >> X_entry;
        while(X_entry < 'A' || X_entry > 'F'){
            cout << "Error. Please input a capital letter between A and F: "; cin >> X_entry;}
        
        switch(X_entry){
            case 'A' : X = 1; break;
            case 'B' : X = 2; break;
            case 'C' : X = 3; break;
            case 'D' : X = 4; break;
            case 'E' : X = 5; break;
            case 'F' : X = 6; break;
        }
        
        cout << "Row: "; cin >> Y;
        while(Y_entry < 0 || Y_entry > 6){
            cout << "Error. Please input a number between 1 and 6: "; cin >> Y_entry;}
        
        cout << "Direction: "; cin >> direction;
        while(direction != "LEFT" && direction != "left" && direction != "RIGHT" && direction != "right"){
            cout << "Error. Please input a direction LEFT or RIGHT: "; cin >> direction;}
        
        if(direction == "l" || direction == "L" || direction == "left" || direction == "LEFT")
            dir = LEFT;
        
        if(direction == "r" || direction == "R" || direction == "right" || direction == "RIGHT")
            dir = RIGHT;
        
        moveValid = validMove(X, Y, dir);
        if(moveValid == false)
            cout << endl << "Invalid move. Try again." << endl;
    }
    
    move(X, Y, dir);
    
    return;
}

bool Board::validMove(int x, int y, DIRECTION dir){
    vector<Checker*> possibleJumpers;
    vector<Checker*> possibleMovers;
    bool validMove(false);
    
    determineJumpMovePossible();
    
    if(jumpPossible == true){
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                if(checkerboard[i][j].get_occupant() != nullptr){
                    if(checkerboard[i][j].get_occupant()->get_color() == get_whoseTurn() &&
                       checkerboard[i][j].get_occupant()->get_jumps() != NONE)
                        possibleJumpers.push_back(checkerboard[i][j].get_occupant());
                }
            }
        }
    }
    
    if(jumpPossible == false){
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                if(checkerboard[i][j].get_occupant() != nullptr){
                    if(checkerboard[i][j].get_occupant()->get_color() == get_whoseTurn() &&
                       checkerboard[i][j].get_occupant()->get_moves() != NONE)
                        possibleMovers.push_back(checkerboard[i][j].get_occupant());
                }
            }
        }
    }
    
    if(jumpPossible == true){
        for(int i = 0; i < possibleJumpers.size(); i++){
            if(possibleJumpers[i]->get_position()->get_x_coordinate() == x &&
               possibleJumpers[i]->get_position()->get_y_coordinate() == y &&
               (possibleJumpers[i]->get_jumps() == dir || possibleJumpers[i]->get_jumps() == BOTH))
                validMove = true;
        }
        return validMove;
    }
    
    if(jumpPossible == false){
        for(int i = 0; i < possibleMovers.size(); i++){
            if(possibleMovers[i]->get_position()->get_x_coordinate() == x &&
               possibleMovers[i]->get_position()->get_y_coordinate() == y &&
               (possibleMovers[i]->get_moves() == dir || possibleMovers[i]->get_moves() == BOTH))
                validMove = true;
        }
        return validMove;
    }
    
    return validMove;
}

void Board::determineJumpMovePossible(){
    jumpPossible = false;
    movePossible = false;
    
    // RED'S TURN //
    if(get_whoseTurn() == RED){
        
        // JUMPS //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                
                // left jump //
                if(checkerboard[i][j].get_occupant() != nullptr &&
                   checkerboard[i][j].get_neighbor(LL)->get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(LL)->get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(LL)->get_neighbor(LL)->get_occupant() == nullptr){
                        
                        checkerboard[i][j].get_occupant()->set_jumps(LEFT);
                        jumpPossible = true;
                    }
                }
                
                // right jump //
                if(checkerboard[i][j].get_occupant() != nullptr &&
                   checkerboard[i][j].get_neighbor(LR)->get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(LR)->get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(LR)->get_neighbor(LR)->get_occupant() == nullptr){
                        
                        if(checkerboard[i][j].get_occupant()->get_jumps() == LEFT)
                            checkerboard[i][j].get_occupant()->set_jumps(BOTH);
                        else
                            checkerboard[i][j].get_occupant()->set_jumps(RIGHT);
                        jumpPossible = true;
                    }
                }
            }
        }
        if(jumpPossible == true)
            return;
        
        // MOVES //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                
                // left move //
                if(checkerboard[i][j].get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(LL)->get_occupant() == nullptr){
                        
                        checkerboard[i][j].get_occupant()->set_moves(LEFT);
                        movePossible = true;
                    }
                }
                
                // right move //
                if(checkerboard[i][j].get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(LR)->get_occupant() == nullptr){
                        
                        if(checkerboard[i][j].get_occupant()->get_moves() == LEFT)
                            checkerboard[i][j].get_occupant()->set_moves(BOTH);
                        else
                            checkerboard[i][j].get_occupant()->set_moves(RIGHT);
                        movePossible = true;
                    }
                }
            }
        }
        
        return; // end of evaluating player RED //
    }
    
    // BLACK'S TURN //
    if(get_whoseTurn() == BLACK){
        
        // JUMPS //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                
                // left jump //
                if(checkerboard[i][j].get_occupant() != nullptr &&
                   checkerboard[i][j].get_neighbor(UL)->get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(UL)->get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(UL)->get_neighbor(UL)->get_occupant() == nullptr){
                        
                        checkerboard[i][j].get_occupant()->set_jumps(LEFT);
                        jumpPossible = true;
                    }
                }
                
                // right jump //
                if(checkerboard[i][j].get_occupant() != nullptr &&
                   checkerboard[i][j].get_neighbor(UR)->get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(UR)->get_occupant()->get_color() == RED &&
                       checkerboard[i][j].get_neighbor(UR)->get_neighbor(UR)->get_occupant() == nullptr){
                        
                        if(checkerboard[i][j].get_occupant()->get_jumps() == LEFT)
                            checkerboard[i][j].get_occupant()->set_jumps(BOTH);
                        else
                            checkerboard[i][j].get_occupant()->set_jumps(RIGHT);
                        jumpPossible = true;
                    }
                }
            }
        }
        if(jumpPossible == true)
            return;
        
        // MOVES //
        for(int i = 1; i < BOARD_SIZE+1; i++){
            for(int j = 1; j < BOARD_SIZE+1; j++){
                
                // left move //
                if(checkerboard[i][j].get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(UL)->get_occupant() == nullptr){
                        
                        checkerboard[i][j].get_occupant()->set_moves(LEFT);
                        movePossible = true;
                    }
                }
                
                // right move //
                if(checkerboard[i][j].get_occupant() != nullptr){  // avoid deref null
                    
                    if(checkerboard[i][j].get_occupant()->get_color() == BLACK &&
                       checkerboard[i][j].get_neighbor(UR)->get_occupant() == nullptr){
                        
                        if(checkerboard[i][j].get_occupant()->get_moves() == LEFT)
                            checkerboard[i][j].get_occupant()->set_moves(BOTH);
                        else
                            checkerboard[i][j].get_occupant()->set_moves(RIGHT);
                        movePossible = true;
                    }
                }
            }
        }
        
        return; // end of evaluating player BLACK //
    }
    
    return;
}

void Board::move(int x, int y, DIRECTION dir){
    
    if(jumpPossible == false){
        
        // normal moves with RED //
        if(checkerboard[x][y].get_occupant()->get_color() == RED){
            if(dir == LEFT){
                checkerboard[x][y].get_neighbor(LL)->set_occupant(checkerboard[x][y].get_occupant()); // set new square's occupant pointer
                checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(LL)); // set checker's position
                checkerboard[x][y].set_occupant(nullptr);
                return;
            }
            if(dir == RIGHT){
                checkerboard[x][y].get_neighbor(LR)->set_occupant(checkerboard[x][y].get_occupant()); // set new square's occupant pointer
                checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(LR)); // set checker's position
                checkerboard[x][y].set_occupant(nullptr);
                return;
            }
        }
        
        
        // normal moves with BLACK //
        if(checkerboard[x][y].get_occupant()->get_color() == BLACK){
            if(dir == LEFT){
                checkerboard[x][y].get_neighbor(UL)->set_occupant(checkerboard[x][y].get_occupant()); // set new square's occupant pointer
                checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(UL)); // set checker's position
                checkerboard[x][y].set_occupant(nullptr);
                return;
            }
            if(dir == RIGHT){
                checkerboard[x][y].get_neighbor(UR)->set_occupant(checkerboard[x][y].get_occupant()); // set new square's occupant pointer
                checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(UR)); // set checker's position
                checkerboard[x][y].set_occupant(nullptr);
                return;
            }
        }
    }
    
    // jump moves with RED //
    if(checkerboard[x][y].get_occupant()->get_jumps() != NONE &&
       checkerboard[x][y].get_occupant()->get_color() == RED){
        if(dir == LEFT){
            checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(LL)->get_neighbor(LL));
            checkerboard[x][y].get_neighbor(LL)->get_neighbor(LL)->set_occupant(checkerboard[x][y].get_occupant());
            checkerboard[x][y].set_occupant(nullptr);
            delete checkerboard[x][y].get_neighbor(LL)->get_occupant();
            checkerboard[x][y].get_neighbor(LL)->set_occupant(nullptr);
            return;
        }
        if(dir == RIGHT){
            checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(LR)->get_neighbor(LR));
            checkerboard[x][y].get_neighbor(LR)->get_neighbor(LR)->set_occupant(checkerboard[x][y].get_occupant());
            checkerboard[x][y].set_occupant(nullptr);
            delete checkerboard[x][y].get_neighbor(LR)->get_occupant();
            checkerboard[x][y].get_neighbor(LR)->set_occupant(nullptr);
            
            return;
        }
    }
    
    // jump moves with BLACK //
    if(checkerboard[x][y].get_occupant()->get_jumps() != NONE &&
       checkerboard[x][y].get_occupant()->get_color() == BLACK){
        if(dir == LEFT){
            checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(UL)->get_neighbor(UL));
            checkerboard[x][y].get_neighbor(UL)->get_neighbor(UL)->set_occupant(checkerboard[x][y].get_occupant());
            checkerboard[x][y].set_occupant(nullptr);
            delete checkerboard[x][y].get_neighbor(UL)->get_occupant();
            checkerboard[x][y].get_neighbor(UL)->set_occupant(nullptr);
            
            return;
        }
        if(dir == RIGHT){
            checkerboard[x][y].get_occupant()->set_position(checkerboard[x][y].get_neighbor(UR)->get_neighbor(UR));
            checkerboard[x][y].get_neighbor(UR)->get_neighbor(UR)->set_occupant(checkerboard[x][y].get_occupant());
            checkerboard[x][y].set_occupant(nullptr);
            delete checkerboard[x][y].get_neighbor(UR)->get_occupant();
            checkerboard[x][y].get_neighbor(UR)->set_occupant(nullptr);
            
            return;
        }
    }
    
    return;
}

void Board::changeMove(){
    countPieces();
    resetCanJumpMove();
    possible_moves.clear();
    
    if(get_whoseTurn() == RED && get_numRed() == 0){
        winner = BLACK;
        gameEnd = true; return;
    }
    
    if(get_whoseTurn() == BLACK && get_numBlack() == 0){
        winner = RED;
        gameEnd = true; return;
    }
    
    if(jumpPossible == false && movePossible == false){
        if(noMovesCounter == 2){
            if(numRed > numBlack){
                winner = RED;
                gameEnd = true;
                return;
            }
            else if(numBlack > numRed){
                winner = BLACK;
                gameEnd = true;
                return;
            }
            else{
                winner = EMPTY;
                gameEnd = true;
                return;
            }
        }
        else
            noMovesCounter++;
    }
    
    if(get_whoseTurn() == BLACK)
        set_whoseTurn(RED);
    else
        set_whoseTurn(BLACK);
    
    return;
}

void Board::countPieces(){
    numRed = 0;
    numBlack = 0;
    
    for(int i = 1; i < BOARD_SIZE+1; i++){
        for(int j = 1; j < BOARD_SIZE+1; j++){
            if(checkerboard[i][j].get_occupant() != nullptr){
                if(checkerboard[i][j].get_occupant()->get_color() == RED)
                    numRed++;
            }
        }
    }
    
    for(int i = 1; i < BOARD_SIZE+1; i++){
        for(int j = 1; j < BOARD_SIZE+1; j++){
            if(checkerboard[i][j].get_occupant() != nullptr){
                if(checkerboard[i][j].get_occupant()->get_color() == BLACK)
                    numBlack++;
            }
        }
    }
    
    return;
}

void Board::resetCanJumpMove(){
    for(int i = 1; i < BOARD_SIZE+1; i++){
        for(int j = 1; j < BOARD_SIZE+1; j++){
            if(checkerboard[i][j].get_occupant() != nullptr){
                checkerboard[i][j].get_occupant()->set_jumps(NONE);
                checkerboard[i][j].get_occupant()->set_moves(NONE);
            }
        }
    }
    return;
}

void Board::printWinner(){
    cout << endl << endl;
    
    if(winner == RED)
        cout << "The winner is: RED" << endl;
    else if(winner == BLACK)
        cout << "The winner is: BLACK" << endl;
    else
        cout << "It's a draw!" << endl << endl;
}


// GLOBAL FUNCTIONS //
void printBanner(){
    
    cout << "        CCCCCCCCCCCCChhhhhhh                                                     kkkkkkkk                                                                                JJJJJJJJJJJ                          " << endl;
    cout << "     CCC::::::::::::Ch:::::h                                                     k::::::k                                                                                J:::::::::J                          " << endl;
    cout << "   CC:::::::::::::::Ch:::::h                                                     k::::::k                                                                                J:::::::::J                          " << endl;
    cout << "  C:::::CCCCCCCC::::Ch:::::h                                                     k::::::k                                                                                JJ:::::::JJ                          " << endl;
    cout << " C:::::C       CCCCCC h::::h hhhhh           eeeeeeeeeeee        cccccccccccccccc k:::::k    kkkkkkk eeeeeeeeeeee    rrrrr   rrrrrrrrr       ssssssssss                    J:::::Jrrrrr   rrrrrrrrr           " << endl;
    cout << "C:::::C               h::::hh:::::hhh      ee::::::::::::ee    cc:::::::::::::::c k:::::k   k:::::kee::::::::::::ee  r::::rrr:::::::::r    ss::::::::::s                   J:::::Jr::::rrr:::::::::r          " << endl;
    cout << "C:::::C               h::::::::::::::hh   e::::::eeeee:::::ee c:::::::::::::::::c k:::::k  k:::::ke::::::eeeee:::::eer:::::::::::::::::r ss:::::::::::::s                  J:::::Jr:::::::::::::::::r         " << endl;
    cout << "C:::::C               h:::::::hhh::::::h e::::::e     e:::::ec:::::::cccccc:::::c k:::::k k:::::ke::::::e     e:::::err::::::rrrrr::::::rs::::::ssss:::::s                 J:::::jrr::::::rrrrr::::::r        " << endl;
    cout << "C:::::C               h::::::h   h::::::he:::::::eeeee::::::ec::::::c     ccccccc k::::::k:::::k e:::::::eeeee::::::e r:::::r     r:::::r s:::::s  ssssss                  J:::::J r:::::r     r:::::r        " << endl;
    cout << "C:::::C               h:::::h     h:::::he:::::::::::::::::e c:::::c              k:::::::::::k  e:::::::::::::::::e  r:::::r     rrrrrrr   s::::::s           JJJJJJJ     J:::::J r:::::r     rrrrrrr        " << endl;
    cout << "C:::::C               h:::::h     h:::::he::::::eeeeeeeeeee  c:::::c              k:::::::::::k  e::::::eeeeeeeeeee   r:::::r                  s::::::s        J:::::J     J:::::J r:::::r                    " << endl;
    cout << " C:::::C       CCCCCC h:::::h     h:::::he:::::::e           c::::::c     ccccccc k::::::k:::::k e:::::::e            r:::::r            ssssss   s:::::s      J::::::J   J::::::J r:::::r                    " << endl;
    cout << "  C:::::CCCCCCCC::::C h:::::h     h:::::he::::::::e          c:::::::cccccc:::::ck::::::k k:::::ke::::::::e           r:::::r            s:::::ssss::::::s     J:::::::JJJ:::::::J r:::::r                    " << endl;
    cout << "   CC:::::::::::::::C h:::::h     h:::::h e::::::::eeeeeeee   c:::::::::::::::::ck::::::k  k:::::ke::::::::eeeeeeee   r:::::r            s::::::::::::::s       JJ:::::::::::::JJ  r:::::r             ...... " << endl;
    cout << "     CCC::::::::::::C h:::::h     h:::::h  ee:::::::::::::e    cc:::::::::::::::ck::::::k   k:::::kee:::::::::::::e   r:::::r             s:::::::::::ss          JJ:::::::::JJ    r:::::r             .::::. " << endl;
    cout << "        CCCCCCCCCCCCC hhhhhhh     hhhhhhh    eeeeeeeeeeeeee      cccccccccccccccckkkkkkkk    kkkkkkk eeeeeeeeeeeeee   rrrrrrr              sssssssssss              JJJJJJJJJ      rrrrrrr             ...... " << endl;
    
    cout << endl << endl;
    cout << "An abridged, AI-driven version of checkers." << endl << endl;
    cout << "Programmed by William Santagata" << endl;
    cout << "Artificial Intelligence I (Grad)" << endl;
    cout << "Spring 2018" << endl << endl << endl;
    
    return;
}



///////////////////////////////////////

void Board::generate_PossibleMoves(){
    for(int i = 1; i < BOARD_SIZE+1; i++){
        for(int j = 1; j < BOARD_SIZE+1; j++){
            
            if(validMove(i,j,LEFT))
                possible_moves.push_back(make_tuple(i,j,LEFT));
            
            if(validMove(i,j,RIGHT))
                possible_moves.push_back(make_tuple(i,j,RIGHT));
        }
    }
    
    random_shuffle(possible_moves.begin(), possible_moves.end());
    
    return;
}

void alpha_beta_search(Board state, int& X, int& Y, DIRECTION& Dir){
    int alpha = -1000;
    int beta = 1000;
    Board copy_state;
    int depth(5);
    
    copy_state = state;
    copy_state.generate_PossibleMoves();

    for(int d = 0; d < depth; d++){
        for(int i = 0; i < copy_state.possible_moves.size(); i++){
            
            if(copy_state.validMove(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[i])))
            copy_state.move(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[1]));
        
            if(utility(copy_state) == max_value(copy_state, alpha, beta)){
                X = get<0>(state.possible_moves[i]);
                Y = get<1>(state.possible_moves[i]);
                Dir = get<2>(state.possible_moves[1]);
                copy_state = state;
                return;
            }
        
            copy_state.changeMove();
            copy_state.generate_PossibleMoves();
        }
    }

    copy_state = state;
    return;
}

int max_value(Board copy_state, int& alpha, int& beta){
    
    int u_val(-1000);
    alpha = 0;
    beta = 0;
    
    //copy_state.generate_PossibleMoves();

    if(copy_state.get_gameEnd()){
        u_val = utility(copy_state);
        return u_val;
    }
 
    u_val = -1000;
    
    for(int i = 0; i < copy_state.possible_moves.size(); i++){
        
        if(copy_state.validMove(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[i])))
            copy_state.move(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[i]));
        
        u_val = max(u_val, min_value(copy_state, alpha, beta));
                
        if(u_val >= beta)
            return u_val;
                
        alpha = max(alpha, u_val);
        
        cout << "MAX: " << u_val << endl;
        copy_state.changeMove();
    }
    
    return u_val;
}


int min_value(Board copy_state, int& alpha, int& beta){
    
    int u_val(1000);
    
    //copy_state.generate_PossibleMoves();
    
    //if(copy_state.get_gameEnd()){
    //    u_val = utility(copy_state);
     //   return u_val;
   // }
    
    u_val = 1000;
    
    for(int i = 0; i < copy_state.possible_moves.size(); i++){
        if(copy_state.validMove(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[i])))
            copy_state.move(get<1>(copy_state.possible_moves[i]), get<0>(copy_state.possible_moves[i]), get<2>(copy_state.possible_moves[i]));
        
        u_val = min(u_val, max_value(copy_state, alpha, beta));
                
        if(u_val <= alpha)
            return u_val;
                
        beta = min(beta, u_val);
        
        cout << "MIN: " << u_val << endl;

        copy_state.changeMove();
    }
    
    return u_val;
}

int utility(Board state){
    
    return state.get_numRed() * 100;
}


