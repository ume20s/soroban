/*
 * ����΂� [ soro.c ]
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

#define		ADD		0			/* �v�Z��(�׸�)�p */
#define		SUB		1
#define		OK		0			/* ���낢��Ǝg���Ǝv�� */
#define		NO		1

#define		UE		10			/* ���b�G���A�̏�� */

/*********************************************************************
 ******* �֐��v���g�^�C�v ********************************************
 ********************************************************************/

/******* main *******************************************************/
void	main(int argc,char *argv[]);	/* ���C�� */
void	ojiisan(void);					/* �f���E���[�h */
void	dentak(void);					/* �f���^�N�E���[�h */
void	mouse(void);					/* �}�E�X�E���[�h */

/******* ojiisan ****************************************************/
void	opening(void);					/* �I�[�v�j���O */
void	kaisetu(void);					/* �ŏ��̉�� */
time_t	keisann(void);					/* �l�o�[�G���f�B���O(�H)�v�Z */
void	ending(time_t end_time);		/* �G���f�B���O */
void	hit_any_key(void);				/* �����̃L�[�������Ă� */
void	pr_soro(char *moji);			/* �X�N���[���G���A��������\�� */
void	inc_in(int *in,int keta);		/* ���͕ϐ��{�{ */
void	disp_in(int *in);				/* ���͕ϐ��\�� */

/******* den_tak mode ***********************************************/
void	disp_window(void);				/* �f���^�N�E�E�C���h�E */
void	disp_res(int *in,int keta);		/* ���͕ϐ��\�� */
void	seikika(int *in,int keta);		/* ���͐��l�̐��K�� */

/******* ojiisan & den-tak ******************************************/
void	disp_soro(int suuji,int p);		/* ���l�̂���΂�\�� */
int		chk_overflow(int *st,int *in,int cal_flag);	/* �����ӂ�`�F�b�N */
void	add_soro(int *st,int in,int p);	/* �w�茅�̑����Z */
void	sub_soro(int *st,int in,int p);	/* �w�茅�̈����Z */

/******* general ****************************************************/
void	set_stop_signal(void);			/* �V�O�i���̃Z�b�g */
void	disp_init(int up);				/* ����΂�̗p�� */
void	get_tama(void);					/* ����΂�̎�擾 */
void	calc_tamaichi(int up);			/* ��`��ʒu�v�Z */
void	quit(void);						/* �I������ */

void	usage(void);					/* �g���� */
void	err_no_memory(void);			/* ����������� */
void	err_signal(void);				/* �V�O�i���������� */
void	err_nandaka(char *mes);			/* �Ȃ񂾂��킩��Ȃ� */

/*********************************************************************
 ******* �L��ϐ��i����΂�֌W�͖ʓ|������S���L��ϐ�(��;) *********
 ********************************************************************/
static char	*tama;					/* ��graphic */
static char	*boh;					/* �_graphic */
static int	leng;					/* graphic�̑傫�� */
static int	x[30];					/* ��`��ʒu�i�w���W�j*/
static int	y_u[2];					/* ��`��ʒu�i���j*/
static int	y_l[5];					/* ��`��ʒu�i����j*/
static char pat[] = {0x55,0x55,0x55,0xaa,0xaa,0xaa};
									/* �y�C���g�p�p�^�[�� */

/*********************************************************************
 ******* ���C�� ******************************************************
 ********************************************************************/
void	main(int argc,char *argv[])
{
	/* �������Ȃ���΃f�t�H���g���� */
	if(argc == 1){
		ojiisan();
		/*NOTREACHED*/
	} else { 
	/* ����������� */
		++argv;
		if(**argv == '-'){
			switch(*(*argv + 1)) {
				case 'd':				/* �f���^�N�E���[�h */
					dentak();
					/*NOTREACHED*/
				case 'm':				/* �}�E�X�E���[�h */
					mouse();
					/*NOTREACHED*/
				default:				/* ������ d,m�ȊO */
					usage();
					/*NOTREACHED*/
			}
		} else {
			usage();					/* �����͂ӂ[ '-'(��) */
			/*NOTREACHED*/
		}
	}
}

/*********************************************************************
 ******* �f�����X�g���[�V�����E���[�h�i�f�t�H���g����j **************
 ********************************************************************/
void	ojiisan(void)
{
	time_t	end_time;		/* ���s���� */
	
	printf("\x1b[2J\x1b[>5h\x1b[>1h");
	opening();							/* ����΂�\���O�̉�b */
	kaisetu();							/* ����΂�𓮂������ */
	end_time = keisann();				/* �v�Z���Ď��s���Ԃ�Ԃ� */
	ending(end_time);					/* ����Ǝ��s���Ԃ̕\�� */
	exit(0);
	/*NOTREACHED*/
}

/******* �I�[�v�j���O ***********************************************/
void	opening(void)
{
	printf("���@�u�˂��˂��C�����������D�v\n");
	printf("�V�l�u�Ȃ����H�v\n\n");
	hit_any_key();
	printf("���@�u�g����΂�h���Ăȁ[�ɁH�v\n");
	printf("�V�l�u�����C���܂�肳�񂪂������鏊�����v\n");
	printf("���@�u����́g��ԁh�v\n");
	printf("�V�l�u��H�@�Ȃ�C�F���Y���̂��Ƃ���D�v\n");
	printf("���@�u����́g�M���o���h�I�v\n");
	printf("�V�l�u���܂����w�Z�֍s���Ƃ��Ɏ����čs���܂̎����́H�v\n");
	printf("���@�u����́g���΂�h�I�v\n");
	printf("�V�l�u���K����Ȃ��c�c�v\n");
	printf("���@�u����́g�{�ԁh�I�v\n");
	printf("�V�l�u���X�̖��O���������c�c�D�v\n");
	printf("���@�u�g�Ŕh�I�v\n");
	printf("�V�l�u�|���c�c�v\n");
	printf("���@�u�g���ԁh�I�I�v\n");
	printf("�V�l�u��̂������v\n");
	printf("���@�u�g����΂�́h�I�I�I�v \n");
	printf("�V�l�u�c�c�v\n");
	printf("���@�u�c�c�v\n\n");
	hit_any_key();
	printf("�V�l�C���u�ǂ��� ����[���₵���[�I�I�I�I�v\n\n");
	hit_any_key();
	printf("���@�i�����������C����̃_�W�����͂�����Ƌꂵ����j\n");
	printf("�V�l�i���������킢��(��)�j\n\n");
	hit_any_key();
	printf("\x1b[3A���@�u���[����Ȃ����ā[�I�v�@�@�@�@�@�@�@�@�@�@�@�@�@\n");
	printf("�V�l�u�킩���Ƃ��C�g����΂�h����낤�H\n");
	printf("�@�@�@����΂���Ă̂͂��Ȃ��C�����ł́w�Z�Ձx�Ə����ĂȁC\n");
	printf("�@�@�@�܂��C���̌v�Z��̎�����ȁD�v\n");
	printf("���@�u�ց[�C�v�Z�킡�c�c�D�v\n");
	printf("�V�l�u��������C�����Ă����ɏ�B����ƁC\n");
	printf("�@�@�@�Ȃ�ƃR���s���[�^���������������o��������D�v\n");
	printf("���@�u�����[�[�[�[���I�H�@�R���s���[�^�����������H�H�H�v\n\n");
	hit_any_key();
	printf("�V�l�u�����C�̂͂悭�e���r��\n");
	printf("�@�@�@�@�@�@�w����΂�΃R���s���[�^�@�h���̑Ό��I�I�x\n");
	printf("�@�@�@�ȂǂƂ����ԑg������Ă������������D�v\n");
	printf("���@�u�i�z���g���ȁH(��)�j\n");
	printf("�@�@�@�˂��˂��C�����������͂���΂�g�������Ƃ���H�v\n");
	printf("�V�l�u�ӂ��� �ӂ��� �ӂ���(��)�D���R�����D\n");
	printf("�@�@�@�̂́w�ǂݏ�������΂�x�Ƃ����Ċw�Z�ŏK����������Ⴊ\n");
	printf("�@�@�@���͈Ⴄ�̂��̂��c�c�D\n");
	printf("�@�@�@�ǂ�C�v���Ԃ�ɂ���΂���g���Ă݂悤���D�v\n");
	printf("���@�u�����C�����������C����΂񎝂��Ă�́H\n");
	printf("�@�@�@���������Ȃ��[�[�[�[�[�I(���h)�v\n\n");
	hit_any_key();
}

/******* ����΂�̉�� *********************************************/
void	kaisetu(void)
{
	disp_init(20);				/* ����΂�\�� */
	printf("\x1b[%d;0H\n",UE);
	pr_soro("�V�l�u���ꂪ�g����΂�h�����イ�������D�v\n");
	pr_soro("���@�u���C����Ȃ猩�����Ƃ������I\n");
	pr_soro("�@�@�@�e���r�̉̍���̎i��҂̐l��\n");
	pr_soro("\n");
	pr_soro("�@�@�@�@�@�w���񂷁C���񂷁C���������񂷁x\n");
	pr_soro("\n");
	pr_soro("�@�@�@�Ƃ������� ������ ������ �����Ă����D\n");
	pr_soro("�@�@�@�ւ��[�C���ꂪ����΂񂾂����񂾂��D\n");
	pr_soro("�@�@�@���C�Ă�����y�킾�Ǝv���Ă�(��)�D�v\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("��ҁi�������C�����C���O�͂����������΂Ȃ񂾁H(��;)�j\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[2A�V�l�u�����͂��������z����D����΂�͂����Ƒ厖�Ɉ���Ȃ��Ⴂ����D\n");
	pr_soro("�@�@�@����͂����ƁC����΂�̊ȒP�Ȏg�����������Ă�낤�D�v\n");
	pr_soro("���@�u��[���C�����ċ����ā[�D�v\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("�V�l�u�܂��g����΂�ł̐��̕\\�����h�ɂ��Ă���D\n");
	pr_soro("�@�@�@�܂��C�c�̗񂪌���\\���Ă��āC\n");
	pr_soro("�@�@�@�E����P�̈ʁC�P�O�̈ʁc�c�ƂȂ��Ă���̂͌������������D�v\n");
	pr_soro("���@�u����D�v\n");
	pr_soro("�V�l�u�ŁC�c�̗������ƁC��Ɖ��ɂ킩��Ă��āC\n");
	pr_soro("�@�@�@��ɂ͂P�C���ɂ͂S�̎�i���܁j�������Ă���D\n");
	pr_soro("�@�@�@���̏�̎삪�g�T�h��\\���āC���̎삪�g�P�h��\\���Ă���̂���D\n");
	pr_soro("�@�@�@�����āC�T�̎�͉��ɉ����������C�P�̎�͏�ɏオ�����Ƃ���\n");
	pr_soro("�@�@�@���̐������g�������h���ɂȂ�����D�v\n");
	
	pr_soro("���@�u�H�v\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("�V�l�u�܂����ȁC����΂�̈�ԉE�̂����Ő�������Ƃ���C\n");
	pr_soro("�@�@�@���́C�T�̎삪��ɂȂ��āC�P�̎�͑S�����ɂ��邩��\n");
	pr_soro("�@�@�@���������������Ă��Ȃ��C�Ƃ������ŁC����́g�O�h�Ȃ̂���D\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(1,1);
	pr_soro("\x1b[1A�@�@�@���ɂ��������ӂ��ɂP�̎���P�Âグ�Ă�����\n");
	pr_soro("�@�@�@�@�g�P�h\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,1);
	pr_soro("\x1b[2A�@�@�@�@�g�Q�h\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(3,1);
	pr_soro("\x1b[2A�@�@�@�@�g�R�h\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(4,1);
	pr_soro("\x1b[2A�@�@�@�@�g�S�h\n");
	pr_soro("�@�@�@�ƁC�Ȃ��Ă����킯����D\n");
	pr_soro("�@�@�@�����āC���̂T��\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(5,1);
	pr_soro("\x1b[1A�@�@�@�Ƃ����ӂ��ɂP�̎��S���߂��ĂT�̎�����낷�����D�v\n");
	pr_soro("���@�u���[��C�Ȃ�ƂȂ��������悤�ȉ���Ȃ��悤�ȁc�c�v\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(0,1);
	pr_soro("�V�l�u�܂��ǂ��D�@������������낤�āD\n");
	pr_soro("�@�@�@���̐����̕\\�����@������ƁC����΂�ł̑����Z���ȒP�ɉ�������D\n");
	pr_soro("�@�@�@�Ⴆ�� �Q�S�U�{�Q�W�R ���v�Z���Ă݂悤���́D\n");
	pr_soro("�@�@�@�܂��C����΂�Ɂg�Q�S�U�h�Ƃ���������������\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,3);
	disp_soro(4,2);
	disp_soro(6,1);
	pr_soro("\x1b[1A�@�@�@�����Ȃ�����D\n");
	pr_soro("�@�@�@�ŁC�����ɂQ�W�R�𑫂��ɂ́C������P�O�O�̈ʂɂQ�C�P�O�̈ʂɂW�C\n");
	pr_soro("�@�@�@�P�̈ʂɂR�����Ԃɓ���Ă����΂����̂���D\n");
	pr_soro("�@�@�@�܂��P�O�O�̈ʂɂQ������ɂ�\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(4,3);
	pr_soro("\x1b[1A�@�@�@�Ăȋ�ɉ��Ɏc���Ă���P�̎����ɏグ������D\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[1A�@�@�@�ŁC���ɂP�O�̈ʂ��Ⴊ�C���̂܂܂ł͂W�͓����D\n");
	pr_soro("�@�@�@���������ꍇ�͎d�����Ȃ��̂ŁC�܂����̒��łS�{�W���v�Z������Ă���\n");
	pr_soro("�@�@�@�P�Q�����D\n");
	pr_soro("�@�@�@������C���̂P�O�̈ʂɂ͂Q�����āC\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(2,2);
	pr_soro("\x1b[1A�@�@�@�P�O�O�̈ʂɂP������������D\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(5,3);
	pr_soro("\x1b[1A�@�@�@�Ō�̂P�̈ʂ̂R�́C���̂܂܂��킦����C�ƁD\n");
	pr_soro("\n");
	hit_any_key();
	disp_soro(9,1);
	pr_soro("\x1b[1A�@�@�@���������킯�ŁC���̂���΂�̏�̐�����ǂނ�\n");
	pr_soro("�@�@�@�T�Q�X�Ƃ����������킩������D�v\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("���@�u�Ȃ񂾂��C�߂�ǂ������l�D�v\n");
	pr_soro("�V�l�u����C�����΂Ȃ���l����ł��w������ɓ����悤�ɂȂ�������D\n");
	pr_soro("�@�@�@�����ɁC����łP�����Q�����R�����c�c�ƁC�������Ƒ����Z���Ă݂悤���H�v\n");
	pr_soro("���@�u����C����Ă���ā[�[�[�D�v\n");
	pr_soro("\n");
	hit_any_key();
}

/******* �l�o�[�G���f�B���O�i�H�j�v�Z *******************************/
time_t	keisann(void)
{
	int		st[30],in[30];				/* ���Z�p�z�� */
	time_t	st_time,end_time;			/* ���s���Ԍv�Z�p */
	int		i;							/* �L�ۖ��� */
	
	set_stop_signal();					/* �I���p�X�g�b�v�L�[�΍� */
	for(i=0; i<25; i++)
		st[i] = in[i] = 0;
	printf("\x1b[2J\x1b[%d;0H\n",UE);
	
	pr_soro("�V�l�u�ł́C�n�߂邼�D\n");
	pr_soro("�@�@�@���j�Z�Ŋ肢�܂��Ắc�c�D�v\n");
	for(i=4; i>=1; i--){
		disp_soro(0,i);
	}
	pr_soro("���@�u�˂��˂��C�����������C\n");
	pr_soro("�@�@�@���́w���j�Z�Ŋ肢�܂��Ắx���Ăǂ������Ӗ��Ȃ́H�v\n");
	pr_soro("�V�l�u��H���ꂩ�H�@����͂���΂���܂�����ȏ�Ԃɂ��Ă�������\n");
	pr_soro("�@�@�@�Ƃ����Ӗ�����D�@�܂��C����΂�Ōv�Z���n�߂�Ƃ���\n");
	pr_soro("�@�@�@���܂蕶��݂����Ȃ������D�v\n");
	pr_soro("���@�u��x���񂪂Ƃ񂿂��l����Ƃ��́w��������x�w�����ρI�x\n");
	pr_soro("�@�@�@�݂����Ȃ��́H�v\n");
	pr_soro("�V�l�u������ƈႤ����Ⴊ�C�܂��C�����l�Ȃ������\n");
	pr_soro("\n");
	hit_any_key();
	pr_soro("\x1b[1A�@�@�@�ł́C�C����蒼���āc�c\n");
	pr_soro("�@�@�@���j�Z�Ŋ肢�܂��Ă�\n");
	inc_in(in,1);
	disp_in(in);
	pr_soro("�~�Ȃ���\n");
	add_soro(st,in[1],1);
	inc_in(in,1);
	disp_in(in);
	add_soro(st,in[1],1);
	pr_soro("�~�Ȃ���c�c\n");
	pr_soro("���@�u�˂��˂��C�����������C\n");
	pr_soro("�@�@�@�ǂ����āw�`�~�Ȃ�x���Č����́H�v\n");
	pr_soro("�V�l�u��H�@������C�܂��C����΂���g���ꍇ�̏K���݂����Ȃ������\n");
	pr_soro("�@�@�@�̂́C����΂�͋�s�̂悤�Ȃ����������Ƃ���ł悭�g���Ă�������\n");
	pr_soro("�@�@�@�����ƁC���̖��c�����낤�āD\n");
	pr_soro("�@�@�@�ł́C�Â����s�����D\n");
	pr_soro("\n");
	hit_any_key();
	
	time(&st_time);						/* �X�^�[�g���� */
	for(;;){
		inc_in(in,1);
		if(chk_overflow(st,in,ADD) != OK){		/* overflow�����炨���܂� */
			break;
		}
		disp_in(in);
		pr_soro("�~�Ȃ���\n");
		for(i=20; i>0; i--)
			add_soro(st,in[i],i);
	}
	time(&end_time);					/* �I������ */
	end_time -= st_time;				/* ���s������end_time �ɓ��� */
	pr_soro("\n");	pr_soro("\n");	pr_soro("\n");	pr_soro("\n");
	pr_soro("�@�@�@�c�c���Ƃ��C���̂���΂�ł͂�����������񂭂�\n");
	pr_soro("�@�@�@����ȏ�͌v�Z�ł��񃏃C�D\n");
	pr_soro("�@�@�@�������C���O���������΂����̂��D�v\n");
	pr_soro("\n");
	hit_any_key();
	return end_time;
}

/******* �G���f�B���O ***********************************************/
void	ending(time_t end_time)
{
	g_cls();
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("�@���́C���̃v���O�����͍�҂������܂Ŏ��s�������͂���܂���D\n");
	printf("�@�ŁC�����̂���̂��C���̃v���O�����̎��s���Ԃł��D\n");
	printf("�@�����ŁC���Ȃ��ɂ��肢������̂ł����C���Ɏ������s�b����\n");
	printf("�@���̃v���O���������s�����R���s���[�^�̖��O��CPU�̃N���b�N��\n");
	printf("�@��҂܂Œm�点�Ă��������܂��񂩁H\n");
	printf("�@��낵�����肢���܂��D\n\n");
	printf("�@���̃v���O���������s����̂� %lu �b������܂����D\n\n",end_time);
	printf("�@�����܂ł��̃v���O�����ɂ������ĉ������āC");
	printf("�{���ɂ��肪�Ƃ��������܂����D\n");
}

/******* �����̃L�[�������Ă������� *********************************/
void	hit_any_key(void)
{
	printf("\33[47m--hit any key--\33[m");
	getch();
	printf("\r                 \r");
}

/******* �X�N���[���G���A�w�蕶����\�� *****************************/
void	pr_soro(char *moji)
{
	/* �v����� moji��\�����Ă���UE�s�ڂ��P��������Ă���̂ł� */
	printf("%s\x1b[s\x1b[%d;0H\x1b[2K\x1b[u",moji,UE);
}

/******* ���͕ϐ��{�{ ***********************************************/
void	inc_in(int *in,int keta)
{
	in[keta]++;
	if(in[keta] >=10){
		in[keta] -= 10;
		inc_in(in,keta+1);
	}
}

/******* ���͕ϐ��\�� ***********************************************/
void	disp_in(int *in)
{
	int		i,j = 0;			/* �L�ۖ��� */
	
	printf("�@�@�@");
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
 ******* �f���^�N�E���[�h ********************************************
 ********************************************************************/
void	dentak(void)
{
	int		in[30],st[30];		/* ���͕ϐ��C����΂�ϐ� */
	int		keta;				/* ���͕ϐ��̌��� */
	int		cal_flag;			/* �v�Z�� */
	int		ch,i;				/* �L�ۖ��� */
	
	for(i=0; i<25; i++)
		in[i] = st[i] = 0;
	keta = 0;
	cal_flag = ADD;
	
	disp_init(40);
	disp_window();
	printf("\x1b[15;4H�@�@���j�Z�ł��`�肢�܂��Ă͂��`�`�`�@�@");
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
					printf("�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@");
					printf("\x1b[19;36H�@�@�@�@");
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
				printf("\x1b[15;4H�����Ă�");
				printf("�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@");
				printf("\x1b[19;36H�@�@�@�@�@");
				cal_flag = ADD;
				disp_res(in,keta);
				break;
			
			case '-':
				if(keta != 0)	break;
				printf("\x1b[15;4H�����Ă�");
				printf("�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@�@");
				printf("\x1b[19;36H�@�@�@�@�@");
				cal_flag = SUB;
				disp_res(in,keta);
				break;
			
			case 0x0d:
			case ' ':
				if(keta == 0)	break;
				seikika(in,keta);
				if(chk_overflow(st,in,cal_flag) != OK){
					printf("\x1b[15;4H�@�@�@�@");
					printf("\x1b[17;4H�@�@�@");
					printf("�����ӂꂪ�N����܂����D�@�@�@�@�@\a");
					keta = 0;
					cal_flag = ADD;
					break;
				}
				if(ch == 0x0d){
					printf("\x1b[19;38H�~�Ȃ�");
				} else {
					printf("\x1b[19;38H�~�ł�");
				}
				printf("\x1b[15;4H�@�@�@�@");
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
				printf("\x1b[15;4H�@�@���j�Z�ł��`�肢�܂��Ă͂��`�`�`�@�@");
				printf("\x1b[19;38H�@�@�@");
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

/******* �f���^�N�E�E�C���h�E *************************************/
void	disp_window(void)
{
	int		i,j;		/* �L�ۖ��� */
	
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
	
	printf("\x1b[14;51H�y�L�[����̐����z");
	printf("\x1b[16;52H�m�O�n�`�m�X�n�F ��������");
	printf("\x1b[18;52H �m �a  �r �n �F �P�����");
	printf("\x1b[20;52H �m���^�[���n �F �`�~�Ȃ�");
	printf("\x1b[22;52H �m�d �r �b�n �F �� �j �Z");
	printf("\x1b[24;52H �m�r�s�n�o�n �F  �I�@��");
}


/******* ���͕ϐ��\�� ***********************************************/
void	disp_res(int *in,int keta)
{
	int	i;			/* �L�ۖ��� */
	
	for(i=20; i>=keta; i--)
		printf("\x1b[17;%dH�@",(42 - (i * 2)));
	
	for(i=keta; i>0; i--){
		printf("\x1b[17;%dH",(42 - ((keta - i) * 2)));
		putch(0x82);
		putch(0x4f + in[i]);
	}
}

/******* ���͐��l�̐��K�� *******************************************/
void	seikika(int *in,int keta)
{
	int		i,j;		/* �L�ۖ��� */
	
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

/******* �������牺�̊֐��͂�����������g���̂Œ��ӂ��� *************/

/******* ���l�̂���΂�\�� *****************************************/
void	disp_soro(int suuji,int p)
{
	int		i;		/* �L�ۖ��� */
	
	if(p > 20){
		err_nandaka("���l�̂���΂�\\���Ō��w�肪�ςł�");
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

/******* �����ӂ�`�F�b�N *******************************************/
int	chk_overflow(int *st,int *in,int cal_flag)
{
	int		i;							/* �L�ۖ��� */
	static int		stl[30];			/* �Ǐ��p�z�� */
	
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
	err_nandaka("cal_flag ���ςł�");
	/*NOTREACHED*/
}

/******* �w�茅�̑����Z *********************************************/
void	add_soro(int *st,int in,int p)
{
	if(p > 20){
		err_nandaka("���Z�Ō����ӂ�ł�");
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

/******* �w�茅�̈����Z *********************************************/
void	sub_soro(int *st,int in,int p)
{
	if(p > 20){
		err_nandaka("�����Z�Ō����ӂ�ł�");
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
 ******* �}�E�X�E���[�h **********************************************
 ********************************************************************/
void	mouse(void)
{
	int		mo_x,mo_y,mo_l,mo_r;		/* �}�E�X�ʒu����у{�^���� */
	int		i;							/* �L�ۖ��� */
	
	if(mo_check() == NO){
		printf("\n���@�u�}�E�X���[�h��");
		printf("�}�E�X�h���C�o��g�ݍ���ł�����s���ĂˁD�v\n");
		exit(0);
	}
	
	disp_init(100);
	mo_disp();
	mo_l = mo_r = 0;
	set_stop_signal();
	for(;;){
		printf("\r");		/* ��Ȃ�����,���ꂪ stop�L�[�΍�(��;) */
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
 ******* �ėp�֐��Q **************************************************
 ********************************************************************/

/******* �X�g�b�v�L�[�̃V�O�i���Z�b�g *******************************/
void	set_stop_signal(void)
{
	if(signal(SIGINT,quit) == (void(*)()) -1)
		err_signal();
		/*NOTREACHED*/
}

/******* �ŏ��̂���΂�̕\�� ***************************************/
void	disp_init(int up)
{
	int	i,j;			/* �L�ۖ��� */
	
	/* �ŏ��̏��� */
	g_init();
	g_screen(0,0);
	g_cls();
	printf("\x1b[2J\x1b[>5h\x1b[>1h");	/* ��ʂ����ꂢ�ɂ��� */
	get_tama();							/* ��̃C���[�W�擾 */
	calc_tamaichi(up);					/* ��ʒu�v�Z */
	
	/* ����΂�̘g */
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
	
	/* �� */
	g_line(19,up+39,621,up+ 41,7,2);
	for(i=1; i<=20; i++)
		g_pset(x[i]+15,up+40,0);
	
	/* ��Ɩ_ */
	for(i=1; i<=20; i++){
		g_put(x[i],y_u[0],tama,leng,0);
		g_put(x[i],y_u[1],boh,leng,0);
		g_put(x[i],y_l[0],boh,leng,0);
		for(j=1; j<5; j++)
			g_put(x[i],y_l[j],tama,leng,0);
	}
}

/******* ����΂�̎�C���[�W�擾 ***********************************/
void	get_tama(void)
{
	int		i,j;		/* �L�ۖ��� */
	
	/* �C���[�W�̑傫�� */
	leng = (int)((30+8)/8)*(14+1)*3 + 4;
	
	/* �C���[�W�̈�̊m��*/
	tama = (char *)malloc(leng);
	if(tama == NULL)
		err_no_memory();
	boh = (char *)malloc(leng);
	if(boh == NULL)
		err_no_memory();
	
	/* ��C���[�W */
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
	
	/* �_�C���[�W */
	g_line(14,0,16,13,7,2);
	for(i=0; i<=13; i+=2)
		g_pset(16,i,0);
	g_get(0,0,29,13,boh,leng);
	g_line(0,0,30,13,0,2);
}

/******* ��`��ʒu�v�Z *********************************************/
void	calc_tamaichi(int up)
{
	int		i;		/* �L�ۖ��� */
	
	for(i=0; i<20; i++)
		x[20 - i] = 20 + 30 * i;
	for(i=0; i<2; i++)
		y_u[i] = up + 10 + (14 * i);
	for(i=0; i<5; i++)
		y_l[i] = up + 43 + (14 * i);
}

/******* �I������ ***************************************************/
void	quit(void)
{
	signal(SIGINT,SIG_IGN);
	if(mo_check() == OK)
		mo_erase();
	g_cls();
	printf("\x1b[>5l\x1b[>1l\x1b[2J");
	printf("\n\n��ҁu\"soro.exe\" ���g���Ă���Ă��肪�Ƃ��D�v\n\n");
	exit(0);
	/*NOTREACHED*/
}

/*********************************************************************
 ******* �e��G���[���� **********************************************
 ********************************************************************/

/******* �I�v�V�����~�X�i�g�����j************************************/
void	usage(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("�V�l�u�g������ soro [-d|-m] ����D�v\n");
	exit(1);
	/*NOTREACHED*/
}

/******* ������������Ȃ� *******************************************/
void	err_no_memory(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("�V�l�u���܂񂪁C������������̂��Ď��s�ł����D�v\n");
	exit(1);
	/*NOTREACHED*/
}

/******* �V�O�i�����Z�b�g�ł��Ȃ� ***********************************/
void	err_signal(void)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("���@�u�ǂ�����signal�֐��ŃG���[���N�������́H�v\n");
	exit(1);
	/*NOTREACHED*/
}

/******* �Ȃ񂾂��킩��Ȃ� *****************************************/
void	err_nandaka(char *mes)
{
	printf("\x1b[>5l\x1b[>1l\x1b[2J\n\n");
	printf("��ҁu%s�D�v\a\n",mes);
	exit(1);
	/*NOTREACHED*/
}