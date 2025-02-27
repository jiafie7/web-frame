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

void post(const Request& req, Response& resp)
{
  // get data from the client's post request body
  std::string name = req.post("name");
  int age = req.post("age");

  // create a json object to store response data
  Json json;
  json["name"] = name;
  json["age"] = age;

  // generate response 
  resp.json(json.toString());
}

int main()
{
  Server* server = Singleton<Server>::getInstance();

  // server->bind("/", root);
  server->bind("/hello", hello);
  server->bind("/post", post);

  server->start();

  return 0;
}
