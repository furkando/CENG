#include<iostream>
using namespace std;

class Buffer {
	int size;
	char *buff;
public:
	Buffer(int s) {
		size = s;
		buff = new char[size];
		cerr << "Buffer(int) :" << s << endl;
	}

	Buffer(const char *str) {
		for (size = 0; str[size] ; size++) ;
		size++;
		buff = new char[size];
		for (int s = 0; s < size; s++)
			buff[s] = str[s];
		cerr << "Buffer(char *) :" << str << endl;
	}

	Buffer(const Buffer &b) {
		size = b.size;
		buff = new char[size];
		for (int s = 0; s < size; s++)
			buff[s] = b.buff[s];
		buff[0] = (buff[0] == '*') ? '#' : '*' ;  // just to mark copy 
		cerr << "Buffer(COPY) :" << size << '=' << buff << endl;
	}

	Buffer(Buffer &&b) {
		size = b.size;
		buff = b.buff;
		b.buff = nullptr;
		buff[0] = (buff[0] == 'm') ? 'M' : 'm' ;  // just to mark move 
		b.size = 0;
		cerr << "Buffer(MOVE) :" << size << '=' << buff << endl;
	}

	~Buffer() {
		if (buff != nullptr)  {
			cerr << "~Buffer() :" << size <<  '=' << buff << endl;
			delete [] buff;
		} else {
			cerr << "~Buffer() : NULL\n";
		}
	}
	
	void print() const {
		if (buff != nullptr)
			cout << "[" << size << "] " << buff << endl;
	}

	void toupper() {
		if (size <= 0)
			return;
		for (int i = 0; i < size ; i++) {
			if (buff[i] >= 97 && buff[i] <= 97+26) 
				buff[i] -= 32;
		}
	}
	
	char & operator[](int i) {
		if (i >= 0 && i < size) 
			return buff[i];
		else
			throw -1; // just any error value
	}

	void operator=(const Buffer &b) {
		delete [] buff;
		size = b.size;
		buff = new char[size];
		for (int s = 0; s < size; s++)
			buff[s] = b.buff[s];
		buff[0] = '+' ;  // just to mark assignment 
		cerr << "Buffer = Buffer :" << size << '=' << buff << endl;
	}

	void operator=(Buffer &&b) {
		delete [] buff;
		size = b.size;
		buff = b.buff;
		buff[0] = '%' ;  // just to mark move assignment 
		b.buff = nullptr;
		b.size = 0;
		cerr << "Buffer <- Buffer :" << size << '=' << buff << endl;
	}
	
};

/* pass by value */

void printupper(Buffer b) {
	b.toupper();
	b.print();
}

/* return as a value */
Buffer allchar(char c, int times) {
	Buffer b1(times), b2(times);

	for (int i = 0; i < times; i++) {
		b1[i] = c+1;
		b2[i] = c;
	}

	b1.print();

	// following is just a trick to avoid copy ellision
	if (c % 2) 
		return b1;
	else
		return b2;

}


/* pass and return as a value */

Buffer pick(Buffer a, Buffer b, int choice) {
	if (choice) {
		a.toupper();
		a.print();
		return a;
	} else {
		b.toupper();
		b.print();
		return b;
	}
}

/* copy elision */
Buffer repeat(char c, int times) {
	Buffer b1(times);

	for (int i = 0; i < times; i++)
		b1[i] = c;
	
	return b1;
}


int main() {
	Buffer A("hello"), B("world"), C(10), D("how are you?");

	cout << "1--pass by value---\n";
	printupper(A);
	cout << "2---return as a value---\n";
	C = allchar('O',30);
	cout << "3---pass by value + return as a value --\n";
	C = pick(A,D,1);
	cout << "4---return as a value with copy elision--\n";
	repeat('A',5).print();
	cout << "5---pass temporary as a value, copy elision---\n";
	printupper(repeat('A',5));
	cout << "6---elision of nested copy constructor calls---\n";
	Buffer Z(Buffer(Buffer(Buffer("why three levels of nesting?"))));
	cout << "7---implicit constructor call-----\n";
	C = "hello world";  // this is a type conversion constructor call
	cout << "8------\n";
	return 0;

}