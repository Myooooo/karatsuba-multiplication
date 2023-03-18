#include <iostream>
#include <sstream>

using namespace std;

// make the digits of the two integers equal by adding zeros to the front
// returns the lengths of the integers
int equalDigits(string* a, string* b) {

    int n, diff;

    // compare the digits of two integers
    if(a->length() > b->length()) {
        // a is longer than b
        n = a->length();
        diff = n - b->length();

        // add zeros to the front of b
        for(int i = 0; i < diff; i++) {
            *b = "0" + *b;
        }
    } else {
        // b is longer than a
        n = b->length();
        diff = n - a->length();

        // add zeros to the front of a
        for(int i = 0; i < diff; i++) {
            *a = "0" + *a;
        }
    }

    return n;
}

// addition of s=a+b using school method
string add(string a, string b, int base) {

    string s;
    int carry = 0;
    int n = equalDigits(&a, &b);

    // iterate through each digit from the lowest
    for(int i = (n - 1); i >= 0; i--) {
        // c(i+1)s(i) ← a(i) + b(i) + c(i)
        carry += (a[i] - '0') + (b[i] - '0');

        // add s(i) to the front of the result
        s = to_string(carry % base) + s;

        // calculate c(i+1)
        carry /= base;
    }

    // add remaining carry bit if exist
    if(carry > 0) s = to_string(carry) + s;

    return s;
}

// subtract b from a using school method subtraction
string sub(string a, string b, int base) {

    string diff;
    int carry = 0;
    int n = equalDigits(&a, &b);

    // iterate through each digit from the lowest
    for(int i = (n - 1); i >= 0; i--) {
        carry = (a[i] - '0') - (b[i] - '0') - carry;
        
        if(carry < 0) {
            // have carry
            diff = to_string(carry + base) + diff;
            carry = 1;
        } else {
            // no carry
            diff = to_string(carry) + diff;
            carry = 0;
        }
    }

    return diff;
}

// multiplication of p=a*b using Karatsuba algorithm
string mul(string a, string b, int base) {

    // get the length of input integer
    int n = equalDigits(&a, &b);

    // cout << "multiply " << a << " and " << b << ": ";

    if(n == 0) {

        // error handling
        return 0;

    } else if(n == 1) {

        int product, carry;

        // base case, one digit multiplication
        product = (a[0] - '0') * (b[0] - '0');
        carry = product / base;

        // cout << to_string(carry) + to_string(product % base) << endl;

        return to_string(carry) + to_string(product % base);

    } else {

        string a1, a0, b1, b0 = "";
        string a1b1, a0b0, kp;
        int k = n / 2;

        // divide a, b into a1, a0, b1, b0
        for(int i = 0; i < (n - k); i++) {
            // extract a1,b1 from upper half of a,b
            a1.push_back(a[i]);
            b1.push_back(b[i]);

            // extract a0,b0 from lower half of a,b
            if(n % 2 == 0) {
                // n is even
                a0.push_back(a[i+k]);
                b0.push_back(b[i+k]);
            } else {
                // n is odd
                if((i+k+1) < n) {
                    a0.push_back(a[i+k+1]);
                    b0.push_back(b[i+k+1]);
                }
            }
        }

        // cout << a1 << " " << a0 << " " << b1 << " " << b0 << endl;

        // a1 * b1
        a1b1 = mul(a1,b1,base);

        // a0 * b0
        a0b0 = mul(a0,b0,base);

        // (a1 + a0) * (b1 + b0)
        kp = mul(add(a1,a0,base),add(b1,b0,base),base);

        // (a1 + a0) * (b1 + b0) - (a1 * b1 + a0 * b0)
        kp = sub(kp,add(a1b1,a0b0,base),base);

        // append zeros to the back
        for(int i = 0; i < k; i++) {
            a1b1 += "00";       // B^2k
            kp.push_back('0');  // B^k
        }
        
        // cout << add(a1b1,add(kp,a0b0,base),base) << endl;

        return add(a1b1,add(kp,a0b0,base),base);
    }
}

// remove the leading zeros of a input string integer
string killZeros(string str) {

    int i = 0;

    // increment index till the first non-zero digit
    while(str[i] == '0') i++;

    // remove leading digits
    str.erase(str.begin(),str.begin() + i);

    return str;
}

int main() {

    // define variables
    string input;
    string I1,I2;       // integers (up to 100 digits)
    int B;              // base (2-10)

    // get input from terminal
    getline(cin,input);

    // format input into integers
    stringstream ss(input);
    ss >> I1 >> I2 >> B;

    // compute and print results
    cout << add(I1,I2,B) << " " << killZeros(mul(I1,I2,B)) << " 0" << endl;

    return 0;
}