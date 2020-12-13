#pragma once
#include <cmath>

#include <memory>


// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants
#include <limits>


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// Utility Functions
inline double fmin(double a, double b) { return a < b ? a : b; }
inline double fmax(double a, double b) { return a > b ? a : b; }

inline double clamp(double x, double min, double max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}




inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include <random>
//返回随机数
inline double random_double(double min=0,double max=1) {
    // returns random real in {0,1)
    static std::uniform_real_distribution<double> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
}
inline int random_int(int min, int max) {
	// Returns a random integer in [min,max].
	return static_cast<int>(random_double(min, max + 1));
}

//计算时间
#include<chrono>

class MyTimer
{
public:
	MyTimer();
	double Mark();
	double Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};



inline MyTimer::MyTimer()
{
	last = std::chrono::steady_clock::now();
}

inline  double MyTimer::Mark()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(last - old);
	return (double)duration.count();
}

inline double MyTimer::Peek() const
{
	return  std::chrono::duration<double>(std::chrono::steady_clock::now() - last).count();
}

// common headers

