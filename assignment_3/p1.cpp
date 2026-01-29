#include <iostream>
#include <vector>
#include <limits>
#include <string> 

using namespace std;

bool has_duplicate_subarray(const vector<int>& nums, int k) {
    int n = static_cast<int>(nums.size());

    for (int start = 0; start <= n - k; ++start) {
        for (int i = start; i < start + k; ++i) {
            for (int j = i + 1; j < start + k; ++j) {
                if (nums[i] == nums[j]) 
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
		int number;
		cin >> number;
		nums.push_back(number);
	}

	int k= get_valid_number("请输入子数组的长度(小于数组的长度)：");

	if (has_duplicate_subarray(nums, k))
		cout << "true" << endl;
	else
		cout << "false" << endl;

	return 0;
}