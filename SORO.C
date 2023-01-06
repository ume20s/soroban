/*
 * そろばん [ soro.c ]
 *      first type  : 1991/02/03
 */

static char rcsid[]="$Header: soro.cv  1.21  91/03/11 21:13:38  UME20  Exp $";

#include <conio.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "g_lio.h"
#include "mouse.h"

#define		ADD		0			/* 計算旗(ﾌﾗｸﾞ)用 */
#define		SUB		1
#define		OK		0			/* いろいろと使うと思う */
#define		NO		1

#define		UE		10			/* お話エリアの上限 */

/*********************************************************************
 ******* 関数プロトタイプ ********************************************
 ********************************************************************/

/******* main *******************************************************/
void	main(int argc,char *argv[]);	/* メイン */
void	ojiisan(void);					/* デモ・モード */
void	dentak(void);					/* デンタク・モード */
void	mouse(void);					/* マウス・モード */

/******* ojiisan ****************************************************/
void	opening(void);					/* オープニング */
void	kaisetu(void);					/* 最初の解説 */
time_t	keisann(void);					/* ネバーエンディング(？)計算 */
void	ending(time_t end_time);		/* エンディング */
void	hit_any_key(void);				/* 何かのキーを押してね */
void	pr_soro(char *moji);			/* スクロールエリア内文字列表示 */
void	inc_in(int *in,int keta);		/* 入力変数＋＋ */
void	disp_in(int *in);				/* 入力変数表示 */

/******* den_tak mode ***********************************************/
void	disp_window(void);				/* デンタク・ウインドウ */
void	disp_res(int *in,int keta);		/* 入力変数表示 */
void	seikika(int *in,int keta);		/* 入力数値の正規化 */

/******* ojiisan & den-tak ******************************************/
void	disp_soro(int suuji,int p);		/* 数値のそろばん表示 */
int		chk_overflow(int *st,int *in,int cal_flag);	/* 桁あふれチェック */
void	add_soro(int *st,int in,int p);	/* 指定桁の足し算 */
void	sub_soro(int *st,int in,int p);	/* 指定桁の引き算 */

/******* general ****************************************************/
void	set_stop_signal(void);			/* シグナルのセット */
void	disp_init(int up);				/* そろばんの用意 */
void	get_tama(void);					/* そろばんの珠取得 */
void	calc_tamaichi(int up);			/* 珠描画位置計算 */
void	quit(void);						/* 終了処理 */

void	usage(void);					/* 使い方 */
void	err_no_memory(void);			/* メモリ足らぬ */
void	err_signal(void);				/* シグナルおかしい */
void	err_nandaka(char *mes);			/* なんだかわからない */

/*********************************************************************
 ******* 広域変数（そろばん関係は面倒だから全部広域変数(汗;) *********
 ********************************************************************/
static char	*tama;					/* 珠graphic */
static char	*boh;					/* 棒graphic */
static int	leng;					/* graphicの大きさ */
static int	x[30];					/* 珠描画位置（Ｘ座標）*/
static int	y_u[2];					/* 珠描画位置（上珠）*/
static int	y_l[5];					/* 珠描画位置（下珠）*/
static char pat[] = {0x55,0x55,0x55,0xaa,0xaa,0xaa};
									/* ペイント用パターン */

/*********************************************************************
 ******* メイン ******************************************************
 ********************************************************************/
void	main(int argc,char *argv[])
{
	/* 引数がなければデフォルト動作 */
	if(argc == 1){
		ojiisan();
		/*NOTREACHED*/
	} else { 
	/* 引数があれば */
		++argv;
		if(**argv == '-'){
			switch(*(*argv + 1)) {
				case 'd':				/* デンタク・モード */
					dentak();
					/*NOTREACHED*/
				case 'm':				/* マウス・モード */
					mouse();
					/*NOTREACHED*/
				default:				/* 引数が d,m以外 */
					usage();
					/*NOTREACHED*/
			}
		} else {
			usage();					/* 引数はふつー '-'(笑) */
			/*NOTREACHED*/
		}
	}
}

/*********************************************************************
 ******* デモンストレーション・モード（デフォルト動作） **************
 ********************************************************************/
void	ojiisan(void)
{
	time_t	end_time;		/* 実行時間 */
	
	printf("\x1b[2J\x1b[>5h\x1b[>1h");
	opening();							/* そろばん表示前の会話 */
	kaisetu();							/* そろばんを動かしつつ解説 */
	end_time = keisann();				/* 計算して実行時間を返す */
	ending(end_time);					/* お礼と実行時間の表示 */
	exit(0);
	/*NOTREACHED*/
}

/******* オープニング ***********************************************/
void	opening(void)
{
	printf("孫　「ねぇねぇ，おじいちゃん．」\n");
	printf("老人「なんぢゃ？」\n\n");
	hit_any_key();
	printf("孫　「“そろばん”ってなーに？」\n");
	printf("老人「ああ，おまわりさんがいつもいる所ぢゃよ」\n");
	printf("孫　「それは“交番”」\n");
	printf("老人「ん？　なら，宇宙刑事のことぢゃ．」\n");
	printf("孫　「それは“ギャバン”！」\n");
	printf("老人「おまえが学校へ行くときに持って行く袋の事かの？」\n");
	printf("孫　「それは“かばん”！」\n");
	printf("老人「練習じゃない……」\n");
	printf("孫　「それは“本番”！」\n");
	printf("老人「お店の名前を書いた……．」\n");
	printf("孫　「“看板”！」\n");
	printf("老人「掃除……」\n");
	printf("孫　「“当番”！！」\n");
	printf("老人「夜のあいさつ」\n");
	printf("孫　「“こんばんは”！！！」 \n");
	printf("老人「……」\n");
	printf("孫　「……」\n\n");
	hit_any_key();
	printf("老人，孫「どうも しつれーしやしたー！！！！」\n\n");
	hit_any_key();
	printf("孫　（おじいちゃん，今回のダジャレはちょっと苦しいよ）\n");
	printf("老人（うっさいわいっ(笑)）\n\n");
	hit_any_key();
	printf("\x1b[3A孫　「そーじゃなくってー！」　　　　　　　　　　　　　\n");
	printf("老人「わかっとるよ，“そろばん”ぢゃろう？\n");
	printf("　　　そろばんってのはだなぁ，漢字では『算盤』と書いてな，\n");
	printf("　　　まぁ，一種の計算器の事だわな．」\n");
	printf("孫　「へー，計算器ぃ……．」\n");
	printf("老人「そうぢゃ，そしてこいつに上達すると，\n");
	printf("　　　なんとコンピュータよりも速く答えを出せるんぢゃ．」\n");
	printf("孫　「えぇーーーーっ！？　コンピュータよりも速くぅ？？？」\n\n");
	hit_any_key();
	printf("老人「おぉ，昔はよくテレビで\n");
	printf("　　　　　　『そろばん対コンピュータ　宿命の対決！！』\n");
	printf("　　　などという番組をやっておったもんぢゃ．」\n");
	printf("孫　「（ホントかな？(笑)）\n");
	printf("　　　ねぇねぇ，おじいちゃんはそろばん使ったことある？」\n");
	printf("老人「ふぉっ ふぉっ ふぉっ(笑)．当然ぢゃよ．\n");
	printf("　　　昔は『読み書きそろばん』といって学校で習ったもんぢゃが\n");
	printf("　　　今は違うのかのぉ……．\n");
	printf("　　　どれ，久しぶりにそろばんを使ってみようか．」\n");
	printf("孫　「えっ，おじいちゃん，そろばん持ってるの？\n");
	printf("　　　すっごいなぁーーーーー！(尊敬)」\n\n");
	hit_any_key();
}

/******* そろばんの解説 *********************************************/
void	kaisetu(void)
{
	disp_init(20);				/* そろばん表示 */
	printf("\x1b[%d;0H\n",UE);
	pr_soro("老人「これが“そろばん”っちゅうもんぢゃ．」\n");
	pr_soro("孫　「あ，これなら見たことがあるよ！\n");
	pr_soro("　　　テレビの歌合戦の司会者の人が\n");
	pr_soro("\n");
	pr_soro("　　　　　『ざんす，ざんす，さいざぁんす』\n");
	pr_soro("\n");
	pr_soro("　　　とか言って かしゃ かしゃ させてたヨ．\n");
	pr_soro("　　　へぇー，あれがそろばんだったんだぁ．\n");
	pr_soro("　　　私，てっきり楽器だと思ってた(笑)．」\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("作者（おいっ，孫っ，お前はいったい何歳なんだ？(汗;)）\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[2A老人「そいつはけしからん奴ぢゃ．そろばんはもっと大事に扱わなきゃいかん．\n");
	pr_soro("　　　それはそうと，そろばんの簡単な使い方を教えてやろう．」\n");
	pr_soro("孫　「わーい，教えて教えてー．」\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("老人「まず“そろばんでの数の表\し方”についてぢゃ．\n");
	pr_soro("　　　まぁ，縦の列が桁を表\していて，\n");
	pr_soro("　　　右から１の位，１０の位……となっておるのは見当がつくぢゃろ．」\n");
	pr_soro("孫　「うん．」\n");
	pr_soro("老人「で，縦の列を見ると，上と下にわかれていて，\n");
	pr_soro("　　　上には１個，下には４個の珠（たま）が入っておる．\n");
	pr_soro("　　　この上の珠が“５”を表\して，下の珠が“１”を表\しておるのぢゃ．\n");
	pr_soro("　　　そして，５の珠は下に下がった時，１の珠は上に上がったときに\n");
	pr_soro("　　　その数字が“入った”事になるんぢゃ．」\n");
	
	pr_soro("孫　「？」\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("老人「つまりぢゃな，そろばんの一番右のけたで説明するとぢゃ，\n");
	pr_soro("　　　今は，５の珠が上になって，１の珠は全部下にあるから\n");
	pr_soro("　　　何も数字が入っていない，という事で，これは“０”なのぢゃ．\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(1,1);
	pr_soro("\x1b[1A　　　次にこういうふうに１の珠を１つづつ上げていくと\n");
	pr_soro("　　　　“１”\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,1);
	pr_soro("\x1b[2A　　　　“２”\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(3,1);
	pr_soro("\x1b[2A　　　　“３”\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(4,1);
	pr_soro("\x1b[2A　　　　“４”\n");
	pr_soro("　　　と，なっていくわけぢゃ．\n");
	pr_soro("　　　そして，次の５は\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(5,1);
	pr_soro("\x1b[1A　　　というふうに１の珠を全部戻して５の珠を下ろすんぢゃ．」\n");
	pr_soro("孫　「うーん，なんとなく解ったような解らないような……」\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(0,1);
	pr_soro("老人「まぁ良い．　すぐ慣れるぢゃろうて．\n");
	pr_soro("　　　この数字の表\現方法が解ると，そろばんでの足し算も簡単に解るんぢゃ．\n");
	pr_soro("　　　例えば ２４６＋２８３ を計算してみようかの．\n");
	pr_soro("　　　まず，そろばんに“２４６”という数字をいれると\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,3);
	disp_soro(4,2);
	disp_soro(6,1);
	pr_soro("\x1b[1A　　　こうなるぢゃろ．\n");
	pr_soro("　　　で，こいつに２８３を足すには，頭から１００の位に２，１０の位に８，\n");
	pr_soro("　　　１の位に３を順番に入れていけばいいのぢゃ．\n");
	pr_soro("　　　まず１００の位に２を入れるには\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(4,3);
	pr_soro("\x1b[1A　　　てな具合に下に残っている１の珠を上に上げるんぢゃ．\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[1A　　　で，次に１０の位ぢゃが，このままでは８は入らん．\n");
	pr_soro("　　　そういう場合は仕方がないので，まず頭の中で４＋８を計算するってぇと\n");
	pr_soro("　　　１２ぢゃろ．\n");
	pr_soro("　　　だから，この１０の位には２を入れて，\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,2);
	pr_soro("\x1b[1A　　　１００の位に１を加えるんぢゃ．\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(5,3);
	pr_soro("\x1b[1A　　　最後の１の位の３は，そのままくわえられる，と．\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(9,1);
	pr_soro("\x1b[1A　　　そういうわけで，このそろばんの上の数字を読むと\n");
	pr_soro("　　　５２９という答えがわかるんぢゃ．」\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("孫　「なんだか，めんどくさいネ．」\n");
	pr_soro("老人「いや，慣れればなんも考えんでも指が勝手に動くようになるもんぢゃ．\n");
	pr_soro("　　　試しに，これで１たす２たす３たす……と，ずうっと足し算してみようか？」\n");
	pr_soro("孫　「うん，やってやってーーー．」\n");
	pr_soro("\n");
	hit_any_key();
}

/******* ネバーエンディング（？）計算 *******************************/
time_t	keisann(void)
{
	int		st[30],in[30];				/* 演算用配列 */
	time_t	st_time,end_time;			/* 実行時間計算用 */
	int		i;							/* 有象無象 */
	
	set_stop_signal();					/* 終了用ストップキー対策 */
	for(i=0; i<25; i++)
		st[i] = in[i] = 0;
	printf("\x1b[2J\x1b[%d;0H\n",UE);
	
	pr_soro("老人「では，始めるぞ．\n");
	pr_soro("　　　ご破算で願いましては……．」\n");
	for(i=4; i>=1; i--){
		disp_soro(0,i);
	}
	pr_soro("孫　「ねぇねぇ，おじいちゃん，\n");
	pr_soro("　　　その『ご破算で願いましては』ってどういう意味なの？」\n");
	pr_soro("老人「ん？これか？　これはそろばんをまっさらな状態にしてください\n");
	pr_soro("　　　という意味じゃ．　まぁ，そろばんで計算を始めるときの\n");
	pr_soro("　　　決まり文句みたいなもんぢゃ．」\n");
	pr_soro("孫　「一休さんがとんちを考えるときの『そもさん』『せっぱ！』\n");
	pr_soro("　　　みたいなもの？」\n");
	pr_soro("老人「ちょっと違うんぢゃが，まぁ，同じ様なもんぢゃ\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[1A　　　では，気を取り直して……\n");
	pr_soro("　　　ご破算で願いましては\n");
	inc_in(in,1);
	disp_in(in);
	pr_soro("円なぁり\n");
	add_soro(st,in[1],1);
	inc_in(in,1);
	disp_in(in);
	add_soro(st,in[1],1);
	pr_soro("円なぁり……\n");
	pr_soro("孫　「ねぇねぇ，おじいちゃん，\n");
	pr_soro("　　　どうして『～円なり』って言うの？」\n");
	pr_soro("老人「ん？　これも，まぁ，そろばんを使う場合の習慣みたいなもんぢゃ\n");
	pr_soro("　　　昔は，そろばんは銀行のようなお金を扱うところでよく使われていたから\n");
	pr_soro("　　　きっと，その名残りぢゃろうて．\n");
	pr_soro("　　　では，つづきを行くぞ．\n");
	pr_soro("\n");
	hit_any_key();
	
	time(&st_time);						/* スタート時刻 */
	for(;;){
		inc_in(in,1);
		if(chk_overflow(st,in,ADD) != OK){		/* overflowしたらおしまい */
			break;
		}
		disp_in(in);
		pr_soro("円なぁり\n");
		for(i=20; i>0; i--)
			add_soro(st,in[i],i);
	}
	time(&end_time);					/* 終了時刻 */
	end_time -= st_time;				/* 実行時刻がend_time に入る */
	pr_soro("\n");	pr_soro("\n");	pr_soro("\n");	pr_soro("\n");
	pr_soro("　　　……っとぉ，このそろばんではけた数が足りんくて\n");
	pr_soro("　　　これ以上は計算できんワイ．\n");
	pr_soro("　　　しかし，お前さんもがんばったのぉ．」\n");
	pr_soro("\n");
	hit_any_key();
	return end_time;
}

/******* エンディング ***********************************************/
void	ending(time_t end_time)
{
	g_cls();
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("　実は，このプログラムは作者もここまで実行した事はありません．\n");
	printf("　で，興味のあるのが，このプログラムの実行時間です．\n");
	printf("　そこで，あなたにお願いがあるのですが，次に示す実行秒数と\n");
	printf("　このプログラムを実行したコンピュータの名前とCPUのクロックを\n");
	printf("　作者まで知らせていただけませんか？\n");
	printf("　よろしくお願いします．\n\n");
	printf("　このプログラムを実行するのに %lu 秒かかりました．\n\n",end_time);
	printf("　ここまでこのプログラムにつき合って下さって，");
	printf("本当にありがとうございました．\n");
}

/******* 何かのキーを押してください *********************************/
void	hit_any_key(void)
{
	printf("\33[47m--hit any key--\33[m");
	getch();
	printf("\r                 \r");
}

/******* スクロールエリア指定文字列表示 *****************************/
void	pr_soro(char *moji)
{
	/* 要するに mojiを表示してからUE行目を１列消去しているのです */
	printf("%s\x1b[s\x1b[%d;0H\x1b[2K\x1b[u",moji,UE);
}

/******* 入力変数＋＋ ***********************************************/
void	inc_in(int *in,int keta)
{
	in[keta]++;
	if(in[keta] >=10){
		in[keta] -= 10;
		inc_in(in,keta+1);
	}
}

/******* 入力変数表示 ***********************************************/
void	disp_in(int *in)
{
	int		i,j = 0;			/* 有象無象 */
	
	printf("　　　");
	for(i=20; i>0; i--){
		if((j == 0) && (in[i] == 0)){
			continue;
		} else {
			j = 1;
			printf("%d",in[i]);
		}
	}
}

/*********************************************************************
 ******* デンタク・モード ********************************************
 ********************************************************************/
void	dentak(void)
{
	int		in[30],st[30];		/* 入力変数，そろばん変数 */
	int		keta;				/* 入力変数の桁数 */
	int		cal_flag;			/* 計算旗 */
	int		ch,i;				/* 有象無象 */
	
	for(i=0; i<25; i++)
		in[i] = st[i] = 0;
	keta = 0;
	cal_flag = ADD;
	
	disp_init(40);
	disp_window();
	printf("\x1b[15;4H　　ご破算でぇ～願いましてはぁ～～～　　");
	set_stop_signal();
	for(;;){
		ch = getch();
		switch(ch){
			case '0':
				if(keta == 0)	break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if(keta == 20)	break;
				if(keta == 0){
					printf("\x1b[15;4H");
					printf("　　　　　　　　　　　　　　　　　　　　");
					printf("\x1b[19;36H　　　　");
				}
				keta++;
				in[keta] = ch - '0';
				disp_res(in,keta);
				break;
			
			case 0x08:
				if(keta == 0)	break;
				keta--;
				disp_res(in,keta);
				break;
			
			case '+':
				if(keta != 0)	break;
				printf("\x1b[15;4H足しては");
				printf("　　　　　　　　　　　　　　　　　　");
				printf("\x1b[19;36H　　　　　");
				cal_flag = ADD;
				disp_res(in,keta);
				break;
			
			case '-':
				if(keta != 0)	break;
				printf("\x1b[15;4H引いては");
				printf("　　　　　　　　　　　　　　　　　　");
				printf("\x1b[19;36H　　　　　");
				cal_flag = SUB;
				disp_res(in,keta);
				break;
			
			case 0x0d:
			case ' ':
				if(keta == 0)	break;
				seikika(in,keta);
				if(chk_overflow(st,in,cal_flag) != OK){
					printf("\x1b[15;4H　　　　");
					printf("\x1b[17;4H　　　");
					printf("桁あふれが起こりました．　　　　　\a");
					keta = 0;
					cal_flag = ADD;
					break;
				}
				if(ch == 0x0d){
					printf("\x1b[19;38H円なり");
				} else {
					printf("\x1b[19;38H円では");
				}
				printf("\x1b[15;4H　　　　");
				if(cal_flag == ADD){
					for(i=keta; i>0; i--)
						add_soro(st,in[i],i);
				} else {
					for(i=keta; i>0; i--)
						sub_soro(st,in[i],i);
				}
				cal_flag = ADD;
				keta = 0;
				break;
			
			case 0x1b:
				printf("\x1b[15;4H　　ご破算でぇ～願いましてはぁ～～～　　");
				printf("\x1b[19;38H　　　");
				keta = 0;
				disp_res(in,keta);
				for(i=20; i>0; i--){
					st[i] = in[i] = 0;
					disp_soro(0,i);
				}
				break;
			
			default:
				break;
		}
	}
	/*NOTREACHED*/
}

/******* デンタク・ウインドウ *************************************/
void	disp_window(void)
{
	int		i,j;		/* 有象無象 */
	
	/* input window */
	g_line(  2,204,368,325,7,1);
	g_line(  2,325,  4,327,7,0);
	g_line(368,325,370,327,7,0);
	g_line(368,204,370,206,7,0);
	g_line(370,206,370,327,7,0);
	g_line(370,327,  4,327,7,0);
	g_paint2(10,213,7,pat,6);
	g_line( 10,212,360,317,0,2);
	g_line( 10,212,360,317,7,1);
	g_line( 10,212, 12,214,7,0);
	g_line( 12,214,360,214,7,0);
	g_line( 12,214, 12,317,7,0);
	
	g_line( 22,274,341,274,7,0);
	for(i=0; i<=20; i++){
		j = 22 + (i * 16);
		g_line(j,272,j,274,7,0);
	}
	
	/* kaisetu */
	g_line(385,190,637,397,7,1);
	g_line(637,190,639,192,7,0);
	g_line(637,397,639,399,7,0);
	g_line(385,397,387,399,7,0);
	g_line(639,192,639,399,7,0);
	g_line(639,399,387,399,7,0);
	
	g_paint2(395,192,7,pat,6);
	g_line(393,198,629,391,0,2);
	g_line(393,198,629,391,7,1);
	g_line(393,198,395,200,7,0);
	g_line(395,200,395,391,7,0);
	g_line(395,200,629,200,7,0);
	
	printf("\x1b[14;51H【キー操作の説明】");
	printf("\x1b[16;52H［０］～［９］： 数字入力");
	printf("\x1b[18;52H ［ Ｂ  Ｓ ］ ： １字後退");
	printf("\x1b[20;52H ［リターン］ ： ～円なり");
	printf("\x1b[22;52H ［Ｅ Ｓ Ｃ］ ： ご 破 算");
	printf("\x1b[24;52H ［ＳＴＯＰ］ ：  終　了");
}


/******* 入力変数表示 ***********************************************/
void	disp_res(int *in,int keta)
{
	int	i;			/* 有象無象 */
	
	for(i=20; i>=keta; i--)
		printf("\x1b[17;%dH　",(42 - (i * 2)));
	
	for(i=keta; i>0; i--){
		printf("\x1b[17;%dH",(42 - ((keta - i) * 2)));
		putch(0x82);
		putch(0x4f + in[i]);
	}
}

/******* 入力数値の正規化 *******************************************/
void	seikika(int *in,int keta)
{
	int		i,j;		/* 有象無象 */
	
	for(i=keta+1; i<=20; i++)
		in[i] = 0;
	
	if(keta == 0)
		return;
	
	for(i=1; i<(int)((keta/2)+1); i++){
		j = in[i];
		in[i] = in[keta - i + 1];
		in[keta - i + 1] = j;
	}
}

/******* こっから下の関数はおじいさんも使うので注意する *************/

/******* 数値のそろばん表示 *****************************************/
void	disp_soro(int suuji,int p)
{
	int		i;		/* 有象無象 */
	
	if(p > 20){
		err_nandaka("数値のそろばん表\示で桁指定が変です");
		/*NOTREACHED*/
	}
	if(suuji >= 5){
		g_put(x[p],y_u[0],boh,leng,0);
		g_put(x[p],y_u[1],tama,leng,0);
		suuji -= 5;
	} else {
		g_put(x[p],y_u[0],tama,leng,0);
		g_put(x[p],y_u[1],boh,leng,0);
	}
	for(i=0; i<5; i++){
		if(i == suuji)
			g_put(x[p],y_l[i],boh,leng,0);
		else
			g_put(x[p],y_l[i],tama,leng,0);
	}
}

/******* 桁あふれチェック *******************************************/
int	chk_overflow(int *st,int *in,int cal_flag)
{
	int		i;							/* 有象無象 */
	static int		stl[30];			/* 局所用配列 */
	
	for(i=0; i<=20; i++)
		stl[i] = st[i];
	stl[21] = 0;
	
	if(cal_flag == ADD){
		for(i=1; i<=20; i++){
			stl[i] += in[i];
			if(stl[i] >= 10){
				stl[i+1] += 1;
			}
		}
		if(stl[21] != 0){
			return NO;
		} else {
			return OK;
		}
	}
	if(cal_flag == SUB){
		for(i=1; i<=20; i++){
			stl[i] -= in[i];
			if(stl[i] < 0){
				stl[i+1] -= 1;
			}
		}
		if(stl[21] != 0){
			return NO;
		} else {
			return OK;
		}
	}
	err_nandaka("cal_flag が変です");
	/*NOTREACHED*/
}

/******* 指定桁の足し算 *********************************************/
void	add_soro(int *st,int in,int p)
{
	if(p > 20){
		err_nandaka("足算で桁あふれです");
		/*NOTREACHED*/
	}
	st[p] += in;
	if(st[p] >= 10){
		st[p] -= 10;
		disp_soro(st[p],p);
		add_soro(st,1,p+1);
	} else {
		disp_soro(st[p],p);
	}
	return;
}

/******* 指定桁の引き算 *********************************************/
void	sub_soro(int *st,int in,int p)
{
	if(p > 20){
		err_nandaka("引き算で桁あふれです");
		/*NOTREACHED*/
	}
	st[p] -= in;
	if(st[p] < 0){
		st[p] += 10;
		disp_soro(st[p],p);
		sub_soro(st,1,p+1);
	} else {
		disp_soro(st[p],p);
	}
	return;
}

/*********************************************************************
 ******* マウス・モード **********************************************
 ********************************************************************/
void	mouse(void)
{
	int		mo_x,mo_y,mo_l,mo_r;		/* マウス位置およびボタン状況 */
	int		i;							/* 有象無象 */
	
	if(mo_check() == NO){
		printf("\n孫　「マウスモードは");
		printf("マウスドライバを組み込んでから実行してね．」\n");
		exit(0);
	}
	
	disp_init(100);
	mo_disp();
	mo_l = mo_r = 0;
	set_stop_signal();
	for(;;){
		printf("\r");		/* 情けないけど,これが stopキー対策(汗;) */
		mo_ichi(&mo_x,&mo_y,&mo_l,&mo_r);
		if((mo_l == 0) && (mo_r == 0))		continue;
		if((mo_y < 110) || (mo_y > 212))	continue;
		mo_x = 20 - (int)((mo_x-20) / 30);
		if((mo_x < 1) || (mo_x > 20))	continue;
		if(mo_l == 1){
			if(mo_y < 138){
				mo_erase();
				mo_y = (int)((mo_y - 110) / 14);
				for(i=0; i<2; i++){
					if(i == mo_y)
						g_put(x[mo_x],y_u[i],boh,leng,0);
					else
						g_put(x[mo_x],y_u[i],tama,leng,0);
				}
				mo_disp();
			}
			if(mo_y > 143){
				mo_erase();
				mo_y = (int)((mo_y - 143) / 14);
				for(i=0; i<5; i++){
					if(i == mo_y)
						g_put(x[mo_x],y_l[i],boh,leng,0);
					else
						g_put(x[mo_x],y_l[i],tama,leng,0);
				}
				mo_disp();
			}
			continue;
		}
		if(mo_r == 1){
			mo_erase();
			g_put(x[mo_x],y_u[0],tama,leng,0);
			g_put(x[mo_x],y_u[1],boh,leng,0);
			g_put(x[mo_x],y_l[0],boh,leng,0);
			for(i=1; i<5; i++)
				g_put(x[mo_x],y_l[i],tama,leng,0);
			mo_disp();
			continue;
		}
	}
	/*NOTREACHED*/
}


/*********************************************************************
 ******* 汎用関数群 **************************************************
 ********************************************************************/

/******* ストップキーのシグナルセット *******************************/
void	set_stop_signal(void)
{
	if(signal(SIGINT,quit) == (void(*)()) -1)
		err_signal();
		/*NOTREACHED*/
}

/******* 最初のそろばんの表示 ***************************************/
void	disp_init(int up)
{
	int	i,j;			/* 有象無象 */
	
	/* 最初の準備 */
	g_init();
	g_screen(0,0);
	g_cls();
	printf("\x1b[2J\x1b[>5h\x1b[>1h");	/* 画面をきれいにして */
	get_tama();							/* 珠のイメージ取得 */
	calc_tamaichi(up);					/* 珠位置計算 */
	
	/* そろばんの枠 */
	g_line(10,up- 4,630,up+122,7,1);
	g_paint2(20,up+10,7,pat,6);
	g_line(20,up+10,620,up+ 37,0,2);
	g_line(20,up+43,620,up+112,0,2);
	g_line(19,up+ 9,621,up+ 38,7,1);
	g_line(19,up+42,621,up+113,7,1);
	g_line( 10,up+122, 10+6,up+122+6,7,0);
	g_line(630,up+122,630+6,up+122+6,7,0);
	g_line(630,up-  4,630+6,up-  4+6,7,0);
	for(i=2; i<=6; i+=2){
		g_line(630+i,up-  4+i,630+i,up+122+i,7,0);
		g_line( 10+i,up+122+i,630+i,up+122+i,7,0);
	}
	
	/* 梁 */
	g_line(19,up+39,621,up+ 41,7,2);
	for(i=1; i<=20; i++)
		g_pset(x[i]+15,up+40,0);
	
	/* 珠と棒 */
	for(i=1; i<=20; i++){
		g_put(x[i],y_u[0],tama,leng,0);
		g_put(x[i],y_u[1],boh,leng,0);
		g_put(x[i],y_l[0],boh,leng,0);
		for(j=1; j<5; j++)
			g_put(x[i],y_l[j],tama,leng,0);
	}
}

/******* そろばんの珠イメージ取得 ***********************************/
void	get_tama(void)
{
	int		i,j;		/* 有象無象 */
	
	/* イメージの大きさ */
	leng = (int)((30+8)/8)*(14+1)*3 + 4;
	
	/* イメージ領域の確保*/
	tama = (char *)malloc(leng);
	if(tama == NULL)
		err_no_memory();
	boh = (char *)malloc(leng);
	if(boh == NULL)
		err_no_memory();
	
	/* 珠イメージ */
	for(i=0; i<7; i++){
		g_line(14-i*2,   i,15+i*2,   i,7,0);
		g_line(14-i*2,13-i,15+i*2,13-i,7,0);
		for(j=14-i; j<=15+i*2; j+=2){
			g_pset(j,   i,0);
			g_pset(j,13-i,0);
		}
		for(j=i; j<=i*2; j++){
			g_pset(14-j,   i,7);
			g_pset(15+j,13-i,0);
		}
		g_pset(15+i*2,13-i,7);
	}
	g_line(8,7,27,7,0,0);
	
	g_get(0,0,29,13,tama,leng);
	g_line(0,0,30,13,0,2);
	
	/* 棒イメージ */
	g_line(14,0,16,13,7,2);
	for(i=0; i<=13; i+=2)
		g_pset(16,i,0);
	g_get(0,0,29,13,boh,leng);
	g_line(0,0,30,13,0,2);
}

/******* 珠描画位置計算 *********************************************/
void	calc_tamaichi(int up)
{
	int		i;		/* 有象無象 */
	
	for(i=0; i<20; i++)
		x[20 - i] = 20 + 30 * i;
	for(i=0; i<2; i++)
		y_u[i] = up + 10 + (14 * i);
	for(i=0; i<5; i++)
		y_l[i] = up + 43 + (14 * i);
}

/******* 終了処理 ***************************************************/
void	quit(void)
{
	signal(SIGINT,SIG_IGN);
	if(mo_check() == OK)
		mo_erase();
	g_cls();
	printf("\x1b[>5l\x1b[>1l\x1b[2J");
	printf("\n\n作者「\"soro.exe\" を使ってくれてありがとう．」\n\n");
	exit(0);
	/*NOTREACHED*/
}

/*********************************************************************
 ******* 各種エラー処理 **********************************************
 ********************************************************************/

/******* オプションミス（使い方）************************************/
void	usage(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("老人「使い方は soro [-d|-m] ぢゃ．」\n");
	exit(1);
	/*NOTREACHED*/
}

/******* メモリが足りない *******************************************/
void	err_no_memory(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("老人「すまんが，メモリが足りのうて実行できんわ．」\n");
	exit(1);
	/*NOTREACHED*/
}

/******* シグナルがセットできない ***********************************/
void	err_signal(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("孫　「どうしてsignal関数でエラーが起こったの？」\n");
	exit(1);
	/*NOTREACHED*/
}

/******* なんだかわからない *****************************************/
void	err_nandaka(char *mes)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("作者「%s．」\a\n",mes);
	exit(1);
	/*NOTREACHED*/
}
