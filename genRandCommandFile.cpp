/**********************************************************
 * Comp 15 - Fall 2019
 * 
 * Created by: Matt Champlin
 * 10/01/19
 *
 * Fun Progam that generates random command file output
 * for testing with MetroSim executables
 *********************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

const string MOVEMETRO     = "m m";
const string METROFINISH   = "m f";
const int    NUMSTATIONS   = 26;    // IMPORTANT! Change to match the  
                                    // number of stations in your station
                                    // file!! Default is for use with 
                                    // stations.txt
const bool   USEFFILENAME  = true;  // IMPORTANT! If this is set to true 
                                    // the program will insert one command
                                    // of type "f [filename]" into the 
                                    // command file!  Set to false to cancel
                                    // this behavior. 

const string NEWFILE       = "outB.txt"; // If USEFILENAME is true, this is the
                                         // name of the new file created with 
                                         // the "f [filename]" command.
bool   initialize(int argc, char *argv[], ofstream &out, int *numptr);
void   genFile(ofstream &out, int numPass);
void   exitMessage(char *argv[]);
string createRandPassenger();


int main(int argc, char *argv[]) {

    srand (time(NULL));
    ofstream commandFile;
    int numPass = 0;
    int *numptr = &numPass;

    if (! initialize(argc, argv, commandFile, numptr)) {
        cerr << "ERROR: Not able to initialize. Check Usage Error above." 
             << endl;
        return 0;
    }
    else {
        genFile(commandFile, numPass);
    }
    exitMessage(argv);
    return 0;
}


/******************************************************************************
*                                  UTILITIES                                  *
******************************************************************************/

// Function:   initialize
// Parameters: argc, argv, out, numptr
// Returns:    True if file given by argv[1] was able to be opened and program
//             was run with correct number of arguments in the command line
//             False if file cannot be opened or created, or wrong number
//             of arguments was given
// Does:       Opens the output file to be used to create the new command file
//             and takes in the number of passengers desired to be created.
bool initialize(int argc, char *argv[], ofstream &out, int *numptr) {
    string filename;
    char  *num;

    if (argc == 3) {
        filename = argv[1];
        num      = argv[2];
    }
    else {
        cerr << "Usage: ./genRandCommandFile [filename] [numPass]" << endl;
        return false;
    }
    out.open(filename);

    if (! out.is_open()) {
        cerr << "File Not Valid" << endl;
        return false;
    }
    *numptr = atoi(num);
    return true;
}

// Function:   genFile
// Parameters: ofstream &out - The desired output file
//             int numPass   - The desired number of passengers to be created
// Returns:    Nothing
// Does:       Prints newly generated passengers to the output file, followed 
//             by the "m m" (move metro) command for as many times as given
//             by the user in the command line (numPass).  Always prints the
//             "m f" command as the last output.  If USEFFILENAME is true,
//             the "f [filename]" command will be inserted right after the last
//             passenger is created, so we get a nice even distribution of 
//             passengers to both output files.
void genFile(ofstream &out, int numPass) {
    
    for (int i = 0; i < numPass; i++) {
        out << createRandPassenger() << endl;
        out << MOVEMETRO << endl;
    }
    if (USEFFILENAME) {
        out << "f " << NEWFILE << endl;
    }
    for (int i = 0; i < numPass * 3; i++) {
        out << MOVEMETRO << endl;
    }
    out << METROFINISH << endl;
}

// Function:   createRandPassenger
// Parameters: None
// Returns:    A string containing a properly formated but random passenger
//             command.
// Does:       Creates a passenger command string by randomly generating a 
//             number between 1 and the number of stations given by the 
//             const string NUMSTATIONS. CRITICAL: You MUST change this number
//             above to match your number of stations!  Memory leaks and other
//             "undefined behavior" may ensue if not properly handled!
string createRandPassenger() {
    string pass = "p";
    
    auto add1 = to_string(rand() % NUMSTATIONS + 1);
    auto add2 = to_string(rand() % NUMSTATIONS + 1);

    return pass + " " + add1 + " " + add2;
}

// Function:   exitMessage
// Parameters: char *argv - The file name given in the command line
// Returns:    Nothing
// Does:       Prints a fun exit message to user, ensuring them that all has
//             gone well and their file has succesfully been created with the
//             filename they specified.
void exitMessage(char *argv[]) {
    cout << "Your new command file has been randomly generated as " 
         << argv[1] << "." << endl;
    cout << ": ) : )       Happy Train Testing!    : ) : )" << endl;
}










