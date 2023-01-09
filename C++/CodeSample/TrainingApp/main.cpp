#include <iostream>
#include "TrainingApp.h"

int main()
{
    //CFileTool file;
    CFileTool file;
    file.Sample();

    return 1;

    CTrainingApp app;
    if (!app.Start())
    {
        return 0;
    }
    app.WaitEnd();
    return 1;
}
