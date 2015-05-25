void function_1(unsigned x) {
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
    return 0;
}
