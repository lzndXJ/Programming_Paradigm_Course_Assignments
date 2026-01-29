/*
============================================
	  使用智能指针的优势
============================================

1. 自动管理内存 
   防止内存泄漏
   对象离开作用域后自动释放

2. 明确表达对象所有权 
   unique_ptr 表示“唯一所有者”
   代码含义更加清晰、安全

3. 更强的异常安全性 
   如果发生异常，unique_ptr 会自动释放资源
   不会出现泄漏或悬空指针

4. 更符合现代 C++ 标准 
   避免 raw pointer + new/delete
   RAII 思想：资源与对象生命周期绑定

5. clone() 返回 unique_ptr 更安全
   保证返回值不会被忘记释放
   防止 clone 产生的对象无人管理

============================================
*/

#include <iostream>
#include <vector>
#include <memory>
#include <typeinfo>

using namespace std;

class Shape {
public:
	virtual ~Shape() = default;  //用基类指针删除派生类对象时确保派生类析构函数能被正确调用
	virtual unique_ptr<Shape> clone() const = 0;
	virtual void draw() const = 0;
};

class Circle :public Shape {
public:
	unique_ptr<Shape> clone() const override {
		return make_unique<Circle>(*this);
	}

	void draw() const override {
		cout << "Drawing Circle\n";
	}
};

class Rectangle : public Shape {
public:
	unique_ptr<Shape> clone() const override {
		return make_unique<Rectangle>(*this);
	}

	void draw() const override {
		cout << "Drawing Rectangle\n";
	}
};

int main()
{
	//创建向量
	vector<unique_ptr<Shape>> shapes;
	shapes.push_back(make_unique<Circle>());
	shapes.push_back(make_unique<Rectangle>());

	//遍历向量并调用clone
	for (auto& s : shapes) {
		auto copy = s->clone();   // 返回 unique_ptr<Shape>

		//验证
		cout << "Original type: " << typeid(*s).name()
			<< " | Cloned type: " << typeid(*copy).name() << endl;
	}

	return 0;
}

