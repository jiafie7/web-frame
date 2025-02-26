#include <iostream>

#include "frame/server.h"
using namespace melon::frame;

int main()
{
  try
  {
    Singleton<Server>::getInstance()->start();
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << '\n';
  }

  return 0;
}
