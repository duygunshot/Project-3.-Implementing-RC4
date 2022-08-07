#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <string.h>

using namespace std;

int hexToDec(string hex)
{
    int size = hex.length();
    int base = 1;
    int dec = 0;
    for (int i = size - 1; i >= 0; i--)
    {
        if (hex[i] >= 'A' && hex[i] <= 'F')
        {
            dec += (int(hex[i]) - 55) * base;
            base = base * 16;
        }
        else if (hex[i] >= '0' && hex[i] <= '9')
        {
            dec += (int(hex[i]) - 48) * base;
            base = base * 16;
        }
    }
    return dec;
}

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
string decryption(vector<string> hex_vector, vector<int> keyStream)
{
    string plain_text = "";
    for (int i = 0; i < hex_vector.size(); i++)
    {
        plain_text = plain_text + char(keyStream[i] ^ hexToDec(hex_vector[i]));
    }
    return plain_text;
}

int main()
{
    ifstream inputFile("hex.txt");
    string hex;
    getline(inputFile, hex);
    string key;
    cout << "Please enter a key: ";
    cin >> key;
    while (key.length() < 5 || key.length() > 32)
    {
        cout << "Please enter a key between 5 bytes and 32: ";
        cin >> key;
    }
    vector<int> keyStream = findKeyStream(hex, key);
    vector<string> hex_vector;

    string a = "";
    for (int i = 0; i < hex.length(); i++)
    {
        if (int(hex[i]) != 32)
        {
            a = a + hex[i];
        }
        else if (int(hex[i] == 32))
        {
            hex_vector.push_back(a);
            a = "";
        }
    }

    string decrypted_text = decryption(hex_vector, keyStream);
    cout << decrypted_text << endl;
}