#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

int is_ascii(const char *str){
	for (int i = 0; str[i] != '\0'; i++){
		if ((unsigned char) str[i] > 127){ //If the input is not ASCII
			return 0;
		}
	}
	return 1; //If input is fully valid  ASCII
}

void print_uppercase_ascii(const char *str){
	for (int i = 0; str[i] != '\0'; i++){
		if (str[i] >= 'a' && str[i] <= 'z'){
			putchar(str[i] - 'a' + 'A');
		} else {
			putchar(str[i]);
		}
	}
}



int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    const char *input = argv[1];

    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");
    printf("Uppercase ASCII: ");
    print_uppercase_ascii(input);
    printf("\n");
    printf("Length in bytes: %lu\n", strlen(input));
    // implement the UTF-8 analyzer here
}
