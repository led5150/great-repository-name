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
#include <time.h>

using namespace std;

const int size = 15;

void makeDefaultBoard(char board[size][size]);
void insertWalls(char board[size][size]);
bool printToFile(char board[size][size], char *argv[]);
void success(char *argv[]);
void tryagain(char board[size][size], char *argv[]);
void printBoard(char board[size][size]);


int main(int argc, char *argv[]) {

    // initialize random seed
    srand(time(NULL));
    // create 15 x 15 char array
    char board[size][size];

    if (argc == 2) {
        makeDefaultBoard(board);
        insertWalls(board);
        if (printToFile(board, argv)) {
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
    
    tryagain(board, argv);

    return 0;
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
// Parameters: char board   - a board of demensions size x size
//             char *argv[] - The filename user specifies
// Returns:    True if board was succesfully created, False if not
// Does:       Prints the board to the file the user specifies in the command
//             line.
bool printToFile(char board[size][size], char *argv[]) {

    ofstream out;
    out.open(argv[1]);
    if (! out.is_open()) {
        cerr << "ERROR: File could not be created" << endl;
        return false;
    }
    else {
        for (int i = 0; i < size; ++i) 
        {
            for (int j = 0; j < size; ++j)
            {
                out << board[i][j];
            }
            out << endl;
        }
    }
    return true;
}


// Function:   tryagain
// Parameters: char board   - a board of demensions size x size
//             char *argv[] - The filename user specifies
// Returns:    Nothing
// Does:       Allows the user to see the board that was just created on cout
//             to make sure it is something interesting and worth testing.
//             If the user is unhappy, they can re-make the board with 
//             new random walls.  Does so recursively within the function itelf
//             until user is happy and types n
void tryagain(char board[size][size], char *argv[]) {
    cout << "Your board looks like: \n\n"; 
    printBoard(board);
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
            makeDefaultBoard(board);
            insertWalls(board);
            if (printToFile(board, argv)) {
                success(argv);
            }
            else {
                cerr << "Board unable to be created" << endl;
                return;
            }
            tryagain(board, argv);
        }
        else {
            cout << "************** FILE NOT UPDATED *************" << endl;
            cout << "Please type 'y' or 'n'!!" << endl << endl;
            // input.clear();
            tryagain(board, argv);
        }
        return;
        
    }
    return;
}

void printBoard(char board[size][size]) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
        {
            cout << board[i][j];
        }
        cout << endl;
    }
}

void success(char *argv[]) {
    cout << "Your random board has been generated as " << argv[1] << " !\n";
    cout << "Lets GO Robots!" << endl;
}



