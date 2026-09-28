#pragma once

#include <fstream>
#include <vector>

#include "../models/ApiKeyEntry.h"
#include "../utils/Crypto.h"

using namespace std;

class ApiKeyService
{
public:
  static vector<ApiKeyEntry> load(string user)
  {
    vector<ApiKeyEntry> keys;

    ifstream file("data/" + user + ".keys");

    string line;

    while (getline(file, line))
    {
      size_t pos = line.find('|');

      if (pos == string::npos)
        continue;

      keys.push_back(
          ApiKeyEntry(
              Crypto::decrypt(line.substr(0, pos)),
              Crypto::decrypt(line.substr(pos + 1))));
    }

    return keys;
  }

  static void save(string user,
                   const vector<ApiKeyEntry> &keys)
  {
    ofstream file("data/" + user + ".keys");

    for (const auto &key : keys)
    {
      file << Crypto::encrypt(key.service)
           << "|"
           << Crypto::encrypt(key.key)
           << endl;
    }
  }

  static int count(string user)
  {
    return load(user).size();
  }
};