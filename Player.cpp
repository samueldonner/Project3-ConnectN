// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
    virtual bool isInteractive() const{return true;}
private:
};

class BadPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

class SmartPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
};

int HumanPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0)
    {
        return -1;
    }
    int tempCol;
    cout << "Move to column: ";
    cin >> tempCol;
    cin.clear();
    
    for(;;) // check if the move is valid
    {
        while( tempCol > s.cols() || tempCol < 1) // check if column is within range
        {
            cout << "Choose valid column: ";
            cin >> tempCol;
            cin.clear();
        }
        for( int i = s.levels()-1; i >= 0; i-- ) // check column is not full
        {
            if(s.checkerAt(tempCol-1,i) == VACANT) //go up from bottom to find first valid space
            {
                return tempCol;
            }
        }
        cout << "Choose valid column: ";
        cin >> tempCol;
        cin.clear();
    }
    
}

int BadPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    if(s.numberEmpty() == 0)
    {
        return -1;
    }
    for( int i = 0; i < s.cols(); i++ )
    {
        if(s.checkerAt(i, 0) == VACANT)
            return i+1;
    }
    return -1;
}

bool completed(int& winner, int& rating, int color, int m_lastMoveCol, int m_levs, int m_cols, int m_inArow, const Scaffold& m_s)
{
    winner = -2;
    if( m_lastMoveCol == -1 )
    {
        return false;
    }
    int lastMoveRow = 0;
    for(int i = 0; i < m_levs; i++)
    {
        if(m_s.checkerAt(m_lastMoveCol-1, i) != VACANT)
        {
            lastMoveRow = i; // find the row of the last move
            break;
        }
    }
    int colorOfLastMove = m_s.checkerAt(m_lastMoveCol-1, lastMoveRow);
    
    int checkersInARow = 0;
    for(int i = lastMoveRow; i < m_s.levels(); i++) // check downward
    {
        if(m_s.checkerAt(m_lastMoveCol-1, i) != colorOfLastMove)
        {
            break;
        }
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        if(winner==color)
        {
            rating = 1;
        }
        else
        {
            rating = -1;
        }
        return true;
    }
    
    
    checkersInARow = 0;
    int firstColInRow = 0;
    for(int i = m_lastMoveCol-1; i >= 0; i--) // go to first in row
    {
        if(m_s.checkerAt(i, lastMoveRow) != colorOfLastMove)
        {
            firstColInRow = i+1;
            break;
        }
    }
    for(int i = firstColInRow; i < m_cols; i++) // check across
    {
        if(m_s.checkerAt(i, lastMoveRow) != colorOfLastMove)
        {
            break;
        }
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        if(winner==color)
        {
            rating = 1;
        }
        else
        {
            rating = -1;
        }
        return true;
    }
    
    
    
    checkersInARow = 0; // check diagonally from to left to botom right
    int firstColInDiagLT = m_lastMoveCol-1;
    int firstRowInDiagLT = lastMoveRow;
    while( firstColInDiagLT>=0 && firstRowInDiagLT>=0 && m_s.checkerAt(firstColInDiagLT, firstRowInDiagLT) == colorOfLastMove)
    {
        firstColInDiagLT--;
        firstRowInDiagLT--;
    }
    firstColInDiagLT++;
    firstRowInDiagLT++;
    while(firstColInDiagLT<m_s.cols() && firstRowInDiagLT<m_s.levels() && m_s.checkerAt(firstColInDiagLT, firstRowInDiagLT) == colorOfLastMove
          )
    {
        firstColInDiagLT++;
        firstRowInDiagLT++;
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        if(winner==color)
        {
            rating = 1;
        }
        else
        {
            rating = -1;
        }
        return true;
    }
    
    
    checkersInARow = 0; // check diagonally from bottom left to top right
    int firstColInDiagLB = m_lastMoveCol-1;
    int firstRowInDiagLB = lastMoveRow;
    int checker = m_s.checkerAt(firstColInDiagLB, firstRowInDiagLB);
    while( firstColInDiagLB>=0 && firstRowInDiagLB<m_s.levels() && m_s.checkerAt(firstColInDiagLB, firstRowInDiagLB) == colorOfLastMove)
    {
        firstColInDiagLB--;
        firstRowInDiagLB++;
    }
    firstColInDiagLB++;
    firstRowInDiagLB--;
    while(firstColInDiagLB<m_s.cols() && firstRowInDiagLB>=0 && m_s.checkerAt(firstColInDiagLB, firstRowInDiagLB) == colorOfLastMove
          )
    {
        firstColInDiagLB++;
        firstRowInDiagLB--;
        checkersInARow++;
    }
    
    if(checkersInARow>=m_inArow)
    {
        winner = colorOfLastMove;
        if(winner==color)
        {
            rating = 1;
        }
        else
        {
            rating = -1;
        }
        return true;
    }
    
    
    if(m_s.numberEmpty()==0) // if board is full game is tied
    {
        winner = TIE_GAME;
        rating = 0;
        return true;
    }
    
    return false;
}


void recursion2(Scaffold& s, double& score, int color, int& winner, int N);

void recursion(Scaffold& s, double& score, int color, int& winner, int N)
{
    for( int i = 0; i < s.cols(); i++ )
    {
        if(!s.makeMove(i+1, color))
            continue;
        //s.display();
        int rating = 0;
        if( completed(winner, rating, color, i+1, s.levels(), s.cols(), N, s) )
        {
            if(rating == 1 || rating == 0)
                score+= rating*(s.cols()*s.levels()) - (s.cols()*s.levels()-s.numberEmpty());
            //s.display();
        }
        else
        {
            recursion2(s, score, !color, winner, N);
        }
        s.undoMove();
    }
}

void recursion2(Scaffold& s, double& score, int color, int& winner, int N)
{
    
    for( int i = 0; i < s.cols(); i++ )
    {
        if(!s.makeMove(i+1, color))
            continue;
        //s.display();
        //cout<< "WORKING";
        int rating = 0;
        if(completed(winner, rating, color, i+1, s.levels(), s.cols(), N, s))
        {
            //s.display();
            if(rating == 1 || rating == 0)
                score-= rating*(s.cols()*s.levels()) - (s.cols()*s.levels()-s.numberEmpty());
        }
        else
        {
            recursion(s, score, !color, winner, N);
        }
        s.undoMove();
    }
}

int bestMove(Scaffold s, int N, int color)
{
    int winner = -2;
    int bestMove = 0;
    int compareScore = INT_MAX;
    //recursion(s, compareScore, color, winner, N);
    
    for( int i = 0; i < s.cols(); i ++ )
    {
        double score = 0;
        if(!s.makeMove(i+1, color))
            continue;
        recursion(s, score, !color, winner, N);
        cout << "SCORE: " << score <<" COL: " << i+1 << endl;
        if( score <= compareScore )
        {
            compareScore = score;
            bestMove = i+1;
        }
        s.undoMove();
    }
    return bestMove;
}

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    return bestMove(s, N, color);
}


//******************** Player derived class functions *************************

// These functions simply delegate to the Impl classes' functions.
// You probably don't want to change any of this code.

HumanPlayer::HumanPlayer(string nm)
 : Player(nm)
{
    m_impl = new HumanPlayerImpl;
}

HumanPlayer::~HumanPlayer()
{
    delete m_impl;
}

int HumanPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

BadPlayer::BadPlayer(string nm)
 : Player(nm)
{
    m_impl = new BadPlayerImpl;
}
 
BadPlayer::~BadPlayer()
{
    delete m_impl;
}
 
int BadPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}

SmartPlayer::SmartPlayer(string nm)
 : Player(nm)
{
    m_impl = new SmartPlayerImpl;
}
 
SmartPlayer::~SmartPlayer()
{
    delete m_impl;
}
 
int SmartPlayer::chooseMove(const Scaffold& s, int N, int color)
{
    return m_impl->chooseMove(s, N, color);
}
