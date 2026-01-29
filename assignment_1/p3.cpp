#include <iostream>
#include <string>
using namespace std;

void ModifyByPoint(string* pstr) {
	if (pstr != NULL) {
		*pstr += " (modify_by_point)";
	}
}

void ModifyByReference(string& rstr) {
	rstr += " (modify_by_reference)";
}

int main() {
	string text = "hello world";
    cout << "原始字符串: " << text << endl;

    // 调用指针版本
    ModifyByPoint(&text);
    cout << "调用指针函数后: " << text << endl;

    // 调用引用版本
    ModifyByReference(text);
    cout << "调用引用函数后: " << text << endl;

    return 0;
}