// Last Modified: Wed 26 Aug 2015 04:53:29 PM EDT
/******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : Averaging.h
*
*   Description : Container class for all the averaged properties of all files
*
*******************************************************************************************/

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>

#ifndef AVERAGING_H
#define AVERAGING_H 

class Averaging
{
private :
    int	     n_;	    // Number of averages stored
    int      cIndex_;        // Current iteration number
    int*     nIter_;	    // Iteration number
    double*  t_;	    // Time for each iteration
    //Averages
    double** vA_;	    // Velocity vector
    double** xA_;	    // Positions vector
    double** fA_;	    // Fluid-Solid interaction forces (fdrag)
    double** uA_;	    // Velocity of the fluid
    double*  rA_;	    // radiuses
    
    //Particle information
    int      np_;           // Number of particles
    double** x_;           // Position
    double** v_;           // Velocity
    double** f_;           // Force
    double** u_;           // Fluid Velocity
    double*  r_;           // Radiuses
    
    double   perc_;         // Percentiles

public :
    Averaging(int,char**,int);
    ~Averaging();
    void allocate(int);
    void setIter(int, int);
    void setV(int, std::vector<double>);
    void setX(int, std::vector<double>);
    void setF(int, std::vector<double>);
    void averageParticles(int, double**, double**, double**, double**);
    void calcAverage();
    std::vector<double> calcPercentageMax(double**, int, double);
    void writeFile(std::string, std::string);
    void writeSingleFile(std::string, std::string, std::string, double**, int);
};
#endif
