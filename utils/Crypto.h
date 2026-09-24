#pragma once
#include <string>
#include <random>
using namespace std;

class Crypto
{
public:
  static string encrypt(string text)
  {
    for (char &c : text)
      c += 3;

    return text;
  }

  static string decrypt(string text)
  {
    for (char &c : text)
      c -= 3;

    return text;
  }
};
