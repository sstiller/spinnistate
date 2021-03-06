// stl
#include <iostream>

// boost
#include <boost/asio.hpp>

// local includes
#include "../src/libspinnistate.h"
#include "../src/chai-script-data-model.h"

int main(void)
{
  boost::asio::io_service ioService;
  ssm::ChaiScriptDataModel dataModel;
  ssm::StateMachine stateMachine(ioService, &dataModel);

  // adding (sub)states
  ssm::State* hello1State = stateMachine.getState("Hello1", ssm::StateType::State, true);
  //ssm::State* hello12State = hello1State->getState("Hello12", ssm::StateType::State, true);
  ssm::State* hello2State = stateMachine.getState("Hello2", ssm::StateType::State, true);
  
  // check parents
//  if(hello1State != hello12State->getParent())
//  {
//    std::cerr << "Error: Wrong parent." << std::endl;
//    return(1);
//  }
//  std::cout << "OK: Parent correct." << std::endl;

  // test action container
  hello1State->addOnEntryAction("OnEntryFunction");
  hello1State->addOnExitAction("OnExitFunction");

  // test exceptions
  try{
    hello1State->getState("unknown");
    std::cerr << "Error: No Exception 1 (get not existing state)." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 1 caught: " << exc.what() << std::endl;
  }
  try{
    hello1State->getState("", ssm::StateType::State, true);
    std::cerr << "Error: No Exception 2 (invalid state name))." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 2 caught: " << exc.what() << std::endl;
  }
//  try{
//    hello12State->getState("Hello1", ssm::StateType::State, true);
//    std::cerr << "Error: No Exception 3 (double state name)." << std::endl;
//    return(1);
//  }catch(std::exception& exc)
//  {
//    std::cout << "OK: Exception 3 caught: " << exc.what() << std::endl;
//  }

  // data model
  try{
    bool result = stateMachine.getDataModel()->evaluateBool("1 < 3");
    if(!result)
    {
      std::cerr << "Error: dataModel.evaluateBool(\"1 < 3\") returned false." << std::endl;
      return(1);
    }
    result = stateMachine.getDataModel()->evaluateBool("4 < 3");
    if(result)
    {
      std::cerr << "Error: dataModel.evaluateBool(\"4 < 3\") returned true." << std::endl;
      return(1);
    }
  }catch(std::exception& exc)
  {
    std::cout << "Error: Exception caught for ChaiScriptDataModel Bool eval: " << exc.what() << std::endl;
    return(1);
  }
  std::cout << "OK: ChaiScriptDataModel" << std::endl;

  // transitions
  ssm::Transition* transition1_2 = hello1State->addTransition("trans 1-2",
                                                              "true");
  transition1_2->addEvent(ssm::Event("trigger1"));
  transition1_2->addTarget(hello2State);
  ssm::Transition* transition2_1 = hello2State->addTransition("trans 2-1",
                                                              "true");
  transition2_1->addEvent(ssm::Event("trigger2"));
  transition2_1->addTarget(hello1State);

  bool transTrue  = transition1_2->conditionsSatisfied(ssm::Event("trigger1"));
  if(!transTrue)
  {
    std::cout << "Error: transTrue not true" << std::endl;
    return(1);
  }
  std::cout << "OK: transTrue true" << std::endl;
    
  bool transFalse = transition1_2->conditionsSatisfied(ssm::Event());
  if(transFalse != false)
  {
    std::cout << "Error: transFalse not false" << std::endl;
    return(1);
  }
  std::cout << "OK: transFalse false" << std::endl;
//  if(! transition1_2->execute("trigger1"))
//  {
//    std::cout << "Error: Transition " << transition1_2->getName() << " was not executed!" << std::endl;
//    return(1);
//  }
//  std::cout << "OK: Transition " << transition1_2->getName() << " executed" << std::endl;


  // init state machine
  stateMachine.init("");
  // start machine
  stateMachine.start(); // go to initial (sub)state

  stateMachine.printConfiguration();
  stateMachine.processExternalEvent(ssm::Event("trigger1"));
  ioService.run();
  stateMachine.printConfiguration();
  
  return(0);
}