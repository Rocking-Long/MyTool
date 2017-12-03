#ifndef _MY_ARRAY_
#define _MY_ARRAY_

#include <string>
using std::string;

//排序用参数
enum Array_Sort {
    Array_Sort_ASC,  //升序
    Array_Sort_DESC  //降序
};


template <class T>
class Array
{
protected:
    T *arr;                 //数组
    unsigned int length;    //数组长度
    string name;            //类名字

public:
    /*****构造函数*****/
    Array();
    Array(int n);          //n为数组长度
    Array(int n, T init);  //n为数组长度，init为初始化参数

    /*****析构函数*****/
    ~Array();

    /*****基本操作*****/
    //访问
    T at(int i) const;
    T& operator[] (int i);
    //赋值
    void operator= (const Array<T> &a);
    void set_name(const string &s);
    //取子集
    Array sub_array(int b, int e);
    //排序
    void array_sort(Array_Sort as);



    /*****获取信息*****/
    unsigned int size() const;
    bool empty() const;

    /*****Debug*****/
    inline void check(int i) const;  //检测溢出

};

#endif
