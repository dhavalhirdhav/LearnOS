#include "drivers/screen.h"

void kmain() {
    clear();

	char str[] = "Welcome to Learn OS. ";
    printf(str);

	char str1[] = "This message has been printed using printf.";
    printf(str1);
}