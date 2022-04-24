#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

std::vector<int> computePQ(int);
int computeEulersTotient(int, int);
int computeDecryptionExponent(int, int);
bool isPublicKeyValid(int, int, int, int, int);
int largeExponentMod(int, int, int);
std::string decryptInput(int, int);
std::string encrypt(int, int, std::string);

using std::cout;
using std::endl;

int main(int, char**) {
    int e, n;
    cout << "Enter e: ";
    std::cin >> e;
    cout << "Enter n: ";
    std::cin >> n;

    cout << "Enter 0 or 1 to Encrypt or Decrypt a message: ";
    int ED;
    std::cin >> ED;

    std::string s;
    switch(ED){
      case 0:
           cout << "Enter message to encrypt: ";
           std::cin.ignore();
           std::getline(std::cin, s);
           cout << encrypt(e, n, s) << endl;
           break;
      case 1:
        cout << decryptInput(e, n) << endl;
        break;
    }
    return 0;
}
//brute force p and q
std::vector<int> computePQ(int n) {
    std::vector<int> ints;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i * j == n) {
                ints.push_back(i);
                ints.push_back(j);
                return ints;
            }
        }
    }
    ints.push_back(1);// if no p and q found then when isPublicKeyValid() gets called it will fail, if nothing else breaks first
    ints.push_back(1);
    return ints;
}

int computeEulersTotient(int p, int q) {
    return (p - 1) * (q - 1);
}

int computeDecryptionExponent(int e, int t) { // d is the inverse of e mod eulers totient
    int z;
    if (e > t) { z = e; }
    else { z = t; }
    for (int i = 1; i < z; i++) {
        for (int j = 1; j < z; j++) {
            if ((i * e) - ((j * t) + 1) == 0) { // find what two values satisfy i * e + j * t = 1
                return i;
            }
        }
    }
    return -1; // if no values found catch this in isPublicKeyValid()
}

int largeExponentMod(int x, int y, int n) {
    int z = 1;
    while (y > 0) { // fancy way of doing mod - x^y may be too large to just do x^y % n
        z *= x;     // y is the exponent, you can break x^y into smaller pieces x * x^y-1 then take the mod of that times whatver remainder was left over last time it was done
        y -= 1;
        z = z % n;
    }
    return z;
}

bool isPublicKeyValid(int p, int q, int t, int e, int de) { // check if the public key is valid
    if (p == q) { return false; } //p may not equal q
    if (std::__gcd(e, t) != 1) { return false; } // e and t must be coprime
    if (de == -1) { return false; } // if de is -1 then no decryption exponent was found
    return true;
}

std::string decryptInput(int e, int n) {
    int p, q;
    std::string output = "";

    std::vector<int> ints = computePQ(n);
    p = ints.at(0);
    q = ints.at(1);

    int t, de;
    t = computeEulersTotient(p, q);
    de = computeDecryptionExponent(e, t);

    if ( !isPublicKeyValid(p, q, t, e, de) ) {
        cout << "Public key is not valid!" << endl;
        return "";
    }

    std::ifstream inFS;
    inFS.open("input.txt");

    if (!inFS.is_open()) {
        cout << "could not open input.txt" << endl;
        exit(EXIT_FAILURE);
    }
    int c;
    while (inFS >> c) {
        int value = largeExponentMod(c, de, n);
        output += value;
    }
    inFS.close();
    return output;
}

std::string encrypt(int e, int n, std::string m) {
    std::string output;
    for (int i = 0; i < m.length(); i++) {
        int num = m.at(i);
        int value = largeExponentMod(num, e, n);
        if (i != 0) { output += ' '; }
        output.append(std::to_string(value));
    }
    return output;
}