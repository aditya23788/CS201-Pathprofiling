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

int main() {
    function_1(100);
    function_2(100);
    return 0;
}
