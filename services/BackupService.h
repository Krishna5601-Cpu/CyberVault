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
    };

    static bool restoreBackup(string currentUser)
    {
        ifstream backup("data/" + currentUser + ".backup");

        if (!backup)
            return false;

        string passwordData;
        string noteData;
        string apiKeyData;
        string line;
        int section = 0;

        while (getline(backup, line))
        {
            line = Crypto::decrypt(line);

            if (line == "===PASSWORDS===")
            {
                section = 1;
                continue;
            }

            if (line == "===NOTES===")
            {
                section = 2;
                continue;
            }

            if (line == "===API_KEYS===")
            {
                section = 3;
                continue;
            }

            if (section == 1)
                passwordData += line + "\n";
            else if (section == 2)
                noteData += line + "\n";
            else if (section == 3)
                apiKeyData += line + "\n";
        }

        backup.close();

        if (section != 3)
            return false;

        ofstream passwords("data/" + currentUser + ".vault");
        ofstream notes("data/" + currentUser + ".notes");
        ofstream apiKeys("data/" + currentUser + ".keys");

        if (!passwords || !notes || !apiKeys)
            return false;

        passwords << passwordData;
        notes << noteData;
        apiKeys << apiKeyData;

        return true;
    };
};
