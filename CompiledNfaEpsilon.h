/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        N/A
 *  Execution:          N/A
 *  Dependencies:       FiniteStateMachine.cpp;
 *
 *  Description:
 *  The CompiledNfaEpsilon class represents a Non-Deterministic Finite Automaton
 *  with Epsilon transitions.
 *
 *  Functionality:
 *  This class allows recognition checks to be performed on an input string.
 *
*******************************************************************************/

#ifndef COMPILEDNFAEPSILON_H
#define COMPILEDNFAEPSILON_H

#include "FiniteStateMachine.cpp"
#include <unordered_map>
#include <unordered_set>

typedef std::unordered_set<int> UnorderedIntSet;
typedef std::unordered_map<TransitionPair, UnorderedIntSet, hashTransitionPair> MapTransitionPairToDestinations;

class CompiledNfaEpsilon {
   public:
      CompiledNfaEpsilon(FiniteStateMachine&);  // overloaded constructor
   
      bool isRecognized(std::string);           // is recognized method

   private:
      CompiledNfaEpsilon();                     // default constructor
   
      // local epsilon character
      const char EPSILON = FiniteStateMachine::EPSILON;
      // internal FiniteStateMachine
      FiniteStateMachine internalFiniteStateMachine;
      // internal representation of the compiled NFA-epsilon as a map
      MapTransitionPairToDestinations nfaEpsilonGraph;
   
      // helper methods
      void addTransitionToGraph(Transition);
      void getEpsilonClosure(UnorderedIntSet&);
      bool isGoalState(UnorderedIntSet);
      void processNextCharacter(char, UnorderedIntSet&);

};

#endif
