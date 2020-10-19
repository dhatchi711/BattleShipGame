/* 
 * File:   Agent.c
 * Author: Deepak
 *
 * Created on May 30, 2020, 5:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Agent.h"
#include "BOARD.h"
#include "Ascii.h"

/*
 * 
 */
typedef struct{
    AgentState state;
    int turnCounter;
}Agent;

static Agent agent;

void AgentInit(void){
    agent.state = AGENT_STATE_START;
    agent.turnCounter = 0;
}

AgentState AgentGetState(void){
    return agent.state;
}

void AgentSetState(AgentState newState){
    agent.state = newState;
}
