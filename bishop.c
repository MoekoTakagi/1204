/*
将棋の駒の角は，斜め方向に升目があいている限りどこへでも移動できる。
特に，空いているその先に敵の駒があれば，その位置に進んでその駒を取ることができる。
さらに，角は成ると馬となり，上下左右４つのマスにある敵の駒も取ることができるようになる。
将棋の駒にとって取ることのできる位置にある（敵の）駒を当たりの駒という。

正整数 n について n×n の升目をもつ盤を考え，馬を互いに当たりにならないように配置することを考え，
最大何個置けるか、またその最大個数における配置は何通りあるかを出力するプログラムを作れ。

入力は，正整数 n だけを並べた１行からなる。
出力は，配置できる最大個数とその最大個数における配置は何通りあるかを空白で区切り、末尾には改行を書き出せ。

行iを0スタートと1スタートでそれぞれi += n+1していく。
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 100 //jが置かれているかどうか

int depth_first_search_left(int i); //深さ優先探索
int depth_first_search_right(int i); //深さ優先探索
void set_bishop(int x);
void remove_bishop(int x);
bool prompted_bishop(); //馬になったとき当たりの駒があるかどうか
void print_state(); //現在の駒の状態を表示する
bool search_slanting_bishop(int i); //斜め方向全てに馬がなかったらtrueを返す
void count_bishop(); //馬の数を数える

static int n;
static bool hit[MAX] = {false};
static int max_counter = 0;
static bool node[MAX];
static int place_patterns = 0;

int main(){
	scanf("%d", &n);
	depth_first_search_left(1);
	printf("%d %d\n", max_counter, place_patterns);

}

int depth_first_search_left(int i){
	int x = i; //xはjを呼ぶ回数かつ引く数
	i = n*(n-i);
	int c = i;

	int j = 0;
	while(j < x){
		if(search_slanting_bishop(i) == true){ //j列に馬が置かれていなければ
			set_bishop(i); //iに馬を置く
	//		printf("馬を置いた場所%d\n", i);
			if(i % (n+1) == 0){
				depth_first_search_right(1);
			}else{
				depth_first_search_left(x+1); //次の行で探索。左側にいる場合
			}
			remove_bishop(i);
		}
		i += n+1;
		j++;
	}

//	print_state();	
//	if(prompted_bishop() == true){
//		print_state();
//		printf("きた\n");
//		count_bishop();
//	}
	if(c % (n+1) == 0){
	//	printf("cは%d\n", c);
		depth_first_search_right(1);
	}else{
		depth_first_search_left(x+1);
	}
	return 1;
}

int depth_first_search_right(int i){
	if(i >= n){
		if(prompted_bishop() == true){
	//		printf("こっち\n");
//			print_state();
			count_bishop();
		}
//		print_state();
		return 1;
	}
//	printf("右\n");
	int x = n-i; //xはjを呼ぶ回数
	int c = i;
	int j = 0;
//	printf("%dがjを呼ぶ回数は%d\n", i, x);
	while(j < x){
		if(search_slanting_bishop(i) == true){ //j列に馬が置かれていなければ
			set_bishop(i); //iに馬を置く
	//		printf("馬を置いた場所%d\n", i);
			depth_first_search_right(c+1); //次の行で探索
			remove_bishop(i); //iの馬を除く
	//		printf("除いた馬%d\n", i);
		}
		i += n+1;
		j++;
	}
	if(c == n-1){
		if(prompted_bishop() == true){
	//		printf("ココ\n");
	//		print_state();
			count_bishop();
		}
//		print_state();
		return 1;
	}else{
		depth_first_search_right(c+1);
	}
	return 1;
}

bool search_slanting_bishop(int i){
	if(i % n == 0){
		return true;
	}
//	printf("iが%dのとき\n", i);

	while(i < n*n && i % n != 0){
		i += n-1;
	//	printf("iは%d\n", i);
		if(node[i] == true){
			return false;
		}
		
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
//	print_state();
	for(i = 0; i < n*n; i++){
		if(node[i] == true && i == 0){ //左上

			if(node[1] == true || node[n] == true){
		//		print_state();
				return false;
			}
		}else if(node[i] == true && i == n-1){ //右上
			if(node[i-1] == true || node[i+n] == true) return false; 
		}else if(node[i] == true && i == n*(n-1)){ //左下
			if(node[i-n] == true || node[i+1] == true) return false;
		}else if(node[i] == true && i == n*n-1){ //右下
			if(node[i-1] == true || node[i-n] == true) return false;
		}else if(node[i] == true && i%n == 0){ //左
			if(node[i+1] == true || node[i-n] == true || node[i+n] == true) return false;
		}else if(node[i] == true && i < n){ //上
			if(node[i-1] == true || node[i+1] == true || node[i+n] == true) return false; 
		}else if(node[i] == true && i % n == n-1){ //右
			if(node[i-1] == true || node[i-n] == true || node[i+n] == true) return false;
		}else if(node[i] == true && i > n*(n-1)){ //下
			if(node[i-n] == true || node[i+1] == true || node[i-1] == true) return false;
		}else if(node[i] == true){ //それ以外
			if(node[i-1] == true || node[i+1] == true || node[i-n] == true || node[i+n] == true){
		//		print_state();
				return false;
			}
		}
	}
//	print_state();
	
	return true; //途中で戻らなければ馬に昇格
}

void count_bishop(){
	int i, x = 0;
	for(i = 0; i < n*n; i++){
	//	printf("%d ", node[i]);
		if(node[i] == true){
			x++;
		}
	}
//	printf("x = %d\n", x);
//	printf("\n");
	if(x > max_counter){
//		print_state();
		max_counter = x;
		place_patterns = 1;
	}else if(x == max_counter){
//		print_state();
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
