#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
    // TODO: rewrite this function
    int r = rand()%96; // randomly select a number between 0 and 95 to convert to ascii for all characters
    r += 32; // convert to ascii
    char c = r; //convert to a character 
    return c; //return the character
}

char *inputString()
{
    // TODO: rewrite this function
    char s[5];
    char c;
    int i = 0;
    int asc;
    for(i=0; i < 5; i++)
	{
		switch(i)
		{
			case 0:
				asc = rand()%4; //choose between 3 characters around the target
				asc += 112; //choose around r
				c = asc; //convert to char
				s[i] = c; //asign it to the string
				break;
			case 1:
				asc = rand()%4; //choose between 3 characters around the target
				asc += 99; //choose around e
				c = asc; //convert to char
				s[i] = c; //asign it to the string
				break;
			case 2:
				asc = rand()%4; //choose between 3 characters around the target
				asc += 113; //choose around s
				c = asc; //convert to char
				s[i] = c; //asign it to the string
				break;
			case 3:
				asc = rand()%4; //choose between 3 characters around the target
				asc += 99; //choose around e
				c = asc; //convert to char
				s[i] = c; //asign it to the string
				break;
			case 4:
				asc = rand()%4; //choose between 3 characters around the target
				asc += 114; //choose around t
				c = asc; //convert to char
				s[i] = c; //asign it to the string
				break;
		}
	}
    s[5] = '\0';//null terminate the string
    return s;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (1)
  {
    tcCount++;
    c = inputChar();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
