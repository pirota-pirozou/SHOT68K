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
	include	DMAADR.H

        * �������[�N
	.xdef	sprtbl
	.xdef	paltbl

	.xdef	sp_ready
	.xdef	int_cou

        * �֐���`
	.xdef _CM_sprite_clr
        .xdef _CM_sprite_on
        .xdef _CM_sprite_off
        .xdef _CM_vsync
        .xdef _CM_sp_set32
        .xdef _CM_sp_set
        .xdef _CM_parts_wrt
        .xdef _CM_parts_clr

        .xdef _CM_dma32
        .xdef _CM_wait_dma_end

        .xdef _CM_def_px2
        .xdef _CM_def_grp_palette

	.xdef _CM_bg_puts



* �j�󂵂Ă͂����Ȃ����W�X�^
* reglist	reg	d3-d7/a3-a5

****************************************
*�@�����l�Ȃ��f�[�^���@�@�@�@�@�@�@�@�@*
****************************************
	.bss
	** �r�o�X�N���[���E���W�X�^�p���[�N **
sprtbl:
	ds.b	1024

	** �r�o�p���b�g�E���W�X�^�p���[�N **
	* ��������������A�g���Ă��Ȃ���������Ȃ�
paltbl:
	ds.b	512

	** ���荞�݃J�E���^�i�O�`�Q�T�T�j **
int_cou:
	ds.b	1
	** sprite_ready FLAG **
sp_ready:
	ds.b	1

	** �c�l�`�|�b���荞�݃x�N�^ **
DMAC_V:
	ds.l	1

**************************************
*�@�����l����f�[�^���@�@�@�@�@�@�@�@*
**************************************
	.data
	** sp_set32�p�f�[�^ **
notable:
	* (0,1)(8,9)��(0,1)(2,3)
	dc.w	0,1,3,2		*�m�[�}��
	dc.w	1,0,2,3		*���E���]
	dc.w	2,3,1,0		*�㉺���]
	dc.w	3,2,0,1		*�㉺���E���]

	** parts_wrt�p�f�[�^ **
notable2:
	dc.w	0,2,1,3		*�m�[�}��
	dc.w	2,0,3,1		*���E���]
	dc.w	1,3,0,2		*�㉺���]
	dc.w	3,1,2,0		*�㉺���E���]

        .text

        ** �X�v���C�g�\�����[�h�@�n�m **
        * void CM_sprite_on(void);
_CM_sprite_on:
	clr.w	int_cou			* int_cou&sp_ready clr

	***** �r�o�X�N���[���q�d�f�@�N���A ****
	lea	$EB0006,a0
	moveq	#127,d0
@@:
	clr.w	(a0)
	addq.l	#8,a0
	dbra	d0,@b

	***** DMA INIT **********
	move.b	#$18,CCR2		* DMA Stop!!
	move.b	#$88,DCR2		* �b�o�t�����D��A�f�o�C�X�|�[�g�P�U�r�b�g
*	move.b	#$C8,DCR2		* �b�o�t������H�H�A�f�o�C�X�|�[�g�P�U�r�b�g
	move.b	#$21,OCR2		* �]���P�ʁF32bit=$21
	move.b	#FC_SUPER_DATA,MFC2
	move.b	#FC_SUPER_DATA,DFC2
	move.b	#5,SCR2			*
	clr.w	MTC2			* Counter Clear!

	ori.b	#$02,$EB0808
	ori.w	#$0040,$E82600

	move	sr,-(sp)		* ���荞�݋֎~

	ori	#%00000111_00000000,sr
	clr.b	$E88019			* MFP_TACR
	lea.l	sprite_int,A1
	move.w	#$00_01,d1
	iocs	_VDISPST
	move.b	#%00001000,$E88019

	move	(sp)+,sr		* ���荞�݋֎~��Ԃ̉���

	tst.l	d0
	beq	@f
	* Error
	moveq.l	#-1,d0
	rts
@@:	* OK
	moveq.l	#0,d0
	rts

	** �X�v���C�g�\�����[�h�@�n�e�e **
        * void CM_sprite_off(void);
_CM_sprite_off:
	push	d0-d1/a0-a1

	suba.l	a1,a1			* �����������荞�݂��֎~
	IOCS	_VDISPST

	pop	d0-d1/a0-a1
	rts

	** ���荞�ݏ������[�`�� **
sprite_int:
*	move.w	sr,d0		*���荞�݂�������B���荞�݃v���O�����ł�
*	subi.w	#$0100,d0	*��������Ă����̂���V�B���ꂪ�Ȃ��ƋȂ��x�ꂽ��Ȃǂ�
*	move.w	d0,sr		*���Q�i���̏������͑��̏��������荞�߂Ȃ��Ȃ邽�߁j���o�܂��B
				*�������A�f�o�b�O���͎ז��Ȃ̂Ŋ������Ă������܂��傤�B�������ł��B
*	andi.b	#$DF,$E88013

*	andi.b	#$FD,$EB0808			* SP_DISP OFF

	**********************	�X�v���C�g���W�X�^�@�c�l�`�]��
	move.b	#$18,CCR2			* DMA Stop!!
	move.l	#sprtbl,MAR2			* SOURCE ADRESS
	move.l	#$EB0000,DAR2			* DIST.ADRESS
	move.w	#$100,MTC2			* SIZE COUNTER
	move.b	#$88,CCR2			* DMA Move start!

@@:	* DMA�I���҂�
	tst.w	MTC2
	bne	@b

	move.b	#1,sp_ready
	rte


* 	*********************
* void CM_vsync();
_CM_vsync:
@@:	* �����������荞�ݑ҂�
	tst.b  sp_ready
	beq	@b
	move.b	#0,sp_ready
@@:
*	btst.b	#4,$E88001			* �����A�����Ԓ��͑҂�
*	beq	@b

*	ori.b	#$02,$EB0808			* SP_DISP ON
	rts

* �X�v���C�g�N���A
* void CM_sprite_clr(void);
_CM_sprite_clr:
*	lea	$EB0006,a0
	lea.l	sprtbl+6,a0
	moveq	#127,d0
@@:
	clr.w	(a0)
	addq.l	#8,a0
	dbra	d0,@b

	rts

*************************************
**    �R�Q�~�R�Q�X�v���C�g�o�t�s   **
**�i���z�r�o�X�N���[���E���W�X�^�p�j*
**                                 **
** �j��F���P�`���U�C���O�C���P    **
**                                 **
**      �r�Q�����Q�������R�Q       **
*************************************
_CM_sp_set32:
					*���P���X�v���C�g�v���[���ԍ�
					*���Q�������W
					*���R�������W
					*���S���p�^�[���R�[�h
					*���T���v���C�I���e�B
	link	a6,#0
reglist reg     d3-d6
	push    reglist
	movem.l	8(a6),d1-d5

	lea.l	sprtbl,a0
	lsl.w	#3,d1
	adda.w	d1,a0			*a0=�����X�N���[�����W�X�^�̂�����������

	lea.l	notable,a1
	btst	#14,d4
	beq	@f
	addq.l	#8,a1
@@:	btst.l	#15,d4
	beq	@f
	lea.l	16(a1),a1
@@:	move.l	d4,d6			*��4�̑ޔ�

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	add.w	#16,d2

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	add.w	#16,d3

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	move.l	d6,d4
	sub.w	#16,d2

	move.w	d2,(a0)+
	move.w	d3,(a0)+

	add.w	(a1)+,d4

	move.w	d4,(a0)+
	move.w	d5,(a0)+
****
	pop	reglist
	unlk	a6

	rts

*************************************
**    �P�U�~�P�U�X�v���C�g�o�t�s   **
**�i���z�r�o�X�N���[���E���W�X�^�p�j*
**                                 **
** �j��F���O                      **
**                                 **
**   �@�@�@�r�Q�����Q�������@�@�@�@**
*************************************
_CM_sp_set:
	link	a6,#0
reglist reg     d3-d5
	push    reglist
	movem.l	8(a6),d1-d5
					*���P���X�v���C�g�v���[���ԍ�
					*���Q�������W
					*���R�������W
					*���S���p�^�[���R�[�h
					*���T���v���C�I���e�B
	lea.l	sprtbl,a0
	move.l	d1,-(sp)
	lsl.w	#3,d1
	adda.w	d1,a0
	move.l	(sp)+,d1

	movem.w	d2-d5,(a0)

        pop	reglist
	unlk	a6
	rts

***********************************
**�@�a�f�Ƀp�[�c��u�����[�`���@ **
**            �i�Q�~�Q�j         **
**�@�j��F���Q�|���T�C���O�|���P **
**                               **
**      �����������Q������       **
***********************************
_CM_parts_wrt:
	link	a6,#0
reglist reg     d3-d5
	push	reglist
	movem.l	8(a6),d1-d4
					*���P���a���y�[�W
					*���Q�������W
					*���R�������W
					*���S���p�^�[���R�[�h
	lea.l	$EBC000,a0
	tst.w	d1
	beq	@f

	lea.l	$2000(a0),a0
@@:
	move.w	d4,d5
	lsl.w	#2,d4			*d4=d4x4
	andi.w	#$00FF,d4
	andi.w	#$FF00,d5
	or.w	d5,d4

	add.w	d2,d2
	add.w	d2,d2
	adda.l	d2,a0			*a0=a0+(x*4)
	lsl.w	#8,d3
	adda.l	d3,a0			*a0=a0+(y*256)

	lea.l	notable2,a1
	btst.l	#14,d4
	beq	@f
	addq.l	#8,a1
@@:
	btst.l	#15,d4
	beq	@f
	lea.l	16(a1),a1
@@:
	move.w	d4,d5			*��4�̑ޔ�

	add.w	(a1)+,d4
	move.w	d4,(a0)+

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)+

	adda.l	#124,a0			*a0=a0+((64-2)*2)

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)+

	move.w	d5,d4
	add.w	(a1)+,d4
	move.w	d4,(a0)

	pop	reglist
	unlk	a6
	rts

***********************************
**�@�a�f�̃p�[�c���������[�`���@ **
**         �i�Q�~�Q�j�@�@�@�@�@  **
**�@�j��F���Q�|���R�C���O�@�@�@ **
**                               **
**      �����������Q������       **
***********************************
* ���P���a���y�[�W
* ���Q�������W
* ���R�������W
_CM_parts_clr:
	link	a6,#0
reglist	reg	d3
	move.l	reglist,-(sp)
	movem.l	8(a6),d1-d3

	lea.l	$EBC000,a0
	tst.w	d1
	beq	@f

	lea.l	$2000(a0),a0
@@:
	add.w	d2,d2
	add.w	d2,d2
	adda.l	d2,a0			*a0=a0+(x*4)
	lsl.w	#8,d3
	adda.l	d3,a0			*a0=a0+(y*256)

	clr.l	(a0)+
	lea.l	124(a0),a0
	clr.l	(a0)

        move.l	(sp)+,reglist
	unlk	a6
	rts

***************************************
*    �R�Q�r�b�g�P�� �c�l�`�]��        *
*                                     *
* d1=#%0000_0101(��۸��ϰ�ޥ�ƭ�َQ��)*
* d2=�J�E���^                         *
* a1=�]�����i�\�[�X�E�������A�h���X�j *
* a2=�]����i�ި�èȰ��ݥ�h�^�n�|�[�g)*
***************************************
_CM_dma32:
@@:     * DMA�]���I���҂�
	tst.w	MTC2
	bne	@b

	link	a6,#0
*reglist reg     d3-d6
*	push	reglist
	movem.l	8(a6),d1-d2/a1-a2

        * DMA�]���J�n
	move.b	#$18,CCR2		*
*	move.b	#$08,DCR2		* (�S�J�o���o���̎��́A�R��)
	move.b	#$88,DCR2		* �b�o�t�����D��A�f�o�C�X�|�[�g�P�U�r�b�g
	move.b	#$21,OCR2		* 32bit=$21
	move.b	#FC_SUPER_DATA,MFC2
	move.b	#FC_SUPER_DATA,DFC2
	move.b	d1,SCR2			*
	move.l	a1,MAR2			* SOURCE ADRESS
	move.l	a2,DAR2			* DIST.ADRESS
	move.w	d2,MTC2			* SIZE COUNTER
	move.b	#$88,CCR2		* Move start!

*	pop	reglist
	unlk	a6
	rts

        * DMA�]���I���҂����[�`��
        * void CM_wait_dma_end(void);
_CM_wait_dma_end:
	tst.w	MTC2
	bne	_CM_wait_dma_end
	rts

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
        * �X�v���C�g�p���b�g��`
	move.l	#240-1,d0
@@:
	move.w	(a0)+,(a1)+
	dbra	d0,@b

        * �L�����N�^��`
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

        * �O���t�B�b�N�p���b�g��`
	moveq	#127,d0
@@:
	move.l	(a1)+,(a0)+
	dbra	d0,@b
	rts

* void CM_bg_puts(const char *str, int x, int y,int col);
_CM_bg_puts:
	link	a6,#0
reglist reg     d3
	push	reglist

	move.l	8(a6),a0		* a0=������A�h���X
	move.l	12(a6),d1		* X���W
	move.l	16(a6),d2		* Y���W
	move.l	20(a6),d3		* �F�R�[�h 0-15
	* VRAM�A�h���X�v�Z
	lea	BG_TXT1,a1		* a1=BG1 TEXT�A�h���X
	add.w	d1,d1
	adda.l	d1,a1
	lsl.w	#7,d2
	adda.l	d2,a1			* A1=BG RAM ADRESS

	* �F�R�[�h�v�Z
	lsl.w	#8,d3
	andi.w	#$0F00,d3

@@:	move.w	d3,d0
	move.b	(a0)+,d0
	beq	@f			* �I�[�܂ł�����I��

	subi.b	#$20,d0			* �����R�[�h��␳
	move.w  d0,(a1)+		* �����R�[�h����������
	bra.s	@b
@@:
	pop	reglist
	unlk	a6
	rts
