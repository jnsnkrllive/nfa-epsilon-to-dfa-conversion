/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       NONE;
 *
 *  Description:
 *  This struct is a collection of the data associated with a Transition in a
 *  Finite State Machine.
 *
 *  Functionality:
 *  Provides a publicly accessible data structure.
 *
*******************************************************************************/

#ifndef TRANSITION_H
#define TRANSITION_H

#include <functional>

struct Transition {
    int source;                                 // id of source node
    char transitionChar;                        // transition character
    int destination;                            // id of destination node
};

// Define the pair representing the source and transitionChar for use in a map
typedef std::pair<int, char> TransitionPair;

// Define the hash function for a TransitionPair
struct hashTransitionPair {
   size_t operator()(const TransitionPair& transitionPair) const {
      return std::hash<int>()(transitionPair.first) ^ std::hash<char>()(transitionPair.second);
   }
};

#endif
