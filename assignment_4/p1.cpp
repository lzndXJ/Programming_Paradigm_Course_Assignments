#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
	string name_;    //用户名
	double balance_; //余额
	string type_;    //类型
public:
	BankAccount(string name = "anonymous", double balance = 0.0, string type = "saving") {
		name_ = name;
		balance_ = balance;
		type_ = type;
	}


	//存钱
	void deposit(double amount) {
		balance_ += amount;
		cout << "存钱成功，当前余额为：" << balance_ << endl;
	}

	//取钱
	void withdraw(double amount) {
		if (amount > balance_) {
			cout << "余额不足，当前的余额为：" << balance_ << endl;
		}
		else {
			balance_ -= amount;
			cout << "取钱成功，当前余额为：" << balance_ << endl;
		}
	}
	
	//显示信息
	void display() {
		cout << "Account Name：" << name_ << endl;
		cout << "Account Balance：" << balance_ << endl;
		cout << "Account Type：" << type_ << endl;
	}

	// 重载相等运算符
	bool operator==(const BankAccount& other) const {
		return name_ == other.name_;
	}

	// 重载不等运算符
	bool operator!=(const BankAccount& other) const {
		return name_ != other.name_;
	}

	// 获取账户名称
	const std::string& get_account_name() const { 
		return name_; 
	}

	// 获取账户余额
	double get_balance() const { 
		return balance_; 
	}

	// 获取账户类型
	const std::string& get_account_type() const { 
		return type_; 
	}
};

int main()
{
	// 创建几个测试账户
	BankAccount account1;  // 使用默认值
	BankAccount account2("John", 1000.0, "current");
	BankAccount account3("anonymous", 500.0, "saving");  // 另一个anonymous账户
	
	cout << "=== Initial State ===" << endl;
	account1.display();
	cout << endl;
	account2.display();
	cout << endl;
	account3.display();
	cout << endl;

	// 测试取款操作
	cout << "=== Testing Withdraw ===" << endl;
	account2.withdraw(300.0);
	account2.display();
	cout << endl;

	// 测试余额不足的情况
	cout << "=== Testing Insufficient Balance ===" << endl;
	account1.withdraw(1000.0);  // 应该显示警告
	cout << endl;

	// 测试相等性操作
	cout << "=== Testing Equality ===" << endl;
	cout << "account1 == account3: " << (account1 == account3) << endl;
	cout << "account1 != account2: " << (account1 != account2) << endl;
	cout << "account1 == account1: " << (account1 == account1) << endl;

	return 0;
}