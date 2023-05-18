* -*-  tab-width : 8 ; mode : m68k ; encode : ShiftJIS -*-
* Z-MUSIC�R���g���[�����C�u���� 680000�A�Z���u���R�[�h
* 2023/05/17 �쐬
* by Pirota Pirozou
*
* ���̃��C�u�����́AZ-MUSIC�ŉ��y����ʉ���炷���̂ł��B

	include iocscall.mac
	include doscall.mac
	include	macro.h

	.xdef	_zmsc_init
	.xdef	_zmsc_play
	.xdef	_zmsc_stop
	.xdef	_zmsc_cont
	.xdef	_zmsc_fadeout
	.xdef	_zmsc_seplay
	.xdef	_zmsc_keepchk

	* Z-MUSIC API �}�N��
Z_MUSIC	macro	func
	moveq.l	func,d1
	trap	#3
	endm

* �j�󂵂Ă͂����Ȃ����W�X�^
* reglist	reg	d3-d7/a3-a5

****************************************
*�@�����l�Ȃ��f�[�^���@�@�@�@�@�@�@�@�@*
****************************************
	.bss

keeped_flg:	.ds.w	1	* �풓�t���O

**************************************
*�@�����l����f�[�^���@�@�@�@�@�@�@�@*
**************************************
	.data

        .text

        * void zmsc_init(void);
_zmsc_init:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	init_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	Z_MUSIC	#$00					* Z-MUSIC������
	Z_MUSIC	#$0D					* Z-MUSIC�����ݒ�
init_ret:
	rts

        * void zmsc_play(void *ptr);
_zmsc_play:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	play_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	movea.l	4(sp),a1				* ���t�f�[�^�i�[�A�h���X
	moveq	#0,d2					* ��������
	Z_MUSIC	#$11					* ZMD�Đ�

	moveq	#0, d0
play_ret:
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	stop_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

reglist	reg	d3-d4
	movem.l reglist,-(sp)
	moveq	#0, d2
	moveq	#0, d3
	moveq	#0, d4
	Z_MUSIC	#$0A					* ���t��~
	movem.l (sp)+,reglist
stop_ret:
	rts

	* void zmsc_cont(void);
_zmsc_cont:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	@f 					* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

reglist	reg	d3-d4
	movem.l reglist,-(sp)
	moveq	#0, d2
	moveq	#0, d3
	moveq	#0, d4
	Z_MUSIC	#$0B					* ���t�ĊJ
	movem.l (sp)+,reglist
@@:
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	@f					* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#64,d2					* �t�F�[�h�A�E�g���x
	Z_MUSIC	#$1A					* �t�F�[�h�A�E�g
@@:
	rts

	.if 0
--------------------------------------------------------------------------------
�t�@���N�V����$12       se_play

�@�\:   ���ʉ����t

����:   d2.l�����g���b�N���犄�荞�܂��邩�i1�`32�j
	a1.l�����t�f�[�^�i�[�A�h���X(���l�Q��)

�߂�l: �Ȃ�

���l�F  ���ʉ��̍\��
Offset  �{0     :$10(.b)               �������A�h���X}
        �{1�`�{6:'ZmuSiC'                            }��������ɖ����Ă��\��Ȃ�
	�{7     :Version Number(.b)                  }
	�{8�`   :���ʃR�}���h                        }
	�{?     :Number of tracks(.w)  ��a1.l�Ŏw�������ׂ��A�h���X(�����A�h���X)
	�Ȃ��A�����t�H�[�}�b�g�ɂ��Ă̏ڂ��������MEASURE12�Q��
--------------------------------------------------------------------------------
	.endif
	* void zmsc_seplay(void *ptr);
_zmsc_seplay:
*	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
*	beq	@f					* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#7, d2					* 7-8 �g���b�N�ōĐ�
	movea.l	4(sp),a1				* ���t�f�[�^�i�[�A�h���X
	Z_MUSIC	#$12					* �r�d�Đ�

	moveq	#0, d0
@@:
	rts

	* int zmsc_keepchk(void);
_zmsc_keepchk:
	* > eq���풓�m�F
	* > mi���풓���Ă��Ȃ�
	move.l  $8c.w,a0
	subq.w  #8,a0
	cmpi.l  #'ZmuS',(a0)+
	bne     chk_drv_err
	cmpi.w  #'iC',(a0)+
	bne     chk_drv_err
	* �풓���Ă���
	moveq	#0,d0
	move.w	(a0)+,d0	* �o�[�W����
	move.w  #1, keeped_flg
	rts

	* �풓���Ă��Ȃ�
chk_drv_err:
	clr.w 	keeped_flg
	moveq.l #-1,d0
	rts
