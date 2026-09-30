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
#include "utils/Colors.h"
#include <thread>
#include <chrono>
#include <limits>
#include "utils/Console.h"
#include "models/ApiKeyEntry.h"
#include "services/ApiKeyService.h"

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

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
void loadingScreen();
void apiKeyMenu(string currentUser);
void addApiKey(string currentUser);
void viewApiKeys(string currentUser);
void searchApiKey(string currentUser);
void deleteApiKey(string currentUser);
string maskApiKey(string key);
void revealApiKey(string currentUser);

int main()
{

  enableANSI();

  loadingScreen();

  int choice;

  while (true)
  {
    cout << BRIGHT_CYAN;
    cout << "============================================\n";
    cout << "              CYBERVAULT v1.0\n";
    cout << "============================================\n";
    cout << RESET;

    cout << GREEN << "[1] Login\n"
         << RESET;
    cout << BLUE << "[2] Register\n"
         << RESET;
    cout << MAGENTA << "[3] About\n"
         << RESET;
    cout << RED << "[4] Exit\n"
         << RESET;

    cout << "\n"
         << YELLOW << "Choose: " << RESET;

    if (!(cin >> choice))
    {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
      cout << "\nInvalid choice!\n";
      continue;
    }

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
        cout << BRIGHT_GREEN
             << "✓ Registration successful!"
             << RESET << endl;
      else
        cout << BRIGHT_RED
             << "✗ Invalid username or password"
             << RESET << endl;
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

#include "services/VaultService.h"
#include "services/NoteService.h"

void vaultMenu(string currentUser)
{
  int choice;

  while (true)
  {
    system("cls");

    int totalPasswords = VaultService::count(currentUser);
    int totalNotes = NoteService::count(currentUser);

    cout << CYAN << BOLD;
    cout << "=========================================\n";
    cout << "         " << currentUser << "'s CyberVault\n";
    cout << "=========================================\n";
    cout << RESET;

    cout << GREEN << "Passwords : " << totalPasswords << RESET << endl;
    cout << MAGENTA << "Notes     : " << totalNotes << RESET << endl;

    int totalKeys = ApiKeyService::count(currentUser);
    cout << BLUE << "API Keys  : " << totalKeys << RESET << endl;

    cout << "\n-----------------------------------------\n";

    cout << "[1] Password Manager\n";
    cout << "[2] Secure Notes\n";
    cout << "[3] API Keys\n";
    cout << "[4] Password Generator\n";
    cout << "[5] Logout\n";

    cout << "\nChoose: ";
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
      apiKeyMenu(currentUser);
      break;
    case 4:
      passwordGeneratorMenu();
      break;
    case 5:
      return;
    default:
      cout << "Invalid Option!\n";
    }
  }
}
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
  vector<PasswordEntry> entries = VaultService::load(currentUser);

  if (entries.empty())
  {
    cout << "\nVault empty or file not found.\n";
    return;
  }

  cout << "\n======= Saved Passwords =======\n";

  for (auto entry : entries)
  {
    cout << "Platform : " << entry.platform << endl;
    cout << "Email    : " << entry.email << endl;
    cout << "Password : " << entry.password << endl;
    cout << "-----------------------------\n";
  }
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

void loadingScreen()
{
  using namespace std::chrono_literals;

  cout << CYAN << BOLD;
  cout << R"(

    ██████╗██╗   ██╗██████╗ ███████╗██████╗
   ██╔════╝╚██╗ ██╔╝██╔══██╗██╔════╝██╔══██╗
   ██║      ╚████╔╝ ██████╔╝█████╗  ██████╔╝
   ██║       ╚██╔╝  ██╔══██╗██╔══╝  ██╔══██╗
   ╚██████╗   ██║   ██████╔╝███████╗██║  ██║
    ╚═════╝   ╚═╝   ╚═════╝ ╚══════╝╚═╝  ╚═╝

              CYBERVAULT
       Secure Local Password Vault

)";

  cout << RESET;

  cout << YELLOW << "Initializing Crypto Engine";
  for (int i = 0; i < 3; i++)
  {
    cout << ".";
    cout.flush();
    std::this_thread::sleep_for(300ms);
  }

  cout << GREEN << " ✓\n";

  cout << YELLOW << "Loading User Database";
  for (int i = 0; i < 3; i++)
  {
    cout << ".";
    cout.flush();
    std::this_thread::sleep_for(300ms);
  }

  cout << GREEN << " ✓\n";

  cout << BRIGHT_GREEN << "\nWelcome to CyberVault!\n\n"
       << RESET;

  std::this_thread::sleep_for(600ms);
};

void apiKeyMenu(string currentUser)
{
  int choice;

  while (true)
  {
    cout << "\n====== API Keys ======\n";
    cout << "1. Add Key\n";
    cout << "2. View Keys\n";
    cout << "3. Search\n";
    cout << "4. Delete\n";
    cout << "5. Back\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      addApiKey(currentUser);
      break;
    case 2:
      viewApiKeys(currentUser);
      break;
    case 3:
      searchApiKey(currentUser);
      break;
    case 4:
      deleteApiKey(currentUser);
      break;
    case 5:
      return;
    }
  }
};

void addApiKey(string currentUser)
{
  string service, key;

  cout << "\nService: ";
  cin >> ws;
  getline(cin, service);

  cout << "API Key: ";
  getline(cin, key);

  ApiKeyEntry entry(service, key);

  vector<ApiKeyEntry> keys = ApiKeyService::load(currentUser);

  keys.push_back(entry);

  ApiKeyService::save(currentUser, keys);

  cout << "\nAPI Key saved successfully!\n";
}

void viewApiKeys(string currentUser)
{
  vector<ApiKeyEntry> keys = ApiKeyService::load(currentUser);

  if (keys.empty())
  {
    cout << "\nNo API keys found.\n";
    return;
  }

  cout << "\n======= Saved API Keys =======\n";

  for (const auto &key : keys)
  {
    cout << "Service : " << key.service << endl;
    cout << "Key     : " << maskApiKey(key.key) << endl;
    cout << "-----------------------------\n";
  }
}

void searchApiKey(string currentUser)
{
  vector<ApiKeyEntry> keys = ApiKeyService::load(currentUser);

  string keyword;

  cout << "\nSearch: ";
  cin >> ws;
  getline(cin, keyword);

  bool found = false;

  cout << "\n========== Results ==========\n\n";

  for (auto key : keys)
  {
    if (key.service.find(keyword) != string::npos)
    {
      cout << "🔐 " << key.service << endl;
      cout << "🔑 " << key.key << endl;
      cout << "----------------------------\n";

      found = true;
    }
  }

  if (!found)
  {
    cout << "No matching API keys found.\n";
  }
};

void deleteApiKey(string currentUser)
{
  vector<ApiKeyEntry> keys = ApiKeyService::load(currentUser);

  string service;

  cout << "\nEnter service to delete: ";
  cin >> ws;
  getline(cin, service);

  vector<ApiKeyEntry> updated;

  bool deleted = false;

  for (auto key : keys)
  {
    if (key.service == service && !deleted)
    {
      deleted = true;
      continue;
    }

    updated.push_back(key);
  }

  ApiKeyService::save(currentUser, updated);

  if (deleted)
    cout << "API Key deleted successfully!\n";
  else
    cout << "Service not found.\n";
};

void revealApiKey(string currentUser)
{
  vector<ApiKeyEntry> keys = ApiKeyService::load(currentUser);

  if (keys.empty())
  {
    cout << "\nNo API keys found.\n";
    return;
  }

  string service;

  cout << "\nEnter service to reveal: ";
  cin >> ws;
  getline(cin, service);

  bool found = false;

  for (const auto &key : keys)
  {
    if (key.service == service)
    {
      cout << "\nService : " << key.service << endl;
      cout << "API Key : " << key.key << endl;

      found = true;
      break;
    }
  }

  if (!found)
  {
    cout << "\nService not found.\n";
  }
};
