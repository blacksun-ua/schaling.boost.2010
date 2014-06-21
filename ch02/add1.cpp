#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace boost;
using namespace std;

void api_example()
{
    {
        // scoped pointer
        scoped_ptr<int> sp(new int);
        *sp = 1;
        *sp.get() = 2;
        if(sp)
            cout << "scoped pointer is valid\n";
        
        sp.reset();
        if(!sp)
            cout << "scoped pointer is null\n";
        
        sp.reset(new int(3));
        cout << "scoped pointer value is " << *sp << "\n";
        cout << "\n";
    }
    
    {
        // shared ptr
        shared_ptr<int> sp(new int);
        *sp = 1;
        *sp.get() = 2;
        if(sp)
            cout << "shared pointer is valid\n";
        
        sp.reset();
        if(!sp)
            cout << "shared pointer is null\n";
        
        sp.reset(new int(3));
        cout << "shared pointer value is " << *sp << "\n";
            
        boost::shared_ptr<int> sp2 = sp;
        cout << "shared pointer " << (sp.unique() ? "is unique" : "has copies") << "\n";
        cout << "shared pointer has " << sp.use_count() << " references\n";
        
        // not valid code
        //cout << "shared pointer [] is " << sp[0] << "\n\n";
        cout << "\n";
    }
    
}

void some_things()
{
    cout << "some things\n";
    // case 1
    {
        shared_ptr<int> sh1(new int(3));
        weak_ptr<int> wp(sh1); 
        {
            shared_ptr<int> sh2 = wp.lock();
            cout << "pointer 1 count: " << sh1.use_count() << ", address " << sh1 
              << " | pointer 1 count: " << sh2.use_count() << ", address " << sh2 << "\n";
            
            // тут уже 2 ссилки, але і дві об"єкта shared_ptr. reset - зменшує лічильник, і тільки якщо він == 1, тоді викликає delete
            sh1.reset();
            cout << "pointer 1 count: " << sh1.use_count() << ", address " << sh1 
              << " | pointer 1 count: " << sh2.use_count() << ", address " << sh2 << "\n";
            // об"єкт залишається жити в sh4
        }
        cout << sh1.use_count() << ", " << sh1 << "\n";
    }
    
    cout << "\n";
}

void shoot()
{
    cout << "shoot in the leg\n";
    
    int* p = new int;
    shared_ptr<int> sh1(p);
    {
        // here we have problems, bacause delete operator calls two times
//        shared_ptr<int> sh2(p);
//        cout << "shared pointers refers to one real pointer, but sh1 counts is [" << sh1.use_count() 
//                                                      << "], and sh2 counts is [" << sh2.use_count() << "]\n";
    }
    {
        // same problem (double free)
//        shared_ptr<int> sh2(&*sh1);
//        cout << "shared pointers refers to one real pointer, but sh1 counts is [" << sh1.use_count() 
//                                                      << "], and sh2 counts is [" << sh2.use_count() << "]\n";
    }
    {
        // this works correctly, but why???
        shared_ptr<int> sh2(sh1.get());
        cout << "shared pointers refers to one real pointer, but sh1 counts is [" << sh1.use_count() << "] and unique = " << sh1.unique() <<
                                                          ", and sh2 counts is [" << sh2.use_count() << "] and unique = " << sh2.unique() << "\n";
        cout << "real object address is " << p << ", stored objects is " << sh1.get() << " and " << sh2.get() << "\n";
        cout << "saved in " << sh1 << "\n";
    }
    cout << "sh1 counts " << sh1.use_count();

    cout << "\n";
}

int main(int argc, char* argv[]) 
{
    api_example();
    some_things();
    shoot();
    return 0;
}
