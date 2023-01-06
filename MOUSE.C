/*
	�}�E�X���C�u����
*/

#include	<dos.h>

#define		NEC		0
#define		MS		1

static char rcs[] = "$Header: mouse.cv  1.1  91/02/06 22:39:44  UME20  Exp $";

int		type;		/* �}�E�X�^�C�v */

/*********************************************************************
 * �}�E�X�h���C�o�̑��݊m�F
 *         �Ԓl : 0=OK  1=NO
 */
int	mo_check(void)
{
	union	REGS	regs;
	
	regs.x.ax = regs.x.bx = 0;
	int86(0x33,&regs,&regs);
	type = ((regs.x.bx == 2) ? MS : NEC);
	return((regs.x.ax == -1u) ? 0 : 1);
}

/*********************************************************************
 * �}�E�X�J�[�\���̕\��
 */
void	mo_disp(void)
{
	union	REGS	regs;
	
	regs.x.ax = 0x01;
	int86(0x33,&regs,&regs);
}

/*********************************************************************
 * �}�E�X�J�[�\������
 */
void	mo_erase(void)
{
	union	REGS	regs;
	
	regs.x.ax = 0x02;
	int86(0x33,&regs,&regs);
}

/*********************************************************************
 * �}�E�X�̈ʒu����у{�^��������Ԃ̎擾
 *         x,y        : ���W
 *         left,right : �{�^���i0=off 1=on�j
 */
void	mo_ichi(int *x,int *y,int *left,int *right)
{
	union	REGS	regs;
	
	*left = *right = 0;
	regs.x.ax = 0x03;
	regs.x.bx = regs.x.cx = regs.x.dx = 0;
	int86(0x33,&regs,&regs);
	*x = regs.x.cx;
	*y = regs.x.dx;
	if(((type == NEC)&&(regs.x.ax == -1u))||((type == MS)&&(regs.x.bx == 1)))
		*left = 1;
	if(((type == NEC)&&(regs.x.bx == -1u))||((type == MS)&&(regs.x.bx == 2)))
		*right = 1;
}