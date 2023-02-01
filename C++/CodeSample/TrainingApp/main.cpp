#include <iostream>
#include "TrainingApp.h"

class Root
{
public:
    Root()
    {
        m_index = 0;
        m_size = 2;
        m_pRoot = new int[m_size];
        ::memset(m_pRoot, INT32_MAX, m_size * sizeof(int));
    }

    ~Root()
    {
        delete[] m_pRoot;
    }

    void Add(int num)
    {
        // 拡張
        if (m_size <= m_index)
        {
            int newSize = m_size * 2;
            int* pData = new int[newSize];
            for (int index = 0; index < newSize; index++)
            {
                pData[index] = INT32_MAX;
            }
            ::memcpy_s(pData, newSize * sizeof(int), m_pRoot, m_size * sizeof(int));
            delete[] m_pRoot;
            m_pRoot = pData;
            printf("Extend size:%d->%d\n", m_size, newSize);
            m_size = newSize;
        }

        m_pRoot[m_index++] = num;
    }

    int GetSize(void)
    {
        return m_size;
    }

    int Get(int index)
    {
        if (m_size <= index) return INT32_MAX;
        return m_pRoot[index];
    }

private:
    int* m_pRoot;
    int m_size;
    int m_index;
};

int main()
{
    Root rt;

    for (int index = 0; index < 20; index++)
    {
        rt.Add(index);
    }

    for (int index = 0; index < rt.GetSize() + 2; index++)
    {
        printf("[%d]%d\n", index, rt.Get(index));
    }

    return 1;
}
