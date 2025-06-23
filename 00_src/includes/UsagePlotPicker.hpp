#ifndef USAGEPLOTPICKER_H
#define USAGEPLOTPICKER_H

#include <qwt_plot_picker.h>
#include <qwt_text.h>
#include <QPointF>

class QwtPlot;

class UsagePlotPicker : public QwtPlotPicker
{
public:
    using QwtPlotPicker::trackerText;
    explicit UsagePlotPicker(int baseSeconds, QwtPlot *plot);

protected:
    QwtText trackerTextF(const QPointF &pos) const override;

private:
    int m_baseSeconds;
};

#endif // USAGEPLOTPICKER_H
