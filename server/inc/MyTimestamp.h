#ifndef _MyTimestamp_h_
#define _MyTimestamp_h_

#include<chrono>
using namespace std::chrono;

class MyTimestamp
{
public:
    MyTimestamp()
    {
        update();
    }
    ~MyTimestamp()
    {}

    void  update()
    {
        _begin = high_resolution_clock::now();
    }
    /**
    *   获取当前秒
    */
    double getElapsedSecond()
    {
        return  getElapsedTimeInMicroSec() * 0.000001;
    }
    /**
    *   获取毫秒
    */
    double getElapsedTimeInMilliSec()
    {
        return this->getElapsedTimeInMicroSec() * 0.001;
    }
    /**
    *   获取微妙
    */
    long long getElapsedTimeInMicroSec()
    {
        return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
    }
protected:
    time_point<high_resolution_clock> _begin;
};

#endif // !_MyTimestamp_h_