#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Elevator.h"
#include "FloorButtonPanel.h"

MainWindow* MainWindow::singleton = nullptr;

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->elevatorStatusFrame->setHidden(1);
    ui->currentElevatorFrame->setHidden(1);
    ui->controlPanelFrame->setHidden(1);
    ui->outageButton->setDisabled(1);
    ui->fireAlarmButton->setDisabled(1);

    connect(ui->rebuildButton, SIGNAL(released()), this, SLOT(startSimulationHandler()));
    connect(ui->outageButton, SIGNAL(clicked()), this, SLOT(outageButtonHandler()));
    connect(ui->fireAlarmButton, SIGNAL(clicked()), this, SLOT(fireAlarmButtonHandler()));

    connect(ui->fromFloorSpin, SIGNAL(valueChanged(int)), this, SLOT(fromFloorSpinHandler()));
    connect(ui->elevatorSelectionSpin, SIGNAL(valueChanged(int)), this, SLOT(elevatorSelectionSpinHandler()));
    connect(ui->directionUp, SIGNAL(clicked()), this, SLOT(upButtonHandler()));
    connect(ui->directionDown, SIGNAL(clicked()), this, SLOT(downButtonHandler()));
    connect(ui->passengerEnterButton, SIGNAL(clicked()), this, SLOT(passengerEnterHandler()));
    connect(ui->passengerExitButton, SIGNAL(clicked()), this, SLOT(passengerExitHandler()));
    connect(ui->doorOpenButton, SIGNAL(pressed()), this, SLOT(doorOpenPressHandler()));
    connect(ui->doorOpenButton, SIGNAL(released()), this, SLOT(doorOpenReleaseHandler()));
    connect(ui->doorCloseButton, SIGNAL(clicked()), this, SLOT(doorCloseHandler()));
    connect(ui->numFloorsSpin, SIGNAL(valueChanged(int)), this, SLOT(numFloorsSpinHandler()));
    connect(ui->localFireAlarmButton, SIGNAL(clicked()), this, SLOT(localFireAlarmHandler()));

    coordinator = nullptr;
    singleton = this;
    ui->eventLog->appendPlainText(QString::fromStdString("Please choose your desired number of elevators (<=4) and floors (<=8) on the left to start the simulation."));
}

MainWindow::~MainWindow() {
    delete ui;
    delete coordinator; // This will delete all the other objects
}

void MainWindow::addEventLog(const std::string &event) {
    ui->eventLog->appendPlainText(QString::fromStdString(event));
}

void MainWindow::startSimulationHandler() {
    numFloors = ui->numFloorsSpin->value();
    numElevators = ui->numElevatorsSpin->value();
    safeFloor = ui->safeFloorSpin->value();
    ui->elevatorStatusFrame->setHidden(false);
    ui->currentElevatorFrame->setHidden(false);
    ui->controlPanelFrame->setHidden(false);
    ui->outageButton->setEnabled(1);
    ui->fireAlarmButton->setEnabled(1);

    // Create a new instance of Coordinator
    coordinator = Coordinator::newInstance();

    QGridLayout *controlPanelLayout = ui->controlPanelLayout;
    // Clear previous buttons from the layout
    QLayoutItem *item;
    while ((item = controlPanelLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    int row = 0;
    int col = 0;

    // Create buttons for each floor
    for (int i = 0; i < numFloors; ++i) {
        QPushButton *button = new QPushButton(QString::number(i), this);
        button->setProperty("floor", i);
        connect(button, &QPushButton::released, this, &MainWindow::targetFloorButtonHandler);
        controlPanelLayout->addWidget(button, row, col, 1, 1);

        col += 1;
        if (col == 3) {
            row += 1;
            col = 0;
        }
    }
    QPushButton *helpButton = new QPushButton("Help", this);
    connect(helpButton, &QPushButton::released, this, &MainWindow::helpButtonHandler);
    controlPanelLayout->addWidget(helpButton, row, col, 1, 1);

    // Create elevator status display
    clearLayout(ui->elevatorStatusLayout);
    QVBoxLayout *elevatorStatusLayout = ui->elevatorStatusLayout;
    for (int i = 0; i < numElevators; ++i) {
        QHBoxLayout *hLayout = new QHBoxLayout();

        QFont labelFont;
        labelFont.setPointSize(12);
        labelFont.setBold(true);

        // Elevator number label
        QLabel *label = new QLabel(QString("E%1:").arg(i));
        label->setObjectName(QString("elevatorLabel_%1").arg(i));
        label->setFont(labelFont);
        hLayout->addWidget(label);

        // Elevator floor indicator (LCD)
        QLCDNumber *lcdNumber = new QLCDNumber(3);
        lcdNumber->setObjectName(QString("elevatorLCD_%1").arg(i));
        lcdNumber->setSegmentStyle(QLCDNumber::Flat);
        hLayout->addWidget(lcdNumber);

        // Elevator status label
        QLabel *statusLabel = new QLabel("IDLE"); // Default status is "IDLE"
        statusLabel->setObjectName(QString("elevatorStatusLabel_%1").arg(i));
        statusLabel->setFont(labelFont);
        statusLabel->setFixedWidth(100);
        hLayout->addWidget(statusLabel);

        elevatorStatusLayout->addLayout(hLayout);
    }

    ui->fromFloorSpin->setRange(0, numFloors - 1);
    ui->fromFloorSpin->setValue(0);
    ui->directionDown->setDisabled(1);
    ui->elevatorSelectionSpin->setRange(0, numElevators - 1);
    ui->eventLog->appendPlainText("System: Elevator Simulation Started");
}

void MainWindow::clearLayout(QLayout *layout) {
    // Delete all widgets and sublayouts so we can create new ones
    while (QLayoutItem *item = layout->takeAt(0)) {
        if (QWidget *widget = item->widget()) delete widget;
        else if (QLayout *childLayout = item->layout()) clearLayout(childLayout);
        delete item;
    }
}

void MainWindow::numFloorsSpinHandler() {
    // Update safe floor spin box to reflect the new number of floors
    ui->safeFloorSpin->setRange(0, ui->numFloorsSpin->value() - 1);
    if (ui->safeFloorSpin->value() > ui->numFloorsSpin->value() - 1) {
        ui->safeFloorSpin->setValue(ui->numFloorsSpin->value() - 1);
    }
}

void MainWindow::outageButtonHandler() {
    ui->eventLog->appendPlainText("System: Power Outage Detected");
    ui->currentElevatorFrame->setHidden(1);
    ui->controlPanelFrame->setHidden(1);
    Coordinator::getInstance()->powerOut();
}

void MainWindow::fireAlarmButtonHandler() {
    ui->eventLog->appendPlainText("System: Fire Alarm Detected");
    ui->currentElevatorFrame->setHidden(1);
    ui->controlPanelFrame->setHidden(1);
    Coordinator::getInstance()->fireAlarm();
}

void MainWindow::localFireAlarmHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    ui->eventLog->appendPlainText("System: Fire Alarm Detected in Elevator " + QString::number(elevatorID) + "!");
    Coordinator::getInstance()->getElevator(elevatorID)->handleFireAlarm();
}

void MainWindow::fromFloorSpinHandler() {
    if (ui->fromFloorSpin->value() == 0) {
        ui->directionUp->setEnabled(1);
        ui->directionDown->setDisabled(1);
    } else if (ui->fromFloorSpin->value() == numFloors - 1) {
        ui->directionUp->setDisabled(1);
        ui->directionDown->setEnabled(1);
    } else {
        ui->directionUp->setEnabled(1);
        ui->directionDown->setEnabled(1);
    }
}

void MainWindow::elevatorSelectionSpinHandler() {
    Elevator *elevator = coordinator->getElevator(ui->elevatorSelectionSpin->value());
    if (!elevator->isIdle()) {
        addEventLog("Elevator " + std::to_string(ui->elevatorSelectionSpin->value()) + ": Currently moving. Please select again later.");
    }
}

void MainWindow::upButtonHandler() {
    coordinator->getFloorButtonPanel(ui->fromFloorSpin->value())->pressButton(UP);
}

void MainWindow::downButtonHandler() {
    coordinator->getFloorButtonPanel(ui->fromFloorSpin->value())->pressButton(DOWN);
}


void MainWindow::passengerEnterHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    // Only allow passengers to enter if the door is open
    if (coordinator->getElevator(elevatorID)->isDoorOpen()) {
        coordinator->getElevator(elevatorID)->passengerEnter();
    } else {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Door is not open!");
    }
}

void MainWindow::passengerExitHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    // Only allow passengers to exit if the door is open
    if (coordinator->getElevator(elevatorID)->isDoorOpen()) {
        coordinator->getElevator(elevatorID)->passengerExit();
    } else {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Door is not open!");
    }
}

void MainWindow::updateDisplay(int elevatorID, int floor, int direction) {
    QLCDNumber *lcdNumber = findChild<QLCDNumber *>(QString("elevatorLCD_%1").arg(elevatorID));
    lcdNumber->display(floor);
    QLabel *statusLabel = findChild<QLabel *>(QString("elevatorStatusLabel_%1").arg(elevatorID));
    if (direction == IDLE) {
        statusLabel->setText("IDLE");
    } else if (direction == UP) {
        statusLabel->setText("UP");
    } else if (direction == DOWN) {
        statusLabel->setText("DOWN");
    }
}

void MainWindow::targetFloorButtonHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    // Should not be able to press the button if elevator is empty
    if (coordinator->getElevator(elevatorID)->isEmpty()) {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Someone must be inside to choose a floor.");
        return;
    }
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int floor = button->property("floor").toInt();
    coordinator->getElevator(elevatorID)->addTargetFloor(floor);
}

void MainWindow::helpButtonHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    // Should not be able to press the button if elevator is empty
    if (coordinator->getElevator(elevatorID)->isEmpty()) {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Someone must be inside to press the help button.");
        return;
    }
    coordinator->getElevator(elevatorID)->handleHelpSignal();
}

void MainWindow::doorOpenPressHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    if (!coordinator->getElevator(elevatorID)->isIdle()) {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Currently moving. Please wait until it stops.");
        return;
    }
    // The empty check is not included for testing purposes
    coordinator->getElevator(elevatorID)->getControlPanel()->pressOpenButton();
}

void MainWindow::doorOpenReleaseHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    if (!coordinator->getElevator(elevatorID)->isDoorOpen()) { // If the press did nothing, the release should do nothing
        return;
    }
    // The empty check is not included for testing purposes
    coordinator->getElevator(elevatorID)->getControlPanel()->releaseOpenButton();
}

void MainWindow::doorCloseHandler() {
    int elevatorID = ui->elevatorSelectionSpin->value();
    if (!coordinator->getElevator(elevatorID)->isDoorOpen()) {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Door is already closed!");
    }
    // Should not be able to press the button if elevator is empty
    if (coordinator->getElevator(elevatorID)->isEmpty()) {
        ui->eventLog->appendPlainText("Elevator " + QString::number(elevatorID) + ": Someone must be inside to close the door.");
        return;
    }
    coordinator->getElevator(elevatorID)->getControlPanel()->pressCloseButton();
}
