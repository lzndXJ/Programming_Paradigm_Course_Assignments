#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <vector>

using namespace std;

bool has_frequent_element(const vector<int>& nums, int k, int f) {
	int n = static_cast<int>(nums.size());

	// 遍历所有起始位置
	for (int start = 0; start <= n - k; ++start) {
		for (int i = start; i < start + k; ++i) {
			map<int, int> freq;
			// 从i开始统计频率
			for (int j = i; j < start + k; ++j) {  
				freq[nums[j]]++;
				if (freq[nums[j]] >= f) {
					return true;
				}
			}
		}
	}
	return false;
}

int get_valid_number(const string& prompt) {
	int num;
	while (true) {
		cout << prompt;
		cin >> num;
		if (cin.fail() || cin.peek() != '\n') {
			cout << "输入错误，请输入正整数！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (num < 1) {
			cout << "输入错误，请输入正整数！" << endl;
			continue;
		}

		break;
	}
	return num;
}

int main()
{
	int n = get_valid_number("请输入数组的长度：");
	vector<int> nums;
	cout << "请输入数组的元素（用空格分隔）：" ;
	for (int i = 0; i < n; i++) {
		int number;
		cin >> number;
		nums.push_back(number);
	}

	int k = get_valid_number("请输入正整数k：");
	int f = get_valid_number("请输入正整数f：");

	if (has_frequent_element(nums, k, f))
		cout << "true" << endl;
	else
		cout << "false" << endl;

	return 0;
}