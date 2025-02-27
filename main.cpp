#include <iostream>

#include "web/server.h"
using namespace melon::web;

// handler for default path "/"
void root(const Request& req, Response& resp)
{
  resp.render("index.html");
}

// handler for path "/hello"
void hello(const Request& req, Response& resp)
{
  std::string name = req.get("name");
  std::string age = req.get("age");
  resp.html("hello, name=" + name + ", age=" + age);
}

int main()
{
  Server* server = Singleton<Server>::getInstance();

  // server->bind("/", root);
  server->bind("/hello", hello);

  server->start();

  return 0;
}
