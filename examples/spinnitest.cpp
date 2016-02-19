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
  ssm::State* hello1State = stateMachine.getState("Hello1", true);
  ssm::State* hello12State = hello1State->getState("Hello12", true);
  ssm::State* hello2State = stateMachine.getState("Hello2", true);
  
  // check parents
  if(hello1State != hello12State->getParent())
  {
    std::cerr << "Error: Wrong parent." << std::endl;
    return(1);
  }
  std::cout << "OK: Parent correct." << std::endl;

  // test action container
  hello1State->setOnEntryAction("OnEntryFunction");
  hello1State->setOnExitAction("OnExitFunction");
  if(hello1State->getOnEntryAction() != "OnEntryFunction")
  {
    std::cerr << "Error: OnEntry wrong!." << std::endl;
    return(1);
  }
  std::cout << "OK: OnEntry correct." << std::endl;
  if(hello1State->getOnExitAction() != "OnExitFunction")
  {
    std::cerr << "Error: OnExit wrong!." << std::endl;
    return(1);
  }
  std::cout << "OK: OnExit correct." << std::endl;

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
    hello1State->getState("", true);
    std::cerr << "Error: No Exception 2 (invalid state name))." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 2 caught: " << exc.what() << std::endl;
  }
  try{
    hello12State->getState("Hello1", true);
    std::cerr << "Error: No Exception 3 (double state name)." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 3 caught: " << exc.what() << std::endl;
  }

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

  // start machine
  stateMachine.start(); // go to initial (sub)state

  // transitions
  ssm::Transition* transition1_2 = hello1State->addTransition("trans 1-2",
                                                         hello2State,
                                                         "trigger1",
                                                         "true");
  bool transTrue  = transition1_2->conditionsSatisfied("trigger1");
  if(!transTrue)
  {
    std::cout << "Error: transTrue not true" << std::endl;
    return(1);
  }
  std::cout << "OK: transTrue true" << std::endl;
    
  bool transFalse = transition1_2->conditionsSatisfied("");
  if(transFalse != false)
  {
    std::cout << "Error: transFalse not false" << std::endl;
    return(1);
  }
  std::cout << "OK: transFalse false" << std::endl;
  if(! transition1_2->execute("trigger1"))
  {
    std::cout << "Error: Transition " << transition1_2->getName() << " was not executed!" << std::endl;
    return(1);
  }
  std::cout << "OK: Transition " << transition1_2->getName() << " executed" << std::endl;

//  ioService.run();
  
  return(0);
}