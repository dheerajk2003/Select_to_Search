// base64_utils.h (or just in your main .c file if small)
#ifndef BASE64_UTILS_H
#define BASE64_UTILS_H

#include <stddef.h> // For size_t

// Calculates the required length for a base64 encoded string
size_t base64_encoded_length(size_t input_length);

// Encodes data into base64
// Returns a newly allocated string which must be freed by the caller.
// Returns NULL on failure.
char *base64_encode(const unsigned char *input, size_t length);

#endif

// base64_utils.c (or just in your main .c file)
#include <stdlib.h>
#include <string.h>

static const char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

size_t base64_encoded_length(size_t input_length) {
    return ((input_length + 2) / 3) * 4;
}

char *base64_encode(const unsigned char *input, size_t length) {
    size_t output_length = base64_encoded_length(length);
    char *encoded_data = malloc(output_length + 1); // +1 for null terminator
    if (encoded_data == NULL) return NULL;

    size_t i = 0, j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (length--) {
        char_array_3[i++] = *(input++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                encoded_data[j++] = base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(size_t k = i; k < 3; k++)
            char_array_3[k] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (size_t k = 0; (k < i + 1); k++)
            encoded_data[j++] = base64_chars[char_array_4[k]];

        while((i++ < 3))
            encoded_data[j++] = '=';
    }

    encoded_data[j] = '\0';
    return encoded_data;
}