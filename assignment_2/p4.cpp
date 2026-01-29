#include <iostream>
#include <string>
#include <limits>
#include <cctype>
using namespace std;

//字符转数字
int char_to_digit(char c) {
	c = toupper(c);
	if (isdigit(c))
		return c - '0';
	return c - 'A' + 10;	
}

//数字转字符
char digit_to_char(int num) {
	if (num < 10)
		return '0' + num;
	return 'A' + (num - 10);
}

//p进制转十进制
long long to_decimal(const string& num, int base) {
	long long result = 0;
	long long power = 1;
	for (int i = num.length() - 1; i >= 0; i--) {
		result += char_to_digit(num[i]) * power;
		power *= base;
	}

	return result;
}

// 十进制转m进制,同时反转
string decimal_to_base_m(long long num, int base) {
	if (num == 0)
		return "0";
	string result = "";
	while (num > 0) {
		result += digit_to_char(num % base);  //加进去的时候就已经反转了
		num /= base;
	}

	return result;
}

// 去除前导零
string remove_leading_zeros(const string& str) {
	size_t start = 0;
	while (start < str.length() && str[start] == '0') {
		start++;
	}
	return (start == str.length()) ? "0" : str.substr(start);
}

//验证输入是否有效
bool validate_input(int p, int m, const string& k) {
	// 检查p和m的范围
	if (p < 2 || p > 20 || m < 2 || m > 20) {
		return false;
	}

	// 检查k是否是有效的p进制数
	for (char c : k) {
		c = toupper(c);
		int digit;
		if (isdigit(c)) {
			digit = c - '0';
		}
		else if (c >= 'A' && c <= 'J') {
			digit = c - 'A' + 10;
		}
		else {
			return false;  // 非法字符
		}

		// 检查数字是否小于p
		if (digit >= p)
			return false;
	}

	return true;
}

// 主要的镜像转换函数
string mirror_transform(int p, int m, const string& k) {
	//p进制转十进制
	long long decimalValue = to_decimal(k, p);

	//十进制转m进制
	string baseM = decimal_to_base_m(decimalValue, m);

	//去除前导零
	string result = remove_leading_zeros(baseM);

	return result;
}

int main()
{
	int p, m;
	string k;

	while (true) {
		cout << "请分别输入p,m,k:";
		cin >> p >> m >> k;
		if (cin.fail() || cin.peek() != '\n') {
			cout << "输入类型错误，请重新输入！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		if (!validate_input(p, m, k)) {
			cout << "输入无效！请确保：" << endl;
			cout << "- p 和 m 在 2-20 之间" << endl;
			cout << "- k 是有效的 " << p << " 进制数（只包含0-9和A-J）" << endl;
			cout << "- k 中的每个数字都小于 " << p << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		
		break;
	}
	// 执行转换
	string result = mirror_transform(p, m, k);

	// 输出结果
	cout << "结果为：" << result << endl;

	return 0;
}