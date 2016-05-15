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
        int checker = checkerAt(m_lastMoveCol-1, i);
        if(checkerAt(m_lastMoveCol-1, i) != VACANT)
        {
            lastMoveRow = i; // find the row of the last move
        }
    }
    int colorOfLastMove = checkerAt(m_lastMoveCol-1, lastMoveRow);
    int checkersInARow = 0;
    for(int i = lastMoveRow; i >= 0; i--)
    {
        int checker = checkerAt(m_lastMoveCol-1, i);
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
    return false;
}

bool GameImpl::takeTurn()
{
    if(m_whoseTurn == RED)
    {
        m_lastMoveCol = m_red->chooseMove(m_s, m_inArow, RED);
        m_s.makeMove(m_lastMoveCol,RED);
        m_whoseTurn = BLACK;
        return true;
    }
    else if( m_whoseTurn == BLACK)
    {
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
        cout << "Press Enter to Continue" << endl;
        cin.ignore(10000, '\n');
    }
    
    cout << "The winner is: ";
    switch(winner)
    {
        case RED:
            cout<< "RED!";
            break;
        case BLACK:
            cout << "BLACK!";
            break;
        case TIE_GAME:
            cout << "TIE GAME!";
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
