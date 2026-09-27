#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "../models/NoteEntry.h"
#include "../utils/Crypto.h"

using namespace std;

class NoteService
{
public:
  static vector<NoteEntry> load(string currentUser)
  {
    vector<NoteEntry> notes;

    ifstream file("data/" + currentUser + ".notes");
    string line;

    while (getline(file, line))
    {
      size_t pos = line.find('|');

      if (pos == string::npos)
        continue;

      notes.push_back(
          NoteEntry(
              Crypto::decrypt(line.substr(0, pos)),
              Crypto::decrypt(line.substr(pos + 1))));
    }

    return notes;
  }

  static void save(string currentUser,
                   const vector<NoteEntry> &notes)
  {
    ofstream file("data/" + currentUser + ".notes");

    for (const auto &note : notes)
    {
      file << Crypto::encrypt(note.title)
           << "|"
           << Crypto::encrypt(note.content)
           << endl;
    }
  }
};

