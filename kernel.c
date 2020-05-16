#include "drivers/screen.h"

void kmain(void) {
	char str[] = "Welcome to Learn OS. ";
	char str1[] = "This message has been printed using printf.";

    clear();
    printf(str);
    printf(str1);
}