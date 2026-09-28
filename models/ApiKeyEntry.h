#pragma once
#include <string>
using namespace std;

class ApiKeyEntry
{
public:
  string service;
  string key;

  ApiKeyEntry(string s, string k)
  {
    service = s;
    key = k;
  }
};
