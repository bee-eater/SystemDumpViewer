#include <./includes/clickLabel.h>
#include "mainwindow.h"

/**
 * @brief Constructor of the clickLabel
 * @param i Index of the label
 * @param MainWindow the QMainWindow which has the on_uiLabelClicked slot.
 * @param parent QWidget
 *
 * In the constructor the index is set to the private variable and the click signal is connected to the slot of the main window.
 * So if the label is clicked, the slot gets the index of the clicked label and you can identify it.
 */
clickLabel::clickLabel(int i, MainWindow* const MainWindow, QWidget * parent)
:QLabel(parent)
{
    this->index = i;
    connect(this, SIGNAL(clicked(int)),MainWindow,SLOT(on_uiLabelClicked(int)));
}

void clickLabel::mouseReleaseEvent ( QMouseEvent * event )
{
    UNUSED(event);
    emit clicked(this->index);
}
