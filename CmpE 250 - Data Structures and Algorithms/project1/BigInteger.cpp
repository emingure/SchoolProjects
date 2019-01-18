/*
Student Name: Muhammed Emin Gure
Student Number: 2014400171
Project Number: 1
Operating System: Kubuntu OS
Compile Status: Compiles correctly
Program Status: Test results are matching without error
Notes: ..

*/

#include "BigInteger.h"
#include <algorithm>

/*ostream& operator<<(ostream &out, const BigInteger &bigInteger)  {
    string str = "";
    Node *head = bigInteger.num->head;
    while (head) {
        str += to_string(head->data);
        head = head->next;
    }
    reverse(str.begin(), str.end());
    if (str == "")
        str = "0";
    out << str;
    return out;
}          */

BigInteger::BigInteger(int number){
    if(!num)
        num = new LinkedList();
    if(number==0){
        num->pushTail(0);
    }else {
        while (number != 0) {
            num->pushTail(number % 10);
            number = number / 10;
        }
    }

}

BigInteger::BigInteger(const string& bigInteger){
    if(!num)
        num = new LinkedList();
    for(const char c : bigInteger){
        num->pushHead((int)c - 48);
    }


}

BigInteger BigInteger::operator+(const BigInteger &list){
    BigInteger ret(0);
    *ret.num = *this->num + *list.num;
    Node * pNode = &(*ret.num->head);
    for(int i = 0; i < ret.num->length;i++) {
        if(pNode->data > 9 ){
            if(!pNode->next) ret.num->pushTail(0);
            pNode->next->data++;
            pNode->data -= 10;
        }
        pNode = pNode->next;
    }

    return ret;
}

BigInteger BigInteger::operator*(const BigInteger &list){
    BigInteger ret(0);

    if(list.num->head->data == 0 || this->num->head->data == 0) {
        return ret;
    }


    Node *p2;
    p2 = &(*list.num->head);

    int len_1 = list.num->length;
    int level = 0;

    for(int i = 0; i < len_1 ; i++ )
    {
        BigInteger temp(0);
        temp = *this * p2->data;
        for(int j = 0 ; j < level ; j++)
            temp.num->pushHead(0);
        ret = ret + temp;
        p2 = p2->next;
        level++;

    }

    return ret;

}

BigInteger BigInteger::operator*(int i){
    BigInteger ret(0);

    if(i == 0)
        return ret;

    Node * p = &(*this->num->head);
    Node * p2 = &(*ret.num->head);

    int length = this->num->length;
    
    for(int t = 0 ; t < length; t++){
        int m = p->data * i ;
        p2->data+=m%10;
        if(!(t==length-1 && m/10 == 0)){
            ret.num->pushTail(m/10);
        }
        if(p2->data>9) {
            p2->next->data += p2->data / 10;
            p2->data %= 10;
        }
        p = p->next;
        p2 = p2->next;
    }


    return ret;
}

BigInteger::BigInteger(const BigInteger &other){
    *this->num = *other.num;
}

BigInteger& BigInteger::operator=(const BigInteger &list){
    *this->num = *list.num;
    return *this;
}

BigInteger::~BigInteger() {
    if (num){
        delete num;
    }
}