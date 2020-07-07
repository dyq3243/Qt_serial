#include "text.h"
#include <QTextEdit>

text::text(QWidget *parent) : QWidget(parent)
{
    QTextEdit *text = new QTextEdit(this);
    text->setGeometry(20,0,220,250);
}
