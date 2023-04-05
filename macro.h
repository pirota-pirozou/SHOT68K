* -*-  tab-width : 8 ; mode : M658k-Assembly ; encode : ShiftJIS -*-
*****************************************
*　アセンブラマクロ定義　v2.00　　　　　　 *
*****************************************
* By Pirota Pirozou, 1991-
	.nlist

H_REV:	equ	16384
V_REV:	equ	32768
HV_REV:	equ	(H_REV|V_REV)

	* ＰＵＳＨマクロ *		ex)push a3-d6
push:	macro	Palam
	movem.l	Palam,-(sp)
	endm

	* ＰＯＰマクロ *		ex)pop	d3-d7/a3
pop:	macro	Palam
	movem.l	(sp)+,Palam
	endm

	* ＡＢＳ．Ｗマクロ *		ex)abs_w d0
abs_w:	macro	Palam			*  ret:d0=abs(d0)
	local	absw_exit

	tst.w	Palam
	bpl	absw_exit
	neg.w	Palam
absw_exit:
	endm

	* ＡＢＳ．Ｌマクロ *		ex)abs_l d0
abs_l:	macro	Palam			*  ret:d0=abs(d0)
	local	absl_exit

	tst.l	Palam
	bpl	absw_exit
	neg.l	Palam
absl_exit:
	endm

	* ＳＧＮ．Ｗマクロ *		ex)sgn_w d0
sgn_w:	macro	Palam			*  ret:d0=sgn(d0)
	local	sgnw_pl,sgnw_mi,sgnw_exit

	tst.w	Palam
	bpl	sgnw_pl
	bmi	sgnw_mi
	clr.w	Palam
	bra	sgnw_exit
sgnw_pl:move.w	#1,Palam
	bra	sgnw_exit
sgnw_mi:move.w	#-1,Palam
sgnw_exit:
	endm

	* ＳＧＮ．Ｌマクロ *		ex)sgn_l d0
sgn_l:	macro	Palam			*  ret:d0=sgn(d0)
	local	sgnl_pl,sgnl_mi,sgnl_exit

	tst.l	Palam
	bpl	sgnl_pl
	bmi	sgnl_mi
	clr.l	Palam
	bra	sgnl_exit
sgnl_pl:move.l	#1,Palam
	bra	sgnl_exit
sgnl_mi:move.l	#-1,Palam
sgnl_exit:
	endm

	* ｂｇ＿ｐｒｔマクロ *
bg_prt:	macro	MSGADR
	lea	MSGADR,a0
	jbsr	BG_PUTS
	endm

	* ｂｇ＿ｓｐｃマクロ *
bg_spc:	macro	SPC_LEN
	move.l	#SPC_LEN,d0
	jbsr	BG_SPC
	endm

	* ｂｇ＿ｐｕｔｃマクロ *
bg_putc:macro	CHR_CODE
	move.b	#CHR_CODE,d0
	jbsr	BG_PUTC
	endm

	* ｂｇ＿ｌｏｃａｔｅマクロ *
bg_locate:macro	X_pos,Y_pos
	moveq	#X_pos,d1
	moveq	#Y_pos,d2
	jbsr	BG_LOC
	endm

	* ｂｇ＿ｃｏｌｏｒマクロ *
bg_color:macro	Col
	move.w	#Col,d0
	jbsr	BG_COL
	endm

	* ｂｇ＿ｐａｇｅマクロ *
bg_page:macro	Pag
	move.w	#Pag,d0
	jbsr	BG_PAGE
	endm

	.list
