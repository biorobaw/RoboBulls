#ifndef GUICOMM_H
#define GUICOMM_H

#include <QThread>
#include <QTimer>


class GuiComm : public QThread
{
    Q_OBJECT

public:
    explicit GuiComm(int interval, QObject* parent = 0);
    ~GuiComm();

signals:
    void valueChanged(int);

private slots:
    void count(void);

protected:
    void run(void);

private:
    int i;
    int inc;
    int intvl;

    QTimer* timer;
};

#endif // GUICOMM_H
