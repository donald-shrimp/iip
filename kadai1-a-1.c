#include <stdio.h>
#include <stdlib.h>
int main(void){
  FILE *fp;
  int c,count=0;
  /*---ファイル・オープン(読み込み用) ---*/
  fp=fopen("test.bmp","rb");
  if (fp==NULL){
    printf("ファイルをオープンできません．\n");
    exit (1);
  }
  printf("ファイルをオープンしました．\n");
  /*---ファイルから1バイトずつ読み込みディスプレイに表示---*/
  c=fgetc(fp);
  while (c!=EOF) {
    printf("%3d\n",c);
    c=fgetc(fp);
      count++;
  }
  /*--- ファイル・クローズ(読み込み用) ---*/
  fclose(fp);
    printf("全部で%dバイト読み込みました。\n",count);
  printf("ファイルをクローズしました．\n");
  return (0);
}
