#include <stdio.h>

int main() {
	printf("Test Case 2\n");
	int a = 0;
	int b = 0;
	int c = 0;
	int d = 20;
	block1:
		printf("a: %d, b: %d, c: %d, e: %d\n",a,b,c,d);
		if (a > d) {
			goto block5;
		}
		if (b%2 == 0) {
			printf("1:%d\n",b);
			goto block2;
		}
		else {
			printf("2:%d\n",b);
			goto block2;
		}
	block2:
		++b;
		++c;
		goto block3;
	block3:
		if (b%3 == 0) {
			printf("3:%d\n",b);
			goto block4;
		}
		else {
			printf("4:%d\n",b);
			goto block4;
		}
	block4:
		++a;
		goto block1;
	block5:
		return 0;
}
