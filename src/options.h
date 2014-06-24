// Last Modified: Mon 23 Jun 2014 10:11:31 AM EDT
/*******************************************************************************************
*
*   Framework for the Statistical Analysis of Particle-Laden Flows
*   
*   
*   Author  : Bruno Blais
*   File    : options.h
*
*   Description :   Routines for parsing the various options related to post-processing
*		    Available options :
*
*		    In developement :
*			- Per iteration averaging
*			- Individual particles trajectories with deterministic or random sampling
*			- Plane porosity profiles
*			- Cylidnrical porosity profiles
*
********************************************************************************************/

#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include "steps.h"
#include "plane.h"
#include "boxes.h"


namespace fs = boost::filesystem;

typedef std::vector<fs::path> vecPath; 

class options
{
private :
    bool averaging_;		    // Enables per iteration averaging
    bool trajectories_;		    // Enables storage of the particle trajectories
    bool planeOn_;		    // Enables planes of porosity
    bool batch_;		    // Enables a batch treatment of data and flushing out of data
    bool box_;			    // Enables region boxing for averaging analysis

    double dt_;			    // Timestep

    std::string caseLabel_;	    // Label to give to the post-processing of the information
    std::string path_;		    // Path of the folder to post-process
    std::string optionsPath_;	    // Path of the option files
    std::string outputPath_;	    // Path of the folder where to put the output files
    std::string extension_;	    // Extension that is considered in the dump file analysis
    vecPath filesPath_;		    // Individual paths to each files
    int nFiles_;		    // Number of files
    int batchFreq_;		    // Frequency of memory flushing for batch process
    
    // Information about boxes for averaging
    int nBox_;
    boxes *boxes_;		    // Cartesian boxes and cylindrical boxes are available
    
    // Informations about planes
    plane plane_;

public :
    options(int,char**);
    ~options();
    void getFilesIdentification();
    void setMode(int argc , char* argv[]);
    void setSteps (steps*);

    //Accessors
    bool getAveraging();
    bool getPlaneOn();
    int getNumberOfFiles();	    // Accessor for the number of files
    double getDt();
    std::string getLabel();
    std::string getPath();
    std::string getOutputPath();

    plane getPlane();
};
#endif
