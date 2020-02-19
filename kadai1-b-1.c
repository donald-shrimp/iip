#include <stdio.h>
#include <stdlib.h>
int main(void)
{
  FILE *fp;
  int byte,num,count=0;
  /*---ファイル・オープン(書き出し用) ---*/
  fp = fopen("test.bmp", "wb");
  if (fp == NULL)
  {
    printf("ファイルをオープンできません．\n");
    exit(1);
  }
  printf("ファイルをオープンしました．\n");


  printf("何バイト書き出しますか：");
  scanf("%d",&byte);
  printf("\n0以上255以下の値を%d個入力して下さい.\n",byte);

      /*---ファイルに1バイトずつ書き出し ---*/
      for (count = 0; count < byte; count++)
  {
    printf("No.%d : ",count+1);
    scanf("%d",&num);
    fputc(num, fp);
  }
  /*--- ファイル・クローズ(書き出し用) ---*/
  fclose(fp);
  printf("\nファイルをクローズしました．\n");
  return (0);
}
