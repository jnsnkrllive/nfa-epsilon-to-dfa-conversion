/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       FiniteStateMachine.h;
 *
 *  Description:
 *  This program converts a NFA-epsilon FiniteStateMachine into an equivalent
 *  DFA FiniteStateMachine.
 *
 *  Functionality:
 *  This function uses an algorithm to convert a formally defined Finite State
 *  Machine that represents a Non-Deterministic Finite Automaton with Epsilon
 *  Transitions into an equivalent Finite State Machine that is a Deterministic
 *  Finite Automaton.
 *
 *  Assumptions:
 *  The FiniteStateMachine passed into the function is a valid NFA-epsilon.
 *  The FiniteStateMachine returned from the function is a valid DFA.
 *
*******************************************************************************/

#include "FiniteStateMachine.h"
#include <functional>
#include <queue>
#include <unordered_map>
#include <unordered_set>

// Hash Function for Unordered Set of Integers 
struct hashIntSet {
   size_t operator()(const std::unordered_set<int>& setOfInts) const {
      size_t hashValue = 0;
      for (std::unordered_set<int>::const_iterator intItr = setOfInts.cbegin();
           intItr != setOfInts.cend();
           intItr++) {
         hashValue = hashValue ^ std::hash<int>()(*intItr);
      }
      return hashValue;
   }
};

// Definitions
typedef std::unordered_set<int> UnorderedIntSet;
typedef std::unordered_set<char> UnorderedCharSet;
typedef std::unordered_map<UnorderedIntSet, int, hashIntSet> MapStatesToInt;
typedef std::queue<UnorderedIntSet> QueueIntSets;

// Data for the conversion algorithm
struct ConversionData {
   FiniteStateMachine dfa;
   FiniteStateMachine nfaEpsilon;
   MapStatesToInt mapSetToDfaNode;
   int nodeNumber = 1;
   QueueIntSets pendingSetsOfNodes;
   UnorderedIntSet stateSet;
};

// Function Prototypes
FiniteStateMachine convertNfaEpsilonToDfa(const FiniteStateMachine&);
void getEpsilonClosure(UnorderedIntSet&, const std::list<Transition>&);
void getGoalNodesForDFA();
void getNextSetOfNodes(UnorderedIntSet&, const UnorderedIntSet&, char);
void getNextTransitionCharacters(UnorderedCharSet&, const UnorderedIntSet&);
void getStartNodeForDFA();
void processCurrentSetOfNodes();
void processNextSetOfNodes(const UnorderedIntSet&, const UnorderedIntSet&, char);
void processTransitionCharacters(const UnorderedCharSet&, const UnorderedIntSet&);

// Global Variables
ConversionData conversionData;

/*******************************************************************************
 * Convert NfaEpsilon to Dfa
 * Takes an NFA-epsilon and converts it to an equivalent DFA.
 * This process takes O(2^n) time where n is the number of nodes in the NFA-e.
 * @param inputNfaEpsilon
 *                      a reference to a NFA-epsilon FiniteStateMachine
 * @return              a DFA FiniteStateMachine
 */
FiniteStateMachine convertNfaEpsilonToDfa(const FiniteStateMachine& inputNfaEpsilon) {
   conversionData.nfaEpsilon = inputNfaEpsilon;
   getStartNodeForDFA();
   while (!conversionData.pendingSetsOfNodes.empty()) {
      processCurrentSetOfNodes();
   }
   getGoalNodesForDFA();
   return conversionData.dfa;
}

/*******************************************************************************
 * Get Epsilon Closure
 * A helper method to add nodes to the current state if an epsilon transition
 * exists.
 * @param states        a reference to a set of states
 * @param transitions   a reference to a list of Transitions
 */
void getEpsilonClosure(UnorderedIntSet& states,
                       const std::list<Transition>& transitions) {
   int inserts = 0;
   for (Transition transition : transitions) {
      if (states.count(transition.source) > 0 &&
          transition.transitionChar == FiniteStateMachine::EPSILON &&
          states.count(transition.destination) == 0) {
         states.insert(transition.destination);
         inserts++;
      }
   }
   // If there is a newly reachable epsilon transition
   // Recursively add more nodes to state set 
   if (inserts > 0) {
      getEpsilonClosure(states, transitions);
   }
}

/*******************************************************************************
 * Get Goal Nodes for DFA
 * A helper method to find the corresponding set of goal nodes from the
 * NFA-epsilon for the DFA.
 */
void getGoalNodesForDFA() {
   for (int node : conversionData.nfaEpsilon.goalNodes) {
      for (MapStatesToInt::const_iterator stateItr = conversionData.mapSetToDfaNode.cbegin();
           stateItr != conversionData.mapSetToDfaNode.cend();
           ++stateItr) {
         if ((stateItr->first).count(node) > 0) {
            int goalNode = conversionData.mapSetToDfaNode.at(stateItr->first);
            conversionData.dfa.goalNodes.insert(goalNode);
         }
      }
   }
}

/*******************************************************************************
 * Get Next Set Of Nodes
 * A helper method to find the next set of nodes from the current set of states
 * in the NFA-epsilon based on the provided transition character
 * @param nextSetOfNodes
 *                      a reference to an unordered set of integers
 * @param currentSetOfNodes
 *                      a reference to an unordered set of integers
 * @param nextCharacter a char representing the transition character to use
 */
void getNextSetOfNodes(UnorderedIntSet& nextSetOfNodes,
                       const UnorderedIntSet& currentSetOfNodes,
                       char nextCharacter) {
   for (Transition transition : conversionData.nfaEpsilon.transitions) {
      if (currentSetOfNodes.count(transition.source) > 0 &&
          nextCharacter == transition.transitionChar) {
         nextSetOfNodes.insert(transition.destination);
      }
   }
   getEpsilonClosure(nextSetOfNodes, conversionData.nfaEpsilon.transitions);
}

/*******************************************************************************
 * Get Next Transition Characters
 * A helper method to find the possible transition characters for the current
 * set of nodes in the NFA-epsilon.
 * @param nextTransitionChars
 *                      a reference to an unordered set of chars
 * @param currentSetOfNodes
 *                      a reference to an unordered set of integers
 */
void getNextTransitionCharacters(UnorderedCharSet& nextTransitionChars,
                                 const UnorderedIntSet& currentSetOfNodes) {
   for (Transition transition : conversionData.nfaEpsilon.transitions) {
      if (currentSetOfNodes.count(transition.source) > 0 &&
          transition.transitionChar != FiniteStateMachine::EPSILON) {
         nextTransitionChars.insert(transition.transitionChar);
      }
   }
}

/*******************************************************************************
 * Get Start Node for DFA
 * A helper method to determine the corresponding start node from the
 * NFA-epsilon for the DFA.
 */
void getStartNodeForDFA() {
   conversionData.stateSet.insert(conversionData.nfaEpsilon.startNode);
   getEpsilonClosure(conversionData.stateSet, conversionData.nfaEpsilon.transitions);
   conversionData.pendingSetsOfNodes.push(conversionData.stateSet);
   conversionData.mapSetToDfaNode[conversionData.stateSet] = conversionData.nodeNumber;
   conversionData.dfa.nodes.insert(conversionData.nodeNumber++);
   conversionData.dfa.startNode = conversionData.mapSetToDfaNode.at(conversionData.stateSet);
}

/*******************************************************************************
 * Process Current Set of Nodes
 * A helper method to get and process the transitions for the next set of nodes
 * on the pending queue.
 */
void processCurrentSetOfNodes() {
   UnorderedIntSet currentSetOfNodes = conversionData.pendingSetsOfNodes.front();
   conversionData.pendingSetsOfNodes.pop();
   UnorderedCharSet nextTransitionChars;
   getNextTransitionCharacters(nextTransitionChars, currentSetOfNodes);
   processTransitionCharacters(nextTransitionChars, currentSetOfNodes);
}

/*******************************************************************************
 * Process Next Set of Nodes
 * A helper method to update the map for the new set of nodes and add a
 * transition to the DFA from the current set of states to the next set of
 * states in the NFA-epsilon.
 * @param nextSetOfNodes
 *                      a reference to an unordered set of integers
 * @param currentSetOfNodes
 *                      a reference to an unordered set of integers
 * @param nextCharacter a char representing the transition character to use
 */
void processNextSetOfNodes(const UnorderedIntSet& currentSetOfNodes,
                           const UnorderedIntSet& nextSetOfNodes,
                           char nextCharacter) {
   if (conversionData.mapSetToDfaNode.count(nextSetOfNodes) == 0) {
      // Add next set of nodes to the pending queue and map it to a new node number
      conversionData.pendingSetsOfNodes.push(nextSetOfNodes);
      conversionData.mapSetToDfaNode[nextSetOfNodes] = conversionData.nodeNumber;
      conversionData.dfa.nodes.insert(conversionData.nodeNumber++);
   }
   // Add the corresponding DFA transition
   Transition theTransition;
   theTransition.source = conversionData.mapSetToDfaNode.at(currentSetOfNodes);
   theTransition.transitionChar = nextCharacter;
   theTransition.destination = conversionData.mapSetToDfaNode.at(nextSetOfNodes);
   conversionData.dfa.transitions.push_front(theTransition);
}

/*******************************************************************************
 * Process Transition Characters
 * A helper method to manage getting a processing the next set of nodes based
 * on the current set of transition characters.
 * @param nextTransitionChars
 *                      a reference to an unordered set of chars
 * @param currentSetOfNodes
 *                      a reference to an unordered set of integers
 */
void processTransitionCharacters(const UnorderedCharSet& nextTransitionChars,
                                 const UnorderedIntSet& currentSetOfNodes) {
   for (char nextCharacter : nextTransitionChars) {
      UnorderedIntSet nextSetOfNodes;
      getNextSetOfNodes(nextSetOfNodes, currentSetOfNodes, nextCharacter);
      processNextSetOfNodes(currentSetOfNodes, nextSetOfNodes, nextCharacter);
   }
}
