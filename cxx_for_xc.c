#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "cxx_for_xc.h"


/* �X�^�e�B�b�N�R���X�g���N�^�̃��X�g */
typedef void (*ctor_t)();
extern ctor_t __CTOR_LIST__[];

/* �X�^�e�B�b�N�R���X�g���N�^�����s���� */
void execute_static_ctors() {
	/*
		XC �� startup �� C++ �Ή����Ă��Ȃ��B
		�X�^�e�B�b�N�R���X�g���N�^�͖����I�Ɏ��s����K�v������B
		�X�^�e�B�b�N�R���X�g���N�^�� __CTOR_LIST__ �Ƃ������x�����Œ񋟂����B
		__CTOR_LIST__[0] �ɂ� -1 ���i�[����Ă���̂ŁA�����ǂݔ�΂��A
		NULL ���o������܂ŁA�R���X�g���N�^�̃|�C���^�Ƃ݂Ȃ����s����B
	*/
	for (int i = 1;; i++) {
		ctor_t ctor = __CTOR_LIST__[i];
		if (ctor == NULL) break;
		ctor();
	}
}

/* �A�v���P�[�V�����I�����Ɏ��s����֐���o�^���� */
void __cxa_atexit(void (*p)()) {
	atexit(p);
}

/* �����N�G���[����̂��ߒ�`���K�v */
void *__dso_handle = 0;

