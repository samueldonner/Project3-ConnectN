// Game.cpp

#include "provided.h"
#include <iostream>
using namespace std;

class GameImpl
{
  public:
    GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black); 
    bool completed(int& winner) const; 
    bool takeTurn(); 
    void play(); 
    int checkerAt(int c, int r) const;
private:
    int m_cols;
    int m_levs;
    int m_inArow;
    Player* m_red;
    Player* m_black;
    Scaffold m_s;
    int m_lastMoveCol;
    int m_whoseTurn;
};

GameImpl::GameImpl(int nColumns, int nLevels, int N, Player* red, Player* black)
: m_s(nColumns, nLevels)
{
    m_cols = nColumns;
    m_levs = nLevels;
    m_inArow = N;
    m_red = red;
    m_black = black;
    m_whoseTurn = RED;
    m_lastMoveCol = -1;
    
}

bool GameImpl::completed(int& winner) const
{
    if( m_lastMoveCol == -1 )
    {
        return false;
    }
    int lastMoveRow = 0;
    for(int i = 0; i < m_levs; i++)
    {
        if(checkerAt(m_lastMoveCol-1, i) != VACANT)
        {
            lastMoveRow = i; // find the row of the last move
            break;
        }
    }
    int colorOfLastMove = checkerAt(m_lastMoveCol-1, lastMoveRow);
    
    int checkersInARow = 0;
    for(int i = lastMoveRow; i < m_s.levels(); i++) // check downward
    {
        if(checkerAt(m_lastMoveCol-1, i) != colorOfLastMove)
        {
            break;
        }
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        return true;
    }
    
    
    checkersInARow = 0;
    int firstColInRow = 0;
    for(int i = m_lastMoveCol-1; i >= 0; i--) // go to first in row
    {
        if(checkerAt(i, lastMoveRow) != colorOfLastMove)
        {
            firstColInRow = i+1;
            break;
        }
    }
    for(int i = firstColInRow; i < m_cols; i++) // check across
    {
        if(checkerAt(i, lastMoveRow) != colorOfLastMove)
        {
            break;
        }
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        return true;
    }
    
    
    
    checkersInARow = 0; // check diagonally from top left to botom right
    int firstColInDiagLT = m_lastMoveCol-1;
    int firstRowInDiagLT = lastMoveRow;
    while( firstColInDiagLT>=0 && firstRowInDiagLT>=0 && checkerAt(firstColInDiagLT, firstRowInDiagLT) == colorOfLastMove)
    {
        firstColInDiagLT--;
        firstRowInDiagLT--;
    }
    firstColInDiagLT++;
    firstRowInDiagLT++;
    while(firstColInDiagLT<m_s.cols() && firstRowInDiagLT<m_s.levels() && checkerAt(firstColInDiagLT, firstRowInDiagLT) == colorOfLastMove
           )
    {
        firstColInDiagLT++;
        firstRowInDiagLT++;
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        return true;
    }
    
    
    checkersInARow = 0; // check diagonally from bottom left to top right
    int firstColInDiagLB = m_lastMoveCol-1;
    int firstRowInDiagLB = lastMoveRow;
    while( firstColInDiagLB>=0 && firstRowInDiagLB<m_s.levels() && checkerAt(firstColInDiagLB, firstRowInDiagLB) == colorOfLastMove)
    {
        firstColInDiagLB--;
        firstRowInDiagLB++;
    }
    firstColInDiagLB++;
    firstRowInDiagLB--;
    while(firstColInDiagLB<m_s.cols() && firstRowInDiagLB>=0 && checkerAt(firstColInDiagLB, firstRowInDiagLB) == colorOfLastMove
          )
    {
        firstColInDiagLB++;
        firstRowInDiagLB--;
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        return true;
    }
    
    
    if(m_s.numberEmpty()==0) // if board is full game is tied
    {
        winner = TIE_GAME;
        return true;
    }
    
    
    return false;
}

bool GameImpl::takeTurn()
{
    int winner = -2;
    if(completed(winner))
    {
        return false;
    }
    if(m_whoseTurn == RED)
    {
        //cout << "Press Enter to Continue" << endl;
        //cin.ignore(10000, '\n');
        m_lastMoveCol = m_red->chooseMove(m_s, m_inArow, RED);
        m_s.makeMove(m_lastMoveCol,RED);
        m_whoseTurn = BLACK;
        return true;
    }
    else if( m_whoseTurn == BLACK)
    {
        //cout << "Press Enter to Continue" << endl;
        //4cin.ignore(10000, '\n');
        m_lastMoveCol = m_black->chooseMove(m_s, m_inArow, BLACK);
        m_s.makeMove( m_lastMoveCol, BLACK );
        m_whoseTurn = RED;
        return true;
    }
    
    return false;
}

void GameImpl::play()
{
    int winner = -1;
    m_s.display();
    while(!completed(winner))
    {
        takeTurn();
        m_s.display();
    }
    m_s.display();
    cout << "The winner is: ";
    switch(winner)
    {
        case RED:
            cout<< "RED!"<<endl;
            break;
        case BLACK:
            cout << "BLACK!"<<endl;
            break;
        case TIE_GAME:
            cout << "TIE GAME!"<<endl;
            break;
    }
}

int GameImpl::checkerAt(int c, int r) const
{
    return m_s.checkerAt(c,r);
} 

//******************** Game functions *******************************

// These functions simply delegate to GameImpl's functions.
// You probably don't want to change any of this code.

Game::Game(int nColumns, int nLevels, int N, Player* red, Player* black)
{
    m_impl = new GameImpl(nColumns, nLevels, N, red, black);
}
 
Game::~Game()
{
    delete m_impl;
}
 
bool Game::completed(int& winner) const
{
    return m_impl->completed(winner);
}

bool Game::takeTurn()
{
    return m_impl->takeTurn();
}

void Game::play()
{
    m_impl->play();
}
 
int Game::checkerAt(int c, int r) const
{
    return m_impl->checkerAt(c, r);
}
