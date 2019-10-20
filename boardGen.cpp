/******************************************************************************
 * Comp 15 - Fall 2019
 * Proj 1 - Ricochet Robots
 * Matt Champlin
 * 10/19/19
 *
 * boardGen.cpp
 * 
 * This program creates a board with randomly generated walls and winning 
 * position!
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>

using namespace std;

const int size = 15;

void makeDefaultBoard(char board[size][size]);
void insertWalls(char board[size][size]);
bool printToFile(vector <char> addSpace, char *argv[]);
void success(char *argv[]);
void tryagain(vector <char> addSpace, char board[size][size], char *argv[]);
void printBoard(vector <char> addSpace);
vector<char> insertSpaces(char board[size][size]);




int main(int argc, char *argv[]) {

    // initialize random seed
    srand(time(NULL));

    // create 15 x 15 char array
    char board[size][size];

    // initialize a vector to be able to add spaces in the correct places
    vector <char> addSpace;

    // Generate that board!
    if (argc == 2) {
        makeDefaultBoard(board);
        insertWalls(board);
        addSpace = insertSpaces(board);

        if (printToFile(addSpace, argv)) {
            success(argv);
        }
        else {
            cerr << "Board unable to be created" << endl;
        }
    }
    else {
        cerr << "Usage: ./boardGen [filename]" << endl;
        cerr << "Try again!" << endl;
        return 0;
    }
    // Allow the user to see the board generated and decide whether or not to
    // keep it!  Some boards are inherently 'boring' and not useful for testing
    // unless the test is to get a No Solution outcome!  This option allows 
    // user to choose.
    tryagain(addSpace, board, argv);

    return 0;
}

// Function:   insertSpaces
// Parameters: char board - a board of demensions size x size
// Returns:    A vector with the board information plus spaces inserted
//             to ensure proper output format
// Does:       Initializes a temp vector, takes the board information from the
//             2d char board and inserts spaces after each board character to
//             ensure the proper format when printing to file.
vector<char> insertSpaces(char board[size][size]) 
{
    vector <char> temp;
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            temp.push_back(board[i][j]);
            if (j < size - 1){
                temp.push_back(' ');
            }
            
        }
    }
    return temp;
}

// Function:   makeDefaultBoard
// Parameters: char board - a board of demensions size x size
// Returns:    Nothing
// Does:       Initializes a board with no walls and one random winning 
//             position.
void makeDefaultBoard(char board[size][size]) {
    int insert[8] = {0, 2, 4, 6, 8, 10, 12, 14};
    int randi = insert[rand() % 7];
    int randj = insert[rand() % 7];
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
        {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    if (i == randi and j == randj) {
                        board[i][j] = '*';
                    }
                    else{
                        board[i][j] = 'X';
                    }
                }
                else {
                    board[i][j] = 'O';
                }
            }
            else {
                board[i][j] = 'O';
            }
        }
    }
}

// Function:   insertWalls
// Parameters: char board - a board of demensions size x size
// Returns:    Nothing
// Does:       Inserts proper vertical or horizontal walls at the correct spot
//             in the board array
void insertWalls(char board[size][size]) {
    char vertWall[10] = {'O','O','O','O','O','O','O','|','|','|'};
    char horzWall[10] = {'O','O','O','O','O','O','O','-','-','-'};
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
        {
            if (i % 2 == 0) {
                if (j % 2 != 0) {
                    board[i][j] = vertWall[rand() % 10];
                }
            }
            else {
                board[i][j] = horzWall[rand() % 10];
            }
        }
    }
}

// Function:   printToFile
// Parameters: vector addSpace - A vector with proper format 
//             char *argv[]    - The filename user specifies
// Returns:    True if board was succesfully created, False if not
// Does:       Prints the board to the file the user specifies in the command
//             line.
bool printToFile(vector <char> addSpace, char *argv[]) 
{

    ofstream out;
    out.open(argv[1]);
    if (! out.is_open()) {
        cerr << "ERROR: File could not be created" << endl;
        return false;
    }
    else {
        for (unsigned int i = 0; i < addSpace.size(); i++) {
            out << addSpace[i];
            if ((i + 1)  % 29 == 0) {
                out << endl;
            }
        }
    }
    return true;
}


// Function:   tryagain
// Parameters: char board   - a board of demensions size x size
//             vector addSpace - A vector with proper format
//             char *argv[] - The filename user specifies
// Returns:    Nothing
// Does:       Allows the user to see the board that was just created on cout
//             to make sure it is something interesting and worth testing.
//             If the user is unhappy, they can re-make the board with 
//             new random walls.  Does so recursively within the function itelf
//             until user is happy and types n
void tryagain(vector <char> addSpace, char board[size][size], char *argv[]) 
{
    cout << "Your board looks like: \n\n"; 
    printBoard(addSpace);
    char input = ' ';
    cout << "Would you like to try again? 'y' or 'n' " << endl;
    cin >> input;
    while (input != 'n') {
        if (input == 'n')
        {
            return;
        }
        if (input == 'y')
        {
            addSpace.clear();
            makeDefaultBoard(board);
            insertWalls(board);
            addSpace = insertSpaces(board);
            if (printToFile(addSpace, argv)) {
                success(argv);
            }
            else {
                cerr << "Board unable to be created" << endl;
                return;
            }
            tryagain(addSpace, board, argv);
        }
        else {
            cout << "************** FILE NOT UPDATED *************" << endl;
            cout << "Please type 'y' or 'n'!!" << endl << endl;
            tryagain(addSpace, board, argv);
        }
        return;
        
    }
    return;
}

// Function:   printBoard
// Parameters: vector addSpace - A vector with proper format
// Returns:    Nothing
// Does:       Prints the board to cout so user can see if they are happy with
//             it
void printBoard(vector <char> addSpace) 
{
    for (unsigned int i = 0; i < addSpace.size(); i++) {
            cout << addSpace[i];
            if ((i + 1)  % 29 == 0) {
                cout << endl;
            }
        }
}


// Function:   success
// Parameters: char argv - the filename specified by user
// Returns:    Nothing
// Does:       Prints to cout if board is successfully created
void success(char *argv[]) {
    cout << "Your random board has been generated as " << argv[1] << " !\n";
    cout << "Lets GO Robots!" << endl;
}



