#include <includes/UsagePlotPicker.hpp>

#include <QTime>
#include <QPointF>

#include <qwt_plot_picker.h>
#include <qwt_text.h>
#include <qwt_plot.h>

UsagePlotPicker::UsagePlotPicker(int baseSeconds, QwtPlot *plot)
    : QwtPlotPicker(plot->canvas())
    , m_baseSeconds(baseSeconds)
{
    // Initialize picker with plot axes and style
    setTrackerMode(QwtPicker::AlwaysOn);
    setRubberBand(QwtPicker::EllipseRubberBand);
    setAxes(QwtPlot::xBottom, QwtPlot::yLeft);
}

QwtText UsagePlotPicker::trackerTextF(const QPointF &pos) const
{
    // Compute totalSeconds as you already do:
    int totalSeconds = m_baseSeconds - (pos.x()/200.0 * m_baseSeconds);
    if (totalSeconds < 0) totalSeconds = 0;

    // Extract days/hours/minutes/seconds
    int days     = totalSeconds / 86400;          // 86400 = 24*3600
    int rem1     = totalSeconds % 86400;
    int hours    = rem1 / 3600;
    int rem2     = rem1 % 3600;
    int minutes  = rem2 / 60;
    int seconds  = rem2 % 60;

    // Build the time string, including only the non-zero highest unit
    QString timeStr;
    if (days > 0) {
        timeStr = QString::asprintf("%dd%02dh%02dm%02ds",
                                    days, hours, minutes, seconds);
    }
    else if (hours > 0) {
        timeStr = QString::asprintf("%02dh%02dm%02ds",
                                    hours, minutes, seconds);
    }
    else if (minutes > 0) {
        timeStr = QString::asprintf("%02dm%02ds",
                                    minutes, seconds);
    }
    else {
        timeStr = QString::asprintf("%02ds", seconds);
    }

    // Format y value with 2 decimals
    QString valueStr = QString::number(pos.y(), 'f', 2);

    QwtText text;
    text.setText(
        QString("<div align=\"center\">"
                "Time: -%1<br/>"
                "Value: %2"
                "</div>")
            .arg(timeStr, valueStr),
        QwtText::RichText
        );
    return text;
}
