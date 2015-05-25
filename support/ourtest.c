void function_1(unsigned x) {
    while (x > 0) {
        --x;
    }
}

void function_2(unsigned x) {
    while (x > 0) {
        --x;
    }
}
void function_3(unsigned x) {
    ENTRY:
        if (!(x > 0)) goto EXIT;
        if (x % 4 == 0) {
            --x;
        }
        else {
            --x;
        }
        goto ENTRY;
    EXIT:
        return;
}


int main() {
    function_1(100);
    function_2(50);
    function_3(200);

	int j = 0;
	for(int i =0;i< 25;i++){
		j++;
	}
    return 0;
}

