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

void print_six_codepoints_substring(const char *str){
	printf("Substring of the first 6 code points:");
    const char *current_char_ptr = str; 
    int codepoints_printed = 0;                

    while (*current_char_ptr != '\0' && codepoints_printed < 6) {
        unsigned int codepoint = 0; 
        int bytes_read = 0;        
        unsigned char byte = (unsigned char)current_char_ptr[0]; 

        if (byte == 0) { 
            bytes_read = 1; 
            codepoint = 0;
        }
        // 1 bytes (0xxxxxxx) - ASCII
        else if ((byte & 0x80) == 0) {
            codepoint = byte;
            bytes_read = 1;
        }
        // 2 bytes (110xxxxx 10xxxxxx)
        else if ((byte & 0xE0) == 0xC0) {
            if (current_char_ptr[1] == '\0' || (current_char_ptr[1] & 0xC0) != 0x80) {
                bytes_read = 1; 
                codepoint = 0;
            } else {
                codepoint = ((unsigned int)(byte & 0x1F) << 6) |
                            ((unsigned int)(current_char_ptr[1] & 0x3F));
                bytes_read = 2;
            }
        }
        // 3 bytes (1110xxxx 10xxxxxx 10xxxxxx)
        else if ((byte & 0xF0) == 0xE0) {
            if (current_char_ptr[1] == '\0' || (current_char_ptr[1] & 0xC0) != 0x80 ||
                current_char_ptr[2] == '\0' || (current_char_ptr[2] & 0xC0) != 0x80) {
                bytes_read = 1; 
                codepoint = 0;
            } else {
                codepoint = ((unsigned int)(byte & 0x0F) << 12) |
                            ((unsigned int)(current_char_ptr[1] & 0x3F) << 6) |
                            ((unsigned int)(current_char_ptr[2] & 0x3F));
                bytes_read = 3;
            }
        }
        // 4字节序列 (11110xxx 10xxxxxx 10xxxxxx 10xxxxxx)
        else if ((byte & 0xF8) == 0xF0) {
            if (current_char_ptr[1] == '\0' || (current_char_ptr[1] & 0xC0) != 0x80 ||
                current_char_ptr[2] == '\0' || (current_char_ptr[2] & 0xC0) != 0x80 ||
                current_char_ptr[3] == '\0' || (current_char_ptr[3] & 0xC0) != 0x80) {
                bytes_read = 1; 
                codepoint = 0;
            } else {
                codepoint = ((unsigned int)(byte & 0x07) << 18) |
                            ((unsigned int)(current_char_ptr[1] & 0x3F) << 12) |
                            ((unsigned int)(current_char_ptr[2] & 0x3F) << 6) |
                            ((unsigned int)(current_char_ptr[3] & 0x3F));
                bytes_read = 4;
            }
        }
        else {
            bytes_read = 1; 
            codepoint = 0;  
        }

        if (codepoint != 0) { 
            for (int i = 0; i < bytes_read; ++i) {
                printf("%c", current_char_ptr[i]);
            }
            codepoints_printed++; 
        } else {
            if (bytes_read == 0) { 
                bytes_read = 1;
            }
        }
        current_char_ptr += bytes_read; 
    }
    printf("'\n"); 
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
