#include "../headers/utils.h"

char* new_string(size_t size){
    char* string = malloc(size);
    memset(string, 0, size);
    return string;
}

int start_with(char* src, char* substr) {
    if (strncmp(src, substr, strlen(substr)) == 0) return 1;
    return 0;
}

void add_string(struct output_string* output, char* string) {
    char* tmp = NULL;
    size_t need_space = strlen(string) * sizeof(char);
    if (need_space > output->free_space) {
        output->size_string += CHUNK;
        tmp = realloc(output->result, output->size_string);
        output->result = tmp;
        output->free_space = CHUNK - (need_space - output->free_space);
    }
    strcat(output->result, string);
}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}

int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}

void ftoa(float n, char* res, int afterpoint){
    int ipart = (int)n;
    float fpart = n - (float)ipart;
    int i = intToStr(ipart, res, 0);
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
        fpart = fpart * pow(10, afterpoint);
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
