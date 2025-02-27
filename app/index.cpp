#include "app/index.h"
using namespace melon::app;

CONTROLLER(Index);

ACTION(Index, index);
void Index::index(const Request& req, Response& resp)
{
  resp.render("index/index.html");
}

ACTION(Index, hello);
void Index::hello(const Request& req, Response& resp)
{
  resp.render("index/hello.html");
}
