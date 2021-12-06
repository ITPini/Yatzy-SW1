/*
Navn: Marcelino Patrick Pini
Email: mpini21@student.aau.dk
Gruppe: Gruppe 8
Studieretning: Software
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define AMOUNT_OF_ROUNDS 15
#define AMOUNT_OF_DICES 5

enum yatzy_game {Ones, Twos, Threes, Fours, Fives, Sixes, One_Pair, Two_Pair,
           Three_Of_a_Kind, Four_Of_a_Kind, Small_Straight, Large_Straight,
           Full_House, Chance, Yatzy};

struct yatzy_round
{
    int round_number;
    int rolls[AMOUNT_OF_DICES];
    int count[6];
    int points;
};

typedef struct yatzy_round Rounds;

void roll_multiple_dies(int round_n, Rounds *round);
void count_dies(int round_n, Rounds *round);
//void generate_round_number(int number, Rounds *round);
void play_yatzy(int round_n, Rounds *round);
void calc_points(int round_n, int count, Rounds *round);
void calc_kind(int round_n, int search, Rounds *round);
void calc_two_pairs(int round_n, Rounds *round);
void calc_straight(int round_n, Rounds *round);
void calc_chance(int round_n, Rounds *round);
int calc_bonus(Rounds* round);
int calc_total_points(Rounds* round);

int main(){
    srand(time(NULL));
    Rounds round[AMOUNT_OF_ROUNDS];
    printf("\nScoreboard:\n");

    // Iterate through each round
    for(int i = 0; i < AMOUNT_OF_ROUNDS; i++){
        generate_round_number(i, round);
        roll_multiple_dies(i, round);
        count_dies(i, round);
        calc_bonus(round);

        play_yatzy(i, round);
    }

    printf("TOTAL    : %d \n", calc_total_points(round));

    return 0;
}
// Forbedring
/*
void generate_round_number(int number, Rounds *round){
    round[number].round_number = number; 
}
*/
// Generate n-size of dies and stores the value in struct array
void roll_multiple_dies(int round_n, Rounds *round){
    for(int i = 0; i < AMOUNT_OF_DICES; i++){
        round[round_n].round_number = round_n+1;
        round[round_n].rolls[i] = rand() % 6 + 1;
    }
}
// Contain every Yatzy turns
void play_yatzy(int round_n, Rounds *round){
    switch (round_n)
    {
    case Ones:
        printf("1-ere   ");
        calc_points(Ones, round[Ones].count[Ones], round);
        break;
    case Twos:
        printf("2-ere   ");
        calc_points(Twos, round[Twos].count[Twos], round);
        break;
    case Threes:
        printf("3-ere   ");
        calc_points(Threes, round[Threes].count[Threes], round);
        break;
    case Fours:
        printf("4-ere   ");
        calc_points(Fours, round[Fours].count[Fours], round);
        break;
    case Fives:
        printf("5-ere   ");
        calc_points(Fives, round[Fives].count[Fives], round);
        break;
    case Sixes:
        printf("6-ere   ");
        calc_points(Sixes, round[Sixes].count[Sixes], round);
        break;
    case One_Pair:
        printf("BONUS    : %d\n\n", calc_bonus(round)); // Print Bonus before lower section
        printf("Et par  ");
        calc_kind(One_Pair, 2, round);
        break;
    case Two_Pair:
        printf("To par  ");
        calc_two_pairs(Two_Pair, round);
        break;
    case Three_Of_a_Kind:
        printf("Tre ens ");
        calc_kind(Three_Of_a_Kind, 3, round);
        break;
    case Four_Of_a_Kind:
        printf("Fire ens");
        calc_kind(Four_Of_a_Kind, 4, round);
        break;
    case Small_Straight:
        printf("Lille   ");
        calc_straight(Small_Straight, round);
        break;
    case Large_Straight:
        printf("Store   ");
        calc_straight(Large_Straight, round);
        break;
    case Full_House:
        printf("Fuld hus");
        calc_two_pairs(Full_House, round);
        break;
    case Chance:
        printf("Chance  ");
        calc_chance(Chance, round);
        break;
    case Yatzy:
        printf("YATZY   ");
        calc_kind(Yatzy, 5, round);
        break;
    default:
        break;
    }

    printf(" : %d             ", round[round_n].points);

    for(int i = 0; i < AMOUNT_OF_DICES; i++){
        printf("%d ", round[round_n].rolls[i]);
    }
    
    printf("\n");
}
// Count total dies of each faces
void count_dies(int round_n, Rounds *round){
    int count = 0;
    for(int t = 0; t < 6; t++){
        for(int i = 0; i < AMOUNT_OF_DICES; i++){
            if(count != 5 && round[round_n].rolls[i] == t + 1){ 
                count++;
            }
        }
        round[round_n].count[t] = count;
        count = 0;
    }
}
// Used for calculation in Yatzy's upper section
void calc_points(int round_n, int count, Rounds *round){
    round[round_n].points = (round_n + 1) * count;
}
// Calculate points for 'n-size of a kind'
void calc_kind(int round_n, int search, Rounds *round){
    round[round_n].points = 0;
    for(int t = 5; t > 0; t--){
        if(round[round_n].count[t] >= search){
            if(round_n != Yatzy){
                round[round_n].points = search * (t + 1);
                break;
            } else {
                round[round_n].points = 50;
                break;
            }
        }
    }
}
// Calculate points for 'n-size of a kind' but for atleast two pairs
void calc_two_pairs(int round_n, Rounds *round){
    int first_pair = 0, second_pair = 0;
    int state = 0;
    round[round_n].points = 0;

    if(round_n == Two_Pair) {
        state = 2;
    } else if (round_n == Full_House) {
        state = 3;
    }

    for(int i = 0; i < 6; i++) {
        if(round[round_n].count[i] > state + 1) {
            first_pair = i + 1;
        }
    }
    for(int i = 0; i < 6; i++) {
        if(round[round_n].count[i] > 1 && i + 1 != first_pair) {
            second_pair = i + 1;
        }
    }

    if(first_pair && second_pair && round_n == Two_Pair) {
        round[round_n].points = (2 * first_pair) + (2 * second_pair);
    } else if (first_pair && second_pair && round_n == Full_House) {
        round[round_n].points = (3 * first_pair) + (2 * second_pair);
    }
}
// Calculate points for small & large straights
void calc_straight(int round_n, Rounds *round){
    int count = 0;
    int state = 0;
    round[round_n].points = 0;

    if(round_n == Small_Straight) { 
        state = 0;
    } else if (round_n == Large_Straight) { 
        state = 1;
    }

    for(int t = (0 + state); t < (5 + state); t++){
        if(round[round_n].count[t] >= 1){
            count++;
        }
    }

    if(count == 5 && round_n == Small_Straight){
        round[round_n].points = 15;
    } else if (count == 5 && round_n == Large_Straight){
        round[round_n].points = 20;
    }
}
void calc_chance(int round_n, Rounds *round){
    int chance = 0, count = 5;
    for(int t = 5; t >= 0; t--){
        if(round[round_n].count[t] > count){
            chance += (t + 1) * count;
            count = 0;
        } else {
            chance += (t + 1) * round[round_n].count[t];
            count -= round[round_n].count[t];
        }
    }
    round[round_n].points = chance;
}
int calc_bonus(Rounds* round){
    int bonus = 0;
    for(int i = 0; i < 6; i++){
        bonus += round[i].points;
    }
    if(bonus >= 63){
        return 50;
    } else {
        return 0;
    }
}
int calc_total_points(Rounds* round){
    int total_points = 0;
    total_points += calc_bonus(round);
    for(int i = 0; i < AMOUNT_OF_ROUNDS; i++){
        total_points += round[i].points;
    }
    return total_points;
}