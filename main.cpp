/*******************************************************************************
 Name:      William Santagata
 Course:    Artificial Intelligence I
 Section:   Graduate
 Semester:  Spring 2018
 Program:   AI Checkers
*******************************************************************************/

// MAIN.CPP //

#include "game_engine.hpp"
//#include "ai_engine.hpp"
using namespace std;

int main(void){
    
    int X, Y;
    DIRECTION DIR;
    Board gameboard;
    
    printBanner();
    gameboard.printBoard();
    
    while(gameboard.get_gameEnd() != true){
       
        gameboard.generate_PossibleMoves();
        
        for(int i = 0; i < gameboard.possible_moves.size(); i++)
            cout << "(" << get<0>(gameboard.possible_moves[i]) << ", " << get<1>(gameboard.possible_moves[i]) << ", " << get<2>(gameboard.possible_moves[i]) << ")" << endl;
        
        gameboard.inputMove(X, Y, DIR);
        gameboard.printBoard();
        gameboard.changeMove();
        
        gameboard.generate_PossibleMoves();
        for(int i = 0; i < gameboard.possible_moves.size(); i++)
            cout << "(" << get<0>(gameboard.possible_moves[i]) << ", " << get<1>(gameboard.possible_moves[i]) << ", " << get<2>(gameboard.possible_moves[i]) << ")" << endl;
        
        alpha_beta_search(gameboard, X, Y, DIR);
        cout << "ALPHA BETA RESULT - X: " << X << " Y: " << Y << " DIR: " << DIR << endl;

        gameboard.inputMove(X, Y, DIR);
        gameboard.printBoard();
        gameboard.changeMove();
        
        /*
        gameboard.inputMove(X, Y, DIR);
        gameboard.printBoard();
        
        gameboard.changeMove();
        
        alpha_beta_search(gameboard, X, Y, DIR);
        cout << "X: " << X << " Y: " << Y << " DIR: " << DIR << endl;

        gameboard.move(Y, X, DIR);
        gameboard.printBoard();
        gameboard.changeMove();
         */
    }
    
    gameboard.printWinner();
    
    return 0;
}


