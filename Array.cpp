#include "Array.h"
#include <cstdlib>
#include <string>
#include <cstdio>
#include <algorithm>

/****************************************************
 *经大量测试
 *useCheck未开启时，Array访问效率与数组效率相差约10%
 *useCheck开启时, Array访问效率约为数组的1/3
****************************************************/

//是否需要检测溢出
#define useCheck


/**********构造函数**********/
template <typename T>
Array<T>::Array()
{
    length = 0;
    arr = nullptr;
}

template <typename T>
Array<T>::Array(int n)
{
    length = n;
    arr = new T[n];
}

template <typename T>
Array<T>::Array(int n, T init)
{
    length = n;
    arr = new T[n];
    for (int i = 0; i < n; i++)
        arr[i] = init;
}

/**********析构函数**********/
template <typename T>
Array<T>::~Array()
{
    delete [] arr;
}

/**********基本操作**********/
template <typename T>
T Array<T>::at(int i) const
{
    #ifdef useCheck
    check(i);
    #endif // useCheck
    return arr[i];
}

template <typename T>
T& Array<T>::operator[] (int i)
{
    #ifdef useCheck
    check(i);
    #endif // useCheck
    return arr[i];
}

template <typename T>
void Array<T>::operator= (const Array &a)
{
    delete [] arr;
    length = a.length;
    arr = new T[length];
    for (int i = 0; i < length; i++)
        arr = a.at(i);
}


template <typename T>
void Array<T>::set_name (const string &s)
{
    name = s;
}

template <typename T>
Array<T> Array<T>::sub_array(int b, int e)
{
    #ifdef useCheck
    check(b);
    check(e);
    #endif
    Array temp(e-b+1);
    int index = 0;
    e++;
    for (int i = b; i != e; i++)
        temp[index++] = arr[i];
    
    return temp;
}

template <typename T>
void Array<T>::array_sort (Array_Sort as)
{
    if (as == Array_Sort_ASC) {
        std::sort(arr, arr+length, [](const T &a, const T &b) {
            return a < b;
        });
    } else if (as == Array_Sort_DESC) {
        std::sort(arr, arr+length, [](const T &a, const T &b) {
            return a > b;
        });
    }
}


/**********获取信息**********/
template <typename T>
unsigned int Array<T>::size() const
{
    return length;
}

template <typename T>
bool Array<T>::empty() const
{
    return length == 0;
}

/**********Debug**********/
template <typename T>
inline void Array<T>::check(int i) const
{
    if (i < 0 || i >= length) {
        printf("数组溢出！\n");
        printf("数组名：%s\n", name.c_str());
        printf("数组长度：%d\n", length);
        printf("溢出下标：%d\n", i);
        system("pause");
    }
}








