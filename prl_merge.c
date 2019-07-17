#include <stdio.h>
#include<time.h>
#include <omp.h>
#define MAX_DATA 1000000

int temp[MAX_DATA];    // 最小でも配列と同じサイズの領域が必要

void Merge(int x[], int left, int right);
void MergeSort(int x[ ], int left, int right);
void main(void);

// 配列 x[] の left から right の要素のマージソートを行う
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
    
    for (k = left; k <= right; k++) {   // 小さい方から配列に戻す
        if (temp[i] <= temp[j])        //ここでソートされる
            x[k] = temp[i++];
        else
            x[k] = temp[j--];}
    
}

//配列内の left から right までをマージする関数
void Merge(int x[], int left, int right){
    int mid,a,b,c,i,j,k;
    
    mid = (left + right) / 2;
    // x[left] から x[mid] を作業領域にコピー
    for (i = left; i <= mid; i++){
        temp[i] = x[i];}
    
    // x[mid + 1] から x[right] は逆順にコピー
    for (i = mid + 1, j = right; i <= right; i++, j--){
        temp[i] = x[j];}
    
    b=left;
    c=right;
    
    for (k = left; k <= right; k++){   // 小さい方から配列に戻す
        if (temp[b] <= temp[c])        //ここでソートされる
            x[k] = temp[b++];
        else
            x[k] = temp[c--];}
    
}

void main(void)
{
    int i,n,m;
    int x[MAX_DATA];
    double start, end;
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
    
    start = omp_get_wtime(); //--------------------start時間計測--------------------------
    
    n = omp_get_max_threads();//スレッド数取得
    
    printf("max threads (set): %d\n", n);
    m = MAX_DATA / n; //１スレッドが担当するデータ数決定
    
#pragma omp parallel sections
{
    //分割したそれぞれをソート
    #pragma omp section
    {
    MergeSort(x, 0*m, 1*m-1);
    //printf("1\n");
    }
    #pragma omp section
    {
    MergeSort(x, 1*m, 2*m-1);
    //printf("2\n");
    }
    #pragma omp section
    {
    MergeSort(x, 2*m, 3*m-1);
    //printf("3\n");
    }
    #pragma omp section
    {
    MergeSort(x, 3*m, 4*m-1);
    //printf("4\n");
    }
}
    
    //ソートしたものをマージ
#pragma omp parallel sections
{
    #pragma omp section
    {
    Merge(x,0*m,2*m-1);
    }
    #pragma omp section
    {
    Merge(x,2*m,4*m-1);
    }
}
    
    Merge(x,0*m,4*m-1);
    
    
    end = omp_get_wtime(); //-------------------end時間計測------------------------------
    
    printf("time : %f s\n",end - start);
    
    //ここからソートした結果をファイルに書き込む(filewrite開始)
    outputfile = fopen("result_prl.txt", "w");  // ファイルを書き込み用にオープン(開く)
    if( outputfile == NULL ) { //ファイルが読み込めない時の処理
        perror("ファイルの書き込みに失敗！\n");
        return 1;
    }
    
    for(i=0; i<MAX_DATA; i++){
        fprintf(outputfile, "%d\n", x[i]); // ファイルに書く
    }
    
    fclose(outputfile);          // ファイルをクローズ(閉じる)
}
