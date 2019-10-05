/*****************************************************************************
 * Comp 15 - Fall 2019
 * 
 * Created by: Matt Champlin
 * 10/01/19
 *
 * Fun Progam that generates random command file output
 * for testing with MetroSim executables
 *
 * NOTES:  10/4/19 - Fixed issue with output file distribution when user 
 *                   asked for small number of passengers (<= 25). File 
 *                   output was uneven, now it is based on the number of
 *                   passengers to be created, which creates a more even
 *                   distribution.
 *         10/4/19 - Added feature - If user runs with 0 or Negative 
 *                   passengers to be created, user is given option to 
 *                   run in Move Metro Only Mode. They also have the 
 *                   option to quit, and the program will "explode" :)
 ****************************************************************************/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <unistd.h>

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
bool   genFile(ofstream &out, int numPass);
void   exitMessage(char *argv[]);
void   errorMessage();
bool   setParameters(int *FCP, int *NM, int *IEM, int *NP);
bool   moveMetroMode(int *IEM);
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
        if (! genFile(commandFile, numPass)) {
            errorMessage();
            return 0;
        }
    }
    exitMessage(argv);
    return 0;
}


/******************************************************************************
*                                  UTILITIES                                  *
******************************************************************************/

// Function:   initialize
// Parameters: int argc      - number of parameters given in command line
//             char *argv[]  - given filename from argv[1] in command line
//             ofstream &out - an output file stream
//             int *numptr   - A pointer to the numPass int in main()
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
// Returns:    True if file created, False if failed
// Does:       Prints newly generated passengers to the output file, followed 
//             by the "m m" (move metro) command for as many times as given
//             by the user in the command line (numPass).  Always prints the
//             "m f" command as the last output.  If USEFFILENAME is true,
//             the "f [filename]" command will be inserted at varying 
//             percentages of the total number of passengers given by user.
//             This is to ensure that we get a nice even distribution 
//             of passengers to both output files.         
bool genFile(ofstream &out, int numPass) {
    int fileCommandPosition = 0;
    int numMoves = 1;
    int insertExtraMoves = 0;

    int *FCP = &fileCommandPosition;
    int *NM  = &numMoves;
    int *IEM = &insertExtraMoves;
    int *NP  = &numPass;

    if (! setParameters(FCP, NM, IEM, NP)) {
        return false;
    }

    for (int i = 0; i < numPass; i++) {
        out << createRandPassenger() << endl;
        out << MOVEMETRO << endl;
        if (USEFFILENAME) {
            if (i == fileCommandPosition and insertExtraMoves == 0) {
                out << "f " << NEWFILE << endl;
            }
        }
    }
    if (insertExtraMoves > 0) {
        for (int i = 0; i < insertExtraMoves; i++) {
            out << MOVEMETRO << endl;
        }
        if (USEFFILENAME) {    
            out << "f " << NEWFILE << endl;
        }
    }

    for (int i = 0; i < numPass * numMoves; i++) {
        out << MOVEMETRO << endl;
    }
    out << METROFINISH << endl;
    return true;
}

// Function:   setParameters *** Helper Function for genFile ***
// Parameters: int *FCP - A pointer to the fileCommandPosition int in genFile()
//             int *NM  - A pointer to the numMoves int in genFile()
//             int *IEM - A pointer to the insertExtraMoves int in genFile()
//             int *NP  - A pointer to the numPass int in genFile()
// Returns:    True if parameters are set, or Move Metro Only Mode is
//             engaged, False if not
// Does:       Sets parameters based on the user specified number of 
//             passengers.  This was implemented to fix situations where
//             small test sizes were asked for, which resulted in uneven
//             distribution of passengers to the two files.  Now the 
//             distribution is based on the number of passengers.
bool setParameters(int *FCP, int *NM, int *IEM, int *NP) {
    // Catch exception if user inputs 0 or a negative number for numPassengers
    // Allows user an "out" to test in Move Metro Only Mode.
    if (*NP <= 0) {
        if (! moveMetroMode(IEM)) {
            return false;
        }
    }
    if (*NP >=1 and *NP <= 10) {
        *NM  = *NM + 5;
        *IEM = NUMSTATIONS;
    }
    else if (*NP > 10 and *NP <= 24) {
        *NM  = *NM + 3;
        *IEM = *NP + (*NP * .6);
    }
    else if (*NP >= 25 and *NP < 50) {
        *NM  = *NM + 2;
        *FCP = *NP * .5;
        *IEM = NUMSTATIONS;
    }
    else if (*NP >= 50 and *NP < 100) {
        *NM  = *NM + 1.5;
        *FCP = *NP * .9;
    }
    else if (*NP >= 100) {
        *NM  = *NM + .1;
        *FCP = *NP * .8;
    }
    return true;
}

// Function:   moveMetroMode
// Parameters: int *IEM - A pointer to the insertExtraMoves int
// Returns:    True if user wants to run in Move Metro Only Mode, False if
//             not or there is bad input from user 
// Does:       Runs program in Move Metro Only Mode!  Will create a file
//             of user specified length with only the move metro command.
bool moveMetroMode(int *IEM) {
    string input;
    cout << "No Passengers to be created...Did you want to test in " 
            "Move Metro Only Mode?" << endl;
    cout << "type 'y' for yes and I'll insert a bunch of move metro "
            "commands" << endl;
    cout << "Type 'n' for no and to quit." << endl;
    cin >> input;
    while (input != "y" or input != "n") {
        if (input == "y") {
            int moves = 0;
            cout << "How many Move Metro commands would you like?" << endl;
            cin >> moves;
            if (cin.fail()) {
                cerr << "That's not a number!!" << endl;
                return false;
            }
            *IEM = moves;
            return true;
        }
        else if (input == "n") {
            return false;
        }
        else {
            cerr << "Bad Input, try again. 'y' or 'n' please" << endl;
            cin >> input;
        }
    }
    return true;
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

// Function:   errorMessage
// Parameters: None
// Returns:    Nothing
// Does:       Prints an error message to cerr if command file not able to 
//             be created for any reason.
void errorMessage() {
    cerr << "Command File was not able to be succesfully created..." << endl;
             cerr << "Possibly due to bad input, too few passengers or "
                     "some other (user) error. :( :( :(" << endl;
             cerr << "Get it right next time! Program exploding in..." << endl;
             cerr << "3" << endl;
             usleep(900000);
             cerr << "2" << endl;
             usleep(900000);
             cerr << "1" << endl;
             usleep(900000);
             cerr << "BOOM!" << endl;
}








