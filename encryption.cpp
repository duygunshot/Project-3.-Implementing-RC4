#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

vector<int> findKeyStream(string plain_text, string key)
{
    int key_size = key.length();
    int S[256];
    int K[256];
    // initialize S array
    for (int i = 0; i < 256; i++)
    {
        S[i] = i;
    }
    // initialize K array
    int j;
    for (int i = 0; i < 256; i++)
    {
        j = i % key_size;
        K[i] = key[j];
    }
    j = 0;
    for (int i = 0; i < 256; i++)
    {
        j = (j + S[i] + K[i]) % 256;
        swap(S[i], S[j]);
    }
    j = 0;
    // Find keyStream
    int text_size = plain_text.length();
    vector<int>
        keyStream;
    int t;
    int index;
    for (int i = 0; i < text_size; i++)
    {
        index = i;
        index = (index + 1) % 256;
        j = (j + S[index]) % 256;
        swap(S[index], S[j]);
        t = (S[index] + S[j]) % 256;
        keyStream.push_back(S[t]);
    }
    return keyStream;
}

string decToHex(int dec)
{
    int i = 0;
    char hex[10];
    while (dec != 0)
    {
        int temp;
        temp = dec % 16;
        if (temp >= 10)
        {
            hex[i] = temp + 55;
            i++;
        }
        else
        {
            hex[i] = temp + 48;
            i++;
        }
        dec = dec / 16;
    }

    string str = "";
    for (int j = i - 1; j >= 0; j--)
        str = str + hex[j];
    return str;
}

vector<string> encryption(string plain_text, vector<int> keyStream)
{

    // Xor and parse number to hex
    int text_size = plain_text.length();

    vector<string> hex_vector;
    for (int i = 0; i < text_size; i++)
    {
        hex_vector.push_back(decToHex(keyStream[i] ^ int(plain_text[i])));
    }
    return hex_vector;
}

int main()
{
    ifstream inputFile("text.txt");
    ofstream outputFile("hex.txt");
    string plain_text;
    getline(inputFile, plain_text);
    string key;
    cout << "Please enter a key: ";
    cin >> key;
    while (key.length() < 5 || key.length() > 32)
    {
        cout << "Please enter a key between 5 bytes and 32: ";
        cin >> key;
    }
    vector<int> keyStream = findKeyStream(plain_text, key);
    string hex = "";
    vector<string> hex_vector = encryption(plain_text, keyStream);
    for (int i = 0; i < hex_vector.size(); i++)
    {
        hex = hex + hex_vector[i];
        hex = hex + " ";
    }

    cout << hex << endl;
    outputFile << hex;
}