#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "updater.h"

const char *const MATCH_STRING = "R_addr=$";
char *inputFileStr = "in.txt";
char *outputFileStr = "out.txt";

int main(void)
{
    const int MATCH_STRING_LEN = strlen(MATCH_STRING);

    FILE *inFile = fopen(inputFileStr, "r");
    if (inFile == NULL)
    {
        printf("could not read input file \"%s\"\n", inputFileStr);
        getchar();
        return 1;
    }
    FILE *outFile = fopen(outputFileStr, "w");

    printf("input file: \"%s\"\noutput file: \"%s\"\n", inputFileStr, outputFileStr);
    puts("start HR modbus address in HEX (default = 0x601):");
    int counting_index = 0x601;
    char line[MAX_LINE_LEN];
    if (scanf("%x", &counting_index) == 0)
        scanf("%s", line);
    getchar();
    const int first_address = counting_index;

    while (fgets(line, sizeof line, inFile))
    {
        char *strp;
        if ((strp = strstr(line, MATCH_STRING)) && notCommented(line))
        {
            // converto counting_index to string
            char num_str[5];
            int num_str_len = sprintf(num_str, "%X", counting_index++);

            // aggiungo 0 all'inizio per avere lunghezza 4
            strp += MATCH_STRING_LEN;

            int target_width = num_str_len;
            fixNumberCustomWidth(strp, target_width);

            for (int i = 0; i < target_width; i++)
            {
                if (i < target_width - num_str_len)
                    *strp++ = '0';
                else
                    *strp++ = num_str[i + num_str_len - target_width];
            }
        }

        fputs(line, outFile);
    }

    fclose(inFile);
    fclose(outFile);

    puts("DONE");
    printf("first address: $%X\n", first_address);
    printf("last address:  $%X\n", counting_index - 1);
    printf("total: %X\n", counting_index - first_address);
    getchar();

    return 0;
}

void fixNumberWidth(char *p)
{
    int count = 0;
    while (isxdigit(p[count]))
        count++;

    if (count < 4)
        memmove(p + (4 - count), p, strlen(p) + 1);
}

void fixNumberCustomWidth(char *p, int n)
{
    int count = 0;
    while (isxdigit(p[count]))
        count++;

    if (count == n)
        return;

    if (count < n)
        memmove(p + n - count, p, strlen(p) + 1);
    else
        memmove(p, p + count - n, strlen(p + count - n) + 1);
}

bool notCommented(const char *inputstr)
{
    char *strp = strstr(inputstr, "(*");
    if (strp == NULL)
        return true;

    for (int i = 0; i < strp - inputstr; i++)
    {
        if (!isspace(inputstr[i]))
            return true;
    }

    return false;
}