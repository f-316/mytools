#include "pch.h"
#include "Event.h"

CEvent::CEvent(BOOL bManualReset, BOOL bInitialState)
	: m_handle(NULL)
{
    LPSECURITY_ATTRIBUTES lpEventAttributes = NULL;
    LPCTSTR lpName = NULL;
    m_handle = CreateEvent(
        lpEventAttributes
        // ���̃p�����[�^�[��TRUE�̏ꍇ�A���̊֐��͎蓮���Z�b�g�C�x���g�I�u�W�F�N�g���쐬���܂��B
        // ���̂��߂ɂ́AResetEvent�֐����g�p���ăC�x���g��Ԃ��m���V�O�i���ɐݒ肷��K�v������܂��B
        // ���̃p�����[�^�[��FALSE�̏ꍇ�A�֐��͎������Z�b�g�C�x���g�I�u�W�F�N�g���쐬���A
        // 1�̑ҋ@���̃X���b�h��������ꂽ��A�V�X�e���ɂ���ăC�x���g��Ԃ������I�Ƀm���V�O�i���Ƀ��Z�b�g����܂��B
        , bManualReset
        // ���̃p�����[�^�[�� TRUE �̏ꍇ�A�C�x���g �I�u�W�F�N�g�̏�����Ԃ��ʒm����܂��B
        // ����ȊO�̏ꍇ�́A���蓖�Ă���܂���B
        , bInitialState
        // �C�x���g �I�u�W�F�N�g�̖��O�ł��B
        // ���O�� MAX_PATH �����ɐ�������Ă��܂��B
        // ���O�̔�r�ł͑啶���Ə���������ʂ���܂��B
        , lpName);

    if (m_handle == NULL)
    {
        return;
    }
}

CEvent::~CEvent()
{
}

void CEvent::Set(void)
{
    ::SetEvent(m_handle);
}

void CEvent::Reset(void)
{
    ::ResetEvent(m_handle);
}
