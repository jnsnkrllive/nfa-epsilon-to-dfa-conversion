/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       Transition.h;
 *
 *  Description:
 *  This struct is a collection of the data associated with a Finite State
 *  Machine.
 *
 *  Functionality:
 *  Provides a publicly accessible data structure.
 *
*******************************************************************************/

#ifndef FINITESTATEMACHINE_H
#define FINITESTATEMACHINE_H

#include "Transition.cpp"
#include <list>
#include <unordered_set>

typedef std::unordered_set<int> UnorderedIntSet;

struct FiniteStateMachine {

   static const char EPSILON = '\0';            // character for epsilon

   UnorderedIntSet nodes;                       // set of nodes in FSM
   int startNode;                               // the starting node of the FSM
   UnorderedIntSet goalNodes;                   // the set of goal nodes in FSM
   std::list<Transition> transitions;           // the list of transitions
};

#endif
