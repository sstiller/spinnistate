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
  std::shared_ptr<ssm::State> hallo1State = stateMachine.getState("Hallo1", true);
  hallo1State->addState(new ssm::State("Hallo11"));
  hallo1State->addState("Hallo12");
  // test exception
  try{
    hallo1State->addState("");
    std::cerr << "No Exception. Error." << std::endl;
    return(1);
  }catch(std::exception& exc)
  {
    std::cout << "Exception caught. OK. (" << exc.what() << std::endl;
  }

  ioService.run();
  
  return(0);
}