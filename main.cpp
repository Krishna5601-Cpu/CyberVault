#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utils/Crypto.h"
#include <random>

using namespace std;

class User
{
public:
  string username;
  string password;

  User(string u, string p)
  {
    username = u;
    password = p;
  }
};

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

// Forward declaration
bool registerUser();
bool loginUser(string &currentUser);
void vaultMenu(string currentUser);
void passwordMenu(string currentUser);
void addPassword(string currentUser);
void viewPasswords(string currentUser);
void searchPassword(string currentUser);
void deletePassword(string currentUser);
vector<PasswordEntry> loadVault(string currentUser);
void notesMenu(string currentUser);
void addNote(string currentUser);
void viewNotes(string currentUser);
void deleteNote(string currentUser);
vector<NoteEntry> loadNotes(string currentUser);
void passwordGeneratorMenu();

int main()
{
  int choice;

  while (true)
  {
    cout << "\n=================================\n";
    cout << "        CYBERVAULT v0.1\n";
    cout << "=================================\n";
    cout << "1. Login\n";
    cout << "2. Register\n";
    cout << "3. About\n";
    cout << "4. Exit\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
    {
      string currentUser;

      if (loginUser(currentUser))
      {
        cout << "\nWelcome " << currentUser << "!\n";
        vaultMenu(currentUser);
      }
      else
      {
        cout << "\nInvalid username or password.\n";
      }

      break;
    }
    case 2:
      if (registerUser())
        cout << "\n[ Registration successful! ]\n";
      else
        cout << "\n[ Username already taken! ]\n";
      break;

    case 3:
      cout << "\nCyberVault\n";
      cout << "Built using Modern C++\n";
      cout << "Developer: Krishna\n";
      break;

    case 4:
      cout << "\nThanks for using CyberVault!\n";
      return 0;

    default:
      cout << "\nInvalid choice!\n";
    }
  }
}

bool registerUser()
{
  string username, password;

  cout << "\n--- Register ---\n";
  cout << "Username: ";
  cin >> username;
  cout << "Password: ";
  cin >> password;

  // 1. Open accounts.txt for reading
  ifstream file("data/accounts.txt");
  string line;

  // 2. Read every line
  while (getline(file, line))
  {
    // Skip empty lines
    if (line.empty())
      continue;

    // 3. Split line by comma
    size_t pos = line.find(',');
    if (pos == string::npos)
      continue; // malformed line, skip

    string existingUser = line.substr(0, pos);

    // 4. If username already exists → return false
    if (existingUser == username)
    {
      file.close();
      return false;
    }
  }
  file.close();

  // 5. Append new user to accounts.txt
  ofstream out("data/accounts.txt", ios::app);

  if (!out)
  {
    cout << "Unable to create account database.\n";
    return false;
  }
  User newUser(username, password);

  out << newUser.username << "," << newUser.password << endl;
  out.close();

  // 6. Create an empty vault file for this user
  ofstream vault("data/" + username + ".vault");
  vault.close();

  // 7. Return true
  return true;
};

bool loginUser(string &currentUser)
{
  string username, password;

  cout << "\n--- Login ---\n";
  cout << "Username: ";
  cin >> username;

  cout << "Password: ";
  cin >> password;

  ifstream file("data/accounts.txt");

  if (!file)
  {
    cout << "No accounts found.\n";
    return false;
  }

  string line;

  while (getline(file, line))
  {
    size_t pos = line.find(',');

    if (pos == string::npos)
      continue;

    string existingUser = line.substr(0, pos);
    string existingPass = line.substr(pos + 1);

    if (existingUser == username &&
        existingPass == password)
    {
      currentUser = existingUser;
      file.close();
      return true;
    }
  }

  file.close();
  return false;
};

void vaultMenu(string currentUser)
{
  int choice;

  while (true)
  {
    cout << "\n=================================\n";
    cout << "      " << currentUser << "'s Vault\n";
    cout << "=================================\n";
    cout << "1. Password Manager\n";
    cout << "2. Secure Notes\n";
    cout << "3. API Keys\n";
    cout << "4. Password Generator\n";
    cout << "5. Logout\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      passwordMenu(currentUser);
      break;

    case 2:
      notesMenu(currentUser);
      break;

    case 3:
      cout << "\n[ API Keys coming soon ]\n";
      break;

    case 4:
      passwordGeneratorMenu();
      break;

    case 5:
      return;

    default:
      cout << "\nInvalid choice!\n";
    }
  }
};

void passwordMenu(string currentUser)
{
  int choice;

  while (true)
  {
    cout << "\n------ Password Manager ------\n";
    cout << "1. Add Password\n";
    cout << "2. View Passwords\n";
    cout << "3. Search Password\n";
    cout << "4. Delete Password\n";
    cout << "5. Back\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      addPassword(currentUser);
      break;
    case 2:
      viewPasswords(currentUser);
      break;
    case 3:
      searchPassword(currentUser);
      break;
    case 4:
      deletePassword(currentUser);
      break;
    case 5:
      return;
    default:
      cout << "Invalid choice!\n";
    }
  }
};

void addPassword(string currentUser)
{
  string platform, email, password;

  cout << "\nPlatform: ";
  cin >> ws;
  getline(cin, platform);

  cout << "Email: ";
  getline(cin, email);

  // Ask if user wants to generate a password automatically
  char choice;
  cout << "Generate password automatically? (y/n): ";
  cin >> choice;
  cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clear leftover newline

  if (choice == 'y' || choice == 'Y')
  {
    int length;
    cout << "Enter password length: ";
    cin >> length;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (length <= 0)
    {
      cout << "Invalid length. Using default length of 16.\n";
      length = 16;
    }

    password = PasswordGenerator::generate(length);
    cout << "Generated Password: " << password << "\n";
  }
  else
  {
    cout << "Password: ";
    getline(cin, password);
  }

  PasswordEntry entry(platform, email, password);

  ofstream file("data/" + currentUser + ".vault", ios::app);

  if (!file)
  {
    cout << "Unable to open vault.\n";
    return;
  }

  file << Crypto::encrypt(entry.platform) << ","
       << Crypto::encrypt(entry.email) << ","
       << Crypto::encrypt(entry.password) << endl;

  file.close();

  cout << "\nPassword saved successfully!\n";
};

void viewPasswords(string currentUser)
{
  ifstream file("data/" + currentUser + ".vault");

  if (!file)
  {
    cout << "Vault not found.\n";
    return;
  }

  string line;

  cout << "\n======= Saved Passwords =======\n";

  while (getline(file, line))
  {
    size_t p1 = line.find(',');
    size_t p2 = line.find(',', p1 + 1);

    string platform = line.substr(0, p1);
    string email = line.substr(p1 + 1, p2 - p1 - 1);
    string password = line.substr(p2 + 1);

    cout << "Platform : " << platform << endl;
    cout << "Email    : " << email << endl;
    cout << "Password : " << password << endl;
    cout << "-----------------------------\n";
  }

  file.close();
};

void searchPassword(string currentUser)
{
  vector<PasswordEntry> entries = loadVault(currentUser);

  string platform;

  cout << "\nSearch platform: ";
  cin >> ws;
  getline(cin, platform);

  bool found = false;

  for (auto entry : entries)
  {
    if (entry.platform == platform)
    {
      cout << "\nPlatform : " << entry.platform << endl;
      cout << "Email    : " << entry.email << endl;
      cout << "Password : " << entry.password << endl;

      found = true;
    }
  }

  if (!found)
    cout << "No password found.\n";
};

void deletePassword(string currentUser)
{
  vector<PasswordEntry> entries = loadVault(currentUser);

  string platform;

  cout << "\nEnter platform to delete: ";
  cin >> ws;
  getline(cin, platform);

  vector<PasswordEntry> updated;

  bool deleted = false;

  for (auto entry : entries)
  {
    if (entry.platform == platform && !deleted)
    {
      deleted = true;
      continue;
    }

    updated.push_back(entry);
  }

  ofstream file("data/" + currentUser + ".vault");

  for (auto entry : updated)
  {
    file << entry.platform << ","
         << entry.email << ","
         << entry.password << endl;
  }

  if (deleted)
    cout << "Password deleted successfully!\n";
  else
    cout << "Platform not found.\n";
};

vector<PasswordEntry> loadVault(string currentUser)
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
};

void notesMenu(string currentUser)
{
  int choice;

  while (true)
  {
    cout << "\n------ Secure Notes ------\n";
    cout << "1. Add Note\n";
    cout << "2. View Notes\n";
    cout << "3. Delete Note\n";
    cout << "4. Back\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      addNote(currentUser);
      break;
    case 2:
      viewNotes(currentUser);
      break;
    case 3:
      deleteNote(currentUser);
      break;
    case 4:
      return;
    default:
      cout << "Invalid choice!\n";
    }
  }
};

void addNote(string currentUser)
{
  string title, content;

  cout << "\nTitle: ";
  cin >> ws;
  getline(cin, title);

  cout << "Content: ";
  getline(cin, content);

  ofstream file("data/" + currentUser + ".notes", ios::app);

  file << Crypto::encrypt(title) << "|"
       << Crypto::encrypt(content)
       << endl;

  cout << "Note saved successfully!\n";
};

vector<NoteEntry> loadNotes(string currentUser)
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
};

void viewNotes(string currentUser)
{
  vector<NoteEntry> notes = loadNotes(currentUser);

  if (notes.empty())
  {
    cout << "No notes found.\n";
    return;
  }

  cout << "\n======= Notes =======\n";

  int i = 1;

  for (auto note : notes)
  {
    cout << i++ << ". " << note.title << endl;
    cout << note.content << endl;
    cout << "---------------------\n";
  }
};

void deleteNote(string currentUser)
{
  vector<NoteEntry> notes = loadNotes(currentUser);

  string title;

  cout << "\nEnter title to delete: ";
  cin >> ws;
  getline(cin, title);

  vector<NoteEntry> updated;

  bool deleted = false;

  for (auto note : notes)
  {
    if (note.title == title && !deleted)
    {
      deleted = true;
      continue;
    }

    updated.push_back(note);
  }

  ofstream file("data/" + currentUser + ".notes");

  for (auto note : updated)
  {
    file << Crypto::encrypt(note.title)
         << "|"
         << Crypto::encrypt(note.content)
         << endl;
  }

  if (deleted)
    cout << "Note deleted successfully!\n";
  else
    cout << "Title not found.\n";
};

void passwordGeneratorMenu()
{
  int length;

  cout << "\nPassword Length (8-32): ";
  cin >> length;

  if (length < 8 || length > 32)
  {
    cout << "Invalid length!\n";
    return;
  }

  string password = PasswordGenerator::generate(length);

  cout << "\nGenerated Password:\n";
  cout << password << endl;
};
