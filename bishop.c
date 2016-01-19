/*
再帰を用いて深さ優先探索をするという意味で考え方は飛車の配置と同様なので省略。
違うところは、角は斜め方向に移動できるので、斜め方向の升目には一つだけ角を置くことを考える。
例えば升目が4*4=16で与えられたら、盤を90°反転させた以下の状態で、飛車と同様に深さ優先探索を行う。

   *
  * *
 * * *
* * * *
 * * *
  * *
   *

上の状態で考えたときに、一番左の升を1行目とし、それぞれの行に対応する列数を求める。
升目が16のときならば、
1行目-1列
2行目-2列
3行目-3列
4行目-4列
5行目-3列
6行目-2列
7行目-1列
となる。
探索を行う場所が、盤の中心より左側か、中心を含む右側かで対応する列数も変わってくるため、
そこで分けて呼び出す探索関数を変えている。
*/

#include <stdio.h>
#include <stdbool.h>

#define MAX 100 //100升まで対応

int depth_first_search_left(int i); //中央より左側の深さ優先探索
int depth_first_search_right(int i); //中央を含む右側の深さ優先探索
void set_bishop(int x); //角を置く
void remove_bishop(int x); //角を除く
bool prompted_bishop(); //当たりの駒がなければ馬に成る
void print_state(); //現在の駒の状態を表示する
bool search_slanting_bishop(int i); //同列の左側の行全てに角がなかったらtrueを返す
void count_bishop(); //馬の数を数える

static int n;
static int max_counter = 0; //馬を配置できる最大個数
static int place_patterns = 0; //最大個数における配置数
static bool node[MAX];

int main(){
	scanf("%d", &n);
	depth_first_search_left(1);
	printf("%d %d\n", max_counter, place_patterns);
}

int depth_first_search_left(int i){
	int x = i; //x行かつ列数
	i = n*(n-i); //i行に関しての1列目の場所をiとする
	int j = 0;
	while(j < x){
		if(search_slanting_bishop(i)){ //左隣の列に角が置かれていなければ
			set_bishop(i); //i番目に角を置く
			if(i % (n+1) == 0)depth_first_search_right(1); //中央まで探索済なら右側の行を探索する
			else depth_first_search_left(x+1); //中央より左側なら次の行を探索する
			remove_bishop(i); //i番目の角を除く
		}
		i += n+1; //次の列を探索
		j++;
	}
	if(i % (n+1) == 0) depth_first_search_right(1);
	else depth_first_search_left(x+1);
	return 1;
}

int depth_first_search_right(int i){
	//最大行数を超えたら当たりがないように全ての升に角を置けたことになるので馬に成るかチェックし馬の数を数える
	if(i >= n){
		if(prompted_bishop()) count_bishop();
		return 1;
	}
	int x = n-i; //列数
	int c = i; //c行
	int j = 0;
	while(j < x){
		if(search_slanting_bishop(i)){ //左隣の列に角が置かれていなければ
			set_bishop(i); //i番目に角を置く
			depth_first_search_right(c+1); //次の行で探索
			remove_bishop(i); //i番目の角を除く
		}
		i += n+1; //次の列を探索
		j++;
	}
	depth_first_search_right(c+1);
	return 1;
}

bool search_slanting_bishop(int i){
	if(i % n == 0 || i >= n*(n-1)) return true; //ひし形の一番左側なら同じ列に角はないとする
	while(i < n*n && i % n != 0){
		i += n-1; //同列の左隣の行を探索
		if(node[i]) return false;
	}
	return true;
}

void set_bishop(int x){
	node[x] = true;
}

void remove_bishop(int x){
	node[x] = false;
}

bool prompted_bishop(){
	int i;
	for(i = 0; i < n*n; i++){
		if(node[i] && i == 0){ //左上
			if(node[1] || node[n]) return false;
		}else if(node[i] && i == n-1){ //右上
			if(node[i-1] || node[i+n]) return false; 
		}else if(node[i] && i == n*(n-1)){ //左下
			if(node[i-n] || node[i+1]) return false;
		}else if(node[i] && i == n*n-1){ //右下
			if(node[i-1] || node[i-n]) return false;
		}else if(node[i] && i%n == 0){ //左
			if(node[i+1] || node[i-n] || node[i+n]) return false;
		}else if(node[i] && i < n){ //上
			if(node[i-1] || node[i+1] || node[i+n]) return false; 
		}else if(node[i] && i % n == n-1){ //右
			if(node[i-1] || node[i-n] || node[i+n]) return false;
		}else if(node[i] && i > n*(n-1)){ //下
			if(node[i-n] || node[i+1] || node[i-1]) return false;
		}else if(node[i]){ //それ以外
			if(node[i-1] || node[i+1] || node[i-n] || node[i+n]) return false;
		}
	}	
	return true; //馬に成る
}

void count_bishop(){
	int i, x = 0;
	for(i = 0; i < n*n; i++){
		if(node[i]) x++; //馬の数
	}
	if(x > max_counter){
		max_counter = x;
		place_patterns = 1;
	}else if(x == max_counter){
		place_patterns++;
	}
}

void print_state(){
	int i, j;
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%d ", node[i*n+j]);
		}
		printf("\n");
	}
	printf("\n");
}
