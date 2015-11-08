/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       NONE;
 *
 *  Purpose:
 *  This is the implementation of the CompiledDfa class.
 *
 *  Functionality:
 *  This class creates a compiled version of a Deterministic Finite Automaton
 *  which allows for O(k) evaluation and recognition of a string where k is the
 *  length of the input string.
 *
 *  Assumptions:
 *  A valid DFA FiniteStateMachine is passed into the constructor. This means
 *  that every unique pair of source node and transition character has exactly
 *  one corresponding destination node, and there are exactly zero epsilon
 *  transitions.
 *
*******************************************************************************/

#include "CompiledDfa.h"

/*******************************************************************************
 * Overloaded Constructor
 * This is public, and creates a Deterministic Finite Automaton (DFA) that can
 * be used to recognize strings. It takes in a valid Finite State Machine, and
 * sets up a local representation of the system for efficient use in the
 * evaluate method.
 * @param finiteStateMachine
 *                      a valid FiniteStateMachine
 */
CompiledDfa::CompiledDfa(FiniteStateMachine& originalFiniteStateMachine) {
   // Update Private Member Variables
   internalFiniteStateMachine = originalFiniteStateMachine;
   // Update Internal Representation
   for (const auto& transition : internalFiniteStateMachine.transitions) {
      addTransitionToGraph(transition);
   }
}

/*******************************************************************************
 * Is Recognized
 * This public method tries to recognize an input string with the internal
 * representation of the FSM provided in the constructor. It takes O(nk) time
 * to complete this process, where k is the length of the input string and n is
 * the number of nodes in the FSM.
 * @param inputStr      a string to check with this DFA
 * @return              true if the input string is recognized
 *                      false if the input string is not recognized
 */
bool CompiledDfa::isRecognized(std::string stringToTest) {
   int currentState = internalFiniteStateMachine.startNode;
   // Loop through the input string, checking for recognition
   for (int i = 0; i < stringToTest.length(); i++) {
      if (currentState == -1) {
         return false;
      }
      processNextCharacter(stringToTest[i], currentState);
   }
   return isGoalState(currentState);
}

/*******************************************************************************
 * Default Constructor
 * This is private, and cannot be accessed by a client using this class.
 */
CompiledDfa::CompiledDfa() {
   // Empty
}

/*******************************************************************************
 * Add Transition to Graph
 * A private helper method to add a transition from the original finite state
 * machine to the internal representation of the compiled DFA.
 * @param transitionToAdd
 *                      a Transition to add to the graph
 */
void CompiledDfa::addTransitionToGraph(Transition transitionToAdd) {
   TransitionPair currentTransitionPair(transitionToAdd.source, transitionToAdd.transitionChar);
   dfaGraph[currentTransitionPair] = transitionToAdd.destination;
}

/*******************************************************************************
 * Is Goal State
 * A private helper method to determine if the current states is also in
 * the set of goal states for the finite state machine.
 * @param states        an int representing a state
 */
bool CompiledDfa::isGoalState(int state) {
   if (internalFiniteStateMachine.goalNodes.count(state) > 0) {
      return true;
   }
   return false;
}

/*******************************************************************************
 * Process Next Character
 * A private helper method to process the next character in the input string
 * during the recognition algorithm. The method updates the current set of
 * states based on a valid transition from a current state on the transition
 * character being processed from the input string.
 * @param characterToProcess
 *                      the next character in the input string to recognize
 * @param currentStates a reference to a set of the current states
 */
void CompiledDfa::processNextCharacter(char characterToProcess, 
                                       int& currentState) {
   TransitionPair testTransitionPair(currentState, characterToProcess);
   if (dfaGraph.count(testTransitionPair) > 0) {
      currentState = dfaGraph.at(testTransitionPair);
   } else {
      currentState = -1;
   }
}
