/*  Rocking1996
 *  Time.h
 *  可用于程序计时与时间统计
 *  用法：
 *    实例化类 Time t
 *    添加ID（保存计时）t.createID("xxx")
 *    ID开始计时  t.begin("xxx")
 *    ID结束计时  t.end("xxx")
 *    时间统计    t.view()——所有时间统计  
 *                t.view({"xx", "xxx"})——部分ID时间统计
 *
 *  例子：
 *  Time myTime;  //一般作为全局变量
 *  int main()
 *  {
 *      myTime.createID("A");
 *      myTime.createID("B");
 *
 *      myTime.begin("A");
 *      for (....)
 *      {......}
 *      myTime.end("A");
 *
 *      myTime.begin("B");
 *      for (....)
 *      {......}
 *      myTime.end("B");
 *
 *      myTime.view();
 *      return 0;
 *  }
 */




#ifndef __MY_TIME_
#define __MY_TIME_
#include <unordered_map>
#include <vector>
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <assert.h>
using  std::string;
using  std::cout;
using  std::left;
using  std::setw;
using  std::unordered_map;
using  std::vector;

/*获取微秒*/
class MicrosecondTimer
{
public:
    inline MicrosecondTimer() {
        QueryPerformanceFrequency(&freq);
    }
    
    inline void Start() {
        QueryPerformanceCounter(&startCount);
    }

    inline double Stop() {
        QueryPerformanceCounter(&endCount);
        UINT64 decrease = endCount.QuadPart - startCount.QuadPart;
        double elapsed =  decrease / (double)freq.QuadPart;
        return elapsed;
    }
    
private:
    LARGE_INTEGER freq;
    LARGE_INTEGER startCount;
    LARGE_INTEGER endCount;
};


/*用于保存ID信息*/
struct TimeNode
{
    string sid;
    long double itime;
    MicrosecondTimer MT;

    TimeNode(const string &id, long double t) {
        sid = id;
        itime = t;
    }
};



/*排序保存id的数组*/
static void insertSort(TimeNode** t, unsigned int size)
{
    for (int i = 1; i < size; i++)
    for (int j = i; j > 0; j--)
        if (t[j]->itime > t[j-1]->itime) { 
            TimeNode *temp = t[j];
            t[j] = t[j-1];
            t[j-1] = temp;
        }
        else break;
}


/**
 *  @brief  计时器
 *
 *  用于计算程序模块运行时间，并
 *  对各个模块时间进行汇总与对比
 */
class Time
{
    public:
        Time() {}
        
        /**
         *  @brief  创建一个ID
         *  @param  id 该id的名字
         *  @param  t  初始时间
         */
        void createID(const string &id, long double t = 0) {
            if (id.empty()) return;
            //id已经存在
            if (house.find(id) != house.end()) {
                cout << "ERROR:\n";
                cout << "ID \"" << id << "\" has exited\n";
                assert(0);
            }
            house[id] = new TimeNode(id, t);
        }
        
        /*获取当前id秒数*/
        long double getSeconds(const string &id) {
            check(id);
            return house[id]->itime;
        }

        /**
         *  @brief  开始计时
         *  @param  id 要开启计时的id
         */
        inline void begin(const string &id) {
            check(id);
            house[id]->MT.Start();
        }
        
        /**
         *  @brief  停止计时
         *  @param  id 要停止计时的id
         */
        inline void end(const string &id) {
            check(id);
            house[id]->itime += house[id]->MT.Stop(); 
        }
        
        /**
         *  @brief  记录汇总
         */
        void view() {
            auto it = house.begin();
            TimeNode* Tarray[house.size()];
            int i = 0;
            while (it != house.end())
                Tarray[i++] = (it++)->second;

            print(Tarray, house.size());
        }
        
        /**
         *  @brief  自定义查看id的时间及其百分比
         *  @param  v 一个保存id的向量，可直接使用{id1, id2, ...}
         */
        void view(const vector<string> v) {
            TimeNode* Tarray[v.size()];
            for (int i = 0; i < v.size(); i++)
            {
                check(v[i]);
                auto it = house.find(v[i]);
                Tarray[i] = it->second;
            }
            
            print(Tarray, v.size());  
        }

    private:
        unordered_map<string, TimeNode*> house;
        
        /*检测id是否存在*/
        inline void check(const string &id) {
            if (house.find(id) == house.end()) {
                cout << "ERROR:\n";
                cout << "ID \"" << id << "\" doesn't exit\n";
                assert(0);
            }
        }
        
        /*格式化打印view结果*/
        void print(TimeNode** Tarray, int size) {
            insertSort(Tarray, size);
            
            //求总时间，用来计算百分比
            long double sum = 0;
            for (int i = 0; i < size; i++)
                sum += Tarray[i]->itime;
            
            //打印
            cout << std::setprecision(6) << std::fixed << left;
            cout << setw(20) << "ID"
                 << setw(20) << "Time(s)"
                 << setw(10) << "Percentage(%)\n";
            for (int i = 0; i < size; i++) {
                cout << setw(20) << Tarray[i]->sid
                     << setw(20) << Tarray[i]->itime
                     << setw(10) << Tarray[i]->itime / sum * 100
                     << '\n';
            }
            cout << setw(20) << "ALL_ID" << setw(20) << sum << setw(10) << 100.0 << std::endl;
        }
};

#endif
