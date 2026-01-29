#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const int  kNumInputs = 25;

int main() {
    vector<float> nums(kNumInputs);

    // 输入 25 个数
    cout << "请输入" << kNumInputs << "个浮点数: " << endl;
    for (int i = 0; i < kNumInputs; i++) {
        while (true) {
            cin >> nums[i];
            if (cin.fail())
            {
                cout << "无效输入，请输入一个浮点数。" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            break;
        }
    }

    // 输出原始向量
    cout << "原始数组: " << endl;
    for (float x : nums) {
        cout << x << " ";
    }
    cout << endl;

    // 平方并覆盖原位置
    for (int i = 0; i < kNumInputs; i++) {
        nums[i] = nums[i] * nums[i];
    }

    // 输出平方后的向量
    cout << "平方后的数组: " << endl;
    for (float x : nums) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
