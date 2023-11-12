#include "parent.h"

#include <stdio.h>
#include <stdlib.h>

const char* CHILD_EXEC_NAME = "child";

int main() {

    ParentRoutine(CHILD_EXEC_NAME);

    return 0;
}