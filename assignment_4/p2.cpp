#include <iostream>
using namespace std;

class Vector3D {
private:
	float x_;
	float y_;
	float z_;
public:
	//构造函数
	Vector3D() {
		cout << "输入x，y，z的值：";
		cin >> x_ >> y_ >> z_;
	}
	Vector3D(const float arr[3]) :x_(arr[0]), y_(arr[1]), z_(arr[2]){}
	Vector3D(float x, float y, float z) :x_(x), y_(y), z_(z){}

	//向量加法
	Vector3D add(const Vector3D& v) const {
		return Vector3D(x_ + v.x_, y_ + v.y_, z_ + v.z_);
	}

	//向量点积
	float dot(const Vector3D& v) const {
		return x_ * v.x_ + y_ * v.y_ + z_ * v.z_;
	}

	//向量叉积
	Vector3D cross(const Vector3D& v) const {
		return Vector3D(y_ * v.z_ - z_ * v.y_, z_ * v.x_ - x_ * v.z_, x_ * v.y_ - y_ * v.x_);
	}

	//打印向量
	void print() const {
		std::cout << "(" << x_ << ", " << y_ << ", " << z_ << ")" << std::endl;
	}

	//加法运算符重载
	Vector3D operator+(const Vector3D& v) const {
		return add(v);
	}

	//减法运算符重载
	Vector3D operator-(const Vector3D& v) const {
		return Vector3D(x_ - v.x_, y_ - v.y_, z_ - v.z_);
	}

	//标量乘法重载
	Vector3D operator*(float scalar) {
		return Vector3D(x_ * scalar, y_ * scalar, z_ * scalar);
	}

	//+=重载
	Vector3D& operator+=(const Vector3D& v) {
		x_ += v.x_;
		y_ += v.y_;
		z_ += v.z_;
		return *this;
	}

	// 运算符重载：下标访问（非const版本）
	float& operator[](int index) {
		if (index == 0) return x_;
		if (index == 1) return y_;
		if (index == 2) return z_;
		throw std::out_of_range("Index out of range");
	}

	// 运算符重载：下标访问（const版本）
	float operator[](int index) const {
		if (index == 0) return x_;
		if (index == 1) return y_;
		if (index == 2) return z_;
		throw std::out_of_range("Index out of range");
	}

	// 获取分量值
	float x() const { return x_; }
	float y() const { return y_; }
	float z() const { return z_; }
};

// 全局运算符重载：标量乘法（标量在左侧）
Vector3D operator*(float scalar, const Vector3D& v) {
	return Vector3D(v.x() * scalar, v.y() * scalar, v.z() * scalar);
}

//测试函数
int main()
{
	cout << "=== Testing Vector3D Class ===" << endl;

	// 测试数组构造函数
	float arr1[3] = { 1.0f, 2.0f, 3.0f };
	float arr2[3] = { 4.0f, 5.0f, 6.0f };

	Vector3D v1(arr1);
	Vector3D v2(arr2);

	cout << "v1: ";
	v1.print();
	cout << "v2: ";
	v2.print();

	// 测试成员函数
	cout << "\n=== Testing Member Functions ===" << endl;
	Vector3D v3 = v1.add(v2);
	cout << "v1.add(v2): ";
	v3.print();

	float dot_product = v1.dot(v2);
	cout << "v1.dot(v2): " << dot_product << endl;

	Vector3D cross_product = v1.cross(v2);
	cout << "v1.cross(v2): ";
	cross_product.print();

	// 测试运算符重载
	cout << "\n=== Testing Operator Overloading ===" << endl;
	Vector3D v4 = v1 + v2;  // 这里调用 operator+，内部使用 add 函数
	cout << "v1 + v2: ";
	v4.print();

	Vector3D v5 = v1 - v2;
	cout << "v1 - v2: ";
	v5.print();

	Vector3D v6 = v1 * 2.0f;
	cout << "v1 * 2.0: ";
	v6.print();

	Vector3D v7 = 3.0f * v1;
	cout << "3.0 * v1: ";
	v7.print();

	// 测试下标访问
	cout << "\n=== Testing Subscript Access ===" << endl;
	cout << "v2[0] = " << v2[0] << endl;
	cout << "v2[1] = " << v2[1] << endl;
	cout << "v2[2] = " << v2[2] << endl;

	return 0;
}