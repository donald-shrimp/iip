#define MAXLENGTH 100
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

void get_data(void);
void processing(void);
unsigned char imgin[3][512][512];
unsigned char header[54];

int main(void){
  get_data();
  processing();
  return 0;
}

void get_data(void){

  FILE *fp;
  int i,j=0,filesize,offset,width,height,bits,c,x,y;
  char filename[MAXLENGTH];
  printf("ファイル名を入力して下さい.：");
  scanf("%s",filename);
  fp=fopen(filename,"rb");


  if (fp==NULL)
    {
      printf("ファイルをオープンできません.\n");
      exit (1);
    }

  while (j<54)
    {
      header[j]=fgetc(fp);
      j++;
    }

  printf("ファイルをオープンしました.\n");
  printf("<ファイルタイプ>\n");
  for(i=0;i<2;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n");
  printf("\n<ファイルサイズ>\n");
  for(i=2;i<6;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  filesize=*(int*)&header[2];
  printf("\n");
  printf("%dバイト\n",filesize);
  printf("\n<予約領域>\n");
  for(i=6;i<10;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n");
  printf("\n<オフセット>\n");
  for(i=10;i<14;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  offset=*(int*)&header[10];;
  printf("\n%dバイト\n",offset);
  printf("\n情報ヘッダサイズ>\n");
  for(i=14;i<18;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<画像の幅>\n");
  for(i=18;i<22;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  width=*(int*)&header[18];;
  printf("\n%d画素\n",width);
  printf("\n<画像の高さ>\n");
  for(i=22;i<26;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  height=*(int*)&header[22];
  printf("\n%dライン\n",height);
  printf("\n<色プレーン数>\n");
  for(i=26;i<28;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }

  printf("\n\n<1画素当たりのヒット数>\n");
  for(i=28;i<30;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  bits=*(int*)&header[28];
  printf("\n%dビット\n",bits);
  printf("\n<圧縮方式>\n");
  for(i=30;i<34;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<画像データサイズ>\n");
  for(i=34;i<38;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<水平解像度>\n");
  for(i=38;i<42;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<垂直解像度>\n");
  for(i=42;i<46;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<色数>\n");
  for(i=46;i<50;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }
  printf("\n\n<重要な色数>\n");
  for(i=50;i<54;i++){
    printf("header[%d]=%02x ",i,header[i]);
  }

  printf("\n\n<挿入ビット数>\n");
  printf("%dバイト\n",filesize-offset-width*height*(bits/8));

  for(c=0;c<3;c++){
    for(y=0;y<height;y++){
      for(x=0;x<width;x++){
        imgin[c][x][y]=fgetc(fp);
      }
    }
  }

  fclose(fp);
  printf("\n\nファイルをクローズしました.\n");
}

void processing(void){
  FILE *fp;
  int c=0,x,y;

  printf("入力画像データを表示します.\n");
  printf("\n<R信号>\n");

  c=0;
    for(y=0;y<4;y++){
      for(x=0;x<4;x++){
	printf("%02x ",imgin[c][x][y]);
      }
      printf("\n");
    }

  printf("\n<G信号>\n");

  c=1;
    for(y=0;y<4;y++){
      for(x=0;x<4;x++){
        printf("%02x ",imgin[c][x][y]);
      }
      printf("\n");
    }

  printf("\n<B信号>\n");

  c=2;
    for(y=0;y<4;y++){
      for(x=0;x<4;x++){
        printf("%02x ",imgin[c][x][y]);
      }
      printf("\n");
    }
}
