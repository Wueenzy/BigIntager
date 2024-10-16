#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class BigIntager{
private:
    unordered_map<int,int> bigNumber;
    int size = 0;
    bool negative = false;
public:
    BigIntager(string);
    void writeBigIntager();
    void plus(BigIntager&);
    void multiple(BigIntager&);
};

BigIntager::BigIntager(string inputNumber) {
    negative = (inputNumber[0] == '-') ? true : false;
    if(negative){
        inputNumber.erase(0,1);
    }
    size = inputNumber.length();
    for(int i = 1; i < size + 1 ; i++){
        bigNumber[i] = inputNumber[size - i] - '0';
    }
}

void BigIntager::writeBigIntager() {
    if(negative){
        cout << "-";
    }
    for(int i = size ; i > 0; i--){
        cout << bigNumber[i];
    }
}

void BigIntager::plus(BigIntager& othBigIntager) {
    int lowSize = (size > othBigIntager.size) ? othBigIntager.size : size;
    unordered_map<int,int> realBigNumber = (size >= othBigIntager.size) ? bigNumber : othBigIntager.bigNumber;
    unordered_map<int,int> fakeBigNumber = (size < othBigIntager.size) ? bigNumber : othBigIntager.bigNumber;
    for(int i = 1; i < lowSize + 1; i++){
        bigNumber[i] = realBigNumber[i] + fakeBigNumber[i];
        while(bigNumber[i] > 9){
            bigNumber[i] -= 10;
            realBigNumber[i+1] = (realBigNumber.count(i+1)) ? ++realBigNumber[i+1] : 1;
        }
    }
}

void BigIntager::multiple(BigIntager& othBigIntager) {
    negative = (negative == othBigIntager.negative) ? false : true;
    vector<int> result(size + othBigIntager.size, 0);
    for (int i = size - 1; i >= 0; i--) {
        for (int j = othBigIntager.size - 1; j >= 0; j--) {
            int _multiple = bigNumber[size-i] * othBigIntager.bigNumber[othBigIntager.size-j];
            int _plus = _multiple + result[i + j + 1];

            result[i + j + 1] = _plus % 10;
            result[i + j] += _plus / 10;
        }
    }

    string resultStr = "";
    for (int num : result) {
        if (!(resultStr.empty() && num == 0)) {
            resultStr.push_back(num + '0');
        }
    }

    size = resultStr.length();
    for(int i = 1; i < size + 1 ; i++){
        bigNumber[i] = resultStr[size - i] - '0';
    }
}