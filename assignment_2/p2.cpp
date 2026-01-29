#include <iostream>
#include <limits>
using namespace std;

long long count_lucky_strings(int n) {
	int vowel_count = (n + 1) / 2; // 偶数下标数
	int consonant_count = n / 2;   // 奇数下标数
	long long result = 1;
	for (int i = 0; i < vowel_count; i++) 
		result *= 5;
	for (int j = 0; j < consonant_count; j++)
		result *= 21;
	return result;
}

int main()
{
	int n;
	while (true) {
		cout << "请输入字符串长度 n (1-10): ";
		cin >> n;

		if (cin.fail()|| cin.peek()!='\n') {
			cout << "输入错误，请输入整数！" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			continue;
		}

		else if (n < 1 || n>10) {
			cout<< "输入错误：n 必须在 1 到 10 之间！" << endl;
			continue;
		}
		break;
	}

	cout << "可生成的 Mao’s Lucky Strings 数量为: "
		<< count_lucky_strings(n) << endl;

	return 0;
}