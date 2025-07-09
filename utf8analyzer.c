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

uint32_t decode_utf8(const char *str, int *len){
		unsigned char c = str[0];
		if ((c & 0x80) == 0){
		    *len = 1;
		    return c;
		} else if ((c & 0xE0) == 0xC0){
		    *len = 2;
		    return ((str[0] & 0x1F) << 6) |
		        (str[1] & 0x3F);
		} else if (( c & 0xF0) == 0xE0){
		    *len = 3;
		    return ((str[0] & 0xF0) << 12) |
		        ((str[1] & 0x3F) << 6) |
			(str[2] & 0x3F);
		} else if (( c & 0xF8) == 0xF0){
		    *len = 4;
		    return ((str[0] & 0x07) << 18) |
		        (( str[1] & 0x3F) << 12) |
			((str[2] & 0x3F) << 6)|
			(str[3] & 0x3F);
		}
} 

int is_animal_emoji (uint32_t codepoint){
	return (codepoint >= 0x1F400 && codepoint <= 0x1F43F) ||
		(codepoint >= 0x1F980 && codepoint <= 0x1F9AE);
}

void print_animal_emojis(const char *str){
	int i = 0;
	while (str[i] != '\0'){
		int len = 0;
		uint32_t cp = decode_utf8(&str[i], &len);
		if(is_animal_emoji(cp)){
			fwrite(&str[i], 1, len, stdout);
		}
		i += len;
	}
	printf("\n");
}
int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: utf8analyzer \"<UTF-8 encoded string>\"\n");
        return 1;
    }

    const char *input = argv[1];
    size_t byte_length = strlen(input);
    printf("Valid ASCII: %s\n", is_ascii(input) ? "true" : "false");
    printf("Uppercase ASCII: ");
    print_uppercase_ascii(input);
    printf("\n");
    printf("Length in bytes: %lu\n", byte_length);
    
    // part 4 
    int cp_count = 0;
    for (int i = 0; i < byte_length; ) {
        unsigned char byte = input[i];
        if (byte < 128) {
            i += 1; 

        } else if ((byte & 0xE0) == 0xC0) {
            i += 2; 
        } else if ((byte & 0xF0) == 0xE0) {
            i += 3; 
        } else if ((byte & 0xF8) == 0xF0) {
            i += 4; 
        } else {
            break;
        }
        cp_count++;
    }
    printf("Number of code points: %d\n", cp_count);
	
    //part 5
    printf("Code points as decimal numbers:");
    for (int i = 0; i < byte_length; ) {
        int cp = 0;
        unsigned char b1 = input[i];
        unsigned char b2 = input[i + 1];
        unsigned char b3 = input[i + 2];
        unsigned char b4 = input[i + 3];

        if (b1 < 128) {
            cp = b1;
            i += 1;
        } else if (b1 >= 192 && b1 <= 223) {
            cp = ((b1 & 0x1F) << 6) | (b2 & 0x3F);
            i += 2;
        } else if (b1 >= 224 && b1 <= 239) {
            cp = ((b1 & 0x0F) << 12) | ((b2 & 0x3F) << 6) | (b3 & 0x3F);
            i += 3;
        } else if (b1 >= 240 && b1 <= 247) {
            cp = ((b1 & 0x07) << 18) | ((b2 & 0x3F) << 12) |
                 ((b3 & 0x3F) << 6) | (b4 & 0x3F);
            i += 4;
        }

        printf(" %d", cp);
    }
    printf("\n");

    //part 6
    printf("Bytes per code point:");
	for (int i = 0; i < byte_length; ) {
    	unsigned char b1 = input[i];
    	int bytes_used = 0;

    if (b1 < 128) {
        bytes_used = 1;
    } else if ((b1 & 0xE0) == 0xC0) {
        bytes_used = 2;
    } else if ((b1 & 0xF0) == 0xE0) {
        bytes_used = 3;
    } else if ((b1 & 0xF8) == 0xF0) {
        bytes_used = 4;
    } else {
        // invalid UTF-8, stop or handle error
        break;
    }

    		printf(" %d", bytes_used);
    		i += bytes_used;
	}
	printf("\n");


    printf("Animal emojis: ");
    print_animal_emojis(input);
}
