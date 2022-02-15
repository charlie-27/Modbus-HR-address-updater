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
    scanf("%x", &counting_index);
    getchar();
    const int first_address = counting_index;

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof line, inFile))
    {
        char *strp;
        if ((strp = strstr(line, MATCH_STRING)) && notCommented(line))
        {
            // converto counting_index to string
            char tempstr[5];
            int tmpstr_len = sprintf(tempstr, "%X", counting_index++);

            // aggiungo 0 all'inizio per avere lunghezza 4
            strp += MATCH_STRING_LEN;

            fixNumberWidth(strp);

            for (int i = 0; i < 4; i++)
            {
                if (i < 4 - tmpstr_len)
                    *strp++ = '0';
                else
                    *strp++ = tempstr[i + tmpstr_len - 4];
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