//
// Created by Alexandr Volkov on 09.04.2017.
//

#include <cstdlib>
#include "biginteger.h"
#include <utility>
#include <stdexcept>
#include <iostream>

using namespace std;


BigInteger::BigInteger(vector<int> digits, bool isMinus) {
    this->digits = digits;
    this->isMinus = isMinus;
}

BigInteger BigInteger::ZERO = BigInteger("0");

BigInteger::BigInteger(std::string v) {
    char *p;
    int intVal = strtol(v.c_str(), &p, 10);
    if (errno == ERANGE) {
        isIntVal = false;
        int r = 0;
        if (v[0] == '-') {
            isMinus = true;
            r = 1;
        }
        for (int i = v.size() - 1; i >= r; i--) {
            digits.push_back(v[i] - '0');
        }
    } else {
        this->valueInt = intVal;
        isIntVal = true;
    }
}

int BigInteger::compareTo(const BigInteger &v) const {
    if (!isIntVal) {
        if (!isMinus && v.isMinus)return 1;
        if (isMinus && !v.isMinus)return -1;
        if (!isMinus && !v.isMinus)return abscompareTo(v);
        if (isMinus && v.isMinus)return -abscompareTo(v);
        return 0;
    } else {
        return abscompareTo(v);
    }
}

int BigInteger::abscompareTo(const BigInteger &v) const {
    if (!isIntVal) {
        if (digits.size() != v.digits.size()) {
            if (digits.size() > v.digits.size())
                return 1;
            else
                return -1;
        }
        for (int i = digits.size() - 1; i >= 0; i--) {
            if (digits[i] != v.digits[i]) {
                if (digits[i] > v.digits[i])
                    return 1;
                else
                    return -1;
            }
        }
        return 0;
    } else {
        return valueInt - v.valueInt;
    }
}

bool operator==(const BigInteger &a, const BigInteger &b) {
    return a.compareTo(b) == 0;
}

bool operator>(const BigInteger &a, const BigInteger &b) {
    return a.compareTo(b) > 0;
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    return a.compareTo(b) < 0;
}

BigInteger BigInteger::biAddExac(BigInteger v) const {
    if (isMinus == v.isMinus) {
        BigInteger first = *this;
        int size = max(this->digits.size(), v.digits.size());
        vector<int> res;
        while (first.digits.size() != size)
            first.digits.push_back(0);
        while (v.digits.size() != size)
            v.digits.push_back(0);
        int r = 0;
        for (int i = 0; i < size; i++) {
            res.push_back(first.digits[i] + v.digits[i] + r);
            if (res[i] >= BASE) {
                res[i] -= BASE;
                r = 1;
            } else {
                r = 0;
            }
        }
        if (r == 1) {
            res.push_back(1);
        }
        return BigInteger(res, isMinus);
    } else {
        BigInteger first = *this;
        BigInteger second = v;
        if (first.isMinus) {
            BigInteger temp = first;
            first = second;
            second = temp;
        }
        if (first.abscompareTo(second) == 0) {
            return ZERO;
        }
        if (first.abscompareTo(second) > 0) {
            second.isMinus = false;
            return first.subtract(second);
        } else {
            second.isMinus = false;
            BigInteger res = second.subtract(first);
            res.isMinus = true;
            return res;
        }
    }
}

std::string BigInteger::value() const {
    if (!isIntVal) {
        string res = "";
        for (int i = digits.size() - 1; i >= 0; i--)
            res += to_string(digits[i]);
        if (isMinus) res = "-" + res;
        return res;
    } else {
        return to_string(valueInt);
    }
}

BigInteger BigInteger::signedSubtract(BigInteger v) const {
    BigInteger res = *this;
    int r = 0;
    while (v.digits.size() != res.digits.size())
        v.digits.push_back(0);
    for (int i = 0; i < res.digits.size(); i++) {
        res.digits[i] -= v.digits[i] + r;
        if (res.digits[i] < 0) {
            res.digits[i] += BASE;
            res.digits[i + 1]--;
        }
    }
    int pos = res.digits.size() - 1;
    while (pos > 0 && res.digits[pos] == 0) {
        pos--;
        res.digits.pop_back();
    }
    return res;
}

BigInteger BigInteger::negate() const {
    if (!isIntVal) {
        BigInteger temp = *this;
        temp.isMinus = !temp.isMinus;
        return temp;
    } else {
        return BigInteger(-valueInt);
    }
}

int BigInteger::intValue() const{
    return valueInt;
}

BigInteger::BigInteger(int v) {
    this->isIntVal = true;
    this->valueInt = v;
}

BigInteger BigInteger::biSubtractExac(const BigInteger v) const {
    // -a-(-b) = b - a
    if (isMinus && v.isMinus) {
        return v.negate().subtract(negate());
    }
    // a-(-b) == a+b
    if (v.isMinus) {
        return this->add(v.negate());
    }
    // (-a)-b == -(a+b)
    if (isMinus) {
        BigInteger res = negate().add(v);
        res.isMinus = !res.isMinus;
        return res;
    }
    if (abscompareTo(v) == 0) {
        return ZERO;
    }
    // a>0, b>0, a<b, то a-b == -(b-a)
    if (compareTo(v) < 0) {
        BigInteger c = v.signedSubtract(*this);
        c.isMinus = true;
        return c;
    } else {
        BigInteger c = signedSubtract(v);
        c.isMinus = false;
        return c;
    }
}

BigInteger BigInteger::intAddExac(BigInteger v) const {
    if (v.valueInt > 0 && INT_MAX - v.valueInt < valueInt ||
        valueInt < 0 && INT_MIN - v.valueInt > valueInt) {
        long long res = valueInt + v.valueInt;
        return BigInteger(to_string(res));
    }
    return BigInteger(valueInt + v.valueInt);
}

BigInteger::BigInteger(std::string v, bool isInt) {
    this->isIntVal = false;
    if (!isInt) {
        int r = 0;
        if (v[0] == '-') {
            isMinus = true;
            r = 1;
        }
        for (int i = v.size() - 1; i >= r; i--) {
            digits.push_back(v[i] - '0');
        }
    }
}

BigInteger BigInteger::add(BigInteger v) const {
    if (isIntVal && v.isIntVal) {
        return intAddExac(v);
    }
    if (!isIntVal && v.isIntVal) {
        return biAddExac(BigInteger(to_string(v.valueInt),false));
    }
    if (isIntVal && !v.isIntVal) {
        return v.biAddExac(BigInteger(to_string(valueInt),false));
    }
    return biAddExac(v);
}

BigInteger BigInteger::intSubstractExac(BigInteger v) const {
    if (v.valueInt < 0 && INT_MAX + v.valueInt < valueInt ||
        v.valueInt > 0 && INT_MIN + v.valueInt > valueInt) {
        long long res = valueInt - v.valueInt;
        return BigInteger(to_string(res));
    }
    return BigInteger(valueInt - v.valueInt);
}

BigInteger BigInteger::subtract(BigInteger v) const {
    if (isIntVal && v.isIntVal) {
        return intSubstractExac(v);
    }
    if (!isIntVal && v.isIntVal) {
        return biSubtractExac(BigInteger(to_string(v.valueInt),false));
    }
    if (isIntVal && !v.isIntVal) {
        return v.biSubtractExac(BigInteger(to_string(valueInt),false));
    }
    return biSubtractExac(v);
}

BigInteger BigInteger::multiply(BigInteger v) const {
    if (isIntVal && v.isIntVal) {
        if (valueInt > 0 && v.valueInt > 0 && INT_MAX / v.valueInt < valueInt ||
            valueInt > 0 && v.valueInt < 0 && INT_MIN / valueInt > v.valueInt ||
            valueInt < 0 && v.valueInt > 0 && INT_MIN / v.valueInt > valueInt ||
            valueInt < 0 && v.valueInt < 0 && INT_MAX / v.valueInt > valueInt) {
            long long r = valueInt * v.valueInt;
            return BigInteger(to_string(r));
        }
        return BigInteger(valueInt * v.valueInt);
    }
    throw logic_error("unsupported operation * for BigInteger");
}

BigInteger BigInteger::divide(BigInteger v) const {
    if (isIntVal && v.isIntVal) {
        if (v.valueInt == 0) {
            throw overflow_error("Divide by zero exception");
        }
        if (valueInt == INT_MIN && v.valueInt == -1) {
            long long r = -valueInt;
            return BigInteger(to_string(r));
        }
        return BigInteger(valueInt / v.valueInt);
    }
    throw logic_error("unsupported operation / for BigInteger");
}

bool BigInteger::isInt() const{
    return isIntVal;
}

bool operator<=(const BigInteger &a, const BigInteger &b) {
    return !(a > b);
}

bool operator>=(const BigInteger &a, const BigInteger &b) {
    return !(a < b);
}

BigInteger operator+(const BigInteger &a, const BigInteger &b) {
    return a.add(b);
}

BigInteger operator-(const BigInteger &a, const BigInteger &b) {
    return a.subtract(b);
}

BigInteger operator-(const BigInteger &a) {
    return a.negate();
}

BigInteger operator/(const BigInteger &a, const BigInteger &b) {
    return a.divide(b);
}
BigInteger operator*(const BigInteger &a, const BigInteger &b) {
    return a.multiply(b);
}

BigInteger operator+=(BigInteger &a, const BigInteger &b) {
    a = a.add(b);
    return a;
}

BigInteger operator-=(BigInteger &a, const BigInteger &b) {
    a = a.subtract(b);
    return a;
}
