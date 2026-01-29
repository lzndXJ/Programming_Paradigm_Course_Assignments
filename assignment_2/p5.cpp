#include <iostream>
#include <string>
#include <limits>
#include <vector>
using namespace std;

// 高精度乘法函数
string multiply_big_int(const string& num1, const string& num2) {
	if (num1 == "0" || num2 == "0")
		return "0";

	int len1 = num1.length();
	int len2 = num2.length();
	vector<int> result(len1 + len2, 0);  // 结果数组，初始化为0

	// 从低位到高位逐位相乘
	for (int i = len1 - 1; i >= 0; i--) {
		for (int j = len2 - 1; j >= 0; j--) {
			int digit1 = num1[i] - '0';
			int digit2 = num2[j] - '0';
			int product = digit1 * digit2;

			int pos1 = i + j;
			int pos2 = i + j + 1;
			int sum = product + result[pos2];

			result[pos2] = sum % 10;
			result[pos1] += sum / 10;
		}
	}

	string resultStr;
	for (int num : result) {
		if (!(resultStr.empty() && num == 0)) {
			resultStr += to_string(num);
		}
	}

	return resultStr;
}

// 输入验证函数
bool is_valid_number(const string& num) {
	//检查是否全部都是数字
	for (char c : num) {
		if (c < '0' || c>'9')
			return false;
	}

	// 检查是否有前导零（除非数字本身就是0）
	if (num.length() > 1 && num[0] == '0')
		return false;

	return true;
}

//获取有效输入
string get_valid_number(const string& prompt) {
	string num;
	while (true) {
		cout << prompt;
		cin >> num;

		if (!is_valid_number(num)) {
			cout << "输入无效！请输入有效的非负整数（无前导零）" << endl;
			continue;
		}
		break;
	}
	return num;
}

int main()
{
	string num1 = get_valid_number("请输入第一个大整数: ");
	string num2 = get_valid_number("请输入第二个大整数: ");

	string result = multiply_big_int(num1, num2);

	cout << "乘积结果: " << result << endl;

	return 0;
}