* -*-  tab-width : 8 ; mode : m68k ; encode : ShiftJIS -*-
* X68000 �X�v���C�g���C�u���� 680000�A�Z���u���R�[�h
* 2023/04/06 �쐬
* by Pirota Pirozou
*
* ���̃��C�u�����́AX68000�̃X�v���C�g���������߂̃��C�u�����ł��B

	include	IOCSCALL.MAC
	include	DOSCALL.MAC
	include	MACRO.H
	include	LABEL.H

        .xdef _CM_def_px2
        .xdef _CM_def_grp_palette

	************************
	**** �X�v���C�g��` ****
	**** FOR PX2        ****
	************************
        * void CM_def_px2((unsigned char *)px2_buf);
        * px2_buf : PX2�f�[�^�̃A�h���X
        * ���̊֐��́APX2�f�[�^���X�v���C�g�ɒ�`���܂��B
        * �������A�L�����N�^�͐擪192�L�����A
        * �iBG1�ʂ̃f�[�^���㏑�����Ȃ����߁j
        * �p���b�g�͐擪1�p���b�g���X�L�b�v���Ē�`���܂��B
        * �i�e�L�X�g�p���b�g��������Ȃ����߁j
_CM_def_px2:
	link	a6,#0
	movea.l	8(a6),a0
	lea	$220(a0),a0
	lea	TEXTPAL+32,a1
	lea	PCG_AREA,a2

	move.l	#240-1,d0
@@:
	move.w	(a0)+,(a1)+
	dbra	d0,@b

	move.l	#12288-1,d0
@@:
	move.w	(a0)+,(a2)+
	dbra	d0, @b

	unlk	a6
	rts

	********************************
	**** �O���t�B�b�N�p���b�g��` ****
	*******************************
	* void CM_def_grp_palette((unsigned short *) grppal_buf);
        * pe2pal_buf : PE2�p���b�g�f�[�^�̃A�h���X
_CM_def_grp_palette:
	movea.l	4(sp),a1
	lea	GRPPAL,a0

	moveq	#127,d0
@@:
	move.l	(a1)+,(a0)+
	dbra	d0,@b
	rts
