/*
	グラフィックライブラリ（Ｇ＿ＬＩＯバージョン）
*/
#include <dos.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

static char rcs[] = "$Header: G_LIO.Cv  1.2  91/02/16 15:02:26  UME20  Exp $";

union REGS		inregs,outregs;
struct SREGS	segregs;
unsigned int	DSEG,GSEG;
unsigned int	IRET = 0xcfcf;

/*********************************************************************
 ******* 関数プロトタイプ ********************************************
 ********************************************************************/
void	setb(int adr,int dat);
void	setw(int adr,int dat);
void	lio(int intno);

/*********************************************************************
 * 初期化（一度は実行しなければならない）
 */
void	g_init(void)
{
	int	i;
	unsigned WORK;
	char	*gwork;
	if((gwork = malloc(0x1400)) == 0){
		fputs("g_lio : メモリが足りません\n",stderr);
	}
	segread(&segregs);
	DSEG = segregs.ds;
	GSEG = DSEG + (int)gwork/16;
	WORK = 0xf990;
	for(i=0; i<16; i++){
		movedata(0xf990,6+i*4,0x0000,0xa0*4+i*4,2);
		movedata(DSEG,(int)&WORK,0x0000,0xa0*4+i*4+2,2);
	}
	movedata(0xf990,6+16*4,0x0000,0xce*4,2);
	WORK = (unsigned)&IRET;
	movedata(DSEG,(int)&WORK,0x0000,0xc5*4,2);
	movedata(DSEG,(int)&DSEG,0x0000,0xc5*4+2,2);
	lio(0xa0);
}

/*********************************************************************
 * 書き込みページと表示ページの設定
 *         act : 書き込みページ（0 or 1 (page)）
 *         dsp : 表示ページ　　（0 or 1 (page)）
 */
void	g_screen(int act,int dsp)
{
	if(dsp == 0)
		dsp = 1;	/* disp 0 page code */
	else
		dsp = 17;	/* disp 1 page code */
	
	setb(0,3);
	setb(1,0);
	setb(2,act);
	setb(3,dsp);
	lio(0xa1);
}

/*********************************************************************
 * 画面消去
 */
void	g_cls()
{
	lio(0xa5);
}

/*********************************************************************
 * パレット変更
 *         pal : パレット番号
 *         col : カラーコード（0=brack .. 7=white)
 */
void	g_palete(int pal,int col)
{
	setb(0,pal);
	setb(1,col);
	lio(0xa4);
}

/*********************************************************************
 * １ドット描画
 *         x,y : 座標
 *         col : 色（0 <= col <= 7）
 */
void	g_pset(int x,int y,int col)
{
	setw(0,x);
	setw(2,y);
	setb(4,col);
	lio(0xa6);
}

/*********************************************************************
 * 直線描画
 *         x0,y0,x1,y1 : 座標（(x0,y0)-(x1,y1)）
 *         col         : 描画色（パレットコード 0..7）
 *         func        : 動作（0=line,1=box,2=boxfill)
 */
void	g_line(int x0,int y0,int x1,int y1,int col,int func)
{
	setw(0,x0);		setw(2,y0);
	setw(4,x1);		setw(6,y1);
	setb(8,col);
	setw(9,func);
	setb(13,0xff);
	setw(16,DSEG);
	lio(0xa7);
}

/*********************************************************************
 * 円描画
 *         x,y : 座標
 *         r   : 半径
 *         c   : 描画色
 */
void	g_circle(int x,int y,int r,int c)
{
	setw(0,x);
	setw(2,y);
	setw(4,r);
	setw(6,r);
	setb(8,c);
	setb(9,0);
	setw(21,DSEG);
	lio(0xa8);
}

/*********************************************************************
 * 塗りつぶし
 *         x,y : 座標
 *         c1  : 塗りつぶし色
 *         c2  : 境界色
 */
void	g_paint(int x,int y,int c1,int c2)
{
	setw(0,x);
	setw(2,y);
	setb(4,c1);
	setb(5,c2);
	setw(6,0x0a00);
	setw(8,0x06a0);
	lio(0xa9);
}

/*********************************************************************
 * タイリングによる塗りつぶし
 *         x,y : 座標
 *         col : 境界色
 *         dat : パターンデータ
 *         len : データ長さ
 */
void	g_paint2(int x,int y,int col,char *dat,int len)
{
	setw(0,x);
	setw(2,y);
	setb(5,len);
	setw(6,(int)dat);
	setw(8,DSEG);
	setb(10,col);
	setw(16,0x0a00);
	setw(18,0x06a0);
	lio(0xaa);
}

/*********************************************************************
 * グラフィックイメージ取得
 *         x0,y0,x1,y1 : 座標
 *         *dat        : 格納先ポインタ
 *         leng        : イメージの大きさ
 */
void	g_get(int x0,int y0,int x1,int y1,char *dat,int leng)
{
	setw(0,x0);		setw(2,y0);
	setw(4,x1);		setw(6,y1);
	setw(8,(int)dat);
	setw(10,DSEG);
	setw(12,leng);
	lio(0xab);
}

/*********************************************************************
 * グラフィックイメージ表示
 *         x,y  : 座標
 *         dat  : 描画イメージ格納ポインタ
 *         leng : イメージの大きさ
 *         mode : 描画モード（0=PUT,1=REV,2=OR,3=AND,4=XOR)
 */
void	g_put(int x,int y,char *dat,int leng,int mode)
{
	setw(0,x);		setw(2,y);
	setw(4,(int)dat);
	setw(6,DSEG);
	setw(8,leng);
	setb(10,mode);
	setb(11,0);
	lio(0xac);
}

/*********************************************************************
 ******* 小物 ********************************************************
 ********************************************************************/
void	setb(int adr,int dat)
{
	movedata(DSEG,(int)&dat,GSEG,adr,1);
}

void	setw(int adr,int dat)
{
	movedata(DSEG,(int)&dat,GSEG,adr,2);
}

void	lio(int intno)
{
	inregs.x.bx = 0;
	segregs.ds = GSEG;
	int86x(intno,&inregs,&outregs,&segregs);
}
