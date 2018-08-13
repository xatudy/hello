
#include<string.h>
#include<stdio.h>

int main()
{
    const char* path="./test.txt";
    FILE* fp=fopen(path,"w");
    if(fp==NULL)
    {
     printf("open failed! %s\n",path);
     return 1;
    }
    
    const char* data = "hehe\n";
    size_t i =0;
    printf("%lu",i);
    for(;i<10;++i)
    {
      fwrite(data,strlen(data),1,fp);
    }
    fclose(fp);
  return 0;
}
