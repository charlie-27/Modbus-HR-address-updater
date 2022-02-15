#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "updater.h"

/*
tutti gli indirizzi devono già essere presenti con lunghezza 4

*/

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
    // printf("hr modbus register (default = 0x601): ");
    puts("start HR modbus address in HEX (default = 0x601):");
    int counting_index = 0x601;
    scanf("%x", &counting_index);
    getchar();
    const int first_address = counting_index;

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof line, inFile))
    {
        char *strp;
        // puts(line);
        // if ((strp = strstr(line, MATCH_STRING)) && (strncmp(line, "(*", 2) != 0))
        if ((strp = strstr(line, MATCH_STRING)) && notCommented(line))
        {
            // converto counting_index to string
            char tempstr[5];
            int tmpstr_len = sprintf(tempstr, "%X", counting_index++);
            // aggiungo 0 all'inizio per avere lunghezza 4
            char num_str[5] = {'0', '0', '0', '0', '0'};
            num_str[4 - tmpstr_len] = '\0';
            strcat(num_str, tempstr);

            strp += MATCH_STRING_LEN;
            for (int i = 0; i < 4; ++i)
                strp[i] = num_str[i];
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

bool notCommented(char *inputstr)
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