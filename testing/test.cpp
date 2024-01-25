// C++ program to demonstrate shared_ptr
#include <iostream>
#include <memory>
using namespace std;
 
class A {
public:
    void show() { cout << "A::show()" << endl; }
};
 
int main()
{
    // creating a shared pointer and accessing the object
    shared_ptr<A> p1(new A);
    // printting the address of the managed object
    cout << p1.get() << endl;
    p1->show();
   
    // creating a new shared pointer that shares ownership
    shared_ptr<A> p2(p1);
    p2->show();
   
    // printing addresses of P1 and P2
    cout << p1.get() << endl;
    cout << p2.get() << endl;
   
    // Returns the number of shared_ptr objects
    // referring to the same managed object.
    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;
   
    // Relinquishes ownership of p1 on the object
    // and pointer becomes NULL
    p1.reset();
    cout << p1.get() << endl;
    cout << p2.use_count() << endl;
    cout << p2.get() << endl;
    /*
    These lines demonstrate that p1 no longer manages an
    object (get() returns nullptr), but p2 still manages the
    same object, so its reference count is 1.
        */
    return 0;
}