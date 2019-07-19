#include <stdio.h>
#include <time.h>
#define MAX_DATA 1000000

int temp[MAX_DATA];    // 最小でも配列と同じサイズの領域が必要

void MergeSort(int x[ ], int left, int right);
void main(void);

// 配列 x[ ] の left から right の要素のマージソートを行う
void MergeSort(int x[ ], int left, int right)
{
    int mid, i, j, k;
    
    if (left >= right)              // 配列の要素がひとつなら
        return;                     // 何もしないで戻る
    
    // ここでは分割しているだけ
    mid = (left + right) / 2;       // 中央の値より
    MergeSort(x, left, mid);        // 左を再帰呼び出し
    MergeSort(x, mid + 1, right);   // 右を再帰呼び出し
    
    // x[left] から x[mid] を作業領域にコピー
    for (i = left; i <= mid; i++){
        temp[i] = x[i];}
    
    // x[mid + 1] から x[right] は逆順にコピー
    for (i = mid + 1, j = right; i <= right; i++, j--){
        temp[i] = x[j];}
    
    i = left;         // i とj は作業領域のデーターを
    j = right;        // k は配列の要素を指している
    
    for (k = left; k <= right; k++){  // 小さい方から配列に戻す
        if (temp[i] <= temp[j])        //ここでソートされる
            x[k] = temp[i++];
        else
            x[k] = temp[j--];}
}

void main(void)
{
    int i;
    int x[MAX_DATA];
    struct timespec startTime, endTime;
    //fileread開始
    char filename[]="testdata1000000.txt";
    FILE *fp;
    FILE *outputfile;         // 出力ストリーム
    fp = fopen(filename,"r"); //readモードでファイルオープン
    
    if( fp == NULL ) { //ファイルが読み込めない時の処理
        perror("ファイルの読み込みに失敗！\n");
        return 1;
    }
    
    for(i=0; i<MAX_DATA; i++){
        fscanf(fp,"%d",&(x[i]) );     // 1行ずつ読む
    }
    
    fclose(fp);
    
    clock_gettime(CLOCK_REALTIME, &startTime); //star時間計測
    MergeSort(x, 0, MAX_DATA - 1); //マージソート実施
    clock_gettime(CLOCK_REALTIME, &endTime); //end時間計測
    
    if (endTime.tv_nsec < startTime.tv_nsec) {
        printf("time : %5ld.%06ld s\n", endTime.tv_sec - startTime.tv_sec - 1,
               endTime.tv_nsec + (long int)1.0e+9 - startTime.tv_nsec);
    } else {
        printf("time : %5ld.%06ld s\n", endTime.tv_sec - startTime.tv_sec,
               endTime.tv_nsec - startTime.tv_nsec);
    }
    
    
    //ここからソートした結果をファイルに書き込む(filewrite開始)
    outputfile = fopen("result_m.txt", "w");  // ファイルを書き込み用にオープン(開く)
    if( outputfile == NULL ) { //ファイルが読み込めない時の処理
        perror("ファイルの書き込みに失敗！\n");
        return 1;
    }
    
    for(i=0; i<MAX_DATA; i++){
        fprintf(outputfile, "%d\n", x[i]); // ファイルに書く
    }
    
    fclose(outputfile);          // ファイルをクローズ(閉じる)
}
