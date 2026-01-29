#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

volatile int sink = 0;  //防止函数被优化

class BaseProcessor {
public:
	//虚函数
	virtual void process() {
		int result = 0;
		for (int i = 0; i < 100; ++i)
			result += i;
		sink = result;
	}

	void compute() {
		int result = 0;
		for (int i = 0; i < 100; ++i)
			result += i;
		sink = result;
	}
};

class AdvancedProcessor :public BaseProcessor {
public:
	void process() override {
		int result = 0;
		for (int i = 0; i < 100; ++i)
			result += i * 2;
		sink = result;
	}

	void compute() {
		int result = 0;
		for (int i = 0; i < 100; ++i)
			result += i * 2;
		sink = result;
	}
};

//测量函数执行时间，单位为纳秒
template<typename Func>
long long measureTime(Func func, int iterations) {
	auto start = high_resolution_clock::now();
	for (int i = 0; i < iterations; i++) {
		func();
	}
	auto end = high_resolution_clock::now();
	return duration_cast<nanoseconds>(end - start).count();
}

int main()
{
	const int ITERATIONS = 1000000;  // 调用次数
	AdvancedProcessor advancedObj;
	BaseProcessor* basePtr = &advancedObj; //向上转型

	cout << "=== 虚函数 vs 非虚函数性能测试 ===" << endl;
	cout << "测试次数: " << ITERATIONS << " 次" << endl << endl;

	// 测试虚函数调用
	long long virtualTime = measureTime([&]() {
		basePtr->process();
		}, ITERATIONS);

	cout << "虚函数调用总时间: " << virtualTime << " 纳秒" << endl;
	cout << "平均每次虚函数调用: " << virtualTime / ITERATIONS << " 纳秒" << endl << endl;

	// 测试非虚函数调用
	long long nonVirtualTime = measureTime([&]() {
		basePtr->compute();  // 非虚函数调用（静态绑定）
		}, ITERATIONS);

	cout << "非虚函数调用总时间: " << nonVirtualTime << " 纳秒" << endl;
	cout << "平均每次非虚函数调用: " << nonVirtualTime / ITERATIONS << " 纳秒" << endl << endl;

	long long avgTimeDifference = virtualTime / ITERATIONS - nonVirtualTime / ITERATIONS;
	cout << "平均时间差: " << avgTimeDifference << " 纳秒" << endl;

	return 0;
}