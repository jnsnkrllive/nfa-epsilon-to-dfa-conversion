/*******************************************************************************
 *  @author             Karl Jansen (kmjansen@uw.edu)
 *  @version            1.2, 03/16/2015
 *
 *  Compilation:        $> g++ main.cpp -o main -std=c++11
 *  Execution:          $> main
 *  Dependencies:       CompiledDfa.cpp; CompiledNfaEpsilon.cpp;
 *                      convertNfaEpsilonToDfa.cpp;
 *
 *  Description:
 *  This program tests various classes for FiniteStateMachine objects.
 *
 *  Functionality:
 *  Creates valid FiniteStateMachine objects and compiles them into the 
 *  corresponding Finite Automaton object. Then tests basic functionality by
 *  evaluating a series of input string for recognition. Also, tests the
 *  conversion of a NFA-epsilon FiniteStateMachine to a DFA FiniteStateMachine,
 *  and tests equivalence by comparing the results of the evaluate method on the
 *  same input string.
 *
 *  Assumptions:
 *  NONE
 *
*******************************************************************************/

#include "CompiledDfa.cpp"
#include "CompiledNfaEpsilon.cpp"
#include "convertNfaEpsilonToDfa.cpp"
#include <iostream>

// Function Prototypes

/*******************************************************************************
 * This is the main driver function of the clientransition. It manages important
 * variables, and calls other functions to do various tasks.
 */
int main() {
   // (ab*|b*c|a*c*) as a NFA-e
   FiniteStateMachine fsmNFAe;
   std::unordered_set<int> nodes;
   nodes.insert(0);
   nodes.insert(1);
   nodes.insert(2);
   nodes.insert(3);
   nodes.insert(4);
   nodes.insert(5);
   nodes.insert(6);
   int startNode = 0;
   std::unordered_set<int> goalNodes;
   goalNodes.insert(2);
   goalNodes.insert(4);
   goalNodes.insert(5);
   goalNodes.insert(6);
   std::list<Transition> transitions;
   Transition transition;
   transition.source = 0;
   transition.transitionChar = FiniteStateMachine::EPSILON;
   transition.destination = 1;
   transitions.push_front(transition);
   transition.source = 1;
   transition.transitionChar = 'a';
   transition.destination = 2;
   transitions.push_front(transition);
   transition.source = 2;
   transition.transitionChar = 'b';
   transition.destination = 2;
   transitions.push_front(transition);
   transition.source = 0;
   transition.transitionChar = FiniteStateMachine::EPSILON;
   transition.destination = 3;
   transitions.push_front(transition);
   transition.source = 3;
   transition.transitionChar = 'b';
   transition.destination = 3;
   transitions.push_front(transition);
   transition.source = 3;
   transition.transitionChar = 'c';
   transition.destination = 4;
   transitions.push_front(transition);
   transition.source = 0;
   transition.transitionChar = FiniteStateMachine::EPSILON;
   transition.destination = 5;
   transitions.push_front(transition);
   transition.source = 5;
   transition.transitionChar = 'a';
   transition.destination = 5;
   transitions.push_front(transition);
   transition.source = 5;
   transition.transitionChar = FiniteStateMachine::EPSILON;
   transition.destination = 6;
   transitions.push_front(transition);
   transition.source = 6;
   transition.transitionChar = 'c';
   transition.destination = 6;
   transitions.push_front(transition);
   fsmNFAe.nodes = nodes;
   fsmNFAe.startNode = startNode;
   fsmNFAe.goalNodes = goalNodes;
   fsmNFAe.transitions = transitions;
   CompiledNfaEpsilon nfaEpsilon(fsmNFAe);

   // Convert NFA-e to DFA
   FiniteStateMachine fsmDFA = convertNfaEpsilonToDfa(fsmNFAe);
   CompiledDfa dfa(fsmDFA);

   // CREATE TEST CASES
   std::list<std::string> positiveStrings;
   std::list<std::string> negativeStrings;
   // Positive Cases
   positiveStrings.push_back("");
   positiveStrings.push_back("a");
   positiveStrings.push_back("aaaaaaaaa");
   positiveStrings.push_back("aaaaaaaaaccccccccc");
   positiveStrings.push_back("aaaaaaaaac");
   positiveStrings.push_back("ab");
   positiveStrings.push_back("abbbbbbbbb");
   positiveStrings.push_back("accccccccc");
   positiveStrings.push_back("bbbbbbbbbc");
   positiveStrings.push_back("bc");
   positiveStrings.push_back("c");
   positiveStrings.push_back("ccccccccc");
   // Negative Cases
   negativeStrings.push_back("aab");
   negativeStrings.push_back("aba");
   negativeStrings.push_back("aca");
   negativeStrings.push_back("b");
   negativeStrings.push_back("bbbbbbbbb");
   negativeStrings.push_back("bbbbbbbbbcc");
   negativeStrings.push_back("bcb");
   negativeStrings.push_back("ca");
   negativeStrings.push_back("cac");
   negativeStrings.push_back("cb");
   
   // RUN TEST CASES
   std::cout << ">> Positive Cases" << std::endl;
   for (std::string testStr : positiveStrings) {
      std::cout << testStr << std::endl;
      std::cout << std::boolalpha << (nfaEpsilon.isRecognized(testStr) && dfa.isRecognized(testStr)) << " : ";
      std::cout << std::boolalpha << nfaEpsilon.isRecognized(testStr) << " & ";
      std::cout << std::boolalpha << dfa.isRecognized(testStr) << std::endl;
   }
   std::cout << ">> Negative Cases" << std::endl;
   for (std::string testStr : negativeStrings) {
      std::cout << testStr << std::endl;
      std::cout << std::boolalpha << !(nfaEpsilon.isRecognized(testStr) && dfa.isRecognized(testStr)) << " : ";
      std::cout << std::boolalpha << nfaEpsilon.isRecognized(testStr) << " & ";
      std::cout << std::boolalpha << dfa.isRecognized(testStr) << std::endl;
   }
   std::cout << std::endl;

   // END
   return 0;
}
