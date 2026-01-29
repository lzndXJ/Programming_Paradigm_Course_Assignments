#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class wordCounter {
private:
	string target_word; //要统计的单词
	int count;   //出现次数

public:
	//构造函数
	wordCounter(const string& word) : target_word(word),count(0){}

	//统计单词出现个数
	bool CountFile(const string& filename) {
		ifstream infile(filename);
		if (!infile) {
			return false;
		}
		count = 0;
		string word;
		while (infile >> word) {
			if (word == target_word) {
				count++;
			}
		}
		return true;
	}

	void PrintResult() {
		cout << "单词" << target_word << "出现次数为: " << count << endl;
	}
};

int main() {
	string filename;
	cout << "请输入要统计的文件名:";
	cin >> filename;

	wordCounter counter("that");
	if (!counter.CountFile(filename)) {             // 根据返回值判断是否成功
		cerr << "无法打开文件: " << filename << endl;
		return 1;                                
	}

	counter.PrintResult();                          // 只有在文件成功打开并统计后才输出
	return 0;
}

