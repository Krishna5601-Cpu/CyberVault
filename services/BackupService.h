#pragma once

#include <fstream>
#include <string>
#include "../utils/Crypto.h"

using namespace std;

class BackupService
{
public:

    static bool createBackup(string currentUser)
    {
        ifstream passwords("data/" + currentUser + ".vault");
        ifstream notes("data/" + currentUser + ".notes");
        ifstream apiKeys("data/" + currentUser + ".keys");

        ofstream backup("data/" + currentUser + ".backup");

        if (!backup)
            return false;

        string line;

        backup << Crypto::encrypt("===PASSWORDS===") << endl;

        while (getline(passwords, line))
        {
            backup << Crypto::encrypt(line) << endl;
        }

        backup << Crypto::encrypt("===NOTES===") << endl;

        while (getline(notes, line))
        {
            backup << Crypto::encrypt(line) << endl;
        }

        backup << Crypto::encrypt("===API_KEYS===") << endl;

        while (getline(apiKeys, line))
        {
            backup << Crypto::encrypt(line) << endl;
        }

        backup.close();

        return true;
    }
};
