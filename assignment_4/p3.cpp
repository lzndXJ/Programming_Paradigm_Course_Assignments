#include <iostream>
#include <string>

using namespace std;

class Author {
private:
	string name_;
public:
	//构造函数
	Author(const string& name) :name_(name){}
	Author(const Author& other):name_(other.name_){}

	//赋值运算符
	Author& operator=(const Author& other) {
		if (this != &other) {
			name_ = other.name_;
		}
		return *this;
	}

	// 获取作者姓名
	const string& getName() const { return name_; }
};

class Publisher {
private:
	string publisher_;
public:
	//构造函数
	Publisher(const string& publisher) :publisher_(publisher){}
	Publisher(const Publisher& other):publisher_(other.publisher_){}

	//赋值运算符
	Publisher& operator=(const Publisher& other) {
		if (this != &other)
			publisher_ = other.publisher_;
		return *this;
	}

	// 获取出版商名称
	const string& getName() const { return publisher_; }
};

class Book :public Author {
private:
	Publisher publisher;
public:
	//构造函数
	Book() :Author("unknown"), publisher("unknown"){}
	Book(const string& authorName, const string& publisherName) :Author(authorName), publisher(publisherName) {}
	Book (const Book& other):Author(other),publisher(other.publisher){}

	//赋值运算符
	Book& operator=(const Book& other) {
		if (this != &other) {
			Author::operator=(other);
			publisher = other.publisher;
		}
		return *this;
	}

	// 获取出版商信息
	const string& getPublisherName() const { return publisher.getName(); }

	// 显示书籍信息
	void display() const {
		cout << "Author: " << getName() << ", Publisher: " << getPublisherName() << endl;
	}
};

// 测试函数
int main() {
    cout << "=== Testing Author Class ===" << endl;
    Author author1("John Doe");
    Author author2 = author1;  // 测试拷贝构造函数
    Author author3("Jane Smith");
    author3 = author1;         // 测试赋值运算符

    cout << "Author1: " << author1.getName() << endl;
    cout << "Author2: " << author2.getName() << endl;
    cout << "Author3: " << author3.getName() << endl;

    cout << "\n=== Testing Publisher Class ===" << endl;
    Publisher pub1("ABC Publishing");
    Publisher pub2 = pub1;     // 测试拷贝构造函数
    Publisher pub3("XYZ Press");
    pub3 = pub1;               // 测试赋值运算符

    cout << "Publisher1: " << pub1.getName() << endl;
    cout << "Publisher2: " << pub2.getName() << endl;
    cout << "Publisher3: " << pub3.getName() << endl;

    cout << "\n=== Testing Book Class ===" << endl;
    // 测试默认构造函数
    Book book1;
    cout << "Default book: ";
    book1.display();

    // 测试带参构造函数
    Book book2("George Orwell", "Secker & Warburg");
    cout << "Parameterized book: ";
    book2.display();

    // 测试拷贝构造函数
    Book book3 = book2;
    cout << "Copied book: ";
    book3.display();

    // 测试赋值运算符
    Book book4("J.K. Rowling", "Bloomsbury");
    cout << "Before assignment: ";
    book4.display();

    book4 = book2;
    cout << "After assignment: ";
    book4.display();

    return 0;
}