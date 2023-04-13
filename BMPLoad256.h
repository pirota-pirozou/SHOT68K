// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// Written by Pirota Pirozou, 2023/04
// BMPLoad256.h
//
// ���̃R�[�h�́A�Q�T�U�F�a�l�o���[�_�[�̃w�b�_�t�@�C�� �ł���B
// xdev68k���œ��삷��悤�ɍ���Ă���B
//
// BMPLoad256.c / BMPLoad256.h �́A���R�ɉ��ς��Ď����̃v���O�����ɑg�ݍ��ނ��Ƃ��\�B
// ���쌠�\�L���K�v�Ȃ��B
// �������A���̃R�[�h���g�p�������Ƃɂ�邢���Ȃ鑹�Q�ɂ��Ă��A��҂͈�؂̐ӔC�𕉂�Ȃ��B

#pragma once
#ifndef ____BMPLOAD256_H____
#define ____BMPLOAD256_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short WORD_t;
typedef unsigned int DWORD_t;
typedef unsigned char BYTE_t;

/// @brief �r�b�g�}�b�v�t�@�C���w�b�_�[
typedef struct {
    WORD_t bfType;
    DWORD_t bfSize;
    WORD_t bfReserved1;
    WORD_t bfReserved2;
    DWORD_t bfOffbits;
} BITMAPFILEHEADER;

/// @brief �r�b�g�}�b�v���w�b�_�[
typedef struct {
    DWORD_t biSize;
    DWORD_t biWidth;
    DWORD_t biHeight;
    WORD_t biPlanes;
    WORD_t biBitcount;
    DWORD_t biCompression;
    DWORD_t biSizeimage;
    DWORD_t biXPelsPerMeter;
    DWORD_t biYPelsPerMeter;
    DWORD_t biClrUsed;
    DWORD_t biClrImportant;
} BITMAPINFOHEADER;

/// @brief �J���[�p���b�g
typedef struct {
    BYTE_t rgbBlue;
    BYTE_t rgbGreen;
    BYTE_t rgbRed;
    BYTE_t rgbReserved;
} RGBQUAD;

/// @brief �Q�T�U�F�r�b�g�}�b�v�t�@�C��
typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD palette[256];
    BYTE_t imgdata[4];
} BMPFILE256, *pBMPFILE256;

#ifdef __cplusplus
extern "C" {
#endif

// �v���g�^�C�v�錾
pBMPFILE256 LoadBMP256(const char *fname);
int PutBMPMemory256(pBMPFILE256 pBMP);
int PutBMPFile256(const char *fname);

#ifdef __cplusplus
}
#endif

#endif // __BMPLOAD256_H__
