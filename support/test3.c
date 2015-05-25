#include <stdio.h>

int main() {
	printf("Test Case 1\n");
	int a = 0;
	int b = 0;
	int c = 0;
	block1:
		printf("1a: %d\n",a);
		if (a > 20) {
			goto block7;
		}
		if (b%2 == 0) {
			printf("1b: %d\n",b);
			goto block2;
		}
		else {
			printf("1c: %d\n",c);
			goto block3;
		}
	block2:
		printf("2a: %d\n",a);
		if (c%3 == 0) {
			printf("2c: %d\n",c);
			goto block3;
		}
		else {
			printf("2b: %d\n",b);
			goto block4;
		}
	block3:
		printf("3b: %d\n",b);
		goto block4;
	block4:
		++b;
		++c;
		printf("4bc: %d",b+c);
		if (a%2 == 0) {
			printf("4a: %d\n",a);
			goto block5;
		}
		else {
			printf("4b: %d\n",b);
			goto block6;
		}
	block5:
		printf("5a: %d\n",a);
	block6:
		printf("6a: %d\n",a);
		++a;
		goto block1;
	block7:
		return 0;
}
