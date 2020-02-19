#include <stdio.h>
#include <stdlib.h>
void get_data(void);
void processing(void);
unsigned char header[54];
unsigned char imgin[3][512][512];

int main(void)
{
  get_data();
  processing();
    
  return 0;
}

void get_data(void)
{
  FILE *fp;
  int count = 0, i = 0, x=0,y=0,z=0;
  char filename[100];
  int filesize;
  int offset, width, height, bits;
  //unsignd char tmp[];
  /*---ファイル・オープン(読み込み用) ---*/
  printf("ファイル名を入力してください：");
  scanf("%s", filename);
  fp = fopen(filename, "rb");
  if (fp == NULL)
  {
    printf("ファイルをオープンできません．\n");
    exit(1);
  }
  printf("ファイルをオープンしました．\n");

  /*---ファイルから1バイトずつ読み込む---*/

  while (count < 54)
  {
    header[count] = fgetc(fp);
    count++;
  }
    
    
    
  //結果を出力するよ
  printf("\n＜ファイルタイプ＞\n");
  while (i < 2)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜ファイルサイズ＞\n");
  while (i < 6)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  filesize = *(int *)&header[2];
  printf("\n%dバイト\n", filesize);

  printf("\n＜予約領域＞\n");
  while (i < 10)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜オフセット＞\n");
  while (i < 14)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  offset = *(int *)&header[10];
  printf("\n%dバイト\n", offset);

  printf("\n＜情報ヘッダサイズ＞\n");
  while (i < 18)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜画像の幅＞\n");
  while (i < 22)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  width = *(int *)&header[18];
  printf("\n%d画素\n", width);

  printf("\n＜画像の高さ＞\n");
  while (i < 26)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  height = *(int *)&header[22];
  printf("\n%dライン\n", height);

  printf("\n＜色プレーン数＞\n");
  while (i < 28)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜１画素当たりのビット数＞\n");
  while (i < 30)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  bits = *(int *)&header[28];
  printf("\n%dビット\n", bits);

  printf("\n＜圧縮方式＞\n");
  while (i < 34)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜画像データサイズ＞\n");
  while (i < 38)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜水平解像度＞\n");
  while (i < 42)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜垂直解像度＞\n");
  while (i < 46)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜色数＞\n");
  while (i < 50)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜重要な色数＞\n");
  while (i < 54)
  {
    printf("header[%d]=%02x ", i, header[i]);
    i++;
  }
  printf("\n");

  printf("\n＜挿入ビット数＞\n");

  printf("%dバイト\n", filesize - offset - width * height * (bits / 8));

    for (x=width-1;x>=0;x--) {
        for (y=0;y<height;y++) {
            for (z=2;z>=0;z--) {
                imgin[z][x][y]=fgetc(fp);
            }
        }
    }
    
  /*--- ファイル・クローズ(読み込み用) ---*/
  fclose(fp);
  printf("ファイルをクローズしました．\n");
}

void processing(void){
    int x,y;
    printf("\n入力画像データを表示します\n\n");
    
    //RGBを表示するよ
    printf("＜R信号＞\n");
    for (x=0;x<4;x++) {
        for (y=0;y<4;y++) {
            printf("%02x ",imgin[0][x][y]);
        }
        printf("\n");
    }
    printf("＜G信号＞\n");
    for (x=0;x<4;x++) {
        for (y=0;y<4;y++) {
            printf("%02x ",imgin[1][x][y]);
        }
        printf("\n");
    }
    printf("＜B信号＞\n");
    for (x=0;x<4;x++) {
        for (y=0;y<4;y++) {
            printf("%02x ",imgin[2][x][y]);
        }
        printf("\n");
    }
}