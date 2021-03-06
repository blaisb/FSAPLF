/*******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : terminal.h
*
*   Description : Container for all the terminal interaction in the code
*
********************************************************************************************/

#ifndef TERMINAL_H
#define TERMINAL_H 
void terminalInit();
void terminalClose();
void terminalLoadBar(unsigned int,unsigned int,unsigned int);
void terminalLoadBarInit();
#endif

