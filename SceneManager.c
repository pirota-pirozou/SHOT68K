// Path: SceneManager.c
// ���̃R�[�h�́A�T���v���v���O�����̃V�[���}�l�[�W���[���� SceneManager.c �ł���B
// �V�[���̍X�V�A�`��A�ؑւ̊Ǘ����s��
//
#include "SceneManager.h"

static FuncPtr0 SceneManager_Func_Init = NULL;
static FuncPtr0 SceneManager_Func_Update = NULL;
static FuncPtr0 SceneManager_Func_Draw = NULL;
static FuncPtr0 SceneManager_Func_Clear = NULL;
static FuncPtr0 SceneManager_Func_VSync = NULL;
static BOOL isSceneChanged = FALSE;
static BOOL isFirst = TRUE;
static int sceneID = -1;
static int nextSceneID = -1;
static pSSceneWork scenes = NULL;

/////////////////////////////////
// �V�[���}�l�[�W���[���̂̏�����
/////////////////////////////////
void SceneManager_Init()
{
    SceneManager_Func_Init = NULL;
    SceneManager_Func_Update = NULL;
    SceneManager_Func_Draw = NULL;
    SceneManager_Func_Clear = NULL;
    SceneManager_Func_VSync = NULL;
    isSceneChanged = FALSE;
    isFirst = TRUE;
    sceneID = nextSceneID = -1;
}

/////////////////////////////////
// �V�[��ID�̊֐��ɐ؂�ւ���
/////////////////////////////////
static void SetChangeFunc(int sid)
{
    pSSceneWork scene = &scenes[sid];
    SceneManager_Func_Init = scene->Init;
    SceneManager_Func_Update = scene->Update;
    SceneManager_Func_Draw = scene->Draw;
    SceneManager_Func_Clear = scene->Clear;
    SceneManager_Func_VSync = scene->VSync;

}

/////////////////////////////////
// �V�[���}�l�[�W���[�F�V�[���ؑ�
/////////////////////////////////
void SceneManager_ChangeScene(int nextscene)
{
    if (sceneID != nextscene)
    {
        nextSceneID = nextscene;
        // �����Change��������AInit���Ă�
        if (isFirst)
        {
            isFirst = FALSE;
            sceneID = nextSceneID;
            SetChangeFunc(sceneID);
            if (SceneManager_Func_Init != NULL)
            {
                SceneManager_Func_Init();
            }
        }
        else
        {
            isSceneChanged = TRUE;
        }

    }
}

/////////////////////////////////
// �V�[���}�l�[�W���[�F�V�[���o�^
/////////////////////////////////
void SceneManager_Regist(pSSceneWork scene)
{
    scenes = scene;
}

/////////////////////////////////
// �V�[���F������
/////////////////////////////////
void Scene_Init()
{
    if (SceneManager_Func_Init != NULL)
    {
        SceneManager_Func_Init();
    }
}

/////////////////////////////////
// �V�[���F�X�V����
/////////////////////////////////
void Scene_Update()
{
    if (SceneManager_Func_Update != NULL)
    {
        SceneManager_Func_Update();
    }
}

/////////////////////////////////
// �V�[���F�`�揈��
/////////////////////////////////
void Scene_Draw()
{
    if (SceneManager_Func_Draw != NULL)
    {
        SceneManager_Func_Draw();
    }
    // �V�[���ؑւ���������A�V�[����؂�ւ���
    if (isSceneChanged)
    {
        isSceneChanged = FALSE;
        Scene_Clear();      // ���݂̃V�[���̃N���A����

        // ���̃V�[���̊֐��|�C���^���Z�b�g
        sceneID = nextSceneID;
        SetChangeFunc(sceneID);

        // ���̃V�[���̏���������
        Scene_Init();
    }

}

/////////////////////////////////
// �V�[���F�ؑ֏���
/////////////////////////////////
void Scene_Clear()
{
    if (SceneManager_Func_Clear != NULL)
    {
        SceneManager_Func_Clear();
    }
}

/////////////////////////////////
// �V�[���FVSync����
/////////////////////////////////
void Scene_VSync()
{
    if (SceneManager_Func_VSync != NULL)
    {
        SceneManager_Func_VSync();
    }
}
