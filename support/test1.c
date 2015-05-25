#include <stdio.h>

int main() {
	printf("Test Case 1\n");
	int a = 0;
	int b = 0;
	int c = 10;
	block1:
		printf("1:%d\n",a+b);
		if (a > c)
			goto block4;
		if (a%2 == 0) {
			printf("2:%d\n",a);
			goto block2;
		}
		else {
			printf("2:%d\n",b);
			goto block2;
		}
	block2:
		++a;
		++b;
		goto block3;
	block3:
		printf("3:%d\n",a+b);
		goto block1;
	block4:
		return 0;
}

