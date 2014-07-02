// Last Modified: Wed 02 Jul 2014 11:37:40 AM EDT
/*******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : terminal.cpp
*
*   Description : Routines for interaction with the terminal in fsaplf
*
********************************************************************************************/

/*******************
*  GENERAL INCLUDES
********************/

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <cmath>

using namespace std;

void terminalInit()
{
    cout << endl << endl;
    cout << "******************************************************************" << endl;
    cout << "* FSAPLF                                                         *" << endl;
    cout << "* Framework for the Statistical Analysis of Particle-Laden Flows *" << endl;
    cout << "* Version 0.2						         *" << endl;
    cout << "* Created by Bruno Blais				         *" << endl;
    cout << "******************************************************************" << endl;
    cout << endl;
}


void terminalProgressInit(int n)
{
    cout << endl ;
    cout << "Progress Bar" << endl;
    cout << "0%   ";
    for (int i=0 ; i<(n/2-10) ; i++)
    {
    cout << " " ;
    }
    cout<<" 100%"<< endl;
}

void terminalProgress(int i, int n)
{
    if (i%2==0)
    {
	cout << "|" ;
	cout.flush();
    }
}



void terminalClose()
{
    cout << endl << endl;
    cout << "******************************************************************" << endl;
    cout << "* FSAPLF has exited normally                                     *" << endl;
    cout << "******************************************************************" << endl;
    cout << endl;
}
