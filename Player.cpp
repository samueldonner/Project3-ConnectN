// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
#include <stack>
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

/*int bestComputerMove( int &result, Scaffold s, int &score, int &depth, int &winner, int color, int N, stack<int> goodMoves, stack<int> goodMoveRating, stack<int> depthStack, stack<int> goodHumanMove);


int bestHumanMove( int &result, Scaffold s, int &score, int &depth, int &winner, int color, int N, stack<int> goodMoves, stack<int> goodMoveRating, stack<int> depthStack, stack<int> goodComputerMove)
{
    int levs = s.levels();
    int cols = s.cols();
    int oppositeColor = RED;
    if(color == RED)
        oppositeColor = BLACK;
    int i=0;
    while(!s.makeMove(i+1, color) && i < cols)
        {
            i++;
        } // make must update scaffold
        s.display();
        depth++;
        int rating = -2;
        completed(winner, rating, color, i+1, levs, cols, N, s); // rate scaffold move
        if(rating == 0 || rating == -1 || rating == 1)
        {
            score+=rating; // store rating
            goodMoves.push(i);
            goodMoveRating.push(rating);
            depthStack.push(depth);
        }
        else{
        goodComputerMove.push(bestComputerMove( result, s, score, depth, winner, oppositeColor, N, goodMoves,goodMoveRating, depthStack, goodComputerMove));
        }
        s.undoMove();
    
    stack<int> ties;
    stack<int> losses;
    stack<int> wins;
    stack<int> depthWin;
    stack<int> depthTie;
    stack<int> depthLoss;
    while( !depthStack.empty() )
    {
        if(goodMoveRating.top() == 0)
        {
            ties.push(goodMoves.top());
            depthTie.push(depthStack.top());
            depthStack.pop();
            goodMoves.pop();
            goodMoveRating.pop();
        }
        else if(goodMoveRating.top() == 1)
        {
            wins.push(goodMoves.top());
            depthWin.push(depthStack.top());
            depthStack.pop();
            goodMoves.pop();
            goodMoveRating.pop();
        }
        else
        {
            losses.push(goodMoves.top());
            depthLoss.push(depthStack.top());
            depthStack.pop();
            goodMoves.pop();
            goodMoveRating.pop();
        }
    }
    
    int bestMove = -2;
    
    if(!losses.empty())
    {
        int bestLossDepth = depthLoss.top();
        while(!losses.empty())
        {
            if(depthLoss.top()<=bestLossDepth)
            {
                bestLossDepth = depthLoss.top();
                bestMove = losses.top();
                losses.pop();
                depthLoss.pop();
            }
            else
            {
                losses.pop();
                depthLoss.pop();
            }
            result = -1;
        }
    }
    
    if(!ties.empty())
    {
    int bestTieDepth = depthTie.top();
    while(!ties.empty() && losses.empty() )
    {
        if(depthTie.top()<=bestTieDepth)
        {
            bestTieDepth = depthTie.top();
            bestMove = ties.top();
            ties.pop();
            depthTie.pop();
        }
        else
        {
            ties.pop();
            depthTie.pop();
        }
        result = 0;
    }
    }
    
    if(!wins.empty())
    {
    int bestWinDepth = depthWin.top();
    while(!wins.empty() && losses.empty() && ties.empty())
    {
        if(depthWin.top()<=bestWinDepth)
        {
            bestWinDepth = depthWin.top();
            bestMove = wins.top();
            wins.pop();
            depthWin.pop();
        }
        else
        {
            wins.pop();
            depthWin.pop();
        }
        result = 1;
    }
    }
    
    
    return bestMove;
}


int bestComputerMove( int &result, Scaffold s, int &score, int &depth, int &winner, int color, int N, stack<int> goodMoves, stack<int> goodMoveRating, stack<int> depthStack, stack<int> goodHumanMove)
{
    int levs = s.levels();
    int cols = s.cols();
    int oppositeColor = RED;
    if(color == RED)
        oppositeColor = BLACK;
    int i = 0;
    while(!s.makeMove(i+1, color) && i<cols)
        {
            i++;
        } // make must update scaffold
        s.display();
        depth++;
        int rating = -2;
        completed(winner, rating, color, i+1, levs, cols, N, s); // rate scaffold move
        if(rating == 0 || rating == -1 || rating == 1)
        {
            score+=rating; // store rating
            goodMoves.push(i);
            goodMoveRating.push(rating);
            depthStack.push(depth);
        }
        else{
        goodHumanMove.push(bestHumanMove( result, s, score, depth, winner, oppositeColor, N, goodMoves,goodMoveRating, depthStack, goodHumanMove));
        }
        s.undoMove();
    
    stack<int> ties;
    stack<int> losses;
    stack<int> wins;
    stack<int> depthWin;
    stack<int> depthTie;
    stack<int> depthLoss;
    while( !depthStack.empty() )
    {
        if(goodMoveRating.top() == color)
        {
            ties.push(goodMoves.top());
            depthTie.push(depthStack.top());
            depthStack.pop();
            goodMoves.pop();
            goodMoveRating.pop();
        }
        else
        {
            wins.push(goodMoves.top());
            depthStack.pop();
            goodMoves.pop();
            goodMoveRating.pop();
        }
    }
    
    int bestMove = -2;
    
    if(!wins.empty())
    {
    int bestWinDepth = depthWin.top();
    while(!wins.empty())
    {
        if(depthWin.top()<=bestWinDepth)
        {
            bestWinDepth = depthWin.top();
            bestMove = wins.top();
            wins.pop();
            depthWin.pop();
        }
        else
        {
            wins.pop();
            depthWin.pop();
        }
        result = 1;
    }
    }
    
    if(!ties.empty())
    {
    int bestTieDepth = depthTie.top();
    while(!ties.empty() && wins.empty() )
    {
        if(depthTie.top()<=bestTieDepth)
        {
            bestTieDepth = depthTie.top();
            bestMove = ties.top();
            ties.pop();
            depthTie.pop();
        }
        else
        {
            ties.pop();
            depthTie.pop();
        }
        result = 0;
    }
    }
    
    if(!losses.empty())
    {
    int bestLossDepth = depthLoss.top();
    while(!losses.empty() && ties.empty() && wins.empty() )
    {
        if(depthLoss.top()<=bestLossDepth)
        {
            bestLossDepth = depthLoss.top();
            bestMove = losses.top();
            losses.pop();
            depthLoss.pop();
        }
        else
        {
            losses.pop();
            depthLoss.pop();
        }
        result = -1;
    }
    }
    return bestMove;
}*/

void recursion2(Scaffold& s, int& score, int color, int& winner, int N);

void recursion(Scaffold& s, int& score, int color, int& winner, int N)
{
    
    for( int i = 0; i < s.cols(); i++ )
    {
        if(!s.makeMove(i+1, color))
            continue;
        s.makeMove(i+1, color);
        
        int rating = 0;
        if( completed(winner, rating, color, i+1, s.levels(), s.cols(), N, s) )
        {
            if(rating == 1 || rating == 0)
                score+= rating;
        }
        else
        {
            int otherColor = RED;
            if(color == RED)
                otherColor = BLACK;
            recursion2(s, score, otherColor, winner, N);
        }
        s.undoMove();
    }
}

void recursion2(Scaffold& s, int& score, int color, int& winner, int N)
{
    
    for( int i = 0; i < s.cols(); i++ )
    {
        if(!s.makeMove(i+1, color))
            continue;
        s.makeMove(i+1, color);
        int rating = 0;
        if(completed(winner, rating, color, i+1, s.levels(), s.cols(), N, s))
        {
            if(rating == 1 || rating == 0)
                score-= rating;
        }
            
        else
        {
            int otherColor = RED;
            if(color == RED)
                otherColor = BLACK;
            recursion(s, score, otherColor, winner, N);
        }
        s.undoMove();
    }
}

int bestMove(Scaffold s, int N, int color)
{
    int winner = -2;
    int score = 0;
    int rating = 0;
    int bestMove = 0;
    recursion(s, score, color, winner, N);
    for( int i = 0; i < s.cols(); i ++ )
    {
        s.makeMove(i+1, color);
        int compareScore = score;
        recursion(s, score, color, winner, N);
        if( score > compareScore )
        {
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
