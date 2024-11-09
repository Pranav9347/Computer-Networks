#include<stdio.h>
#include<string.h>

int n;
char data[50];
char bs[50];
void bit_stuff()
{
    int scount=0,i=0;
    char c = 'a';
    while(c!='\0')
    {
        c = data[i];
        if(c == '1')
            scount++;
        if(scount == 5)
        {
            //move data bits to the right by 1 and stuff 0:
            for(int j=n+1;j>i+1;j--)
            {
                data[j]=data[j-1];
            }
            data[i+1]='0';
            scount=0;
        }
        i++;
    }
}
void receiver()
{
    int scount=0,i=0;
    char c = 'a';
    while(c!='\0')
    {
        c = data[i];
        if(c == '1')
            scount++;
        if(scount == 5)
        {
            //move data bits to the right by 1 and stuff 0:
            for(int j=i+1;j<n+1;j++)
            {
                data[j]=data[j+1];
            }
            n--;
            scount=0;
        }
        i++;
    }
    return;
}
int main()
{
    printf("Enter the data input: ");
    scanf(" %s",data);
    n = strlen(data);
    bit_stuff(data);
    printf("Data after bit stuffing: %s\n",data);
    n = strlen(data);
    receiver();
    printf("Original string of bits: %s\n",data);
    return 0;
}