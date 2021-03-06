#include "MultiBuyElement.h"
#include "translator.h"
#include <QDebug>

MultiBuyElement::MultiBuyElement(int index, MultiBuyController* c, Inventory* i,
                                 QFont f, set<int>* selectedVeges,
                                 vector<int>& vegeIndexToActual,
                                 Translator* mTranslator):
  mForm(new QFormLayout()),
  controller(c),
  mInventory(i),
  font(f),
  scrollEat(new ScrollEater(this))
{
  /* Line */
  QFrame* line = new QFrame();
  line->setGeometry(QRect(/* ... */));
  line->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
  line->setFrameShadow(QFrame::Sunken);
  mForm->addRow(line);

  /* Vegetable */
  vegeDrop = new QComboBox();
  vegeDrop->installEventFilter(scrollEat);
  connect(vegeDrop, SIGNAL(activated(int)), controller,SLOT(vegeDropChanged(int)));
  vegeDrop->addItem(mInventory->getVegetableByIndex(index)->getVegetablename().c_str());
  for( int i = 0; i < mInventory->getVegNum(); i++){
    if(selectedVeges->find(i) == selectedVeges->end() ){
      vegeDrop->addItem(mInventory->getVegetableByIndex(i)->getVegetablename().c_str());
      vegeIndexToActual.push_back(i);
    }
  }

  vegeDrop->setFont(font);
  QString vegeLabel = QString(mTranslator->translate("菜名子").c_str());
  QLabel* qVegeLabel = new QLabel(vegeLabel);
  qVegeLabel->setFont(font);
  mForm->addRow(qVegeLabel, vegeDrop);

  /* Amount */
  amount = new QLineEdit();
  QString label5 = QString(mTranslator->translate("買了多少?").c_str());
  QLabel* qlabel5 = new QLabel(label5);
  qlabel5->setFont(font);
  mForm->addRow(qlabel5, amount);

  /* Price */
  price = new QLineEdit();

  QString label6 = QString(mTranslator->translate("進價多少?").c_str());
  QLabel* qlabel6 = new QLabel(label6);
  qlabel6->setFont(font);
  mForm->addRow(qlabel6, price);

}

MultiBuyElement::~MultiBuyElement()
{
  QLayoutItem* item;

  int i = 0;
  while ( ( item = mForm->takeAt( 0 ) ) != NULL ) {

     if(i != 2 ){
        delete item->widget();
     }

     delete item;
     i++;
  }

  mForm->deleteLater();
  vegeDrop->deleteLater();
  delete scrollEat;
}

QFormLayout* MultiBuyElement::getElement(){
  return mForm;
}

QComboBox* MultiBuyElement:: getVegeDrop(){
  return vegeDrop;
}

void MultiBuyElement:: updateVegeDrops( set<int> selectedVeges,
                                         vector<int>& actualIndexes){
  /* Save first index */
  int firstIndex = actualIndexes[0];

  /* Clear actual indexes */
  actualIndexes.clear();
  vegeDrop->clear();

  /* Put the first index back in */
  actualIndexes.push_back(firstIndex);
  vegeDrop->addItem(mInventory->getVegetableByIndex(firstIndex)
                    ->getVegetablename().c_str());

  /* Re-add all the other vegetable names that aren't selected */
  for(int i = 0; i <  mInventory->getVegNum(); i++){
    if(selectedVeges.find(i) == selectedVeges.end()){
       vegeDrop->addItem(mInventory->getVegetableByIndex(i)
                             ->getVegetablename().c_str());
      actualIndexes.push_back(i);
    }
  }
}

string MultiBuyElement:: getAmount(){
  return amount->text().toStdString();
}

string MultiBuyElement:: getPrice(){
  return price->text().toStdString();
}







