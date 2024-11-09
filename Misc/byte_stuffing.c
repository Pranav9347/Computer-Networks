#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define DATA_SIZE 20
#define FLAG_BYTE "7E"
#define ESC "7D"
char** data;
char** stuffed;
char** final_data;
int j=0;
    int n;

void byte_stuff(char** data,int n)
{
    for(int i=0;i<n;i++)
    {
        if(strcmp(data[i],FLAG_BYTE)==0)
        {
            stuffed=(char**)realloc(stuffed,2*sizeof(char*)+sizeof(char*)*(j));
            stuffed[j]=(char*)malloc(sizeof(char)*3);
            stuffed[j+1]=(char*)malloc(sizeof(char)*3);
            strcpy(stuffed[j++],"7D");
            strcpy(stuffed[j++],"5E");
            continue;
        }
        if(strcmp(data[i],ESC)==0)
        {
            stuffed=(char**)realloc(stuffed,2*sizeof(char*)+sizeof(char*)*(j));
            stuffed[j]=(char*)malloc(sizeof(char)*3);
            stuffed[j+1]=(char*)malloc(sizeof(char)*3);
            strcpy(stuffed[j++],"7D");
            strcpy(stuffed[j++],"5D");
        }
        else
        {
            stuffed=(char**)realloc(stuffed,sizeof(char*)+sizeof(char*)*(j));
            stuffed[j]=(char*)malloc(sizeof(char)*3);
            strcpy(stuffed[j++],data[i]);
        } 
    }
    for(int i=0;i<j;i++)
    {
        printf("%s ",stuffed[i]);
    }
}
void remove_byte_stuff(char** stuffed)
{
    int k=0;
    final_data=(char**)malloc(sizeof(char*)*n);
    for(int i=0;i<n;i++)
        final_data[i]=(char*)malloc(sizeof(char)*3);
    for(int i=0;i<j;i++)
    {
        if(strcmp(stuffed[i],"7D")==0 && strcmp(stuffed[i+1],"5E")==0)
        {
            strcpy(final_data[k++],"7E");
            i++;
            continue;
        }
        if(strcmp(stuffed[i],"7D")==0 && strcmp(stuffed[i+1],"5D")==0)
        {
            strcpy(final_data[k++],"7D");
            i++;
            continue;
        }
        else
        {
            strcpy(final_data[k++],stuffed[i]);
        } 
    }
    for(int i=0;i<k;i++)
        printf("%s ",final_data[i]);
}

int main()
{
    printf("Enter the size of data-frame: ");
    scanf("%d",&n);
    data=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++)
        data[i]=(char*)malloc(3*sizeof(char));
    
    printf("Enter the input data(in hex with space after each byte): ");
    for(int i=0;i<n;i++)
        scanf(" %s",data[i]);
    for(int i=0;i<n;i++)
        printf("%s ",data[i]);
        printf("\n");
    printf("Original data: ");
        printf("\n");
    printf("Data after byte stuffing: ");
    byte_stuff(data,n);
    printf("\n");
    printf("Data after removing byte stuffing: ");
    remove_byte_stuff(stuffed);
    printf("\n");
    return 0;
}