#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "utils/Crypto.h"
#include <random>
#include "models/User.h"
#include "models/PasswordEntry.h"
#include "models/NoteEntry.h"
#include "utils/PasswordGenerator.h"
#include "services/AuthService.h"
#include "services/VaultService.h"

using namespace std;

// Forward declaration
void vaultMenu(string currentUser);
void passwordMenu(string currentUser);
void addPassword(string currentUser);
void viewPasswords(string currentUser);
void searchPassword(string currentUser);
void deletePassword(string currentUser);
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

      if (AuthService::loginUser(currentUser))
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
      if (AuthService::registerUser())
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
  vector<PasswordEntry> entries =
      VaultService::load(currentUser);

  string keyword;

  cout << "\nSearch: ";
  cin >> ws;
  getline(cin, keyword);

  bool found = false;

  cout << "\n========== Results ==========\n";

  cout << "\n========== Results ==========\n\n";

  for (auto entry : entries)
  {
    if (entry.platform.find(keyword) != string::npos)
    {
      cout << "🌐 " << entry.platform << endl;
      cout << "📧 " << entry.email << endl;
      cout << "🔑 " << entry.password << endl;
      cout << "----------------------------\n";

      found = true;
    }
  };

  if (!found)
  {
    cout << "No matching passwords found.\n";
  }
};

void deletePassword(string currentUser)
{
  vector<PasswordEntry> entries =
      VaultService::load(currentUser);

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

  VaultService::save(currentUser, updated);

  if (deleted)
    cout << "Password deleted successfully!\n";
  else
    cout << "Platform not found.\n";
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
