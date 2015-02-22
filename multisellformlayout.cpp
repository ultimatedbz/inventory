#include "multisellformlayout.h"
#include <QDebug>

MultiSellFormLayout::MultiSellFormLayout(int queryNum, QDialog* d, Inventory* i, QFont f):
  mFormArray(new QFormLayout[queryNum]),
  dialog(d),
  mInventory(i),
  font(f)
{
  /* Loop and keep making new dropboxes and stuff for each vegetable */
  for( int i = 0; i < queryNum; i++){

    QFrame* line = new QFrame();
    line->setGeometry(QRect(/* ... */));
    line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
    line->setFrameShadow(QFrame::Sunken);
    mFormArray[i].addRow(line);

    /* Vegetable */

    QComboBox* vegeDrop = new QComboBox(dialog);
    vegeDrop->addItem(mInventory->getVegetableByIndex(i)->getVegetablename().c_str());

    vegeDrop->setFont(font);
    mFormArray[i].addRow("Vegetable", vegeDrop);

    /* Amount */
    QLineEdit *amount = new QLineEdit(dialog);

    QString label5 = QString("Amount");
    mFormArray[i].addRow(label5, amount);

    /* In Stock */
    QComboBox* remainingDrop = new QComboBox(dialog);
    remainingDrop->addItem(mInventory->getVegetableByIndex(i)->formatRemaining(i).c_str());
    remainingDrop->setFont(font);

    QToolButton *tb = new QToolButton();
    tb->setText("+");
    QObject::connect(tb, SIGNAL(clicked()),this, SLOT(addRemaining()));

    QHBoxLayout* hLay = new QHBoxLayout();
    hLay->addWidget(remainingDrop);
    hLay->addWidget(tb);

    mFormArray[i].addRow("In Stock", hLay);

    /* Price */
    QLineEdit *price = new QLineEdit(dialog);

    QString label6 = QString("Price");
    mFormArray[i].addRow(label6, price);

  }
}

/*TODO
  Update fields when vegetable is changed, when in stock is changed,
        when add button is pressed
*/

MultiSellFormLayout::~MultiSellFormLayout()
{

}

QFormLayout* MultiSellFormLayout::getMSElement(int i){
  return &(mFormArray[i]);
}

void MultiSellFormLayout::addRemaining(){
qDebug()<<"ran";
}


