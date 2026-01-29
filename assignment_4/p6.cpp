#include <iostream>
#include <string>
#include <cmath>
using namespace std;

struct Point {
	int x, y;

	double dist() const {
		return sqrt(x * x + y * y);
	}
};

template<typename T>
T find_min(const T& a, const T& b, const T& c) {
	const T* minPtr = &a;
	if (!(*minPtr < b)) minPtr = &b; // 小于或等于 -> 更新
	if (!(*minPtr < c)) minPtr = &c;
	return *minPtr;
}

bool operator<(const string& a, const string& b) {
	return a.size() < b.size();  // 短的 string 小
}

bool operator<(const Point& a, const Point& b) {
	return a.dist() < b.dist();   // 欧几里得距离小
}

// ====== 针对 Point* 的模板特化 ======
template<>
Point* find_min<Point*>(Point* const& a, Point* const& b, Point* const& c) {
    Point* minPtr = a;
    if (!(*minPtr < *b)) minPtr = b;
    if (!(*minPtr < *c)) minPtr = c;
    return minPtr;
}


// ====== 测试 ======
int main() {
    // int
    cout << find_min(1, 2, 3) << endl;

    // double
    cout << find_min(1.3, 2.4, 0.6) << endl;

    // char
    cout << find_min('a', 'b', 'c') << endl;

    // string
    cout << find_min(string("hello"), string("hi"), string("he")) << endl;

    // Point
    Point p1{ 1,2 }, p2{ 3,4 }, p3{ 0,1 };
    Point resPoint = find_min(p1, p2, p3);
    cout << resPoint.x << "," << resPoint.y << endl;

    // Point*
    Point* pp1 = &p1;
    Point* pp2 = &p2;
    Point* pp3 = &p3;
    Point* resPtr = find_min(pp1, pp2, pp3);
    cout << resPtr->x << "," << resPtr->y << endl;

    return 0;
}