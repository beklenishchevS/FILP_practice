#include <iostream>
#include "SmartPtr.cpp"


using namespace std;
int main() {
    SmartPtr<int> ptr = new int(5);
    SmartPtr<int> e = new int(4);
    SmartPtr<int> e1 = new int(3);
    ptr = e;
    ptr = e1;
    cout << *ptr << endl;
    cout << *e << endl;
    cout << e.getNumberOfRef() << endl;
    cout << e1.getNumberOfRef() << endl;
    e1.release();
    cout << e1.getNumberOfRef() << endl;
    return 0;
}
