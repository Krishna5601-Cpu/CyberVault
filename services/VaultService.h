#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "../models/PasswordEntry.h"
#include "../utils/Crypto.h"

using namespace std;

class VaultService
{
public:
  static vector<PasswordEntry> load(string currentUser)
  {
    vector<PasswordEntry> entries;

    ifstream file("data/" + currentUser + ".vault");
    string line;

    while (getline(file, line))
    {
      size_t p1 = line.find(',');
      size_t p2 = line.find(',', p1 + 1);

      if (p1 == string::npos || p2 == string::npos)
        continue;

      entries.push_back(
          PasswordEntry(
              Crypto::decrypt(line.substr(0, p1)),
              Crypto::decrypt(line.substr(p1 + 1, p2 - p1 - 1)),
              Crypto::decrypt(line.substr(p2 + 1))));
    }

    return entries;
  }

  static void save(string currentUser,
                   vector<PasswordEntry> entries)
  {
    ofstream file("data/" + currentUser + ".vault");

    for (auto entry : entries)
    {
      file << Crypto::encrypt(entry.platform) << ","
           << Crypto::encrypt(entry.email) << ","
           << Crypto::encrypt(entry.password)
           << endl;
    }
  }
};