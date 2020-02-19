#include <stdio.h>
#include <stdlib.h>
#define NUM 100

int main(void)
{

  FILE *fp;
  int c;
  int i;
  int n=0;
  int x;
  int j;
  int count=0;
  unsigned char ary[NUM];

  fp=fopen("test.bmp","rb");

  j=fgetc(fp);

  printf("読み込み用ファイルをオープンしました.\n");


  while(j!=EOF){
    ary[n]=j;
    j=fgetc(fp);
    n++;
  }
  printf("%d個のデータを読み込み、配列に格納しました.\n",n);

  fclose(fp);

  printf("読み込み用ファイルをクローズしました.\n");

  printf("\n");

  printf("配列の内容を表示します.");

  fp=fopen("testcp.bmp","wb");

  i=0;
  while(i<n){
    fputc(ary[i],fp);
    printf("\nary[%d]=%d",i+1,ary[i]);
    i++;
  }


  printf("\n\n書き出し用ファイルをオープンしました.\n");

  printf("配列に格納されていた%d個のデータを書き出しました.\n",n);

  fclose(fp);

  printf("書き出し用ファイルをクローズしました.\n");

  return (0);
}
