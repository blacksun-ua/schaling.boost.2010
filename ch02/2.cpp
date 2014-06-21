/*
Optimize the following program:

#include <vector> 

template <typename T> 
T *create() 
{ 
  return new T; 
} 

int main() 
{ 
  std::vector<int*> v; 
  v.push_back(create<int>()); 
}  
*/

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

template <typename T>
boost::shared_ptr<T> create_ptr()
{
    return boost::shared_ptr<T>(new T);
}

template <typename T>
T* create() 
{
    return new T;
}

void first_variant()
{
    std::vector<boost::shared_ptr<int> > v;
    v.push_back(create_ptr<int>());
}


void second_variant()
{
    boost::ptr_vector<int> v;
    v.push_back(create<int>());    
}

int main() 
{
    first_variant();
    second_variant();
    return 0;
}
