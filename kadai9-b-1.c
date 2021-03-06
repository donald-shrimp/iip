#include <stdio.h>

void get_data(void);
void processing(void);
void put_data(void);
void rgb_to_ybr(void);
void ybr_to_rgb(void);
int round_off(double data);
int limit(int data);


unsigned char header[54];
unsigned char imgin[3][512][512];
unsigned char imgout[3][512][512];
double tmp_r2y[3][512][512] = {};
double tmp_y2r[3][512][512] = {};
int filesize, width, height, ins_byte;
const double convert[3][3] = {  //変換式(RGB -> YCbCr)
    {0.2990, 0.5870, 0.1140},
    {-0.1687, -0.3313, 0.5000},
    {0.5000, -0.4187, -0.0813}};
const double convert_rev[3][3] = {  //変換式(YCbCr -> RGB)
    {1.0000, 0.0000, 1.4020},
    {1.0000, -0.3441, -0.7141},
    {1.0000, 1.7720, 0.0000}};

int main(void) {
  get_data();
  rgb_to_ybr();
  
  processing();

  ybr_to_rgb();
  put_data();
  return 0;
}

void processing(void) {
  int x, y, z,yellow[256]={0},max=0,maxcollor,normalyellow[256];

  for(z=0;z<256;z++){
    for (x = 0; x < height; x++) {
      for (y = 0; y < width; y++) {
        if (z==imgin[0][x][y]) {
          yellow[z]++;
        }
      }
    }
  }

  printf("\n＜最大度数＞\n");
  
  for(z=0;z<256;z++){
    if(max<yellow[z]){
      max=yellow[z];
      maxcollor=z;
    }
  }
  printf("画素値\t%d : \t%d \n",maxcollor,max);
  for(z=0;z<256;z++){
    normalyellow[z]=(int)((double)yellow[z]/(double)max*100+0.5);
  } 
  

  printf("\n＜度数表＞\n");
  for(z=0;z<256;z++){
    printf("画素値\t%d : \t%d -->　\t%d \n",z,yellow[z],normalyellow[z]);
  }  

  // ヒストグラム付き画像を出力するよ
  for(z=0;z<256;z++){
    for (x = 0; x < height; x++) {
      for (y = 0; y < width; y++) {
        imgout[0][x][y] = imgin[0][x][y];
        imgout[1][x][y] = imgin[1][x][y];
        imgout[2][x][y] = imgin[2][x][y];
        if(y<256&&x>height-100){
          imgout[0][x][y] = 0;
          imgout[1][x][y] = 128;
          imgout[2][x][y] = 128;
          if(height-100-normalyellow[z]>=0){
            imgout[0][x][y] = 255;
            imgout[1][x][y] = 128;
            imgout[2][x][y] = 128;
          }
        }
      }
    }
  }

  printf("\n入力画像データをコピーして出力画像データを作成しました.\n");
}

void get_data(void)  //ヘッダのデータを取得
{
  FILE *fp;            //ファイルポインタ
  char filename[100];  //ファイル名格納
  int i, j, k;         //ループカウンタ

  //参照ファイル名入力
  printf("入力ファイル名を入力してください：");
  scanf("%s", filename);

  //バイナリファイル読み取りでfopen
  fp = fopen(filename, "rb");

  if (fp == NULL)  //エラー処理
  {
    printf("ファイルが存在しませんでした\n");
    return;
  }

  printf("ファイルをオープンしました\n");

  //配列にデータ格納
  for (i = 0; i < 54; i++) {
    header[i] = fgetc(fp);
  }

  filesize = *(int *)&header[2];
  width = *(int *)&header[18];   //幅
  height = *(int *)&header[22];  //高さ
  ins_byte = *(int *)&header[2] - *(int *)&header[10] -
             *(int *)&header[18] * *(int *)&header[22] *
                 (*(short *)&header[28] / 8);  //挿入バイト数

  //以下データ出力部
  // printf("＜ファイルタイプ＞\nheader[0]=%02x
  // header[1]=%02x\n",header[0],header[1]);

  printf("\n＜ファイルサイズ＞\n");
  printf("\n%dバイト\n", filesize);

  // printf("\n＜予約領域＞\nheader[6]=%02x header[7]=%02x header[8]=%02x
  // header[9]=%02x\n",header[6],header[7],header[8],header[9]);

  printf("\n＜オフセット＞\n");
  printf("\n%dバイト\n", *(int *)&header[10]);

  // printf("\n＜情報ヘッダサイズ＞\nheader[14]=%02x header[15]=%02x
  // header[16]=%02x
  // header[17]=%02x\n",header[14],header[15],header[16],header[17]);

  printf("\n＜画像の幅＞\n");
  printf("\n%d画素\n", width);

  printf("\n＜画像の高さ＞\n");
  printf("\n%dライン\n", height);

  // Printf("\n＜色プレーン数＞\nheader[26]=%02x
  // header[27]=%02x\n",header[26],header[27]);

  printf("\n＜１画素当たりのビット数＞\n");
  printf("\n%oバイト\n", *(short *)&header[28]);

  // printf("\n＜圧縮方式＞\nheader[30]=%02x header[31]=%02x header[32]=%02x
  // header[33]=%02x\n",header[30],header[31],header[32],header[33]);

  // printf("\n＜画像データサイズ＞\nheader[34]=%02x header[35]=%02x
  // header[36]=%02x
  // header[37]=%02x\n",header[34],header[35],header[36],header[37]);

  // printf("\n＜水平解像度＞\nheader[38]=%02x header[39]=%02x header[40]=%02x
  // header[41]=%02x\n",header[38],header[39],header[40],header[41]);

  // printf("\n＜垂直解像度＞\nheader[42]=%02x header[43]=%02x header[44]=%02x
  // header[45]=%02x\n",header[42],header[43],header[44],header[45]);

  // printf("\n＜色数＞\nheader[46]=%02x header[47]=%02x header[48]=%02x
  // header[49]=%02x\n",header[46],header[47],header[48],header[49]);

  // printf("\n＜重要な色数＞\nheader[50]=%02x header[51]=%02x header[52]=%02x
  // header[53]=%02x\n",header[50],header[51],header[52],header[53]);

  printf("\n＜挿入ビット数＞\n%dバイト\n", ins_byte);

  //画像データ格納
  for (i = height - 1; i >= 0; i--) {
    for (j = 0; j < width; j++) {
      for (k = 2; k >= 0; k--) {
        imgin[k][i][j] = fgetc(fp);
      }
    }
  }

  fclose(fp);
  printf("ファイルをクローズしました\n\n");
}

void put_data(void) {
  int i, j, k;
  char new_fname[100];
  printf("出力ファイル名を入力してください：");
  scanf("%s", new_fname);

  FILE *fp;
  fp = fopen(new_fname, "w");

  if (fp == NULL) {
    printf("ファイルをオープンできません\n");
    return;
  }

  printf("ファイルをオープンしました\n");

  for (int i = 0; i < 54; i++) {
    fputc(header[i], fp);
  }

  for (i = height - 1; i >= 0; i--) {
    for (j = 0; j < width; j++) {
      for (k = 2; k >= 0; k--) {
        fputc(imgout[k][i][j], fp);
      }
    }
  }

  fclose(fp);
  printf("ファイルをクローズしました\n");
}

void rgb_to_ybr(void) {
  int i, j, k;
  /*
  char rgb[3]={"RGB"};
  char ycbcr[3][2]={"Y","Cb","Cr"};

  printf("入力信号＜RGB＞\n");

  for(i=0;i<3;i++)
      {
          //出力信号が何か出力
          printf("---%c---\n",rgb[i]);

          for(j=0;j<height;j++)
          {
              for(k=0;k<width;k++)
                  {
                      printf("%02x ",imgin[i][j][k]);
                      if((k+1)%width==0)printf("\n");
                  }
          }
          printf("\n");
      }
  */

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      for (k = 0; k < 3; k++) {
        tmp_r2y[0][i][j] += convert[0][k] * imgin[k][i][j];
        tmp_r2y[1][i][j] += convert[1][k] * imgin[k][i][j];
        tmp_r2y[2][i][j] += convert[2][k] * imgin[k][i][j];
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < height; k++) {
        if (i == 0)
          imgin[i][j][k] = limit(round_off(tmp_r2y[i][j][k]));
        else
          imgin[i][j][k] = limit(round_off(tmp_r2y[i][j][k]) + 128);
      }
    }
  }

  /*
  printf("入力信号＜YCbCr＞\n");
  for(i=0;i<3;i++)
      {
          //出力信号が何か出力
          printf("---%s---\n",ycbcr[i]);

          for(j=0;j<height;j++)
          {
              for(k=0;k<width;k++)
                  {
                      printf("%.2x ",imgin[i][j][k]);
                      if((k+1)%width==0)printf("\n");
                  }
          }
      printf("\n");
      }
  */
}

void ybr_to_rgb(void) {
  /*
  char rgb[3]={'R','G','B'};
  char ycbcr[3][2]={"Y","Cb","Cr"};

  printf("出力信号＜YCbCr＞\n");
  for(i=0;i<3;i++)
      {
          //出力信号が何か出力
          printf("---%s---\n",ycbcr[i]);

          for(j=0;j<height;j++)
          {
              for(k=0;k<width;k++)
                  {
                      printf("%02x ",imgout[i][j][k]);
                      if((k+1)%width==0)printf("\n");
                  }
          }
          printf("\n");
      }
  */
  short hoge[3][512][512] = {};

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        hoge[i][k][j] = imgout[i][k][j];
      }
    }
  }

  for (int i = 1; i < 3; i++) {
    for (int j = 0; j < height; j++) {
      for (int k = 0; k < width; k++) {
        hoge[i][k][j] -= 128.0;
      }
    }
  }

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < 3; k++) {
        tmp_y2r[0][i][j] += convert_rev[0][k] * hoge[k][i][j];
        tmp_y2r[1][i][j] += convert_rev[1][k] * hoge[k][i][j];
        tmp_y2r[2][i][j] += convert_rev[2][k] * hoge[k][i][j];
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < width; j++) {
      for (int k = 0; k < height; k++) {
        imgout[i][j][k] = limit(round_off(tmp_y2r[i][j][k]));
      }
    }
  }

  /*
  printf("出力信号＜RGB＞\n");
  for(i=0;i<3;i++)
      {
          //出力信号が何か出力

          printf("---%c---\n",rgb[i]);

          for(j=0;j<height;j++)
          {
              for(k=0;k<width;k++)
                  {
                      printf("%.2X ",imgout[i][k][j]);
                      if((k+1)%width==0)printf("\n");
                  }
          }
          printf("\n");
      }
  */
}

int round_off(double data) {
  if (data >= 0)
    data += 0.5;
  else
    data -= 0.5;

  return (int)data;
}

int limit(int data) {
  if (data > 255)
    data = 255;
  else if (data < 0)
    data = 0;

  return data;
}

