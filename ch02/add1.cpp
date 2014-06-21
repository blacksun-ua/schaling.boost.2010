#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void api_example()
{
    
}

void some_things()
{
    int* p1 = new int;
    shared_ptr<int> sh1(p1);
    {
//        shared_ptr<int> sh2(p);
//        cout << sh2.use_count() << "\n";
        // тут буде бубух, бо оператор delete викликається 2 рази
    }
    
    cout << sh1.use_count() << "\n";
    
    shared_ptr<int> sh3(new int(3));
    weak_ptr<int> wp(sh3); 
    // тут взагалі цікаво, тому що ссилка на об"єкт тільки одна..., а одна тому що в конструктор передається по ссилкі
    
    //sh3.reset();
    {
        shared_ptr<int> sh4 = wp.lock();
        cout << sh3.use_count() << ", " << sh3 << " | " << sh4.use_count() << ", " << sh4 <<  "\n";
        // тут уже 2 ссилки, але і дві об"єкта shared_ptr. reset - зменшує лічильник, і тільки якщо він == 1, тоді викликає delete
        sh3.reset();
        cout << sh3.use_count() << ", " << sh3 << " | " << sh4.use_count() << ", " << sh4 <<  "\n";
        // об"єкт залишається жити в sh4
    }
    cout << sh3.use_count() << ", " << sh3 << "\n";
    
    cout << "Hello\n" << endl;
}

void shoot()
{
    
}

int main(int argc, char* argv[]) 
{
    api_example();
    some_things();
    shoot();
    return 0;
}
