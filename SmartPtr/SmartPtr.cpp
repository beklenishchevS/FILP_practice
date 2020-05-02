

#include <iostream>

using namespace std;
template <typename T>
class SmartPtr{
private:
    T *obj;
    int numberOfRef;

    void deleteObj()
    {
        delete obj;
    }

public:
    SmartPtr(T *obj)
    {
        this->obj = obj;
        numberOfRef = 1;
    }

    void set(T *object)
    {
        *this->obj = *object;
    }

    T* get()
    {
        return obj;
    }

    void release()
    {
        numberOfRef--;
        if (numberOfRef == 0)
        {
            deleteObj();
        }
    }

    ~SmartPtr()
    {
        deleteObj();
    }

    T* operator ->()
    {
        return get();
    }

    T&operator *()
    {
        return *obj;
    }

    int getNumberOfRef()
    {
        return numberOfRef;
    }

    T& operator =(SmartPtr& t)
    {
        set(t.get());
        t.numberOfRef++;
        return *this->obj;
    }
};