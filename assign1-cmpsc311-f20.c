////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmpsc311-f16-assign1.c
//  Description    : This is the main source code for for the first assignment
//                   of CMPSC311 at Penn State University.  See the related
//                   assignment page for details.
//
//   Author        : Steven Zeng
//   Last Modified : 9/17/2020
//

// Include Files
#include <stdio.h>
#include <cmpsc311_util.h>
#include <utime.h>
#include <gcrypt.h>

// Defines
#define NUM_CARDS 52
#define MAX_CARDS 11
#define SPADE   "\xE2\x99\xA0"
#define CLUB    "\xE2\x99\xA3"
#define HEART   "\xE2\x99\xA5"
#define DIAMOND "\xE2\x99\xA6"

// Functions

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_card
// Description  : print the card from the integer value
//
// Inputs       : card - the card to print
// Outputs      : 0 always

int print_card( int card ) {

    // CODE HERE
    char card_faces[] = "234567891JQKA";
    char *card_suits[] = { SPADE, CLUB, HEART, DIAMOND };
    int suit = card / 13, cardty = card % 13;
    if ( cardty == 8 ) {
        printf( "10%s  ", card_suits[suit] );
    } else {
        printf( "%c %s  ", card_faces[cardty], card_suits[suit] );
    }

    // Return zero
    return( 0 );
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : print_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always

int print_cards(int cards[], int num_cards) {
    for(int i =0; i<num_cards; i++){
        print_card(cards[i]);
        if(i>0 && (i+1)%13 == 0){
            printf("\n");
        }
    }
    return(0);
 
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : swap_cards
// Description  : given an array and 2 indexes, switches them
//
// Inputs       : cards - the array of cards
//                first - first index
//                second - second index
// Outputs      : None

void swap_cards(int cards[], int first, int second){
    int temp = cards[first];
    cards[first] = cards[second];
    cards[second] = temp;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : shuffle_cards
// Description  : print a number of cards (no more than 13 on one line)
//
// Inputs       : cards - the array of cards to print
//                num_cards - the number of cards to print
// Outputs      : 0 always
int shuffle_cards( int cards[], int num_cards) {
    for(int i = 0; i<num_cards-1; i++){
        swap_cards(cards, i, getRandomValue(i, 51));
    }
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : hand_value
// Description  : return the value of the hand
//
// Inputs       : cards - the array of cards in the hand
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int hand_value( int cards[], int num_cards ) {
    int temp;
    int total = 0;
    int total_aces = 0;
    for(int i = 0; i<num_cards; i++){
        temp = cards[i]%13+2;
        if(temp>10){
            if(temp<14){
                temp = 10;
            }
            else{
                temp=11;
                total_aces++;
            }
        }
        total+=temp;
    }
    if(total>21 && total_aces>0){
        for(int i = 0; i<total_aces; i++){
            total-=10;
            if(total<=21){
                break;
            }
        }
    }
    return(total);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : sort_cards
// Description  : sort a collection of cards
//
// Inputs       : hand - the cards to sort
//                num_cards - the number of cards in the hand
// Outputs      : 0 always

int sort_cards( int hand[], int num_cards ) {
    int checker;
    for(int i = 0; i<num_cards-1; i++){
        checker =0;
        for(int j = 0; j<num_cards-i-1; j++   ){
            if(hand[j]%13>hand[j+1]%13){
                swap_cards(hand, j, j+1);
                checker++;
            }else if(hand[j]/13>hand[j+1]/13 && hand[j]%13==hand[j+1]%13){
                swap_cards(hand, j, j+1);
                checker++;
            }
        }
        if(checker ==0){
            break;
        }
    }
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : dealer_play
// Description  : dealer decision to hit or stand (hit on anything less 
//                than 17)
//
// Inputs       : hand - cards dealer has
//                num_cards - the number of cards in player hand
// Outputs      : 0 = stand, 1 = hit

int dealer_play( int hand[], int num_cards ) {
    if(hand_value(hand, num_cards)<=16){
        return(1);
    }
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : player_play
// Description  : player decision to hit or stand
//
// Inputs       : hand - cards player has
//                num_cards - the number of cards in player hand
//                dealer_card - the dealers face up card
// Outputs      : 0 = stand, 1 = hit

int player_play( int hand[], int num_cards, int dealer[] ) {
    int player_value = hand_value(hand, num_cards);
    int dealer_value = hand_value(dealer, 1);
    if(player_value <20 && player_value<dealer_value){
        return(1);
    }
    else if(player_value<14){
        return(1);
    }
    return(0);
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : draw_card
// Description  : Pick a card from deck
//
// Inputs       : deck - the whole deck of cards
//                hand - the hand to receive the card
//                hand_count - number of cards in hand
//                deck_count - number of cards used in deck
// Outputs      : None
void draw_card(int hand[], int deck[], int *hand_count, int *deck_count){
    hand[*hand_count] = deck[*deck_count];
    *hand_count= *hand_count+1;
    *deck_count= *deck_count+1;
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : play_hand
// Description  : play a hand of black jack
//
// Inputs       : deck - the whole deck of cards
//                num_cards - the number of cards in the deck
//                player_money - pointer to player amount of money
// Outputs      : 1 if player wins, 0 if dealer wins
int play_hand( int deck[], int num_cards, float *player_money ) {
    shuffle_cards(deck, num_cards);
    int player_hand[11];
    int player_cards = 0;
    int dealer_cards = 0;
    int dealer_hand[11];
    int dealing_card = 0;
    printf("\n---- New hand ---- \n");
    draw_card(player_hand, deck, &player_cards, &dealing_card);
    draw_card(player_hand, deck, &player_cards, &dealing_card);
    draw_card(dealer_hand, deck, &dealer_cards, &dealing_card);
    printf("Dealer Cards: ");
    print_cards(dealer_hand, dealer_cards);
    printf(" XX \n");
    printf("Player Cards: ");
    print_cards(player_hand, player_cards);
    printf("\n");
    if(hand_value(player_hand, player_cards)== 21){
        *player_money+=7.5;
        printf("Player has Blackjack!, Wins $7.50\n");
        return(1);
    }
    while(hand_value(player_hand, player_cards)<=21){
        if(player_play(player_hand, player_cards, dealer_hand)){
            draw_card(player_hand, deck, &player_cards, &dealing_card);
            printf("Player hit (%d): ", hand_value(player_hand, player_cards));
            print_cards(player_hand, player_cards);
            printf("\n");
        }else{
            printf("Player stands (%d): ", hand_value(player_hand, player_cards));
            print_cards(player_hand, player_cards);
            printf("\n\n");
            break;
        }
    }
    if(hand_value(player_hand, player_cards)>21){
        printf("Player BUSTS ... dealer wins!");
        *player_money-=5;
        return(0);
    }
    while(hand_value(dealer_hand, dealer_cards)<=21){
         if(dealer_play(dealer_hand, dealer_cards)){
            draw_card(dealer_hand, deck, &dealer_cards, &dealing_card);
            printf("Dealer hit (%d): ", hand_value(dealer_hand, dealer_cards));
            print_cards(dealer_hand, dealer_cards);
            printf("\n");
        }else{
            printf("Dealer stands (%d): ", hand_value(dealer_hand, dealer_cards));
            print_cards(dealer_hand, dealer_cards);
            printf("\n\n");
            break;
        }
    }       
    if(hand_value(dealer_hand, dealer_cards)>21){
        printf("Dealer BUSTS ... player wins! \n");
        *player_money+=5;
        return(1);
    }
    if(hand_value(dealer_hand, dealer_cards)>=hand_value(player_hand, player_cards)){
        printf("Dealer wins \n");
        *player_money-=5;
        return(0);
    }else{
        printf("Player wins \n");
        *player_money+=5;
        return(1);
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// Function     : show_player_money_histogram
// Description  : Display a histogram of the player funds by hands
//
// Inputs       : money_rounds - the list of player money by hand number
//                last_round - the hand number where the game stopped
// Outputs      : 0 always

int show_player_money_histogram( float money_rounds[], int last_round ) {
    int height = 200;
    printf("                                             Player Cash by Round");
    printf("\n      ");
    for(int i = 1; i<101;i++){
            printf("-");
    }
    printf("\n");
    for(int i = 0; i < 41; i++){
        if(height<100){
            printf(" ");
        }
        if(height<10){
            printf(" ");
        }
        printf("%d | ", height);
        for(int j = 0; j<100; j++){
            if(height<money_rounds[j]){
                printf("X");
            }else{
                printf(".");
            }
        }
        printf("\n");
        height -=5;
    }
    printf("      ");
    for(int i = 1; i<101;i++){
        if(i%10 == 0){
            printf("%d", i/10);
        }else{
            printf(" ");
        }
    }
    printf("\n      ");
    for(int i = 1; i<101;i++){
        printf("%d", i%10);
    }
    return(0);
}


////////////////////////////////////////////////////////////////////////////////
//
// Function     : main
// Description  : The main function for the CMPSC311 assignment #1
//
// Inputs       : argc - the number of command line parameters
//                argv - the parameters
// Outputs      : 0 if successful test, -1 if failure

int main( int argc, char **argv ) {

    /* Local variables */
    int cmp311_deck[NUM_CARDS];  // This is the deck of cards
    float money;
    int wins;
    float money_rounds[100];
    /* Preamble information */
    printf( "CMPSC311 - Assignment #1 - Fall 2020\n\n" );
    srand(time(NULL)) ;

    /* Step #1 - create the deck of cards */
    for(int i = 0; i<52; i++){
        cmp311_deck[i] = i;
    }
    /* Step #2 - print the deck of cards */
    print_cards(cmp311_deck, 52);
    printf("\n");
    /* Step #4 - shuffle the deck */
    shuffle_cards(cmp311_deck, 52);
    /* Step #5 - print the shuffled deck of cards */
    print_cards(cmp311_deck, 52);
    printf("\n");
    /* Step #6 - sort the cards */
    sort_cards(cmp311_deck, 52);
    /* Step #7 - print the sorted deck of cards */
    print_cards(cmp311_deck, 52);
    printf("\n");
    /* Step #9 - deal the hands */
    money = 100;
    money_rounds[0] = 100;
    wins = 0;
    int counter = 0;
    while(counter<100 && money>=5){
        wins += play_hand(cmp311_deck, 52, &money);
        counter++;
        money_rounds[counter] = money;
        printf("After hand %d, player has %.2f left \n", counter, money);
        printf("------------\nBlackjack done - player won %d out of %d hands (%.2f).\n", wins, counter, money);
    }
    show_player_money_histogram(money_rounds, counter);
    /* Step 10 show historgrapm */
    /* Exit the program successfully */
    printf( "\n\nCMPSC311 - Assignment #1 - Spring 2020 Complete.\n" );
    return( 0 );
}