#include <iostream>
using namespace std;

int main()
{
  int choice;

  while (true)
  {
    cout << "\n=============================\n";
    cout << "      CYBERVAULT v1.0\n";
    cout << "=============================\n";
    cout << "1. Login\n";
    cout << "2. Create Vault\n";
    cout << "3. About\n";
    cout << "4. Exit\n";
    cout << "Choose: ";

    cin >> choice;

    switch (choice)
    {
    case 1:
      cout << "Login module coming soon...\n";
      break;

    case 2:
      cout << "Vault creation coming soon...\n";
      break;

    case 3:
      cout << "CyberVault - Built by Krishna\n";
      break;

    case 4:
      cout << "Exiting CyberVault...\n";
      return 0;

    default:
      cout << "Invalid choice!\n";
    }
  }
}