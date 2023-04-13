// -*-  tab-width : 4 ; mode : C ; encode : ShiftJIS -*-
// Written by Pirota Pirozou, 2023/04
// BMPLoad256.c
//
// ���̃R�[�h�́A�Q�T�U�F�a�l�o���[�_�[�̃\�[�X�R�[�h �ł���B
// xdev68k���œ��삷��悤�ɍ���Ă���B
//
// BMPLoad256.c / BMPLoad256.h �́A���R�ɉ��ς��Ď����̃v���O�����ɑg�ݍ��ނ��Ƃ��\�B
// ���쌠�\�L���K�v�Ȃ��B
// �������A���̃R�[�h���g�p�������Ƃɂ�邢���Ȃ鑹�Q�ɂ��Ă��A��҂͈�؂̐ӔC�𕉂�Ȃ��B

#include <iocslib.h>
#include <doslib.h>
#include "BMPLoad256.h"

// xdev68k����malloc()/free()���g���Ƃ��܂����삵�Ȃ��̂ŁA
// ����ɁADOS�R�[����MALLOC()/MFREE()���g���֐����g�p���܂��B
#define dos_malloc(siz)    	MALLOC((siz))
#define dos_free(ptr)   	MFREE((int)(ptr))

// word�l�̃G���f�B�A�����X���b�v���܂�
static inline WORD_t swap_endian_word(WORD_t value)
 {
    return (value << 8) | (value >> 8);
}

// dword�l�̃G���f�B�A�����X���b�v���܂�
static inline DWORD_t swap_endian_dword(DWORD_t value)
 {
    return (value << 24) |
           ((value << 8) & 0x00FF0000) |
           ((value >> 8) & 0x0000FF00) |
           (value >> 24);
}

/// @brief BMP256�F�摜�̓ǂݍ���
/// @param fname �t�@�C����
/// @return ����
/// @retval 0 ���s
/// @retval 1 ����
int LoadBMP256(const char *fname)
{
	FILE *fp = fopen(fname, "rb");
    if (fp == NULL)
	{
        fprintf(stderr, "�t�@�C�����J���܂���ł���\n");
        return 0;
    }

    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    RGBQUAD palette[256];

    fread(&fileHeader, sizeof(fileHeader), 1, fp);
    fread(&infoHeader, sizeof(infoHeader), 1, fp);
    fread(&palette, sizeof(RGBQUAD), 256, fp);

	// �G���f�B�A���̃X���b�v�����܂�
	// �w�b�_�[�̏��́A���g���G���f�B�A���ŋL�q����Ă��܂�
	fileHeader.bfType = swap_endian_word(fileHeader.bfType);
	fileHeader.bfSize = swap_endian_dword(fileHeader.bfSize);
	fileHeader.bfReserved1 = swap_endian_word(fileHeader.bfReserved1);
	fileHeader.bfReserved2 = swap_endian_word(fileHeader.bfReserved2);
	fileHeader.bfOffbits = swap_endian_dword(fileHeader.bfOffbits);

	infoHeader.biSize = swap_endian_dword(infoHeader.biSize);
	infoHeader.biWidth = swap_endian_dword(infoHeader.biWidth);
	infoHeader.biHeight = swap_endian_dword(infoHeader.biHeight);
	infoHeader.biPlanes = swap_endian_word(infoHeader.biPlanes);
	infoHeader.biBitcount = swap_endian_word(infoHeader.biBitcount);
	infoHeader.biCompression = swap_endian_dword(infoHeader.biCompression);
	infoHeader.biSizeimage = swap_endian_dword(infoHeader.biSizeimage);
	infoHeader.biXPelsPerMeter = swap_endian_dword(infoHeader.biXPelsPerMeter);
	infoHeader.biYPelsPerMeter = swap_endian_dword(infoHeader.biYPelsPerMeter);
	infoHeader.biClrUsed = swap_endian_dword(infoHeader.biClrUsed);
	infoHeader.biClrImportant = swap_endian_dword(infoHeader.biClrImportant);

	// �f�o�b�O�p�F�w�b�_�[�̏���\�����܂�
//	printf("bfType = %X\n", fileHeader.bfType);
//	printf("biBitCount = %d\n", infoHeader.biBitcount);
//	printf("biWidth = %d\n", infoHeader.biWidth);
//	printf("biHeight = %d\n", infoHeader.biHeight);
//	INKEY();

    if (fileHeader.bfType != 0x4D42 // 'BM'
		 		|| infoHeader.biBitcount != 8)
	{
        fprintf(stderr, "�T�|�[�g����Ă��Ȃ��t�H�[�}�b�g�ł�\n");
        fclose(fp);
        return 0;
    }

    int imageSize = infoHeader.biWidth * infoHeader.biHeight;
    BYTE_t *imageData = (BYTE_t *)dos_malloc(imageSize);
	if (imageData < 0)
	{
        fprintf(stderr, "���������m�ۂł��܂���B\n");
		dos_free(imageData);
        fclose(fp);
        return 1;
	}
    fread(imageData, 1, imageSize, fp);
    fclose(fp);

	// pallete ���g�p���āA�K�v�ȑ�����s���܂�
	WORD_t *pal = (WORD_t *)0xE82000;
	for (int i = 0; i < 256; i++)
	{
		BYTE_t r = palette[i].rgbRed >> 3;
		BYTE_t g = palette[i].rgbGreen >> 3;
		BYTE_t b = palette[i].rgbBlue >> 3;
		WORD_t col = (b << 1) | (r << 6) | (g << 11);
		pal[i] = col;
	}

	// �摜�f�[�^��VRAM�ɓ]�����܂�
	WORD_t *vram = (WORD_t *)0xC00000;
	int width = infoHeader.biWidth;
	int height = infoHeader.biHeight;

	for (int y = 0; y < height; ++y)
	{
		WORD_t* vramLine = &vram[y * 512];
		const BYTE_t *imageLine = &imageData[(height - y - 1) * width];

		for (int x = 0; x < width; ++x)
		{
			vramLine[x] = imageLine[x];
		}
	}

	// ��������������܂�
    dos_free(imageData);
    return 1;
}