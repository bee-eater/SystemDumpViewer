#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include "mainwindow.h"
#include <QMouseEvent>
#include <QString>
#include <QSignalMapper>
#include <QLabel>
#include <QDebug>

/**
 * @brief The clickLabel class is derived from QLabel and emmits a signal
 *          containing its own signal.
 *
 * The clickLabel has to be initialized with a unique index if you want to identify the clicked label.
 * In this case it is used on the startscreen to jump to the clicked section.
 */
class clickLabel : public QLabel
{
Q_OBJECT
public:
    clickLabel(int i, MainWindow* const MainWindow, QWidget * parent = 0);
    ~clickLabel(){}

signals:
    void clicked(int);

private:
    int index;

protected:
    void mouseReleaseEvent ( QMouseEvent * event ) ;

};

#endif // CLICKLABEL_H
