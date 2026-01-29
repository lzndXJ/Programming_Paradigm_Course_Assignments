#include <cmath>
#include <iostream>
#include <vector>
#include <limits>
#include <string> 

using namespace std;

bool has_nearby_duplicate(const vector<int>& nums, int max_distance, int max_difference) {
	int array_length = static_cast<int>(nums.size());

	for (int start = 0; start <= array_length - max_distance; ++start) {
		for (int i = start; i < start + max_distance; ++i) {
			for (int j = i + 1; j < start + max_distance; ++j) {
				if (abs(nums[i] - nums[j])<=max_difference)
					return true;
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
	cout << "请输入数组的元素：";
	for (int i = 0; i < n; i++) {
		int input_value;
		cin >> input_value;
		nums.push_back(input_value);
	}

	int k = get_valid_number("请输入正整数k：");
	int t = get_valid_number("请输入正整数t：");

	if (has_nearby_duplicate(nums, k,t))
		cout << "true" << endl;
	else
		cout << "false" << endl;

	return 0;
}