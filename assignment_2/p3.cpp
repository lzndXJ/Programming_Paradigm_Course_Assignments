#include <iostream>
#include <vector>
#include <limits>

using namespace std;

vector<int> break_chocolate(int n) {
	vector<int> parts;
	int current = 2;

	// 从2开始逐个添加
	while (n >= current) {
		parts.push_back(current);
		n -= current;
		current += 1;
	}

	// 处理剩余的部分
	if (n > 0) {
		// 如果剩余值等于最后一个数，需要先给最后一个数加1，不然后面按次序给每一位加1会多出1
		if (n == parts.back()) {
			parts.back() += 1;
			n -= 1;
		}

		// 将剩余的值从后往前分配到各个部分
		for (int i = parts.size() - 1; i >= 0 && n > 0; i--) {
			parts[i] += 1;
			n--;
		}
	}

	return parts;
}

int main()
{
	int n;
	while (true) {
		cout << "请输入巧克力总长度 n (3-10000): ";
		cin >> n;

		if (cin.fail() || cin.peek() != '\n') {
			cout << "输入错误，请输入整数！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}
		if (n < 3 || n > 10000) {
			cout << "输入错误：n 必须在 3 到 10000 之间！" << endl;
			continue;
		}
		break;
	}

	vector<int> result = break_chocolate(n);
	cout << "巧克力应该被分为：";
	for (size_t i = 0; i < result.size(); i++) {
		cout << result[i] << " ";
	}
	cout << endl;
}