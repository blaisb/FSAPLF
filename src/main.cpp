/*******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : main.cpp
*
*   Description : Main file for FSAPLF code
*
********************************************************************************************/

/*******************
*  GENERAL INCLUDES
********************/
#include <iostream>
#include <cmath>
#include <string>
#include <iomanip>
#include <fstream>
#include <omp.h>

/********************
* HEADER INCLUDES
********************/
#include "options.h"
#include "steps.h"
#include "particles.h"
#include "averaging.h"
#include "terminal.h"
#include "trajectories.h"
#include "Pca.h"

using namespace std;

int main(int argc, char* argv[])
{
     // Initilization of the code
    terminalInit();

    // Declarations and constructors
    steps* stp;
    averaging avg;
    trajectories trj;


    //Options constructor parses the input from the terminal ---> this is to be deprecated
    options opt(argc, argv);

    // Constructors that use the new API
    Pca pca(argc, argv,opt.getNumberOfFiles());


    // Calculate number of files in the folder
    opt.getFilesIdentification();

    // Allocate the steps
    stp = new steps[opt.getNumberOfFiles()];
   
    // If averaging if on, allocate that up
    if(opt.getAveraging()) avg.allocate(opt.getNumberOfFiles());

    // Transfer information to each individual step
    opt.setSteps(stp);

    // Begin progress bar
    terminalLoadBarInit();
    
    // Parallelism will be done on this loop using openmp, it does not work right now 
    //#pragma omp parallel for
    for (int i=0 ; i<opt.getNumberOfFiles(); i++)
    {
	    stp[i].load();
	   // std::cout << "File iteration :\t"<<stp[i].getIter()<<std::endl;
	   
	    if (opt.getTrajectories() && i==0)
	    {
		trj.allocate(opt.getTrajectoriesType(), opt.getNumberOfFiles(),stp[i].getNumberParticles(),stp[i].getIds());
	    }
	   
            if (opt.getAveraging())
            {
                stp[i].average();
                avg.setIter(i,stp[i].getIter());
                avg.setV(i,stp[i].getAverageV());
                avg.setX(i,stp[i].getAverageX());
                avg.setF(i,stp[i].getAverageF());
	    }

	    if (opt.getPlaneOn())
	    {
		//Copy the plane class to the local steps
		stp[i].setPlane(opt.getPlane());
		stp[i].giveParticlesToPlane();
		stp[i].planeAnalysis();
		stp[i].writePlane(opt.getOutputPath(), opt.getLabel());
	    }

	    if (opt.getTrajectories())
	    {
		trj.setStep(i, stp[i].getNumberParticles(),stp[i].getIds(),stp[i].getXArray());
	    }

            if (opt.getPca())
            {
                pca.manage(stp[i].getIter(),stp[i].getNumberParticles(),stp[i].getId(),stp[i].getXArray());
                pca.write(opt.getOutputPath(), opt.getLabel(),stp[i].getIter());
            }

           // Memory flushing
            if (i%opt.getBatchFreq()==0 && i>opt.getBatchFreq())
            {
                //Flush the batchFreq_ previous iterations
                for (int j =(i-2*opt.getBatchFreq()) ; j<(i-opt.getBatchFreq())  ; j++)
                {
                   stp[j].~steps(); 
                }
            }

	    // Write progress bar
	   terminalLoadBar(i,opt.getNumberOfFiles(),50);
    }

    // Output of global results

    if (opt.getAveraging()) avg.writeFile(opt.getOutputPath(), opt.getLabel());

    terminalClose();
    return 0; 
}


