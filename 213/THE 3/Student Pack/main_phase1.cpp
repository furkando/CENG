#include "Article.h"

void create_article()
{
	//std::cout << std::endl << "---------------- phase 2 -----------------" << std::endl << std::endl;
	Article article(7, 3112327, 3);
	std::vector<int> path;
    article.printTable();
}
int main()
{
	std::cout << std::endl << "---------------- phase 1 -----------------" << std::endl << std::endl;
	create_article();
	return 0;
}
