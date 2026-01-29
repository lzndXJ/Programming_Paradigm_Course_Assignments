#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class StockAnalyzer {
private:
    vector<int> prices;
    int maxProfit;

public:
    StockAnalyzer() : maxProfit(0) {}

    void setPrices(const vector<int>& p) {
        prices = p;
    }

    void calculateMaxProfit() {
        if (prices.size() < 2) {
            maxProfit = 0;
            return;
        }

        int minPrice = prices[0];
        maxProfit = 0;

        for (size_t i = 1; i < prices.size(); i++) {
            if (prices[i] < minPrice)
                minPrice = prices[i];
            else if (prices[i] - minPrice > maxProfit)
                maxProfit = prices[i] - minPrice;
        }
    }

    void printResult() const {
        cout << "最大利润为: " << maxProfit << endl;
    }
};

int main() {
    int n;
    while (true) {
        cout << "请输入股票价格的天数: ";
        cin >> n;
        if (cin.fail()) {
            cout << "输入错误，请重新输入！" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else if (n<=0){
            cout << "天数必须大于0！" << endl;
            continue;
        }
        break;
    }

    vector<int> prices(n);
    cout << "请输入每天的股票价格: ";
    for (int i = 0; i < n; i++) {
        cin >> prices[i];
    }

    StockAnalyzer sa;
    sa.setPrices(prices);
    sa.calculateMaxProfit();
    sa.printResult();

    return 0;
}
