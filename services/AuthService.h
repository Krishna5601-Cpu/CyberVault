#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "../models/User.h"

using namespace std;

class AuthService
{
public:
  static bool registerUser()
  {
    string username, password;

    cout << "\nUsername: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    ifstream file("data/accounts.txt");
    string line;

    while (getline(file, line))
    {
      size_t pos = line.find(',');

      if (pos == string::npos)
        continue;

      if (line.substr(0, pos) == username)
        return false;
    }

    User user(username, password);

    ofstream out("data/accounts.txt", ios::app);
    out << user.username << "," << user.password << endl;

    ofstream vault("data/" + username + ".vault");
    vault.close();

    return true;
  }

  static bool loginUser(string &currentUser)
  {
    string username, password;

    cout << "\nUsername: ";
    cin >> username;

    cout << "Password: ";
    cin >> password;

    ifstream file("data/accounts.txt");
    string line;

    while (getline(file, line))
    {
      size_t pos = line.find(',');

      if (pos == string::npos)
        continue;

      string u = line.substr(0, pos);
      string p = line.substr(pos + 1);

      if (u == username && p == password)
      {
        currentUser = u;
        return true;
      }
    }

    return false;
  }
};