/*************************************************/
/* Luke McDougall                                */
/* 11/03/2021                                    */
/* CS-241 Section 1                              */
/*************************************************/

#include <stdio.h>
#include "lcg.h"

/* function prototypes */
int encryptData(unsigned long m, unsigned long c, int ch, unsigned long x);
int decryptData(unsigned long m, unsigned long c, int ch, unsigned long x);

/***************************************************************
 * Reads in a text file char by char using '\n' as an indicator
 * to read the next line and ',' what section of the input is
 * being read, either to convernt the current string to a number
 * or start converting the data.
 * Expect the input to be in the format: 
 *          action lcg_m, lcg_c, Data \n
 * An error is thrown if the input does not match the format
 **************************************************************/
int main()
{
    int currentChar;
    int currentPosition;
    int action;
    int lineNum;
    int error;

    unsigned long m;
    unsigned long ci;
    unsigned long random;

    currentPosition = 0;
    action = 0;
    error = 0;
    m = 0;
    ci = 0;
    random = 0;
    lineNum = 1;

    while ((currentChar = getchar()) != EOF)
    {
        if (currentPosition == 0 && currentChar == 'e')
        {
            action = 1;
        }
        else if (currentPosition == 0 && currentChar == 'd')
        {
            action = 2;
        }
        if (currentChar != ',' && currentChar != '\n')
        {
            if (currentPosition == 0 && 
            currentChar >= '0' && currentChar <= '9')
            {
                m = 10 * m + (currentChar - '0');
            }
            else if (currentPosition == 1 && 
            currentChar >= '0' && currentChar <= '9')
            {
                ci = 10 * ci + (currentChar - '0');
            }
            else if (currentPosition == 2)
            {
                if (action == 1)
                {
                    struct LinearCongruentialGenerator lcg1;
                    lcg1 = makeLCG(m, ci);
                    printf("%5d: ", lineNum);
                    lineNum++;
                    if (lcg1.m == 0)
                    {
                        printf("Error");
                        while((currentChar = getchar()) != '\n');
                    }
                    else
                    {
                        random = getNextRandomValue(&lcg1);

                        while (currentChar != '\n' && error == 0)
                        {
                            error = encryptData(m, ci, currentChar, random);
                            currentChar = getchar();
                            random = getNextRandomValue(&lcg1);
                        }
                    }
                    printf("\n");
                }
                else if (action == 2)
                {
                    struct LinearCongruentialGenerator lcg2;
                    lcg2 = makeLCG(m, ci);
                    printf("%5d: ", lineNum);
                    lineNum++;
                    if (lcg2.m == 0)
                    {
                        printf("Error");
                        while((currentChar = getchar()) != '\n');
                    }
                    else
                    {
                        random = getNextRandomValue(&lcg2);
                        while (currentChar != '\n' && error == 0)
                        {
                            error = decryptData(m, ci, currentChar, random);
                            currentChar = getchar();
                            random = getNextRandomValue(&lcg2);
                        }
                    }
                    printf("\n");
                }
                else
                {
                    printf("%5d: ", lineNum);
                    lineNum++;
                    printf("Error\n");
                }
                m = ci = 0;
                currentPosition = 0;
                action = error = 0;
            }
        }
        if (currentChar == ',')
        {
            if (currentPosition == 0)
            {
                currentPosition = 1;
            }
            else if (currentPosition == 1)
            {
                currentPosition = 2;
            }
        }
        if (currentChar == '\n' && currentPosition != 0)
        {
            printf("%5d: ", lineNum);
            lineNum++;
            printf("Error\n");
            m = ci = 0;
            currentPosition = 0;
            action = 0;
        }
    }
    return 0;
}

/***************************************************************
 * Takes in 4 arguments, unsigned long m and c are used from
 * lcg to determine if an error is a present(m or c being 0)
 * while ch is the char that will be encrypted and x is the
 * random number generated from the lcg. returns 1 if an error
 * is present and returns 0 if not.
 **************************************************************/
int encryptData(unsigned long m, unsigned long c, int ch, unsigned long x)
{
    int out = ch ^ (x % 128);

    if (m == 0 || c == 0)
    {
        printf("Error\n");
        return 1;
    }

    if (out < 32)
    {
        printf("*%c", '@' + out);
    }
    else if (out == 127)
    {
        printf("*#");
    }
    else if (out == '*')
    {
        printf("**");
    }
    else
    {
        printf("%c", out);
    }
    return 0;
}

/***************************************************************
 * Takes in 4 arguments, unsigned long m and c are used from
 * lcg to determine if an error is a present(m or c being 0)
 * while ch is the char that will be decrypted and x is the
 * random number generated from the lcg. returns 1 if an error
 * is present and returns 0 if not.
 **************************************************************/
int decryptData(unsigned long m, unsigned long c, int ch, unsigned long x)
{
    int out = ch ^ (x % 128);

    if (m == 0 || c == 0)
    {
        printf("Error\n");
        return 1;
    }

    if (ch == '*')
    {
        ch = getchar();
        if (ch == '*')
        {
            out = ch ^ (x % 128);
            printf("%c",out);
        }
        else if (ch == '#')
        {
            out = 127 ^ (x % 128);
            printf("%c",out);
        }
        else if (ch > 32 && ch < 127)
        {
            out = (ch - '@') ^ (x % 128);
            printf("%c",out);
        }
        else if (ch == 32)
        {

            printf("Error");
            return 1;
        }
    }
    else
    {
        printf("%c", out);
    }
    return 0;
}