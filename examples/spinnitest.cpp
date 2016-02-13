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
  ssm::State& hallo1State = stateMachine.getState("Hallo1", true);
  ssm::State& hallo12State = hallo1State.getState("Hallo12", true);
  // test exception
  try{
    hallo1State.getState("unknown");
    std::cerr << "No Exception 1. Error." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "Exception 1 caught. OK. (" << exc.what() << std::endl;
  }
  try{
    hallo1State.getState("", true);
    std::cerr << "No Exception 2. Error." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "Exception 2 caught. OK. (" << exc.what() << std::endl;
  }

  ioService.run();
  
  return(0);
}