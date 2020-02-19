#include <stdio.h>
#include <stdlib.h>
void get_data(void);
void rgb_to_ybr(void);
void processing(void);
void ybr_to_rgb(void);
void put_data(void);

unsigned char header[54];
unsigned char imgin[3][512][512];
unsigned char imgout[3][512][512];
int filesize = 0, offset = 0, width = 0, height = 0, bit = 0, byte = 0;

int main(void) {
  get_data();
  rgb_to_ybr();
  processing();
  ybr_to_rgb();
  put_data();
  return 0;
}

void processing(void) {
    int x, y, z;
  //配列にコピーするよ

  for (x = width-1; x >= 0; x--) {
    for (y = 0; y < height; y++) {
      for (z = 2; z >= 0; z--) {
        imgout[z][x][y] = imgin[z][x][y];
      }
    }
  }
  printf("\n入力画像データをコピーして出力画像データを作成しました.\n");
}

void get_data(void) {
  FILE *fp;
  char filename[20];
  int i, j, l, c;
  printf("入力ファイル名を入力して下さい:");
  scanf("%s", filename);
  printf("ファイルをオープンしました。\n");
  for (i = 0; i < 54; i++) {
    header[i] = fgetc(fp);
  }
  filesize = header[2] + header[3] * 16 * 16 + header[4] * 16 * 16 * 16 * 16 +
             header[5] * 16 * 16 * 16 * 16 * 16 * 16;
  offset = header[10] + header[11] * 16 * 16 + header[12] * 16 * 16 * 16 * 16 +
           header[13] * 16 * 16 * 16 * 16 * 16 * 16;
  width = header[18] + header[19] * 16 * 16 + header[20] * 16 * 16 * 16 * 16 +
          header[21] * 16 * 16 * 16 * 16 * 16 * 16;
  height = header[22] + header[23] * 16 * 16 + header[24] * 16 * 16 * 16 * 16 +
           header[25] * 16 * 16 * 16 * 16 * 16 * 16;
  bit = header[28] + header[29] * 16 * 16;
  byte = filesize - offset - width * height * (bit / 8);
  i = 0;
  c = fgetc(fp);
  for (l = height - 1; l >= 0; l--) {
    for (j = 0; j < width; j++) {
      for (i = 2; i >= 0; i--) {
        imgin[i][j][l] = c;
        c = fgetc(fp);
      }
    }
  }
  /* printf("<ファイルタイプ>\n");
     while(i<2){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("<ファイルサイズ>\n");
  /* while(i<6){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("%dバイト\n", filesize);
  /*printf("<予約領域>\n");
    while(i<10){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");*/
  printf("<オフセット>\n");
  /* while(i<14){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("%dバイト\n", offset);
  /* printf("<情報ヘッダサイズ>\n");
     while(i<18){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("<画像の幅>\n");
  /* while(i<22){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("%d画素\n", width);
  printf("<画像の高さ>\n");
  /* while(i<26){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("%dライン\n", height);
  /* printf("<色プレーン数>\n");
     while(i<28){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("<1画像あたりのビット数>\n");
  /* while(i<30){
     printf("header[%d]=%02x ",i,header[i]);
     i++;
     }
     printf("\n");*/
  printf("%dビット\n", bit);
  /*printf("<圧縮方式>\n");
    while(i<34){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");
    printf("<画像データサイズ>\n");
    while(i<38){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");
    printf("<水平解像度>\n");
    while(i<42){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");
    printf("<垂直解像度>\n");
    while(i<46){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");
    printf("<色数>\n");
    while(i<50){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");
    printf("<重要な色数>\n");
    while(i<54){
    printf("header[%d]=%02x ",i,header[i]);
    i++;
    }
    printf("\n");*/
  printf("<挿入ビット数>\n");
  printf("%dバイト\n", byte);
  fclose(fp);
  printf("ファイルをクローズしました\n");
}

void rgb_to_ybr(void) {
  int i, j;
  int Y, Cb, Cr;

  double sarasu[3][3] = {{0.2990, 0.5870, 0.1140},
                         {-0.1687, -0.3313, 0.5000},
                         {0.5000, -0.4187, -0.0813}};
  /*printf("<入力信号(RGB)>\n");
  printf("---R---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgin[0][j][i]);
    }
    printf("\n");
  }
  printf("---G---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgin[1][j][i]);
    }
    printf("\n");
  }
  printf("---B---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgin[2][j][i]);
    }
    printf("\n");
    }*/
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      Y = sarasu[0][0] * imgin[0][i][j] + sarasu[0][1] * imgin[1][i][j] +
          sarasu[0][2] * imgin[2][i][j];
      Cb = sarasu[1][0] * imgin[0][i][j] + sarasu[1][1] * imgin[1][i][j] +
           sarasu[1][2] * imgin[2][i][j];
      Cr = sarasu[2][0] * imgin[0][i][j] + sarasu[2][1] * imgin[1][i][j] +
           sarasu[2][2] * imgin[2][i][j];

      if (Y >= 0.0) {
        Y = (int)(Y + 0.5);
      } else {
        Y = (int)(Y - 0.5);
      }
      imgin[0][i][j] = (int)Y;

      if (Cb > 0.0) {
        Cb = (int)(Cb + 0.5);
      } else {
        Cb = (int)(Cb - 0.5);
      }
      Cb = Cb + 128;
      if (Cb < 0) {
        Cb = 0;
      } else if (Cb > 255) {
        Cb = 255;
      }
      imgin[1][i][j] = (int)Cb;

      if (Cr > 0.0) {
        Cr = (int)(Cr + 0.5);
      } else {
        Cr = (int)(Cr - 0.5);
      }
      Cr = Cr + 128;
      if (Cr < 0) {
        Cr = 0;
      } else if (Cr > 255) {
        Cr = 255;
      }
      imgin[2][i][j] = (int)Cr;
    }
  }

  /*  printf("<入力信号(YCbCr)>\n");
  printf("---Y---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgin[0][j][i]);
    }
    printf("\n");
  }
  printf("---Cb---\n");
  for(i=0;i<4;i++){
  for(j=0;j<4;j++){
      printf("%02X ",imgin[1][j][i]);
      }
    printf("\n");
    }
  printf("---Cr---\n");
  for(i=0;i<height;i++){
  for(j=0;j<4;j++){
    printf("%02X ",imgin[2][j][i]);
      }
    printf("\n");
    }*/
}

void ybr_to_rgb(void) {
  int i, j;
  int R, G, B;
  double resarasu[3][3] = {{1.0000, 0.0000, 1.4020},
                           {1.0000, -0.3441, -0.7141},
                           {1.0000, 1.7720, 0.0000}};
  /*printf("入力画像データをコピーして出力画像データを作成しました。\n");
    printf("<出力信号(YCbCr)>\n");
    printf("---Y---\n");
    for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[0][j][i]);
    }
    printf("\n");
  }
  printf("---Cb---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[1][j][i]);
    }
    printf("\n");
  }
  printf("---Cr---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[2][j][i]);
    }
    printf("\n");
    }*/
  for (j = 0; j < height; j++) {
    for (i = 0; i < width; i++) {
      R = resarasu[0][0] * imgout[0][i][j] +
          resarasu[0][1] * (imgout[1][i][j] - 128) +
          resarasu[0][2] * (imgout[2][i][j] - 128);
      G = resarasu[1][0] * imgout[0][i][j] +
          resarasu[1][1] * (imgout[1][i][j] - 128) +
          resarasu[1][2] * (imgout[2][i][j] - 128);
      B = resarasu[2][0] * imgout[0][i][j] +
          resarasu[2][1] * (imgout[1][i][j] - 128) +
          resarasu[2][2] * (imgout[2][i][j] - 128);
      if (R >= 0.0) {
        R = (int)(R + 0.5);
      } else {
        R = (int)(R - 0.5);
      }
      imgout[0][i][j] = (int)R;
      if (G >= 0.0) {
        G = (int)(G + 0.5);
      } else {
        G = (int)(G - 0.5);
      }
      if (G < 0) {
        G = 0;
      } else if (G > 255) {
        G = 255;
      }
      imgout[1][i][j] = (int)G;

      if (B >= 0.0) {
        B = (int)(B + 0.5);
      } else {
        B = (int)(B - 0.5);
      }
      if (B < 0) {
        B = 0;
      } else if (B > 255) {
        B = 255;
      }
      imgout[2][i][j] = (int)B;
    }
  }
  /*printf("<出力信号(RGB)>\n");
  printf("---R---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[0][j][i]);
    }
    printf("\n");
  }
  printf("---G---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[1][j][i]);
    }
    printf("\n");
  }
  printf("---B---\n");
  for(i=0;i<4;i++){
    for(j=0;j<4;j++){
      printf("%02X ",imgout[2][j][i]);
    }
    printf("\n");
    }*/
}

void put_data(void) {
  FILE *fp;
  char filename[20];
  int i, j;

  printf("出力ファイル名を入力して下さい：");
  scanf("%s", filename);

  fp = fopen(filename, "wb");

  if (fp == NULL) {
    printf("ファイルをオープンできません.\n");
    exit(1);
  }

  printf("ファイルをオープンしました.\n");

  for (i = 0; i < 54; i++) {
    fputc(header[i], fp);
  }
  for (i = height - 1; i >= 0; i--) {
    for (j = 0; j < width; j++) {
      fputc(imgout[2][j][i], fp);
      fputc(imgout[1][j][i], fp);
      fputc(imgout[0][j][i], fp);
    }
  }

  fclose(fp);
  printf("ファイルをクローズしました.\n");
}
