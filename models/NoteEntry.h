#pragma once
#include <string>
using namespace std;

class NoteEntry
{
public:
  string title;
  string content;

  NoteEntry(string t, string c)
  {
    title = t;
    content = c;
  }
};