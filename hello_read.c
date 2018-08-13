
#include<string.h>
#include<stdio.h>


int main()
{
    FILE* fp=fopen("myfile","r");
    if(fp==NULL)
    {

        printf("open error\n");
    }
    const char* data ="hehe\n";
    char buf[1024];
    while(1){

        size_t s =fread(buf,1,strlen(data),fp);
        if(s>0){

            buf[s]=0;
            printf("%s",buf);
        }
        if(feof(fp))
        {
            break;
        }
    }
    fclose(fp);
    return 0;
}
