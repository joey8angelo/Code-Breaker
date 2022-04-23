#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>

std::vector<int> computePQ(int);
int computeEulersTotient(int, int);
int computeDecryptionExponent(int, int);
bool isPublicKeyValid(int, int, int, int, int);
int largeExponentMod(int, int, int);

int main(int, char**) {
    int e, n, p, q, t, de, m, c;
    std::cout << "Enter e: ";
    std::cin >> e;
    std::cout << "Enter n: ";
    std::cin >> n;
    std::cout << "Enter m: ";
    std::cin >> m;

    std::vector<int> ints = computePQ(n);
    p = ints.at(0);
    q = ints.at(1);

    t = computeEulersTotient(p, q);
    de = computeDecryptionExponent(e, t);

    if ( !isPublicKeyValid(p, q, t, e, de) ) {
        std::cout << "Public key is not valid!" << std::endl;
        return 1;
    }

    int outputDataControl = 1;
    std::string output;
    while (m > 0) {
        std::cout << "Enter encrypted character: ";
        std::cin >> c;
        int value = largeExponentMod(c, de, n);
        if (value >= 4 && value <= 29) { // 4-29 are A-Z these values can be offset by 61 to equal the ascii representation which can be stored as a char
            char o = value + 61;
            output += o;
        }
        else if (value == 30) { output += ' ';}
        else if (value == 31) { output += '\"';}
        else if (value == 32) { output += '.';}
        else if (value == 33) { output += ',';}
        else if (value == 34) { output += '\'';}
        m -= 1;
        
        if (outputDataControl == 1) {
            std::cout << p << ' ' << q << ' ' << t << ' ' << de << std::endl;
            outputDataControl = 0;
        }

        std::cout << value << ' ';
    }
    std::cout << std::endl;
    std::cout << output << std::endl;
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