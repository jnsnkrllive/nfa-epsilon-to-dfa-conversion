/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       FiniteStateMachine.cpp;
 *
 *  Description:
 *  The CompiledDfa class represents a Deterministic Finite Automaton.
 *
 *  Functionality:
 *  This class allows recognition checks to be performed on an input string.
 *
*******************************************************************************/

#ifndef COMPILEDDFA_H
#define COMPILEDDFA_H

#include "FiniteStateMachine.cpp"
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_set<int> UnorderedIntSet;
typedef std::unordered_map<TransitionPair, int, hashTransitionPair> MapTransitionPairToDestination;

class CompiledDfa {
   public:
      CompiledDfa(FiniteStateMachine&);         // overloaded constructor
   
      bool isRecognized(std::string);           // is recognized method

   private:
      CompiledDfa();                            // default constructor
   
      // internal FiniteStateMachine
      FiniteStateMachine internalFiniteStateMachine;
      // internal representation of the compiled DFA as a map
      MapTransitionPairToDestination dfaGraph;
   
      // helper methods
      void addTransitionToGraph(Transition);
      bool isGoalState(int);
      void processNextCharacter(char, int&);

};

#endif
