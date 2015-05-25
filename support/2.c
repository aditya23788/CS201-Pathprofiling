void function_1(unsigned x) {
    unsigned y = x;

    ENTRY_1:
        if (!(x > 0)) goto EXIT_1;
        --x;
        goto ENTRY_1;
    EXIT_1:
        x = y;

    ENTRY_2:
        if (!(x > 0)) goto EXIT_2;
        --x;
        goto ENTRY_2;
    EXIT_2:
        return;
}

int main() {
    function_1(100);
    return 0;
}
