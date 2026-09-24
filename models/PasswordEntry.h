#pragma once
#include <string>
using namespace std;

class PasswordEntry
{
public:
  string platform;
  string email;
  string password;

  PasswordEntry(string p, string e, string pass)
  {
    platform = p;
    email = e;
    password = pass;
  }
};