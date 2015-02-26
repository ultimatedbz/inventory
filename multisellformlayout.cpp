#include "multisellformlayout.h"
#include <QDebug>

MultiSellFormLayout::MultiSellFormLayout(int index, QDialog* d, Inventory* i, QFont f):
  mForm(new QFormLayout()),
  dialog(d),
  mInventory(i),
  font(f),
  first(1)
{
  /* Loop and keep making new dropboxes and stuff for each vegetable */
  QFrame* line = new QFrame();
  line->setGeometry(QRect(/* ... */));
  line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
  line->setFrameShadow(QFrame::Sunken);
  mForm->addRow(line);

  /* Vegetable */

  QComboBox* vegeDrop = new QComboBox(dialog);
  vegeDrop->addItem(mInventory->getVegetableByIndex(index)->getVegetablename().c_str());

  vegeDrop->setFont(font);
  mForm->addRow("Vegetable", vegeDrop);

  /* Line */
  QFrame* line2 = new QFrame();
  line2->setGeometry(QRect(/* ... */));
  line2->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
  line2->setFrameShadow(QFrame::Sunken);
  mForm->addRow(line2);

  /* Amount */
  QLineEdit *amount = new QLineEdit(dialog);

  QString label5 = QString("Amounts");
  mForm->addRow(label5, amount);

  /* In Stock */
  QComboBox* remainingDrop = new QComboBox(dialog);
  remainingDrop->addItem(mInventory->getVegetableByIndex(index)->formatRemaining(index).c_str());
  remainingDrop->setFont(font);

  QToolButton *tb = new QToolButton();
  tb->setText("+");
  QObject::connect(tb, SIGNAL(clicked()),this, SLOT(addRemaining()));

  QHBoxLayout* hLay = new QHBoxLayout();
  hLay->addWidget(remainingDrop);
  hLay->addWidget(tb);

  mForm->addRow("In Stock", hLay);

  /* Price */
  QLineEdit *price = new QLineEdit(dialog);

  QString label6 = QString("Price");
  mForm->addRow(label6, price);


}

/*TODO
  Update fields when vegetable is changed, when in stock is changed,
        when add button is pressed
*/

MultiSellFormLayout::~MultiSellFormLayout()
{

}

QFormLayout* MultiSellFormLayout::getElement(){
  return mForm;
}

void MultiSellFormLayout::addRemaining(){
  QComboBox* remainingDrop = new QComboBox(dialog);
  remainingDrop->addItem(mInventory->getVegetableByIndex(0)->formatRemaining(0).c_str());
  remainingDrop->setFont(font);
  //don't need to deal with qhboxlayout, only the label. How do we change label?
  if(first){
    ((QLabel*) mForm->itemAt(6)->widget())->setText("");
    first = 0;
  }else
    ((QLabel*) mForm->itemAt(mForm->count() - 2)->widget())->setText("");

  mForm->insertRow(4,"In Stock", remainingDrop);
}


