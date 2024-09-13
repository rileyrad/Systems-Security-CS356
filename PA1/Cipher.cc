#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string XOR(string inputFile, string key) {
    for (int i = 0; i < inputFile.length(); ++i) {
        inputFile[i] = key[i % 16] ^ inputFile[i];
    }
    return inputFile;
}

string swapBits(string inputFile, string key) {
    int start = 0;
    int end = inputFile.length() - 1;
    int i = 0;
    while (start != end) {
        int keyBit = key[i % 16] % 2;
        if (keyBit == 1) {
            swap(inputFile[start], inputFile[end]);
            --end;
            if (start == end) {
                break;
            }
        }
        ++start;
        ++i;
    }
    return inputFile;
}

void blockCipherEncrypt(string inputFile, ofstream& outputFile, string key) {
    // add padding
    while (inputFile.length() % 16 != 0) {
        inputFile += 0x81;
    }

    // encrypt
    inputFile = XOR(inputFile, key);

    // swap
    inputFile = swapBits(inputFile, key);

    // output result
    outputFile << inputFile;
}

void blockCipherDecrypt(string inputFile, ofstream& outputFile, string key) {
    // swap
    inputFile = swapBits(inputFile, key);

    // decrypt
    inputFile = XOR(inputFile, key);

    // remove padding
    while (inputFile[inputFile.length() - 1] == (char)0x81) {
        inputFile = inputFile.substr(0, inputFile.length() - 1);
    }

    // output result
    outputFile << inputFile;
}

void streamCipher(string inputFile, ofstream& outputFile, string key) {
    inputFile = XOR(inputFile, key);

    outputFile << inputFile;
}

int main (int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " [B]lock/[S]tream inputfile outputfile keyfile [E]ncrypt/[D]ecrypt" << endl;
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];
    string keyFile = argv[4];
    string operation = argv[5];

    // Argument error handling
    if (mode != "B" && mode != "S") {
        cerr << "Invalid mode. Use 'B' for block cipher or 'S' for stream cipher." << endl;
        return 1;
    }

    ifstream input(inputFile, ios::binary);
    if (!input) {
        cerr << "Input file not found." << endl;
        return 1;
    }

    ofstream output(outputFile, ios::binary);
    if (!output) {
        cerr << "Error opening output file." << endl;
        return 1;
    }

    ifstream key(keyFile, ios::binary);
    if (!key) {
        cerr << "Key file not found." << endl;
        return 1;
    }

    if (operation != "E" && operation != "D") {
        cerr << "Invalid operation. Use 'E' for encryption or 'D' for decryption." << endl;
        return 1;
    }

    // Turn key into a string
    string key_string; 
    key >> key_string;

    // Turn input into a string
    string inputStream;
    char c;
    while (input.get(c)) {
        inputStream += c;
    }

    // Output an empty file if the input is empty
    if (inputStream.length() == 0) {
        output << "";
        return 0;
    }

    // Perform ciphers
    if (mode == "B") {
        if (operation == "E") {
            blockCipherEncrypt(inputStream, output, key_string);
        } else {
            blockCipherDecrypt(inputStream, output, key_string);
        }
    } else {
        streamCipher(inputStream, output, key_string);
    }

    input.close();
    output.close();
    key.close();

    return 0;
}
