/******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : Particles.cpp
*
*   Description :   Manipulation of the information concerning the particle at a single
*		    iteration
*
*******************************************************************************************/


//*******************
//  GENERAL INCLUDES
//*******************
#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

/********************
* HEADER INCLUDES
********************/
#include "Particles.h"

#define PI 3.14159265359
#define verbose 0

Particles::Particles() : vAvg_(24), xAvg_(24), fAvg_(24), uAvg_(24) 
{
    np_ = 0;
    ids_ = NULL;
    v_ = NULL;
    x_ = NULL;
    f_ = NULL;
    r_ = NULL;
    u_ = NULL;
}

Particles::~Particles()
{
    for (int i = 0 ; i< np_ ; i++)
    {
	delete x_[i];
	delete v_[i];
	delete f_[i];
	delete u_[i];
    }
    delete x_;
    delete v_;
    delete f_;
    delete u_;
    delete ids_;
    delete r_;
}

void Particles::allocate(int n)
{
    //Allocate local memory for the information about the Particles
    np_ = n;
    
    ids_ = new int[n];
    r_ = new double[n];
    v_ = new double*[n];
    x_ = new double*[n];
    f_ = new double*[n];
    u_ = new double*[n];

    for (int i = 0 ; i < np_ ; i++)
    {
	v_[i] = new double[4];
	x_[i] = new double[4];
	f_[i] = new double[4];
	u_[i] = new double[4];
    }
}

void Particles::load(std::ifstream *ficIn)
{
    //Identifiers for the start tokens
    unsigned int x =9999;
    unsigned int v =9999;
    unsigned int f =9999;
    unsigned int id=9999;
    unsigned int r =9999;
    unsigned int u =9999;

    //Declaration
    std::string buffer;
    std::string line;
    std::vector<std::string> tokens;

    //Get the lines
    std::getline((*ficIn),buffer);

    //Parse the header of the files
    boost::algorithm::split(tokens, buffer, boost::algorithm::is_any_of(" "));

    //Loop through all the tokens to identify the variables (x,v,f,ids,r)
    for (unsigned int i=0 ; i < tokens.size() ; i++)
    {
        if(tokens[i]=="vx") v=i-2;
        if(tokens[i]=="fx") f=i-2;
        if(tokens[i]=="id") id=i-2;
        if(tokens[i]=="x") x=i-2;
        if(tokens[i]=="radius") r=i-2;
    }   
    
    // Sanity check to make sure x v f r ids have been found
    if (x>99 || v>99 || id>99 || r>99) std::cerr << "One of the core variable is missing (x,v,ids,r) x: "
        << x << " v: " << v << " id: "<< id << " radius : " << r << std::endl;


    // Begin of the file reader
    if(verbose) std::cout << "NEW GENERIC INPUT READER : " << std::endl; 
    for(int i=0 ; i<np_ ; i++)
    {
        //Cast into the right variable
        
            std::getline((*ficIn),buffer);
        /*
            boost::algorithm::split(tokens, buffer, boost::algorithm::is_any_of(" "));
                ids_[i] = atoi(tokens[id].c_str());
                r_[i]  = atof(tokens[r].c_str());
                for (int k=0 ; k<3 ; k++) x_[i][k] = atof(tokens[x+k].c_str());
                for (int k=0 ; k<3 ; k++) v_[i][k] = atof(tokens[v+k].c_str());
                for (int k=0 ; k<3 ; k++) f_[i][k] = atof(tokens[f+k].c_str());
                */


            std::istringstream row(buffer);
            double field;
            unsigned int j=0;
            while (row>> field)
            {
                if(verbose) std::cout<< field <<'\t';
                if (j==id) ids_[i] = int(field);
                if (ids_[i] >2*np_ && j==id) std::cout<<"j = " << j <<" Erreur" << ids_[i] << std::endl;
                else if (j==r) r_[i] = field;
                else if (j<(x+3) && j>=x)
                {
                    x_[i][j-x] = field;
                }
                else if (j<(f+3) && j>=f)
                {
                    f_[i][j-f] = field;
                }
                else if (j<(v+3) && j>=v)
                {
                    v_[i][j-v] = field;
                }
                j++;
            }
    }
    // Variables that are not present in the input file are zeroed
    if (u>100)
    {
        for (int i=0 ; i<np_ ; i++)
        {
            for (int j=0 ; j<3 ; j++) u_[i][j]=0.;
        }
    }
    if (f>100)
    {
        for (int i=0 ; i<np_ ; i++)
        {
            for (int j=0 ; j<3 ; j++) f_[i][j]=0.;
        }
    }

    //Calculate the norm of the vectors and store them for future use
    calcNorm();
}

void Particles::print()
{
    std::cout << "Averaged quantities \n";
    std::cout << "vAvg : " << vAvg_[0] << " " << vAvg_[1] << " " << vAvg_[2] << " " << vAvg_[3] << "\n" ;
    std::cout << "xAvg : " << xAvg_[0] << " " << xAvg_[1] << " " << xAvg_[2] << " " << xAvg_[3] << "\n" ;
    std::cout << "fAvg : " << fAvg_[0] << " " << fAvg_[1] << " " << fAvg_[2] << " " << fAvg_[3] << "\n" ;
    std::cout << "uAvg : " << uAvg_[0] << " " << uAvg_[1] << " " << uAvg_[2] << " " << uAvg_[3] << "\n" ;
}

void Particles::calcNorm()
{
    // Calculate the norm of each per-particle quantities
    for (int i=0 ; i<np_ ; i++)
    {
	v_[i][3]= sqrt(v_[i][0]*v_[i][0] + v_[i][1]*v_[i][1] + v_[i][2]*v_[i][2]);
	x_[i][3]= sqrt(x_[i][0]*x_[i][0] + x_[i][1]*x_[i][1] + x_[i][2]*x_[i][2]);
	f_[i][3]= sqrt(f_[i][0]*f_[i][0] + f_[i][1]*f_[i][1] + f_[i][2]*f_[i][2]);
	u_[i][3]= sqrt(u_[i][0]*u_[i][0] + u_[i][1]*u_[i][1] + u_[i][2]*u_[i][2]);
    }
}

//*************
// Accessors
//*************

int Particles::getNumber() {return np_;}

double* Particles::getV(int id)	{return v_[id];}

double* Particles::getX(int id) {return x_[id];}

double* Particles::getRArray() {return r_;}

double** Particles::getXArray() {return x_;}

double** Particles::getVArray() {return v_;}

double** Particles::getFArray() {return f_;}

double** Particles::getUArray() {return u_;}

std::vector<int> Particles::getIds()
{
    std::vector<int> ids(np_);
    
    for (int i=0 ; i<np_ ; i++)
    {
	ids[i]=ids_[i];
    }

    return ids;
}

int* Particles::getId() {return ids_;}


