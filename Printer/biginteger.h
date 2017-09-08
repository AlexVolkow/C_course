//
// Created by Alexandr Volkov on 09.04.2017.
//

#ifndef PRINTER_BIGINTEGER_H
#define PRINTER_BIGINTEGER_H

#include <vector>
#include <string>

class BigInteger {
private:
    const static int BASE = 10;
    std::vector<int> digits;
    bool isMinus = false;

    bool isIntVal = false;
    int valueInt = 0;

    BigInteger(std::vector<int> digits, bool isMinus);

    BigInteger(std::string v, bool isInt);

    BigInteger signedSubtract(BigInteger v) const;

    BigInteger intAddExac(BigInteger v) const;

    BigInteger biAddExac(BigInteger v) const;

    BigInteger intSubstractExac(BigInteger v) const;

    BigInteger biSubtractExac(BigInteger v) const;

public:
    std::string value() const;

    int intValue() const;

    BigInteger(std::string v);

    BigInteger(int v);

    BigInteger add(BigInteger v) const;

    BigInteger subtract(BigInteger v) const;

    BigInteger multiply(BigInteger v) const;

    BigInteger divide(BigInteger v) const;

    BigInteger negate() const;

    bool isInt() const;

    int compareTo(const BigInteger &v) const;

    int abscompareTo(const BigInteger &v) const;

    friend bool operator == (const BigInteger &a, const BigInteger &b);

    friend bool operator > (const BigInteger &a, const BigInteger &b);

    friend bool operator < (const BigInteger &a, const BigInteger &b);

    friend bool operator <=(const BigInteger &a, const BigInteger &b);

    friend bool operator >=(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator+(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator+=(BigInteger &a, const BigInteger &b);

    friend BigInteger operator-=(BigInteger &a, const BigInteger &b);

    friend BigInteger operator-(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator-(const BigInteger &a);

    friend BigInteger operator*(const BigInteger &a, const BigInteger &b);

    friend BigInteger operator/(const BigInteger &a, const BigInteger &b);

    static BigInteger ZERO;
};
#endif //PRINTER_BIGINTEGER_H
