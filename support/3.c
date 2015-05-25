void function_1(unsigned x) {
    unsigned y = x;
    unsigned z = x;

    while (y > 0) {
        z = x;
        while (z > 0) {
            --z;
        }
        --y;
    }
}

int main() {
    function_1(10);
    return 0;
}
