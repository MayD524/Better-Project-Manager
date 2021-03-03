/*
    Author: Ryan Draskovics
    Name: Project manager
*/

#include <iostream>
#include <filesystem>
#include <direct.h>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <map> // just best we need it once

#define cwd _getcwd

// most functions also are in acidic shell (will release eventually)

using namespace std;
using namespace filesystem;
map<string, string> config_data;

vector<string> readFile(string filename)
{
    string line;
    fstream fileRead(filename);
    vector<string> returnVal;
    if (fileRead.is_open())
    {
        while (getline(fileRead, line))
        {
            returnVal.push_back(line);
        }
    }
    else {
        printf("Cannot open %s\n", filename.c_str());
        exit(EXIT_FAILURE);
    }

    return returnVal;
}

vector<string> split(string str, string delim, int count = 0)
{
    vector<string> Vec;
    size_t pos = 0;
    string token;
    int i = 0;
    if (count != 0)
    {
        while ((pos = str.find(delim)) != string::npos || i <= count)
        {
            token = str.substr(0, pos);
            str.erase(0, pos + delim.length());
            Vec.push_back(token);
            i++;
        }
    }
    else {
        while ((pos = str.find(delim)) != string::npos)
        {
            token = str.substr(0, pos);
            str.erase(0, pos + delim.length());
            Vec.push_back(token);
        }
    }
    Vec.push_back(str); // appending the rest of the string


    return Vec;
}

string ConfigGet(string key)
{

    auto pos = config_data.find(key);

    if (pos == config_data.end())
    {
        // handle the error;
        return "NULL";
    }
    else {
        return pos->second;
    }
}

string getCWD()
{
    char buff[FILENAME_MAX];
    cwd(buff, FILENAME_MAX);
    return string(buff);
}

bool exists(string filename)
{
    ifstream infile(filename);
    return infile.good();
}

void MakeFile(string filename)
{
    fstream file;
    file.open(filename, ios::out);

    if (!file)
    {
        cout << "Error making file" << endl;
        exit(EXIT_FAILURE);
    }
    printf("Created file '%s'", filename.c_str());
    file.close();
    return;
}

void writeFile(vector<string> lines, string outFile)
{
    if (!exists(outFile))
    {
        MakeFile(outFile);
    }
    ofstream writeFile(outFile);
    for (const auto& e : lines) writeFile << e << "\n";
}

void startup(string configFile)
{
    printf("Loading %s\n", configFile.c_str());
    string delim = "=";
    vector<string> readData = readFile(configFile);
    for (int i = 0; i < readData.size(); i++)
    {
        vector<string> splitData = split(readData[i], delim);
        config_data.insert({ splitData[0], splitData[1] });
    }

}

int main(int argc, char *argv[])
{
    string dir = weakly_canonical(path(argv[0])).parent_path().string();
    string helpFile = dir + "\\" + "help.txt";
    string config_File = dir + "\\" + "config.bpm";

    printf("Better Project Manager v1.0.1\n");
    startup(config_File);
    cout << argv[0] << endl;
    string opt = argv[1];

    if (argc > 1)
    {
        if (opt == "--help")
        {
            vector <string> tmp = readFile(helpFile);
            for (int i = 0; i < tmp.size(); i++)
            {
                cout << tmp[i] << endl;
            }
        } 

        else if (opt == "new")
        {
            if (argc > 2)
            {
                char buffer[FILENAME_MAX * 2]; // you never know
                string fileType = argv[2];
                vector <string> tmp;
                transform(fileType.begin(), fileType.end(), fileType.begin(), [](unsigned char c) { return toupper(c); });
                string fType = ConfigGet(fileType);
                string dir = getCWD();
                sprintf_s(buffer, "copy %s %s", fType.c_str(), dir.c_str());
                system(buffer);
            }
            else {
                printf("Insufficient amount of arguements.");
            }
        }

        else if (opt == "run")
        {
            ifstream FileCheck;
            if (argc > 2)
            { 
                FileCheck.open(argv[2]);
                if (FileCheck)
                {
                    system("run.bat");
                }
                else
                {
                    printf("This project does not contain a run.bat file.");
                }
            }
            else {
                printf("Insufficient amount of arguements.");
            }
        }

        else if (opt == "build")
        {
            ifstream FileCheck;
            if (argc > 2)
            {
                FileCheck.open(argv[2]);
                if (FileCheck)
                {
                    system("build.bat");
                }
                else
                {
                    printf("This project does not contain a run.bat file.");
                }
            }
            else {
                printf("Insufficient amount of arguements.");
            }
        }

        else if (opt == "add")
        {
            if (argc > 3)
            {
                
                vector<string> tmp = readFile(config_File);
                string key = argv[2];
                transform(key.begin(), key.end(), key.begin(), [](unsigned char c) { return toupper(c); });
                string path = argv[3];
                string addString = key + "=" + path;

                tmp.push_back(addString);
                writeFile(tmp, config_File);
                cout << "Added new config option" << endl;
            }
            else {
                cout << "Add expects 2 arguemnts" << endl;
            }
        }

        else
        {
            printf("Unknown or unused input. Run --help if you need help.");
        }
        
    }
    else
    {
        printf("Insuffecient amount of arguements.\n");
    }
    
    return 0;
}