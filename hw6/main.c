#include "include/library.h"

void func3() {
    LOG_ERROR("Произошла ошибка!");
}
void func2() {
    LOG_DEBUG("debug");
    func3();
}
void func1() {
    LOG_INFO("Just info");
    func2();
}

int main(void) {
    loggerInit("error.log");
    func1();
    loggerClear();
    return 0;
}
