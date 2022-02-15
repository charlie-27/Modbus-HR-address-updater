#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "updater.h"

/*
tutti gli indirizzi devono già essere presenti con lunghezza 4

*/

const char *const MATCH_STRING = "R_addr=$";
// int  = 8;
int counting_index = 0xa6b;

int main(void)
{
    const int MATCH_STRING_LEN = strlen(MATCH_STRING);
    char *inputFileStr = "in.txt";
    char *outputFileStr = "out.txt";

    FILE *inFile = fopen(inputFileStr, "r");
    if (inFile == NULL)
    {
        printf("could not read input file\n");
        return 1;
    }
    FILE *outFile = fopen(outputFileStr, "w");

    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof line, inFile))
    {
        char *strp;
        // puts(line);
        // if ((strp = strstr(line, MATCH_STRING)) && (strncmp(line, "(*", 2) != 0))
        if ((strp = strstr(line, MATCH_STRING)) && notCommented(line))
        {
            // converto counting_index to string con 0 all'inizio per renderlo lunghezza 4
            char tempstr[5];
            int tmpstr_len = sprintf(tempstr, "%X", counting_index++);
            // strlen(tempstr);
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

    return 0;
}

bool notCommented(char *inputstr)
{
    char *strp = strstr(inputstr, "(*");
    if (strp == NULL)
        return true;
    // if (strp == inputstr)
    //     return false;

    for (int i = 0; i < strp - inputstr; i++)
    {
        // if (inputstr[i] != ' ')
        if (!isspace(inputstr[i]))
            return true;
    }
    return false;
}