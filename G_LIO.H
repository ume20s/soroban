/*
 * Ｇ＿ＬＩＯを用いたグラフィックライブラリの
 * プロトタイプ宣言
 */
void	g_init(void);
void	g_screen(int act,int dsp);
void	g_cls(void);
void	g_palete(int pal,int col);
void	g_pset(int x,int y,int col);
void	g_line(int x0,int y0,int x1,int y1,int col,int func);
void	g_circle(int x,int y,int r,int c);
void	g_paint(int x,int y,int c1,int c2);
void	g_paint2(int x,int y,int col,char *dat,int len);
void	g_get(int x0,int y0,int x1,int y1,char *dat,int leng);
void	g_put(int x,int y,char *dat,int leng,int mode);
