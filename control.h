#ifndef CONTROL_H
#define CONTROL_H

#endif // CONTROL_H

class Control
{
private:
    const float MAX_VOLTAGE = 4;
    const float MIN_VOLTAGE = -4;
    const int MAX_LEVEL = 30;
    const int MIN_LEVEL = 0;

public:
    Control()
    {

    }

    float MainWindow::voltageControl(float voltage);
    int MainWindow::levelControl(int level);

};
