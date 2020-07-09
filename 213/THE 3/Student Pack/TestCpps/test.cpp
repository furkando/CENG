#include <iostream>

int main()
{
	std::cout << std::endl << "---------------- phase 1 -----------------" << std::endl << std::endl;
    int key=10;
    int count=0,result;
    while(key) {
    if (key&1){
      count++;
    }
    key>>=1;  
    }
    // result = count * h1_param;
    std::cout << count << std::endl;
	return 0;
}
