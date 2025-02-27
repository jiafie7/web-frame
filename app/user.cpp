#include "app/user.h"
using namespace melon::app;

CONTROLLER(User);

ACTION(User, profile);
void User::profile(const Request & req, Response & resp)
{
  resp.render("user/profile.html");
}
