// stl
#include <iostream>

// boost
#include <boost/asio.hpp>

// local includes
#include "../src/libspinnistate.h"

int main(void)
{
  boost::asio::io_service ioService;
  ssm::StateMachine stateMachine(ioService);

  // adding (sub)states
  ssm::State& hello1State = stateMachine.getState("Hello1", true);
  ssm::State& hello12State = hello1State.getState("Hello12", true);
  
  // check parents
  if(&hello1State != hello12State.getParent())
  {
    std::cerr << "Error: Wrong parent." << std::endl;
    return(1);
  }
  std::cout << "OK: Parent correct." << std::endl;
     
  // test exceptions
  try{
    hello1State.getState("unknown");
    std::cerr << "Error: No Exception 1." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 1 caught. (" << exc.what() << std::endl;
  }
  try{
    hello1State.getState("", true);
    std::cerr << "Error: No Exception 2." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "OK: Exception 2 caught. (" << exc.what() << std::endl;
  }

  ioService.run();
  
  return(0);
}