// main.cpp

#include "provided.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
    SmartPlayer bp("Homer");
    BadPlayer hp("Marge");
    Game g(3, 3, 3, &bp, &hp);
    g.play();
    
    /*Scaffold s(3, 2);
    s.makeMove(2, RED);
    s.display();
    s.makeMove(1, BLACK);
    s.display();
    s.makeMove(1, RED);
    s.display();
    //s.undoMove();
    //s.display();
    //s.undoMove();
    //s.display();
    //s.undoMove();
    //s.undoMove();
    //s.display();
    HumanPlayer sam("Sam");
    BadPlayer robot("Robot");
    //cout<<robot.chooseMove(s,3, BLACK);
    s.makeMove( robot.chooseMove(s,3, BLACK), BLACK);
    s.display();
    s.makeMove( robot.chooseMove(s,3, RED), RED);
    s.display();
    s.makeMove( robot.chooseMove(s,3, BLACK), BLACK);
    s.display();
    s.makeMove( robot.chooseMove(s,3, RED), RED);
    s.display();
    //cout << sam.chooseMove(s, 3, BLACK);*/
    
    
	/*
    string name;
	cout << "What's your name?  ";
	getline(cin, name);
	HumanPlayer hp(name);
	SmartPlayer sp("Computer");
	Game g1(4, 3, 3, &hp, &sp);
	g1.play();
	cout << endl << "This time I'll go first." << endl;
	Game g2(4, 3, 3, &sp, &hp);
	g2.play();
     */
}
