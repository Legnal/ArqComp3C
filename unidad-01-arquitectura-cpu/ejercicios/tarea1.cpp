//Fibonacci

#include<iostream>
using namespace std;
#include<stdlib.h>

int main(){
    int n, a=0, b=1, c;
    cout<<"Cuantos numeros mostar: ";
    cin>>n;
    cout<<a<<" "<<b<<" ";
    for(int i=2;i<n;i++){
        c=a+b;
        cout<<c<<" ";
        a=b;
        b=c;
    }
    return 0;
}
