/*******************************************************************************
 Name:      William Santagata
 Course:    Artificial Intelligence I
 Section:   Graduate
 Semester:  Spring 2018
 Program:   AI Checkers
 *******************************************************************************/

// AI_ENGINE.HPP //
#include "game_engine.hpp"
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace std;

void alpha_beta_search(Board state, int& X, int& Y, DIRECTION& Dir);
int max_value(Board state, int& alpha, int& beta);
int min_value(Board state, int& alpha, int& beta);
int utility(Board state);


void alpha_beta_search(Board state, int& X, int& Y, DIRECTION& Dir){
    int dir_set(0);
    int alpha = -1000;
    int beta = 1000;
    DIRECTION dir_one;
    DIRECTION dir_two;
    Board copy_state;
    
    copy_state = state;
    dir_set = rand() % 2;
    
    if(dir_set == 0){
        dir_one = LEFT;
        dir_two = RIGHT;
    }
    else{
        dir_one = RIGHT;
        dir_two = LEFT;
    }
    
    for(int x = 1; x < BOARD_SIZE+1; x++){
        for(int y = 1; y < BOARD_SIZE+1; y++){
            if(copy_state.validMove(x,y,dir_one)){
                copy_state.move(x,y,dir_one);
                
                if(utility(copy_state) == max_value(copy_state, alpha, beta)){
                    X=x; Y=y; Dir=dir_one; return;}
                
                copy_state = state;
                copy_state.move(x,y,dir_two);
                
                if(utility(copy_state) == max_value(copy_state, alpha, beta)){
                    X=x; Y=y; Dir=dir_one; return;}
            }
        }
    }
    
    return;
}

int max_value(Board state, int& alpha, int& beta){
    
    int u_val(-1000);
    Board copy_state;
    alpha = 0;
    beta = 0;
    
    copy_state = state;
    
    if(state.get_gameEnd()){
        u_val = utility(state);
        return u_val;
    }
    
    u_val = -1000;
    
    for(int x = 1; x < BOARD_SIZE+1; x++){
        for(int y = 1; y < BOARD_SIZE+1; y++){
            
            if(state.validMove(x,y,LEFT)){
                copy_state.move(x,y,LEFT);
                u_val = max(u_val, min_value(copy_state, alpha, beta));
                
                if(u_val >= beta)
                    return u_val;
                
                alpha = max(alpha, u_val);
                copy_state = state;
            }
            
            if(state.validMove(x,y,RIGHT)){
                copy_state.move(x,y,RIGHT);
                u_val = max(u_val, min_value(copy_state, alpha, beta));
                
                if(u_val >= beta)
                    return u_val;
                
                alpha = max(alpha, u_val);
                copy_state = state;
            }
        }
    }
    
    return u_val;
}


int min_value(Board state, int& alpha, int& beta){
    
    int u_val(1000);
    Board copy_state;
    
    copy_state = state;
    
    if(state.get_gameEnd()){
        u_val = utility(state);
        return u_val;
    }
    
    u_val = 1000;
    
    for(int x = 1; x < BOARD_SIZE+1; x++){
        for(int y = 1; y < BOARD_SIZE+1; y++){
            
            if(state.validMove(x,y,LEFT)){
                copy_state.move(x,y,LEFT);
                u_val = min(u_val, max_value(copy_state, alpha, beta));
                
                if(u_val <= alpha)
                    return u_val;
                
                beta = min(beta, u_val);
                copy_state = state;
            }
            
            if(state.validMove(x,y,RIGHT)){
                copy_state.move(x,y,RIGHT);
                u_val = min(u_val, max_value(copy_state, alpha, beta));
                
                if(u_val <= alpha)
                    return u_val;
                
                beta = min(beta, u_val);
                copy_state = state;
            }
        }
    }
    
    return u_val;
}

int utility(Board state){
    
    return state.get_numRed();
}




