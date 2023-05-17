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

	moveq	#0, d0
init_ret:
	rts

        * void zmsc_play(int);
_zmsc_play:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	play_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#0, d0
play_ret:
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	stop_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#0, d0

stop_ret:
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	fadeout_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#0, d0
fadeout_ret:
	rts

	* void zmsc_seplay(void);
_zmsc_seplay:
	tst.w	keeped_flg				* �풓�t���O���`�F�b�N
	beq	seplay_ret				* �풓���Ă��Ȃ��ꍇ�͉������Ȃ�

	moveq	#0, d0
seplay_ret:
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
	move.w  d0, keeped_flg
	rts

	* �풓���Ă��Ȃ�
chk_drv_err:
	clr.w 	keeped_flg
	moveq.l #-1,d0
	rts
