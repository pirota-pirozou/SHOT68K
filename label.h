* -*-  tab-width : 4 ; mode : M68k-Assembly ; encode : ShiftJIS -*-
************************
** X68000 �J���p���x�� **
**   By Pirota Pirozou **
************************
	.nlist
******** �f�[�^�����p
NULL		equ	$00		* �k��
CR		equ	$0D		* �L�����b�W�E���^�[��
LF		equ	$0A		* ���C���t�B�[�h
EOF		equ	$1D		* �G���h�E�I�u�E�t�@�C��
ESC		equ	$1B		* �d�r�b�R�[�h�@�n�܂�

STD		equ	$01		* �t�@�C���n���h���E�W���o��
STDERR		equ	$02		* �t�@�C���n���h���E�W���G���[�o��

******** �V�X�e���֌W�A�h���X
GRAM		equ	$C00000		* �O���t�B�b�N�u�q�`�l
GRPPAL		equ	$E82000		* �O���t�B�b�N�p���b�g�E���W�X�^
GRP0_X		equ	$E80018		* �X�N���[���O�@�w
GRP0_Y		equ	$E8001A		* �X�N���[���O�@�x
GRP1_X		equ	$E8001C		* �X�N���[���P�@�w
GRP1_Y		equ	$E8001E		* �X�N���[���P�@�x
GRP2_X		equ	$E80020		* �X�N���[���Q�@�w
GRP2_Y		equ	$E80022		* �X�N���[���Q�@�x
GRP3_X		equ	$E80024		* �X�N���[���R�@�w
GRP3_Y		equ	$E80026		* �X�N���[���R�@�x

TEXTRAM		equ	$E00000		* �e�L�X�g�u�q�`�l
TEXTPAL		equ	$E82200		* �e�L�X�g�p���b�g�E���W�X�^
TEXT_X		equ	$E80014		* �e�L�X�g�@�w
TEXT_Y		equ	$E80016		* �e�L�X�g�@�x

SP_SCR		equ	$EB0000		* �r�o�X�N���[���E���W�X�^
SP_PAL		equ	TEXTPAL		* �r�o�p���b�g�E���W�X�^

PCG_AREA	equ	$EB8000		* �o�b�f�G���A
BG_TXT0		equ	$EBC000		* �a�f�e�L�X�g�O
BG_TXT1		equ	$EBE000		* �a�f�e�L�X�g�P
BG0_SCRX	equ	$EB0800		* �a�f�X�N���[�����W�X�^�O�@�w
BG0_SCRY	equ	$EB0802		* �a�f�X�N���[�����W�X�^�O�@�x
BG1_SCRX	equ	$EB0804		* �a�f�X�N���[�����W�X�^�P�@�w
BG1_SCRY	equ	$EB0806		* �a�f�X�N���[�����W�X�^�P�@�x
BG_CNTRL	equ	$EB0808		* �a�f�R���g���[��

	.list
