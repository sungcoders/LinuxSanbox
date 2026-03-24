#include <iostream>
#include "sample.h"

int main() {
    sample s;
    s.printText();
    LOGI("This is an info message: {}", 3.14);
    LOGE("This is an error message: {}", "error details");
    return 0;
}
