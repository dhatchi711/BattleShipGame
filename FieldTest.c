/* 
 * File:   FieldTest.c
 * Author: Arka
 *
 * Created on June 2, 2020, 7:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include "Field.h"
#include "BattleBoats.h"
#include "BOARD.h"

/*
 * 
 */
int main() {
    BOARD_Init();
    int testCounter = 0;
    Field myField;
    Field oppField;
    Field my2;
    Field opp2;
    FieldInit(&myField, &oppField);
    FieldInit(&my2, &opp2);
    int i;
    int j;
    //Field Initialize Tests
    int initializeTest = 1;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            if ((myField.grid[i][j]) != 0) {
                initializeTest = 0;
            }
        }
    }
    i = 0;
    j = 0;
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            if ((oppField.grid[i][j]) != 5) {
                initializeTest = 0;
            }
        }
    }
    if (initializeTest == 0) {
        printf("Failed 2/2 Field Initializer Test\n");
    } else {
        testCounter++;
        printf("Passed 2/2 Field Initializer Test\n");
    }

    //Field Add Boat Test
    if ((FieldAddBoat(&myField, 0, 7, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) == SUCCESS)) {
        testCounter++;
    } else {
        printf("Failed Field Add Boat Test1\n");
    }
    if ((FieldAddBoat(&myField, 0, 0, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE) == SUCCESS)) {
        testCounter++;
    } else {
        printf("Failed Field Add Boat Test2\n");
    }
    if ((FieldAddBoat(&myField, 0, 2, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) == SUCCESS)) {
        testCounter++;
    } else {
        printf("Failed Field Add Boat Test3\n");
    }
    if ((FieldAddBoat(&myField, -1, 15, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) == STANDARD_ERROR)) {
        testCounter++;
    } else {
        printf("Failed Field Add Boat Test4\n");
    }
    if ((FieldAddBoat(&myField, 0, 6, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE) == SUCCESS)) {
        testCounter++;
    } else {
        printf("Failed Field Add Boat Test5\n");
    }
    FieldPrint_UART(&myField, &oppField);
    if (testCounter == 6) {
        printf("Passed 5/5 Field Add Boat Tests\n");
    }

    //Field Get Square Status Test
    if ((FieldGetSquareStatus(&myField, 8, -1)) == FIELD_SQUARE_INVALID) {
        testCounter++;
    } else {
        printf("Failed Get Square Status Test\n");
    }
    if ((FieldGetSquareStatus(&myField, 0, 0)) == FIELD_SQUARE_LARGE_BOAT) {
        testCounter++;
    } else {
        printf("Failed Get Square Status Test\n");
    }
    if ((FieldGetSquareStatus(&myField, 0, 6)) == FIELD_SQUARE_HUGE_BOAT) {
        printf("Passed 3/3 Get Square Status test\n");
        testCounter++;
    } else {
        printf("Failed Get Square Status Test\n");
    }
    //testCounter is 9
    if (FieldSetSquareStatus(&myField, 0, 0, 6) == FIELD_SQUARE_LARGE_BOAT) {
        testCounter++;
    } else {
        printf("Failed Set Square Status Test\n");
    }
    if (FieldSetSquareStatus(&myField, 0, 0, 3) == FIELD_SQUARE_HIT) {
        printf("Pass 2/2 Set Square Status Test\n");
        testCounter++;
    } else {
        printf("Failed Set Square Status Test\n");
    }
    //test counter is 11
    //Field Register Enemy Attack Tests
    //initialize Guess Data
    GuessData oppGuess;
    oppGuess.row = 0;
    oppGuess.col = 0;
    if (FieldRegisterEnemyAttack(&myField, &oppGuess) == 3) {
        if (myField.largeBoatLives == 4) {
            if (oppGuess.result == RESULT_HIT) {
                testCounter++;
            }
        }
        FieldUpdateKnowledge(&oppField, &oppGuess);
    }
    oppGuess.row = 1;
    oppGuess.col = 0;
    if (FieldRegisterEnemyAttack(&myField, &oppGuess) == 3) {
        if (myField.largeBoatLives == 3) {
            if (oppGuess.result == RESULT_HIT) {
                testCounter++;
            }
        }
        FieldUpdateKnowledge(&oppField, &oppGuess);
    }
    oppGuess.row = 2;
    oppGuess.col = 0;
    if (FieldRegisterEnemyAttack(&myField, &oppGuess) == 3) {
        if (myField.largeBoatLives == 2) {
            if (oppGuess.result == RESULT_HIT) {
                testCounter++;
            }
        }
        FieldUpdateKnowledge(&oppField, &oppGuess);
    }
    oppGuess.row = 3;
    oppGuess.col = 0;
    if (FieldRegisterEnemyAttack(&myField, &oppGuess) == 3) {
        if (myField.largeBoatLives == 1) {
            if (oppGuess.result == RESULT_HIT) {
                testCounter++;
            }
        }
        FieldUpdateKnowledge(&oppField, &oppGuess);
    }
    oppGuess.row = 4;
    oppGuess.col = 0;
    if (FieldRegisterEnemyAttack(&myField, &oppGuess) == 3) {
        if (myField.largeBoatLives == 0) {
            if (oppGuess.result == RESULT_LARGE_BOAT_SUNK) {
                testCounter++;
                printf("Passed 5/5 Register Enemy Attack Test\n");
            }
        }
        FieldUpdateKnowledge(&oppField, &oppGuess);
    }
    if (oppField.grid[0][0] == 6) {
        if (oppField.grid[4][0]) {
            printf("Passed 5/5 Field Update Knownledge Tests\n");
        }
    }
    //Field Get Boat States Test
    //Field Get boat is tested earlier as well, it is tested 2 times in total
    if (FieldGetBoatStates(&myField) == 11) {
        printf("Passed 2/2 Get Boat States Test\n");
    } else {
        printf("Failed Get Boat States Test\n");
    }
    FieldInit(&myField, &oppField);
    if (FieldAIPlaceAllBoats(&my2) == SUCCESS) {
    }
    FieldInit(&my2, &opp2);
    if (FieldAIPlaceAllBoats(&my2) == SUCCESS) {
        printf("Passed 2/2 Field AI Place All Boats Test\n");
    }
    GuessData guessData = FieldAIDecideGuess(&opp2);
    if (guessData.row < FIELD_ROWS) {
        if (guessData.col < FIELD_COLS) {
            testCounter++;
        }
    } else {
        printf("Failed Field AI Decide Guess\n");
    }
    guessData = FieldAIDecideGuess(&opp2);
    if (guessData.row < FIELD_ROWS) {
        if (guessData.col < FIELD_COLS) {
            testCounter++;
            printf("Passed 2/2 Field AI Decide Guess\n");
        }
    } else {
        printf("Failed Field AI Decide Guess\n");
    }
    FieldPrint_UART(&my2, &opp2);
    if (testCounter == 18) {
        printf("PASSED ALL TESTS FROM TEST HARNESS");
    }
    while (1);
}