#include "pch.h"
#include "Event.h"

CEvent::CEvent(BOOL bManualReset, BOOL bInitialState)
	: m_handle(NULL)
{
    LPSECURITY_ATTRIBUTES lpEventAttributes = NULL;
    LPCTSTR lpName = NULL;
    m_handle = CreateEvent(
        lpEventAttributes
        // このパラメーターがTRUEの場合、この関数は手動リセットイベントオブジェクトを作成します。
        // そのためには、ResetEvent関数を使用してイベント状態をノンシグナルに設定する必要があります。
        // このパラメーターがFALSEの場合、関数は自動リセットイベントオブジェクトを作成し、
        // 1つの待機中のスレッドが解放された後、システムによってイベント状態が自動的にノンシグナルにリセットされます。
        , bManualReset
        // このパラメーターが TRUE の場合、イベント オブジェクトの初期状態が通知されます。
        // それ以外の場合は、割り当てされません。
        , bInitialState
        // イベント オブジェクトの名前です。
        // 名前は MAX_PATH 文字に制限されています。
        // 名前の比較では大文字と小文字が区別されます。
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
