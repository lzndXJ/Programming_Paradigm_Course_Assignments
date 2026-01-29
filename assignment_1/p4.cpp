#include <iostream>
#include <string>
using namespace std;

typedef struct {
	string name;
	string place;
	int age;
} Person;

int main() {
	Person p1 = { "Peter","Shanghai",20 };

	cout << "初始信息：" << endl;
	cout << "姓名: " << p1.name << endl;
	cout << "城市: " << p1.place << endl;
	cout << "年龄: " << p1.age << endl;

	Person* pstr = &p1;

	pstr->name = "Allen";
	pstr->place = "Beijing";
	pstr->age = 22;

	cout << "通过指针修改后的信息：" << endl;
	cout << "姓名: " << pstr->name << endl;
	cout << "城市: " << pstr->place << endl;
	cout << "年龄: " << pstr->age << endl;

	return 0;
}