// Player.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

class HumanPlayerImpl
{
  public:
    int chooseMove(const Scaffold& s, int N, int color);
    virtual bool isInteractive() const{return true;}
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
    
    for(;;) // check if the move is valid
    {
        while( tempCol > s.cols() || tempCol < 1) // check if column is within range
        {
            cout << "Choose valid column: ";
            cin >> tempCol;
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

int SmartPlayerImpl::chooseMove(const Scaffold& s, int N, int color)
{
    return -1;  //  This is not always correct; it's just here to compile
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
