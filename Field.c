/* 
 * File:   Field.c
 * Author: Deepak
 *
 * Created on June 2, 2020, 7:57 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Field.h"
#include "BOARD.h"
/*
 * 
 */
/**
 * This function is optional, but recommended.   It prints a representation of both
 * fields, similar to the OLED display.
 * @param f The field to initialize.
 * @param p The data to initialize the entire field to, should be a member of enum
 *                     SquareStatus.
 */

#define NoBoatLife 0

void FieldPrint_UART(Field *own_field, Field * opp_field) {
    int i; //for iterating through rows
    int j; //for iterating through cols
    //prints both my field and opp field for visual tesing
    printf("my board vals\n");
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            printf("%d ", own_field->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    i = 0;
    j = 0;
    printf("opp board vals\n");
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            printf("%d ", opp_field->grid[i][j]);
        }
        printf("\n");
    }
}

/**
 * FieldInit() will initialize two passed field structs for the beginning of play.
 * Each field's grid should be filled with the appropriate SquareStatus (
 * FIELD_SQUARE_EMPTY for your own field, FIELD_SQUARE_UNKNOWN for opponent's).
 * Additionally, your opponent's field's boatLives parameters should be filled
 *  (your own field's boatLives will be filled when boats are added)
 * 
 * FieldAI_PlaceAllBoats() should NOT be called in this function.
 * 
 * @param own_field     //A field representing the agents own ships
 * @param opp_field     //A field representing the opponent's ships
 */
void FieldInit(Field *own_field, Field * opp_field) {
    int i; //for iterating through rows
    int j; //for iterating through cols
    for (i = 0; i < FIELD_ROWS; i++) {
        for (j = 0; j < FIELD_COLS; j++) {
            //set my grid to empty and opp grid to unknown
            opp_field->grid[i][j] = FIELD_SQUARE_UNKNOWN;
            own_field->grid[i][j] = FIELD_SQUARE_EMPTY;
        }
    }
    //set own field's boat lives to 0
    own_field->smallBoatLives = FIELD_SQUARE_EMPTY;
    own_field->mediumBoatLives = FIELD_SQUARE_EMPTY;
    own_field->largeBoatLives = FIELD_SQUARE_EMPTY;
    own_field->hugeBoatLives = FIELD_SQUARE_EMPTY;
    //set opp boat lives to corresponding boat lives
    opp_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
    opp_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
    opp_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
    opp_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
}

/**
 * Retrieves the value at the specified field position.
 * @param f     //The Field being referenced
 * @param row   //The row-component of the location to retrieve
 * @param col   //The column-component of the location to retrieve
 * @return  FIELD_SQUARE_INVALID if row and col are not valid field locations
 *          Otherwise, return the status of the referenced square 
 */
SquareStatus FieldGetSquareStatus(const Field *f, uint8_t row, uint8_t col) {
    if (row >= (FIELD_ROWS)) { //check for invalid row entry
        return FIELD_SQUARE_INVALID;
    }
    if (row < 0) { //check for invalid row entry (less than index 0)
        return FIELD_SQUARE_INVALID;
    }
    if (col >= (FIELD_COLS)) { //check for invalid col entry
        return FIELD_SQUARE_INVALID;
    }
    if (col < 0) { //check for invalid col entry (less than index 0)
        return FIELD_SQUARE_INVALID;
    } else { //return the current val of coordinates
        return f->grid[row][col];
    }
}

/**
 * This function provides an interface for setting individual locations within a Field struct. This
 * is useful when FieldAddBoat() doesn't do exactly what you need. For example, if you'd like to use
 * FIELD_SQUARE_CURSOR, this is the function to use.
 * 
 * @param f The Field to modify.
 * @param row The row-component of the location to modify
 * @param col The column-component of the location to modify
 * @param p The new value of the field location
 * @return The old value at that field location
 */
SquareStatus FieldSetSquareStatus(Field *f, uint8_t row, uint8_t col,
        SquareStatus p) {
    int tempVal; //store old val of coordinate
    tempVal = f->grid[row][col]; //set new val
    f->grid[row][col] = p; //set new val
    return tempVal; //return old val
}

/**
 * FieldAddBoat() places a single ship on the player's field based on arguments 2-5. Arguments 2, 3
 * represent the x, y coordinates of the pivot point of the ship.  Argument 4 represents the
 * direction of the ship, and argument 5 is the length of the ship being placed. 
 * 
 * All spaces that
 * the boat would occupy are checked to be clear before the field is modified so that if the boat
 * can fit in the desired position, the field is modified as SUCCESS is returned. Otherwise the
 * field is unmodified and STANDARD_ERROR is returned. There is no hard-coded limit to how many
 * times a boat can be added to a field within this function.
 * 
 * In addition, this function should update the appropriate boatLives parameter of the field.
 *
 * So this is valid test code:
 * {
 *   Field myField;
 *   FieldInit(&myField,FIELD_SQUARE_EMPTY);
 *   FieldAddBoat(&myField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_SMALL);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_MEDIUM);
 *   FieldAddBoat(&myField, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_TYPE_HUGE);
 *   FieldAddBoat(&myField, 0, 6, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_TYPE_SMALL);
 * }
 *
 * should result in a field like:
 *      0 1 2 3 4 5 6 7 8 9
 *     ---------------------
 *  0 [ 3 3 3 . . . 3 . . . ]
 *  1 [ 4 4 4 4 . . 3 . . . ]
 *  2 [ . . . . . . 3 . . . ]
 *  3 [ . . . . . . . . . . ]
 *  4 [ . . . . . . . . . . ]
 *  5 [ . . . . . . . . . . ]
 *     
 * @param f The field to grab data from.
 * @param row The row that the boat will start from, valid range is from 0 and to FIELD_ROWS - 1.
 * @param col The column that the boat will start from, valid range is from 0 and to FIELD_COLS - 1.
 * @param dir The direction that the boat will face once places, from the BoatDirection enum.
 * @param boatType The type of boat to place. Relies on the FIELD_SQUARE_*_BOAT values from the
 * SquareStatus enum.
 * @return SUCCESS for success, STANDARD_ERROR for failure
 */
uint8_t FieldAddBoat(Field *own_field, uint8_t row, uint8_t col, BoatDirection dir, BoatType boat_type) {
    //assign the boat to a local variable
    BoatType addingBoat = boat_type; //store boat type to check for boat length
    int i; //for iterating through rows
    int j; //for iterating through cols
    int rowStore = row; //temp row store for checking spacing for boat placing
    int colStore = col; //temp row store for checking spacing for boat placing
    int boatSize = 0; //keep track of boat's size for cross-checking
    int fieldBoatVal = 0; //stores the boat's size
    //check for boat size
    if (addingBoat == FIELD_BOAT_TYPE_SMALL) {
        boatSize = 3;
        fieldBoatVal = FIELD_SQUARE_SMALL_BOAT;
    }
    if (addingBoat == FIELD_BOAT_TYPE_MEDIUM) {
        boatSize = 4;
        fieldBoatVal = FIELD_SQUARE_MEDIUM_BOAT;
    }
    if (addingBoat == FIELD_BOAT_TYPE_LARGE) {
        boatSize = 5;
        fieldBoatVal = FIELD_SQUARE_LARGE_BOAT;
    }
    if (addingBoat == FIELD_BOAT_TYPE_HUGE) {
        boatSize = 6;
        fieldBoatVal = FIELD_SQUARE_HUGE_BOAT;
    }
    //if east direction is selected
    int invalidFlag = 0;
    if (dir == FIELD_DIR_EAST) {
        //check if required spaces for boat are empty
        for (i = 0; i < boatSize; i++) {
            if (FieldGetSquareStatus(own_field, row, colStore) == FIELD_SQUARE_INVALID) {
                invalidFlag = 1;
                return STANDARD_ERROR;
            }
            if (FieldGetSquareStatus(own_field, row, colStore) > 0) {
                invalidFlag = 1;
                return STANDARD_ERROR;
            }
            //increase index of column
            colStore++;
        }
        //if spaces are valid and empty continue adding boat
        //reset colStore to be reused
        colStore = col;
        if (invalidFlag == 0) {
            for (j = 0; j < boatSize; j++) {
                //adds the boat size number onto field
                own_field->grid[row][colStore] = fieldBoatVal;
                colStore++;
            }
            //BOAT TYPE
            if (addingBoat == FIELD_BOAT_TYPE_SMALL) {
                own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
            }
            if (addingBoat == FIELD_BOAT_TYPE_MEDIUM) {
                own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
            }
            if (addingBoat == FIELD_BOAT_TYPE_LARGE) {
                own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
            }
            if (addingBoat == FIELD_BOAT_TYPE_HUGE) {
                own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
            }
        }
    }
    if (dir == FIELD_DIR_SOUTH) {
        //check if required spaces for boat are empty
        for (i = 0; i < boatSize; i++) {
            if (FieldGetSquareStatus(own_field, rowStore, col) == FIELD_SQUARE_INVALID) {
                invalidFlag = 1;
                return STANDARD_ERROR;
            }
            if (FieldGetSquareStatus(own_field, rowStore, col) > 0) {
                invalidFlag = 1;
                return STANDARD_ERROR;
            }
            //increase index of column
            rowStore++;
        }
        //if spaces are valid and empty continue adding boat
        //reset rowStore for reuse
        rowStore = row;
        if (invalidFlag == 0) {
            for (j = 0; j < boatSize; j++) {
                //adds the boat size number onto field
                own_field->grid[rowStore][col] = fieldBoatVal;
                rowStore++;
            }
            if (addingBoat == FIELD_BOAT_TYPE_SMALL) {
                own_field->smallBoatLives = FIELD_BOAT_SIZE_SMALL;
            }
            if (addingBoat == FIELD_BOAT_TYPE_MEDIUM) {
                own_field->mediumBoatLives = FIELD_BOAT_SIZE_MEDIUM;
            }
            if (addingBoat == FIELD_BOAT_TYPE_LARGE) {
                own_field->largeBoatLives = FIELD_BOAT_SIZE_LARGE;
            }
            if (addingBoat == FIELD_BOAT_TYPE_HUGE) {
                own_field->hugeBoatLives = FIELD_BOAT_SIZE_HUGE;
            }
        }
    }
    return SUCCESS;
}

/**
 * This function registers an attack at the gData coordinates on the provided field. This means that
 * 'f' is updated with a FIELD_SQUARE_HIT or FIELD_SQUARE_MISS depending on what was at the
 * coordinates indicated in 'gData'. 'gData' is also updated with the proper HitStatus value
 * depending on what happened AND the value of that field position BEFORE it was attacked. Finally
 * this function also reduces the lives for any boat that was hit from this attack.
 * @param f The field to check against and update.
 * @param gData The coordinates that were guessed. The result is stored in gData->result as an
 *               output.  The result can be a RESULT_HIT, RESULT_MISS, or RESULT_***_SUNK.
 * @return The data that was stored at the field position indicated by gData before this attack.
 * placed the below for reference
typedef struct {
    uint8_t row; // Row of the coordinate that was guessed
    uint8_t col; // Column of the coordinate guessed
    ShotResult result; // result of a shot at this coordinate
 */
SquareStatus FieldRegisterEnemyAttack(Field *own_field, GuessData *opp_guess) {
    SquareStatus oldFieldVal = FieldGetSquareStatus(own_field,
            opp_guess->row, opp_guess->col); //set old val
    //set hit statements for each type of boat
    if (oldFieldVal == FIELD_SQUARE_SMALL_BOAT) {
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
        own_field->smallBoatLives--;
        if (own_field->smallBoatLives <= NoBoatLife) {
            opp_guess->result = RESULT_SMALL_BOAT_SUNK;
        } else {
            opp_guess->result = RESULT_HIT;
        }
    }
    if (oldFieldVal == FIELD_SQUARE_MEDIUM_BOAT) {
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
        own_field->mediumBoatLives--;
        if (own_field->mediumBoatLives <= NoBoatLife) {
            opp_guess->result = RESULT_MEDIUM_BOAT_SUNK;
        } else {
            opp_guess->result = RESULT_HIT;
        }
    }
    if (oldFieldVal == FIELD_SQUARE_LARGE_BOAT) {
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
        own_field->largeBoatLives--;
        if (own_field->largeBoatLives <= NoBoatLife) {
            opp_guess->result = RESULT_LARGE_BOAT_SUNK;
        } else {
            opp_guess->result = RESULT_HIT;
        }
    }
    if (oldFieldVal == FIELD_SQUARE_HUGE_BOAT) {
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_HIT;
        own_field->hugeBoatLives--;
        if (own_field->hugeBoatLives <= NoBoatLife) {
            opp_guess->result = RESULT_HUGE_BOAT_SUNK;
        } else {
            opp_guess->result = RESULT_HIT;
        }
    }
    if (oldFieldVal == FIELD_SQUARE_EMPTY) {
        own_field->grid[opp_guess->row][opp_guess->col] = FIELD_SQUARE_MISS;
        opp_guess->result = RESULT_MISS;
    }
    return oldFieldVal;
}

/**
 * This function updates the FieldState representing the opponent's game board with whether the
 * guess indicated within gData was a hit or not. If it was a hit, then the field is updated with a
 * FIELD_SQUARE_HIT at that position. If it was a miss, display a FIELD_SQUARE_EMPTY instead, as
 * it is now known that there was no boat there. The FieldState struct also contains data on how
 * many lives each ship has. Each hit only reports if it was a hit on any boat or if a specific boat
 * was sunk, this function also clears a boats lives if it detects that the hit was a
 * RESULT_*_BOAT_SUNK.
 * @param f The field to grab data from.
 * @param gData The coordinates that were guessed along with their HitStatus.
 * @return The previous value of that coordinate position in the field before the hit/miss was
 * registered.
 */
SquareStatus FieldUpdateKnowledge(Field *opp_field, const GuessData *own_guess) {
    //set oppField's coordinate val
    int fieldVal = opp_field->grid[own_guess->row][own_guess->col];
    //statements to update appropriate variables for each case(hit, miss or sunk)
    if (own_guess->result == RESULT_HIT) {
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_MISS) {
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_EMPTY;
    }
    if (own_guess->result == RESULT_SMALL_BOAT_SUNK) {
        opp_field->smallBoatLives = NoBoatLife;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_MEDIUM_BOAT_SUNK) {
        opp_field->mediumBoatLives = NoBoatLife;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_LARGE_BOAT_SUNK) {
        opp_field->largeBoatLives = NoBoatLife;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    if (own_guess->result == RESULT_HUGE_BOAT_SUNK) {
        opp_field->hugeBoatLives = NoBoatLife;
        opp_field->grid[own_guess->row][own_guess->col] = FIELD_SQUARE_HIT;
    }
    return fieldVal;
}

/**
 * This function returns the alive states of all 4 boats as a 4-bit bitfield (stored as a uint8).
 * The boats are ordered from smallest to largest starting at the least-significant bit. So that:
 * 0b00001010 indicates that the small boat and large boat are sunk, while the medium and huge boat
 * are still alive. See the BoatStatus enum for the bit arrangement.
 * @param f The field to grab data from.
 * @return A 4-bit value with each bit corresponding to whether each ship is alive or not.
 */
uint8_t FieldGetBoatStates(const Field *f) {
    //store the bitfield of all boat lives from huge to small
    BoatStatusFlag bitfield = NoBoatLife;
    //if a boat life doesn't equal NoBoatLife add to bitfield
    if (f->hugeBoatLives != NoBoatLife) {
        bitfield |= FIELD_BOAT_STATUS_HUGE;
    }
    if (f->largeBoatLives != NoBoatLife) {
        bitfield |= FIELD_BOAT_STATUS_LARGE;
    }
    if (f->mediumBoatLives != NoBoatLife) {
        bitfield |= FIELD_BOAT_STATUS_MEDIUM;
    }
    if (f->smallBoatLives != NoBoatLife) {
        bitfield |= FIELD_BOAT_STATUS_SMALL;
    }
    return bitfield;
}

/**
 * This function is responsible for placing all four of the boats on a field.
 * 
 * @param f         //agent's own field, to be modified in place.
 * @return SUCCESS if all boats could be placed, STANDARD_ERROR otherwise.
 * 
 * This function should never fail when passed a properly initialized field!
 */
uint8_t FieldAIPlaceAllBoats(Field *own_field) {
    int boatCount = 1; //variable to keep track of number of boats deployed
    int row;
    int col;
    int dirChoose; //variable to randomize direction of placement
    int boatNotDeployed = 1; //flag to check boat deployment
    //deploy from large to small boats
    //randomly choose row, col, dir and attempt placing until placed
    //then move onto to the next boat and repeat until boatCount = 4
    if (boatCount == 1) {
        while (boatNotDeployed) {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS;
            dirChoose = rand() % 2;//2 because options are either east or west
            if (dirChoose == 1) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_EAST, FIELD_BOAT_TYPE_HUGE) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
            if (dirChoose == 0) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_HUGE) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
        }
        boatCount++;
    }
    if (boatCount == 2) {
        boatNotDeployed = 1;
        while (boatNotDeployed) {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS;
            dirChoose = rand() % 2;//2 because options are either east or west
            if (dirChoose == 1) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_EAST, FIELD_BOAT_TYPE_LARGE) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
            if (dirChoose == 0) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_LARGE) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
        }
        boatCount++;
    }
    if (boatCount == 3) {
        boatNotDeployed = 1;
        while (boatNotDeployed) {
            row = rand() % FIELD_ROWS;
            col = rand() % FIELD_COLS;
            dirChoose = rand() % 2;//2 because options are either east or west
            if (dirChoose == 1) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_EAST, FIELD_BOAT_TYPE_MEDIUM) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
            if (dirChoose == 0) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_MEDIUM) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
        }
        boatCount++;
    }
    if (boatCount == 4) {
        boatNotDeployed = 1;
        while (boatNotDeployed) {
            row = rand() % FIELD_ROWS;
            col = rand() & FIELD_COLS;
            dirChoose = rand() & 2;//2 because options are either east or west
            if (dirChoose == 1) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_EAST, FIELD_BOAT_TYPE_SMALL) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
            if (dirChoose == 0) {
                if (FieldAddBoat(own_field, row, col, FIELD_DIR_SOUTH, FIELD_BOAT_TYPE_SMALL) == SUCCESS) {
                    boatNotDeployed = 0;
                };
            }
        }
        boatCount++;
    }
    return SUCCESS;
}

/**
 * Given a field, decide the next guess.
 *
 * This function should not attempt to shoot a square which has already been guessed.
 *
 * You may wish to give this function static variables.  If so, that data should be
 * reset when FieldInit() is called.
 * 
 * @param f an opponent's field.
 * @return a GuessData struct whose row and col parameters are the coordinates of the guess.  The 
 *           result parameter is irrelevant.
 */
GuessData FieldAIDecideGuess(const Field *opp_field) {
    GuessData guess;

    //while row col not unknown
    //keep calling it
    int row = rand() % FIELD_ROWS;
    int col = rand() % FIELD_COLS;
    while (opp_field->grid[row][col] != FIELD_SQUARE_UNKNOWN) {
        row = rand() % FIELD_ROWS;
        col = rand() % FIELD_COLS;
    }
    guess.row = row;
    guess.col = col;
    return guess;
}

/** 
 * For Extra Credit:  Make the two "AI" functions above 
 * smart enough to beat our AI in more than 55% of games.
 */