#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class BigIntager{
private:
    unordered_map<int,int> bigNumber;
    bool negative = false;
    bool afterNegative = false;
public:
    BigIntager(string = "0");
    void writeBigIntager();
    void plus(BigIntager&,BigIntager&);
    void subtract(BigIntager&,BigIntager&);
    void divide(BigIntager&,BigIntager&);
    void multiple(BigIntager&,BigIntager&);
    bool biggerThan(BigIntager&,BigIntager&);
    bool absoluteBiggerThan(BigIntager&,BigIntager&);
    bool equal(BigIntager&,BigIntager&);
    bool isZero();
    int getSize();
    string getNumber();
};

BigIntager::BigIntager(string inputNumber) {
    negative = (inputNumber[0] == '-') ? true : false;
    if(negative){
        inputNumber.erase(0,1);
    }
    int size = inputNumber.length();
    for(int i = 1; i < size + 1 ; i++){
        bigNumber[i] = inputNumber[size - i] - '0';
    }
}

void BigIntager::writeBigIntager() {
    if(negative && !isZero()){
        cout << "-";
    }
    bool isZero = false;
    int zeroCount = 0;

    for(int i = getSize() ; i > 0; i--){
        if(zeroCount == getSize() - i && bigNumber[i] == 0 && i != 1){
            zeroCount++;
            continue;
        }
        cout << bigNumber[i];
    }
}

void BigIntager::plus(BigIntager& BigNumber, BigIntager& othBigNumber) {
    if(BigNumber.negative != othBigNumber.negative && !afterNegative){
        afterNegative = true;
        subtract(BigNumber,othBigNumber);
        return;
    }
    negative = BigNumber.negative;
    int bigSize = (BigNumber.getSize() <= othBigNumber.getSize()) ? othBigNumber.getSize() : BigNumber.getSize();
    unordered_map<int,int> realBigNumber = (absoluteBiggerThan(BigNumber, othBigNumber)) ? BigNumber.bigNumber : othBigNumber.bigNumber;
    unordered_map<int,int> fakeBigNumber = (!absoluteBiggerThan(BigNumber, othBigNumber)) ? BigNumber.bigNumber : othBigNumber.bigNumber;
    for(int i = 1; i < bigSize + 1; i++){
        bigNumber[i] = realBigNumber[i] + fakeBigNumber[i];
        while(bigNumber[i] > 9){
            realBigNumber[i+1] = (realBigNumber.count(i+1)) ? ++realBigNumber[i+1] : 1;
        }
    }
}

void BigIntager::subtract(BigIntager& BigNumber, BigIntager& othBigNumber) {
    if(BigNumber.negative != othBigNumber.negative && !afterNegative){
        afterNegative = true;
        plus(BigNumber, othBigNumber);
        return;
    }
    negative = absoluteBiggerThan(BigNumber, othBigNumber) ? BigNumber.negative : !BigNumber.negative;
    int bigSize = (BigNumber.getSize() <= othBigNumber.getSize()) ? othBigNumber.getSize() : BigNumber.getSize();
    unordered_map<int,int> realBigNumber = (absoluteBiggerThan(BigNumber, othBigNumber)) ? BigNumber.bigNumber : othBigNumber.bigNumber;
    unordered_map<int,int> fakeBigNumber = (!absoluteBiggerThan(BigNumber, othBigNumber)) ? BigNumber.bigNumber : othBigNumber.bigNumber;
    for(int i = 1; i < bigSize + 1; i++){
        bigNumber[i] = realBigNumber[i] - fakeBigNumber[i];
        if(bigNumber[i] < 0){
            bigNumber[i] += 10;
            realBigNumber[i+1]--;
            for(int j = 1; j < bigSize - i; j++){
                if(realBigNumber[i+j] < 0){
                    realBigNumber[i+j+1]--;
                    realBigNumber[i+j] += 10;
                }
            }
        }
    }
}

void BigIntager::divide(BigIntager& BigNumber, BigIntager& othBigNumber) {
    BigNumber.negative = (BigNumber.negative == othBigNumber.negative) ? false : true;
    negative = BigNumber.negative;

    if(othBigNumber.isZero()){
        throw invalid_argument("Divided 0?");
        return;
    }

    if(absoluteBiggerThan(othBigNumber, BigNumber)){
        bigNumber[1] = 0;
        return;
    }

    BigIntager current = BigNumber;

    int count = 0;
    while (absoluteBiggerThan(current, othBigNumber)) {
        count++;
        current.subtract(current, othBigNumber);
    }

    for(int i = 1; count >= 1; i++){
        bigNumber[i] = count%10;
        count = count/10;
    }
}

void BigIntager::multiple(BigIntager& BigNumber, BigIntager& othBigNumber) {
    negative = (BigNumber.negative == othBigNumber.negative) ? false : true;
    vector<int> result(BigNumber.getSize() + othBigNumber.getSize(), 0);
    for (int i = BigNumber.getSize() - 1; i >= 0; i--) {
        for (int j = othBigNumber.getSize() - 1; j >= 0; j--) {
            int _multiple = BigNumber.bigNumber[BigNumber.getSize()-i] * othBigNumber.bigNumber[othBigNumber.getSize()-j];
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

    int size = resultStr.length();
    for(int i = 1; i < size + 1 ; i++){
        bigNumber[i] = resultStr[size - i] - '0';
    }
}

bool BigIntager::biggerThan(BigIntager& BigNumber, BigIntager &othBigNumber) {
    if(BigNumber.negative){
        if(othBigNumber.negative){
            int size = (BigNumber.getSize() >= othBigNumber.getSize()) ? BigNumber.getSize() : othBigNumber.getSize();
            for(int i = size; i > 0;--i){
                if(BigNumber.bigNumber[i] == 0 && othBigNumber.bigNumber[i] == 0){
                    continue;
                }
                if(BigNumber.bigNumber[i] > othBigNumber.bigNumber[i]){
                    return false;
                }else{
                    return true;
                }
            }
        }else{
            return false;
        }
    }else{
        if(othBigNumber.negative){
            return true;
        }else{
            int size = (BigNumber.getSize() >= othBigNumber.getSize()) ? BigNumber.getSize() : othBigNumber.getSize();
            for(int i = size; i > 0;--i){
                if(BigNumber.bigNumber[i] == 0 && othBigNumber.bigNumber[i] == 0){
                    continue;
                }
                if(BigNumber.bigNumber[i] > othBigNumber.bigNumber[i]){
                    return true;
                }else{
                    return false;
                }
            }
        }
    }
    return false;
}

bool BigIntager::equal(BigIntager& BigNumber, BigIntager &othBigNumber) {
    int zeroCount = 0;
    for(int i = BigNumber.getSize(); i > 0; i--){
        if(zeroCount == BigNumber.getSize() - i && BigNumber.bigNumber[i] == 0 && i != 1){
            BigNumber.bigNumber.erase(i);
            zeroCount++;
            continue;
        }
        break;
    }

    zeroCount = 0;
    for(int i = othBigNumber.getSize(); i > 0; i--){
        if(zeroCount == othBigNumber.getSize() - i && othBigNumber.bigNumber[i] == 0 && i != 1){
            othBigNumber.bigNumber.erase(i);
            zeroCount++;
            continue;
        }
        break;
    }

    if(BigNumber.negative == othBigNumber.negative){
        if(BigNumber.bigNumber == othBigNumber.bigNumber){
            return true;
        }
    }
    return false;
}

bool BigIntager::absoluteBiggerThan(BigIntager& BigNumber, BigIntager &othBigNumber) {
    int size = (BigNumber.getSize() >= othBigNumber.getSize()) ? BigNumber.getSize() : othBigNumber.getSize();
    for(int i = size; i > 0;--i){
        if(BigNumber.bigNumber[i] == 0 && othBigNumber.bigNumber[i] == 0){
            continue;
        }
        if(BigNumber.bigNumber[i] > othBigNumber.bigNumber[i]){
            return true;
        }else{
            return false;
        }
    }
    return false;
}

bool BigIntager::isZero(){
    int zeroCount = 0;
    for(int i = getSize() ; i > 0; i--){
        if(zeroCount == getSize() - i && bigNumber[i] == 0){
            zeroCount++;
            continue;
        }
        return false;
    }
    return true;
}

int BigIntager::getSize() {
    return bigNumber.size();
}

string BigIntager::getNumber() {
    string result = "";
    int zeroCount = 0;
    for(int i = getSize(); i > 0; i--){
        if(zeroCount == getSize() - i && bigNumber[i] == 0 && i != 1){
            bigNumber.erase(i);
            zeroCount++;
            continue;
        }
        break;
    }

    for(int i = getSize(); i > 0; i--){
        result += bigNumber[i];
    }

    return result;
}
