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

// Forward declaration
bool registerUser();

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
      cout << "\n[ Login module coming soon ]\n";
      break;

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
  out << username << "," << password << endl;
  out.close();

  // 6. Create an empty vault file for this user
  ofstream vault("data/" + username + ".vault");
  vault.close();

  // 7. Return true
  return true;
}