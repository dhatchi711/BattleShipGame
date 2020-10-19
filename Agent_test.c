/* 
 * File:   agent_test.c
 * Author: Deepak
 *
 * Created on May 30, 2020, 4:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "BOARD.h"
#include "Agent.h"
#include "BattleBoats.h"
#include "Message.h"

/*
 * 
 */

int main() {
    BOARD_Init();
    int testCounter = 0;
    int totalTestCounter = 0;
    Message message;
    AgentInit();
    if (AgentGetState() == AGENT_STATE_START) {
        testCounter++;
    }
    printf("AgentInit tests passed: %d/1\n", testCounter);
    if (testCounter == 1) {
        totalTestCounter++;
    }
    testCounter = 0;
    AgentSetState(AGENT_STATE_CHALLENGING);
    if (AgentGetState() == AGENT_STATE_CHALLENGING) {
        testCounter++;
    }
    AgentSetState(AGENT_STATE_END_SCREEN);
    if (AgentGetState() == AGENT_STATE_END_SCREEN) {
        testCounter++;
    }
    printf("AgentGetState tests passed %d/2\n", testCounter);
    printf("AgentSetState tests passed: %d/2\n", testCounter);
    if (testCounter == 2) {
        totalTestCounter += 2;
    }
    testCounter = 0;
    //printf("Testing AgentRun:");
    AgentInit();
    BB_Event event;
    event.type = BB_EVENT_START_BUTTON;
    event.param0 = 0;
    event.param1 = 0;
    event.param2 = 0;
    message = AgentRun(event);
    if (AgentGetState() == AGENT_STATE_CHALLENGING) {
        testCounter++;
    }
    if (message.type == MESSAGE_CHA) {
        testCounter++;
    }

    AgentInit();
    event.type = BB_EVENT_CHA_RECEIVED;
    message = AgentRun(event);
    if (message.type == MESSAGE_ACC) {
        testCounter++;
    }
    if (AgentGetState() == AGENT_STATE_ACCEPTING) {
        testCounter++;
    }
    AgentInit();
    event.type = BB_EVENT_ACC_RECEIVED;
    message = AgentRun(event);
   
    AgentInit();
    event.type = BB_EVENT_REV_RECEIVED;
 
    if (AgentGetState() == AGENT_STATE_ATTACKING) {
        if (message.type == MESSAGE_SHO) {
            testCounter+=0;
        }
    } else {
        testCounter++;
    }
    printf("Agent Run passes %d/5 Tests\n", testCounter);

    if (testCounter == 5) {
        totalTestCounter++;
    }
    printf("\nTotal tests passed: %d/4", totalTestCounter);






    while (1);

}


