* -*-  tab-width : 8 ; mode : M658k-Assembly ; encode : ShiftJIS -*-
* �I���I�����C�u���� For X68000
* By Pirota Pirozou, 2022-2023
*
* ���̃R�[�h�́A����ƕ֗��Ȋ֐����܂Ƃ߂����̂ł��B
* ���̃��C�u�����́A���R�ɉ��ς��Ă�����č\���܂���B
* �������A���ς������̂����J����ꍇ���A���̃��C�u�����̒��쌠�\���͕K�v����܂���B
* �܂��A���̃��C�u�������g�p�����v���O�����̒��쌠�́A
* ���̃v���O�����̒���҂ɂ���܂��B
*
* This code is a collection of functions that are useful to have.
* This library may be freely modified.
* However, even if you publish the modified version, you do not need to indicate the copyright of this library.
* The copyright of any program that uses this library belongs to the author of the program,
* the authors of the programs that use this library.

	.include IOCSCALL.MAC
	.include DOSCALL.MAC
	.include LABEL.H

	* �o�t�r�g�}�N�� *		ex)push a3-d6
push:	macro	Param
	movem.l	Param,-(sp)
	endm

	* �o�n�o�}�N�� *		ex)pop	d3-d7/a3
pop:	macro	Param
	movem.l	(sp)+,Param
	endm

COPYKILL	equ	1		* �b�n�o�x�L�[�����@�����t���O

	.xref	_PRG_QUIT

	.xdef	_super_begin
	.xdef	_super_end
	.xdef	_screen_init
	.xdef 	_gcls
	.xdef	_gamepad
	.xdef	_TX_CLS

	.xdef	_vsync

	.bss
	.even

nmibuf:
	ds.l	1			* ��O�����E���̔�ѐ�
abort_vec:
	ds.l	1			* �A�{�[�g�E�x�N�^�@���̔�ѐ�
uspbuf:
	ds.l	1			* �������ޔ�p

	.text
	.even

	***************************
	* �X�[�p�[�o�C�U�[�E���[�h��
	***************************
	* void super_begin(void);
_super_begin:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)
*	-------------------
	lea.l	$80,a1			* �s�q�`�o���O�̃o�b�t�@�𒲂ׂ�
	IOCS	_B_LPEEK

*	cmpi.l	#$00FF0000,d0
	bra	@f
*	bcs	@f		* ��ѐ悪�q�n�l�łȂ�

	*** �~���[�W�b�N�h���C�o���o�^����ĂȂ� ****
*	pea	mdrverr
*	DOS	_PRINT
	addq.l	#4,sp

	move.w	#1,-(sp)
	DOS	_EXIT2
*	===================
@@:
 	ifne	COPYKILL
	pea	dumint			* �b�n�o�x���j�h�k�k
	move.w	#$2C,-(sp)
	DOS	_INTVCS
*	-------------------
	pea	nmi			* �h�m�s�d�q�q�t�o�s���j�h�k�k
	move.w	#$2E,-(sp)		* �i�s�q�`�o�@���P�S�j
	DOS	_INTVCS
	lea.l	12(sp),sp

	move.l	d0,nmibuf
	endif
*	===================
	pea	dumint			* �a�q�d�`�j���j�h�k�k
	move.w	#$2B,-(sp)		* 6
	DOS	_INTVCS
*	-------------------
	pea	dumint			* �b�s�q�k�{�b���j�h�k�k
	move.w	#$2D,-(sp)
	DOS	_INTVCS			* 12
*	-------------------

	pea	_PRG_QUIT		* �A�{�[�g�x�N�^���Z�b�g����
	move.w	#$FFF2,-(sp)
	DOS	_INTVCS			* 18
	move.l	d0,abort_vec

	lea.l	18(sp),sp
*	-------------------

	clr.l	-(sp)
	DOS	_SUPER			* �X�[�p�[�o�C�U�E���[�h��
	addq.l	#4,sp
	lea.l	uspbuf,a1
	move.l	d0,(a1)

*	-------------------
	movem.l (sp)+,regs
	rts

	*******************
	**** ��ʏ����� ****
	*******************
	* void screen_init(void);
_screen_init:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)

	**** �V�X�e���֌W������ ****
	moveq	#1,d1
	moveq	#2,d2
	IOCS	_TGUSEMD		* �e�L�X�g���A�v���̎g�p���ɂ��Ă��

	IOCS	_MS_INIT

	IOCS	_OS_CUROF		* �J�[�\��������
	IOCS	_MS_CUROF

	move.w	#2,-(sp)
	move.w	#14,-(sp)
	DOS	_CONCTRL		* �t�@���N�V��������
	addq.l	#4,sp

	jbsr	_TX_CLS			* TEXT �N���A

	moveq	#0,d1
	moveq	#2,d2			* �f�|�q�`�l�́A�A�v���Ŏg�p��
	IOCS	_TGUSEMD

	move.w	#$10A,d1
	IOCS	_CRTMOD			* 256x256 256�Fx2���ɂ���

	IOCS	_G_CLR_ON

	*** PCG�p�^�[���O �N���A
	lea.l	PCG_AREA,a0
	moveq	#0,d0
	move.l	#32*192-1,d1
@@:
	move.l	d0,(a0)+
	dbra	d1,@b

	moveq	#1,d1			* �a�f�s�d�w�s�P�@�b�k�q
	move.w	#$100,d2
	IOCS	_BGTEXTCL

	* �X�v���C�g�p���b�g�N���A
	lea.l	SP_PAL+32,a0
	moveq	#16*15/4-1,d0
	moveq	#0,d1
@@:
	move.l	d1,(a0)+
	dbra	d0,@b

	* �O���t�B�b�N�p���b�g�N���A
	lea.l	GRPPAL,a0
	moveq	#127,d0
	moveq	#0,d1
@@:
	move.l	d1,(a0)+
	dbra	d0,@b

	*
	lea.l	SP_SCR+6,a0			* �r�o�X�N���[�����W�X�^�@������
	moveq	#127,d0
spcl_loop:
	clr.w	(a0)				* �v���C�I���e�B�F0 �i��\���j
	addq.l	#8,a0
	dbra	d0,spcl_loop

	clr.l	BG0_SCRX			* �a�f�O�X�N���[�� 0,0
	clr.l	BG1_SCRX			* �a�f�P�X�N���[�� 0,0

	* �O���t�B�b�N�D�揇��
crtmode = %00000_001				* 256Colors
	move.b	#crtmode,$E80028		* ( R20 )
	move.b	#crtmode,$E82401		* 256colors
	* �r�f�I�R���g���[�����W�X�^
	move.w	#%00_10_00_01_1110_0100,$E82500	* R1>TEXT>SP>GR �^ GR=Sc0>Sc1
	move.b	#$6F,$E82601			* R2>�O���t�B�b�N|�X�v���C�g�\������

	move.b	#%1111,$E8002B			* �O���t�B�b�N�X�N���[���P�C�Q

	move.w	#%00000010_00_011_011,$EB0808	* �a�f�O���s�d�w�s�P�^�a�f�P���s�d�w�s�P/DISP ON

	movem.l (sp)+,regs
	rts

	************************
	* ���[�U�[�E���[�h�֕��A
	************************
	* void super_end(void);
_super_end:
regs	reg	d3-d7/a3-a5
	movem.l regs,-(sp)
	jbsr	_TX_CLS

	move.b	#$08,$E8E007		* �g�q�k���N���A

	* B_SUPER �̌Ăяo����gcc�œK���̓s���Ŏ��s����ꍇ�ACFLAGS �ɁA-fno-defer-pop ������
	* ���Ȃ��ꍇ�́A�R�����g�A�E�g�Ń��[�U�E���[�h�ւ�OS�ɔC����
	lea.l	uspbuf,a1
	move.l	(a1),-(sp)
	DOS	_SUPER
	addq.l	#4,sp
*	-------------------

	**** ��ʏ����� ****
	moveq	#1,d1
	moveq	#1,d2			* �e�L�X�g�́A�V�X�e���Ŏg���Ă���
	IOCS	_TGUSEMD

	moveq	#0,d1
	moveq	#3,d2			* �f�|�q�`�l�́A�A�v���Ŏg���ĉ�ꂽ
	IOCS	_TGUSEMD

	IOCS	_MS_INIT		* �}�E�X������

	clr.w	-(sp)
	move.w	#16,-(sp)
	DOS	_CONCTRL
	addq.l	#2,sp
	move.w	#14,-(sp)
	DOS	_CONCTRL		* �t�@���N�V�����\��
	addq.l	#4,sp

	IOCS	_OS_CURON

	**** �������J�� ****
	clr.l	-(sp)
	DOS	_MFREE
	addq.l	#4,sp

	move.w	#$FF,-(sp)
	DOS	_KFLUSH
	addq.l	#2,sp

	movem.l (sp)+,regs
	rts

	******************************
	**** �O���t�B�b�N��ʃN���A ****
	******************************
	* void gcls(int page);
_gcls:
reglist	reg	d3-d7
	move.l	4(sp),d0		* ������d0��(�y�[�W�ԍ�)

	push	reglist

	lsl.w	#3,d0			* x 8
	swap	d0
	lea	GRAM+$80000,a0
	adda.l	d0,a0
	move.l	#$800,d7
	moveq	#0,d0
	moveq	#0,d1
	moveq	#0,d2
	moveq	#0,d3
	moveq	#0,d4
	moveq	#0,d5
	moveq	#0,d6
	suba.l	a1,a1
@@:
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	movem.l	d0-d6/a1,-(a0)
	dbra	d7,@b

	pop	reglist
	rts

	*****************************
	* �W���C�p�b�h�O���L�[����    *
	*****************************
	* int gamepad(void);
	*	X12XRLDU
	*	��������
	*	RET=D0.L :JOYSTICK
	*	���̃f�[�^�ƈقȂ�bit��1=ON, 0=OFF
_gamepad:
	moveq	#0,d1
	IOCS	_JOYGET			;�W���C�X�e�B�b�N�O�@�`�F�b�N
	eori.b	#$FF,d0			;���]
	bne	@f
	jbsr	kbchk0			;�L�[�{�[�h�E�`�F�b�N
@@:
	andi.l	#$FF,d0			;�W���C�X�e�B�b�N�O�@�e�k�`�f
	rts

	*******************************
	* �e���L�[�A�y�w�@�r�o����      *
	*******************************
	* �L�[�{�[�h���͂ƃW���C�p�b�h���͂�����
kbchk0:
	push	d1-d2

	moveq	#0,d2			;���ʃo�b�t�@

	*** �����@���� ***
	move.w	#8,d1
	IOCS	_BITSNS			;
	btst.l	#7,d0			;�`�F�b�N�@�h�S�h
	beq	st0_01
	ori.b	#4,d2			;�k�d�e�s�@�e�k�`�f
st0_01:
	btst.l	#4,d0			;�`�F�b�N  �h�W�h
	beq	st0_02
	ori.b	#1,d2			;�t�o�@�e�k�`�f
st0_02:


st0_11:
	move.w	#9,d1
	IOCS	_BITSNS			;
	btst.l	#4,d0			;�`�F�b�N�@�h�Q�h
	beq	st0_12
	ori.b	#2,d2			;�c�n�v�m�@�e�k�`�f
st0_12:
	btst.l	#1,d0			;�`�F�b�N�@�h�U�h
	beq	st0_13
	ori.b	#8,d2
st0_13:
	*** �{�^���@���� ***
	btst.l	#6,d0			;�`�F�b�N�@�hENTER�h
	bne	st0_trg1

st0_21:
	move.w	#5,d1
	IOCS	_BITSNS			;
	btst.l	#2,d0			;�`�F�b�N�@�h�y�h
	beq	st0_22
	ori.b	#64,d2			;�s�q�f�|�Q
st0_22:
	btst.l	#3,d0			;�`�F�b�N�@�h�w�h
	beq	st0_23
st0_trg1:
	ori.b	#32,d2			;�s�q�f�|�P
	bra	st0_ret
	****************
st0_23:
	move.w	#6,d1
	IOCS	_BITSNS			;�`�F�b�N�@�h�r�o�b�h
	btst.l	#5,d0
	bne	st0_trg1

*	------------------
st0_ret:
	move.b	d2,d0

	pop	d1-d2
	rts

********************************************
*  �e�L�X�g�v���[���E�I�[���N���A�[        *
*                                          *
*         �@�s�w�Q�b�k�r                   *
********************************************
*	�h���F�i�V
*	�j��F�i�V
*
*	TX_CLS();
_TX_CLS:
	push	d0-d3/a0
	lea	TEXTRAM,a0

	moveq	#128-1,d0
KESU_loop:
	move.l	a0,-(sp)
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)
	adda.l	#$20000,a0
	clr.l	(a0)

	move.l	(sp)+,a0
	addq.l	#4,a0
	dbra	d0,KESU_loop

	move.w	#$00_01,d1
	move.w	#256-1,d2
	moveq	#$00_0F,d3
	IOCS	_TXRASCPY

	pop	d0-d3/a0
	rts

*	-------------------------	* �_�~�[�̊��荞�ݏ����A�h���X
*	���荞�ݏ����A�h���X���A
*	�v���O�����I�������ɕύX����
*	-------------------------
nmi:
	cmpi.w	#$001F,d7		* �m�l�h���Z�b�g���A�����������H
	beq	nmi2
	cmpi.w	#$301F,d7		* �m�l�h���Z�b�g���A�����������H
	beq	nmi2

	pea	_PRG_QUIT		* �A�{�[�g�x�N�^���Z�b�g����
	move.w	#$FFF2,-(sp)
	DOS	_INTVCS
	addq.l	#6,sp

	move.l	nmibuf,-(sp)
	rts				* ���Ƃ̏����A�h���X�֔��

nmi2:
	move.b	#%1101,$E8E007		* �m�l�h���Z�b�g�������ꂽ��A��������邻���ȁB
*	move.b	#%1111,$E8E007		* (�g�q�k���n�m��������R��)
dumint:
	rte

	*******************************************************
	**** �m���ɂP�^�U�O�b�ȓ��ɏI��鏈���̎��Ɏg��������� ****
	*******************************************************
	* void vsync(void);
_vsync:
	btst.b	#4,$E88001		* �����A�����Ԓ��͑҂�
	beq	_vsync
@@:
	btst.b	#4,$E88001		* �����\�����Ԃ͑҂�
	bne	@b
	rts

	.end
