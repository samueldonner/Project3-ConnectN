// Scaffold.cpp

#include "provided.h"
#include <iostream>
#include <vector>
#include <stack>
using namespace std;

class ScaffoldImpl
{
  public:
    ScaffoldImpl(int nColumns, int nLevels); 
    int cols() const;
    int levels() const;
    int numberEmpty() const;
    int checkerAt(int column, int level) const; 
    void display() const; 
    bool makeMove(int column, int color); 
    int undoMove();
private:
    int m_cols;
    int m_lev;
    vector<vector<int>> grid;
    stack<int> undoStack;
    // stack
    
};

ScaffoldImpl::ScaffoldImpl(int nColumns, int nLevels)
{
    if( nColumns < 0 || nLevels < 0 )
    {
        cerr << "nColumns or nLevels is less than 1" << endl;
        exit(EXIT_FAILURE);
    }
    m_cols = nColumns;
    m_lev = nLevels;
    
    grid.resize(m_lev); // resized vectpr to have m_lev rows
    for( int i = 0; i < m_lev; i++ )
    {
        grid[i].resize(m_cols); // resized vector to have m_cols columns
    }
    
    for( int i = 0; i < m_lev; i++ )
    {
        for( int j = 0; j < m_cols; j++ )
        {
            grid[i][j] = VACANT; // fill grid with vacant slots
        }
    }
}

int ScaffoldImpl::cols() const
{
    return m_cols;  //  return number of columns
}

int ScaffoldImpl::levels() const
{
    return m_lev;  //  returns number of levels
}

int ScaffoldImpl::numberEmpty() const
{
    int nEmpty = 0; // declare 0 empty spots
    for( int i = 0; i < m_lev; i++ )
    {
        for( int j = 0; j < m_cols; j++ )
        {
            if(grid[i][j] == VACANT)
            {
                nEmpty++; // count number of vacant slots
            }
        }
    }
    
    return nEmpty;
}

int ScaffoldImpl::checkerAt(int column, int level) const
{
    switch( grid[level][column] )
    {
        case RED:
            return RED; // return red if grid is red
            break;
        case BLACK:
            return BLACK; // return black if grid is balck
            break;
        default:
            return VACANT; // return vacant if grid is vacant
    }
}

void ScaffoldImpl::display() const
{
    for( int i = 0; i < m_lev; i++ )
    {
        for( int j = 0; j < m_cols; j++ )
        {
            cout<<"|"; // start row with |
            if(checkerAt(j,i) == RED)
            {
                cout<<"R"; // if red print R
            }
            else if(checkerAt(j,i) == BLACK)
            {
                cout<<"B"; // if black print B
            }
            else
            {
                cout<<" "; // otherwise print space
            }
        }
        cout<<"|"<<endl; // end row with |
    }
    for( int i = 0; i < m_cols; i++ )
    {
        cout << "+-"; // create bottom
    }
    cout << "+" << endl;
}

bool ScaffoldImpl::makeMove(int column, int color)
{
    if(column < 1 && color != RED && color != BLACK)
    {
        return false; // make sure parameters are valid
    }
    for( int i = m_lev-1; i >= 0; i-- )
    {
        if(grid[i][column-1] == VACANT) //go up from bottom to find first valid space
        {
            grid[i][column-1] = color; // assign grid spot to the correct color
            undoStack.push(column-1); // push column number of most recent move onto the undo stack
            return true;
        }
    }
    return false;
}

int ScaffoldImpl::undoMove()
{
    if(numberEmpty() == m_lev*m_cols)
        return 0;
    int uColumn = undoStack.top(); // look at the most recent move
    undoStack.pop(); // remove move from stack
    for( int i = 0; i < m_lev; i++ ) // look for occupied spot spot
    {
        if(grid[i][uColumn] != VACANT) //go up from bottom to find first occupied
        {
            grid[i][uColumn] = VACANT; // make grid spot vacant
            return uColumn;
        }
    }
    return 0;
}

//******************** Scaffold functions *******************************

// These functions simply delegate to ScaffoldImpl's functions.
// You probably don't want to change any of this code.

Scaffold::Scaffold(int nColumns, int nLevels)
{
    m_impl = new ScaffoldImpl(nColumns, nLevels);
}
 
Scaffold::~Scaffold()
{
    delete m_impl;
}
 
Scaffold::Scaffold(const Scaffold& other)
{
    m_impl = new ScaffoldImpl(*other.m_impl);
}
 
Scaffold& Scaffold::operator=(const Scaffold& rhs)
{
    if (this != &rhs)
    {
        Scaffold temp(rhs);
        swap(m_impl, temp.m_impl);
    }
    return *this;
}
 
int Scaffold::cols() const
{
    return m_impl->cols();
}

int Scaffold::levels() const
{
    return m_impl->levels();
}

int Scaffold::numberEmpty() const
{
    return m_impl->numberEmpty();
}

int Scaffold::checkerAt(int column, int level) const
{
    return m_impl->checkerAt(column, level);
}
 
void Scaffold::display() const
{
    m_impl->display();
}
 
bool Scaffold::makeMove(int column, int color)
{
    return m_impl->makeMove(column, color);
}
 
int Scaffold::undoMove()
{
    return m_impl->undoMove();
}
