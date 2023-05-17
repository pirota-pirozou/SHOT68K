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

**************************************
*�@�����l����f�[�^���@�@�@�@�@�@�@�@*
**************************************
	.data

        .text

        * void zmsc_init(void);
_zmsc_init:
	moveq	#0, d0
	rts

        * void zmsc_play(int);
_zmsc_play:
	moveq	#0, d0
	rts

	* void zmsc_stop(void);
_zmsc_stop:
	moveq	#0, d0
	rts

	* void zmsc_fadeout(void);
_zmsc_fadeout:
	moveq	#0, d0
	rts

	* void zmsc_seplay(void);
_zmsc_seplay:
	moveq	#0, d0
	rts

	* int zmsc_keepchk(void);
_zmsc_keepchk:
	moveq	#0, d0
	rts
