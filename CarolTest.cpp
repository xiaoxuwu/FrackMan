#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Map m;
    m.insert("Cool",1);
    m.insert("Cat",2);
    m.insert("Saves",3);
    m.insert("The",4);
    m.insert("Day",5);

    Map n;
    n.insert("Super",10);
    n.insert("Duper",12);
    n.insert("Day",5);

    Map result;

    subtract(m,n,result);
    assert(result.contains("Cool") && result.contains("Cat") && result.contains("Saves") && result.contains("The")); //result contains the correct keys

    subtract(n,m,result);//not the same as m-n;
    assert(result.size()==2 && !result.contains("Day")); //test result is correct


    subtract(m,m,result); //m-m
    assert(result.empty()); // m-m=0

    subtract(m,result,result); // result should equal m
    assert(result.size()==5 && result.contains("Day"));

    subtract(m,m,m);//should make m empty
    assert(m.empty());

    subtract(m,n,result);//result should still be empty;
    assert(result.empty());
}
