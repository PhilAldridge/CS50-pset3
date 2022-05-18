/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

//set blank tile position
int blankposi;
int blankposj;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }
    
    //set blank tile position
    blankposi = d-1;
    blankposj = d-1;

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    int num = d*d-1;  //sets value of largest number to work as a reverse counter
    
    for(int i=0; i<d; i++)
    {
        for(int j=0;j<d; j++)   //iterates through both i and j
        {
            if(i==d-1 && j>=d-3)    //catches special cases
            {
                if(j==d-1)
                {
                    board[i][j] = 100;  //value given for blank tile
                } else if(d%2!=0) {
                    board[i][j] = num;
                    num--;
                } else if(j==d-3) {     //switches position of 1 and 2 on even numbered boards
                    board[i][j] = 1;
                } else {
                    board[i][j] = 2;
                }
            } else {            //if not a special case, puts counter value on to current board position
                board[i][j] = num;
                num--;
            }
        }
    }
    
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i=0; i<d; i++)
    {
        for(int j=0;j<d; j++)       //iterates through i and j again
        {
            if(board[i][j]==100)    //blank tile
            {
                printf(" _  ");
            } else if (board[i][j]<10) {        //prints numbers and spaces (1 space extra for single-digit numbers so they line up)
                printf(" %i  ", board[i][j]);
            } else {
                printf("%i  ", board[i][j]);
            }
        }
        printf("\n");   //new line after each row completes
    }
    printf("\n");   //new line at end of drawing
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    if (tile<1) //makes sure 0 isn't entered (which could make blank tile go off the board)
    {
        return false;
    } 
    
    if(board[blankposi][blankposj+1] == tile)   //checks to the right of the blank tile. If user input is there, swaps places with blank tile
    {
        board[blankposi][blankposj+1] = 100;
        board[blankposi][blankposj] = tile;
        blankposj ++;
        return true;
    } else if(board[blankposi][blankposj-1] == tile) {  //checks left
        board[blankposi][blankposj-1] = 100;
        board[blankposi][blankposj] = tile;
        blankposj --;
        return true;
    } else if(board[blankposi+1][blankposj] == tile) {  //checks down
        board[blankposi+1][blankposj] = 100;
        board[blankposi][blankposj] = tile;
        blankposi ++;
        return true;
    } else if(board[blankposi-1][blankposj] == tile) {  //checks up
        board[blankposi-1][blankposj] = 100;
        board[blankposi][blankposj] = tile;
        blankposi --;
        return true;
    } else {    //return false if tile not found in a valid position;
        return false;
    }
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int counter = 1;
    
    for(int i=0; i<d; i++)
    {
        for(int j=0;j<d; j++)   //iterates through i and j again
        {
            if(i==d-1 && j==d-1)    //this means we are at the last position. All others tiles in place therefore game is won
            {
                return true;
            } else if (board[i][j] != counter) {    //if tile in current position is incorrect then game is not won
                return false;
            }
            counter ++;
        }
    }
    return false;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}