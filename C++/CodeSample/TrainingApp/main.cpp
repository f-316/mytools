#include <iostream>
#include "TrainingApp.h"

int main()
{
    CTrainingApp app;
    if (!app.Start())
    {
        return 0;
    }
    app.WaitEnd();
    return 1;
}
