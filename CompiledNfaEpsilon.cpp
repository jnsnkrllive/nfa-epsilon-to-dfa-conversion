/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       NONE;
 *
 *  Purpose:
 *  This is the implementation of the CompiledNfaEpsilon class.
 *
 *  Functionality:
 *  This class creates a compiled version of a Deterministic Finite Automaton
 *  which allows for O(nk) evaluation and recognition of a string where k is the
 *  length of the input string and n is the number of nodes in the FSM.
 *
 *  Assumptions:
 *  A valid NFA-epsilon FiniteStateMachine is passed into the constructor. This
 *  means that every unique pair of source node and transition character has at 
 *  least one corresponding destination node or there is at least one epsilon
 *  transition.
 *
*******************************************************************************/

#include "CompiledNfaEpsilon.h"

/*******************************************************************************
 * Overloaded Constructor
 * This is public, and creates a Non-Deterministic Finite Automaton with Epsilon
 * Transitions (NFA-e) that can be used to recognize strings. It takes in a
 * valid Finite State Machine, and sets up a local representation of the system
 * for efficient use in the evaluate method.
 * @param finiteStateMachine
 *                      a valid FiniteStateMachine
 */
CompiledNfaEpsilon::CompiledNfaEpsilon(FiniteStateMachine& originalFiniteStateMachine) {
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
 * @param inputStr      a string to check with this NfaEpsilon
 * @return              true if the input string is recognized
 *                      false if the input string is not recognized
 */
bool CompiledNfaEpsilon::isRecognized(std::string stringToTest) {
   UnorderedIntSet currentStates;
   currentStates.insert(internalFiniteStateMachine.startNode);
   getEpsilonClosure(currentStates);
   // Loop through the input string, checking for recognition
   for (int i = 0; i < stringToTest.length(); i++) {
      if (currentStates.empty()) {
         return false;
      }
      processNextCharacter(stringToTest[i], currentStates);
   }
   return isGoalState(currentStates);
}

/*******************************************************************************
 * Default Constructor
 * This is private, and cannot be accessed by a client using this class.
 */
CompiledNfaEpsilon::CompiledNfaEpsilon() {
   // Empty
}

/*******************************************************************************
 * Add Transition to Graph
 * A private helper method to add a transition from the original finite state
 * machine to the internal representation of the compiled NFA-epsilon.
 * @param transitionToAdd
 *                      a Transition to add to the graph
 */
void CompiledNfaEpsilon::addTransitionToGraph(Transition transitionToAdd) {
   TransitionPair currentTransitionPair(transitionToAdd.source, transitionToAdd.transitionChar);
   UnorderedIntSet destinations;
   if (nfaEpsilonGraph.count(currentTransitionPair) > 0) {
      destinations = nfaEpsilonGraph.at(currentTransitionPair);
   }
   destinations.insert(transitionToAdd.destination);
   nfaEpsilonGraph[currentTransitionPair] = destinations;
}

/*******************************************************************************
 * Get Epsilon Closure
 * A private helper method to add nodes to the current state if an epsilon
 * transition exists.
 * @param states        a reference to a set of states
 */
void CompiledNfaEpsilon::getEpsilonClosure(UnorderedIntSet& states) {
   int inserts = 0;
   for (auto sourceState : states) {
      TransitionPair epsilonTransitionPair(sourceState, EPSILON);
      if (nfaEpsilonGraph.count(epsilonTransitionPair) > 0) {
         for (const auto& destinationState : nfaEpsilonGraph.at(epsilonTransitionPair)) {
            if (states.count(destinationState) == 0) {
               states.insert(destinationState);
               inserts++;
            }
         }
      }
   }
   // If there is a newly reachable epsilon transition
   // Recursively add more nodes to the set of states 
   if (inserts > 0) {
      getEpsilonClosure(states);
   }
}

/*******************************************************************************
 * Is Goal State
 * A private helper method to determine if any of the current states is also in
 * the set of goal states for the finite state machine.
 * @param states        a set of states
 */
bool CompiledNfaEpsilon::isGoalState(UnorderedIntSet states) {
   for (const auto& state : states) {
      if (internalFiniteStateMachine.goalNodes.count(state) > 0) {
         return true;
      }
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
void CompiledNfaEpsilon::processNextCharacter(char characterToProcess, 
                                              UnorderedIntSet& currentStates) {
   UnorderedIntSet nextStates;
   for (const auto& sourceState : currentStates) {
      TransitionPair testTransitionPair(sourceState, characterToProcess);
      if (nfaEpsilonGraph.count(testTransitionPair) > 0) {
         for (const auto& destinationState : nfaEpsilonGraph.at(testTransitionPair)) {
            nextStates.insert(destinationState);
         }
      }
   }
   getEpsilonClosure(nextStates);
   currentStates = nextStates;
}
