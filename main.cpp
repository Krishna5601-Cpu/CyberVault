#include <iostream>
#include <fstream>
#include <string>
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

// Forward declaration
bool registerUser();
bool loginUser(string &currentUser);
void vaultMenu(string currentUser);
void passwordMenu(string currentUser);
void addPassword(string currentUser);
void viewPasswords(string currentUser);

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
    cout << "4. Logout\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      passwordMenu(currentUser);
      break;

    case 2:
      cout << "\n[ Secure Notes coming soon ]\n";
      break;

    case 3:
      cout << "\n[ API Keys coming soon ]\n";
      break;

    case 4:
      cout << "\nLogged out successfully.\n";
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
    cout << "3. Back\n";
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

  cout << "Password: ";
  getline(cin, password);

  PasswordEntry entry(platform, email, password);

  ofstream file("data/" + currentUser + ".vault", ios::app);

  if (!file)
  {
    cout << "Unable to open vault.\n";
    return;
  }

  file << entry.platform << ","
       << entry.email << ","
       << entry.password << endl;

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
