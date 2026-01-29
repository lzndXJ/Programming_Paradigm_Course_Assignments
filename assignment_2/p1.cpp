#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

using namespace std;

bool is_valid_lowercase(const string& s) {
	if (s.empty())
		return false;
	for (char c : s) {
		if (!isalpha(c) || !islower(c))
			return false;
	}
	return true;
}

string make_palindrome(string s) {
	int i = 0;
	int j = s.size() - 1;
	while (i < j) {
		if (s[i] != s[j]) {
			char smaller = min(s[i], s[j]);
			s[i] = s[j] = smaller;
		}
		i++;
		j--;
	}
	return s;
}

int main() 
{
	string s;
	while (true) {
		cout << "请输入一个全小写字母字符串: ";
		cin >> s;
		
		if (!is_valid_lowercase(s)) {
			cout << "输入错误：仅允许小写字母字符！" << endl;
			continue;
		}
		break;
	}

	cout << "转换后的最小字典序回文串为: " << make_palindrome(s) << endl;

	return 0;
}