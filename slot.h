// Luke Fallon
// 26 October 2013
// Slot Simulator

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define WHEELSIZE  30
#define NUMWHEELS   3
#define TWOX        1
#define RED7        2
#define PINK7       3
#define BLUE7       4
#define BAR5        5
#define BAR         6
#define STARTINGBAL 100

// To eventually be used between each game
struct player{
    char name[20];
    int balance;
    int winnings;
    int losses;
    int used;
};

int slotMain(struct player player[], int playerNumber);
void printSlotLogo();
int menu();
void playSlots(struct player player[], int playerNumber);
void createPayline(int array[]);
void createPaytable(int array[], char payTable[][7]);
void printPayline(char payTable[][7]);
void payOut(int wager, int array[], struct player player[], int playerNumber);
void showTable();
void updateSaves(struct player p[]);

int slotMain(struct player player[], int playerNumber){

    srand(time(NULL));

    printSlotLogo();

    // Initialization of player struct will take place outside slots eventually

    // Main loop of the game
    int choice = 1;
    while(choice!=3){
        // Get user input
        choice = menu();

        if(choice == 1)
            // Play the slot game
            playSlots(player, playerNumber);
        else if(choice == 2)
            // Look at the pay out table
            showTable();
        else if(choice == 3)
            // Quit
            printf("Thanks for playing!\n");
        else
            // Only executes if user did not choose a valid option
            printf("Sorry, that is not a valid input! Try again.\n\n");
    }
    return 0;
}
// Prints the "logo" of the Lucky Seven Slots parlor at the beginning of the game
void printSlotLogo(){
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("###                                   ###\n");
    printf("### Welcome to Lucky 7's Slot Parlor! ###\n");
    printf("###                                   ###\n");
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n");
}
// Prompts the user to enter a choice from a printed menu,
// returns the integer value of the choice.
int menu(){

    int choice;

    // Print the starting menu
    printf("Select an option from the menu below!\n");
    printf("1. Play some slots!\n");
    printf("2. View pay table.\n");
    printf("3. Quit\n");

    // Get user input
    printf(">> ");
    scanf("%d", &choice);

    return choice;
}
// Requires a struct of type player, the main loop of the slots game.
// Generates a random payline, sees if the user won anything, and
// display the net gain at the end of the session.
void playSlots(struct player player[], int playerNumber){

    int payline[NUMWHEELS];
    char paytable[NUMWHEELS][7];
    int i,j;
    int wager;

    while(1){

        wager = getBet(player, playerNumber);
        // If user choice to quit, exit immediately.
        if(wager == 0)
            break;
        // Pull the lever! (Generate 3 random numbers, 1 for each wheel)
        for(i = 0; i < NUMWHEELS; i++)
            payline[i] = (rand()%WHEELSIZE) + 1;

        createPayline(payline);
        createPaytable(payline, paytable);
        printPayline(paytable);
        payOut(wager, payline, player, playerNumber);
    }
    if(player[playerNumber].winnings >= player[playerNumber].losses)
        printf("\nIn this session you had a net gain of $%d.\n\n", player[playerNumber].winnings - player[playerNumber].losses);
    else
        printf("\nIn this session you had a net loss of $%d.\n\n", player[playerNumber].losses - player[playerNumber].winnings);

    updateSaves(player);
}
// Displays the user's account balance. Gets a wager from the user (1,2 or 3) and checks if valid.
int getBet(struct player player[], int playerNumber){

    int wager;

    printf("\nYour current balance is: $%d .\n", player[playerNumber].balance);
    printf("You may bet 1, 2, or 3 dollars. The more you wager, the more you win!\n");
    printf("How much would you like to wager? Note: To quit, enter 0.\n");
    printf(">> ");

    scanf("%d", &wager);



    int valid = 0;
    while(!valid){
        if(wager == 0)
            return 0;
        // Wager cannot be more than 3 or less than 1
        else if(wager < 1 || wager > 3){
            printf("Sorry, that is not a valid wager!\nHow much would you like to wager?\n");
            printf(">> ");
            scanf("%d", &wager);
        }
        // The user can not make a wager if they are out of money.
        else if(player[playerNumber].balance == 0){
            printf("Uh oh! Looks like you're out of money!\n");
            // Will enter a getMoreChips() function
            printf(">> ");
            scanf("%d", &wager);
        }
        // Wager cannot exceed present balance.
        else if(player[playerNumber].balance - wager < 0){
            printf("Uh oh! Looks like you don't have enough money for that wager!\n");
            printf("Please enter a smaller wager:\n");
            printf(">> ");
            scanf("%d", &wager);
        }
        // If non of these conditions flag, bet is valid
        else
            valid = 1;
    }
    // Update the balance
    player[playerNumber].balance -= wager;
    player[playerNumber].losses += wager;
    return wager;
}
// Convert the random numbers into numbers 0-6 relating to symbols
void createPayline(int array[]){
    int i;
    for(i = 0; i < NUMWHEELS; i++){
        if(array[i] > 21)
            array[i] = 0;
        else if(array[i] >= 21)
            array[i] = TWOX;
        else if(array[i] >= 19)
            array[i] = RED7;
        else if(array[i] >= 16)
            array[i] = PINK7;
        else if(array[i] >= 12)
            array[i] = BLUE7;
        else if(array[i] >= 7)
            array[i] = BAR5;
        else if(array[i] >= 1)
            array[i] = BAR;
    }
}
// Creates an array with the string representationg of the
// numberic values in createPayline
void createPaytable(int array[], char payTable[][7]){
    int i;
    for(i = 0; i < NUMWHEELS; i++){
        if(array[i] == TWOX)
            strcpy(payTable[i], "  2x  ");
        else if(array[i] == RED7)
            strcpy(payTable[i], "Red  7");
        else if(array[i] == PINK7)
            strcpy(payTable[i], "Pink 7");
        else if(array[i] == BLUE7)
            strcpy(payTable[i], "Blue 7");
        else if(array[i] == BAR5)
            strcpy(payTable[i], "5 BAR ");
        else if(array[i] == BAR)
            strcpy(payTable[i], "  BAR ");
        else if(array[i] == 0)
            strcpy(payTable[i], "  --- ");
    }
}
// Prints the spin given a char array from createPaytable
void printPayline(char payTable[][7]){
    printf("Here's the spin:");
    printf("\n**********************************\n");
    printf("*          *          *          *\n");
    printf("* %s   *  %s  *  %s  *\n", payTable[0], payTable[1], payTable[2]);
    printf("*          *          *          *\n");
    printf("**********************************\n\n");
}
// Given the wager, player struct, and numeric representation
// of payline, calculates winnings (if any) and updates the
// balance and total winnings.
void payOut(int wager, int array[], struct player player[], int playerNumber){

    int winnings = 0;

    if(array[0] == TWOX && array[1] == TWOX && array[2] == TWOX){
        winnings = 1200 * wager;
        printf("JACKPOT!! You won $%d!\n", winnings);
    }
    else if(array[0] == RED7 && array[1] == RED7 && array[2] == RED7){
        winnings = 200 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == PINK7 && array[1] == PINK7 && array[2] == PINK7){
        winnings = 100 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == BLUE7 && array[1] == BLUE7 && array[2] == BLUE7){
        winnings = 60 * wager;
        printf("WOAH!! You won $%d!\n", winnings);
    }
    else if(array[0] == BAR5 && array[1] == BAR5 && array[2] == BAR5){
        winnings = 50 * wager;
        printf("Nice! You won $%d!\n", winnings);
    }
    else if(array[0] == BAR && array[1] == BAR && array[2] == BAR){
        winnings = 10 * wager;
        printf("You won $%d!\n", winnings);
    }
    else if((array[0] == RED7 || array[0] == BLUE7 || array[0] == PINK7) && (array[1] == RED7 || array[1] == BLUE7 || array[1] == PINK7) && (array[2] == RED7 || array[2] == BLUE7 || array[2] == PINK7)){
        winnings = 30 * wager;
        printf("Nice! You won $%d!\n", winnings);
    }
    else if((array[0] == BAR5 || array[0] == BAR) && (array[1] == BAR5 || array[1] == BAR) && (array[2] == BAR5 || array[2] == BAR)){
        winnings = 5 * wager;
        printf("You won $%d!\n", winnings);
    }
    else if(array[0] == TWOX || array[1] == TWOX || array[2] == TWOX){
        winnings = 2 * wager;
        printf("You won $%d!\n", winnings);
    }
    else
        printf("Sorry, you didn't win anything on that spin!\n");
    printf("\n");
    // Add the winnings to the balance
    player[playerNumber].balance += winnings;
    player[playerNumber].winnings += winnings;
}
// Prints a visual representation of the payout for
// each winning spin combination.
void showTable(){
    printf("\n****************************************************");
    printf("\n*    Wager   *    1   *   2   *   3   *    Wager   *");
    printf("\n****************************************************");
    printf("\n*   3 of 2x  *  1,200 * 2,400 * 3,600 *   3 of 2x  *");
    printf("\n*  3 Red 7's *   200  *  400  *  600  *  3 Red 7's *");
    printf("\n* 3 Pink 7's *   100  *  200  *  300  * 3 Pink 7's *");
    printf("\n* 3 Blue 7's *    60  *  120  *  180  * 3 Blue 7's *");
    printf("\n*  3 5Bar's  *    50  *  100  *  150  *  3 5Bar's  *");
    printf("\n*  Any 3 7's *    30  *   60  *   90  *  Any 3 7's *");
    printf("\n*   3 Bar's  *    10  *   20  *   30  *   3 Bar's  *");
    printf("\n*  Any 3 Bar *     5  *   10  *   15  *  Any 3 Bar *");
    printf("\n*   1 of 2x  *     2  *    4  *    6  *   1 of 2x  *");
    printf("\n****************************************************\n\n");
}
