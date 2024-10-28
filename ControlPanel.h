#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

class Elevator;

class ControlPanel {
public:
    explicit ControlPanel(Elevator* elevator);
    void chooseDestination(int floor) const;
    void pressHelpButton() const;
    void pressOpenButton() const;
    void releaseOpenButton() const;
    void pressCloseButton() const;

private:
    Elevator* elevator;
};

#endif //CONTROLPANEL_H
