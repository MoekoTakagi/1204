/*
将棋の駒の角は，斜め方向に升目があいている限りどこへでも移動できる。
特に，空いているその先に敵の駒があれば，その位置に進んでその駒を取ることができる。
さらに，角は成ると馬となり，上下左右４つのマスにある敵の駒も取ることができるようになる。
将棋の駒にとって取ることのできる位置にある駒を当たりの駒という。
正整数 n について n×n の升目をもつ盤を考え，馬を互いに当たりにならないように配置することを考え，
最大何個置けるか、またその最大個数における配置は何通りあるかを出力するプログラムを作る。
入力は，正整数 n だけを並べた１行からなる。
出力は，配置できる最大個数とその最大個数における配置は何通りあるかを空白で区切り、末尾には改行を書き出す。
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100

int depth_first_search_left(int i); //中央より左側の深さ優先探索
int depth_first_search_right(int i); //中央を含む右側の深さ優先探索
void set_bishop(int x); //角を置く
void remove_bishop(int x); //角を除く
bool prompted_bishop(); //当たりの駒があるかどうか調べ馬に成る
void print_state(); //現在の駒の状態を表示する
bool search_slanting_bishop(int i); //斜め方向全てに角がなかったらtrueを返す
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
	int x = i; //x列
	i = n*(n-i); //i行の1列目の場所
	int j = 0;
	while(j < x){
		if(search_slanting_bishop(i)){ //左隣の列に角が置かれていなければ
			set_bishop(i); //i番目に角を置く
			if(i % (n+1) == 0){ //中央まで探索済なら1番目の列を探索する
				depth_first_search_right(1);
			}else{
				depth_first_search_left(x+1); //中央より左側なら次の行を探索する
			}
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
	//最大行数を超えたら全ての升に当たらないように置けたことになるので馬に成るかチェックし馬の数を数える
	if(i >= n){
		if(prompted_bishop()) count_bishop();
		return 1;
	}
	int x = n-i; //x列
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
	if(c == n-1){ //最後の行まで見て、馬になるものを探し馬を数える
		if(prompted_bishop()) count_bishop();
		return 1;
	}else{ //最後の行でなければ次の行を探索
		depth_first_search_right(c+1);
	}
	return 1;
}

bool search_slanting_bishop(int i){
	if(i % n == 0) return true; //一番左側なら同じ列に角はないとする

	while(i < n*n && i % n != 0){ //i列目が一番左側じゃなくマス目を越えないとき馬に成るかチェック
		i += n-1;
		if(node[i] == true) return false;
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
	return true; //馬に昇格
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
