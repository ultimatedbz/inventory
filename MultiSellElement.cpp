#include "MultiSellElement.h"
#include <QDebug>

MultiSellElement::MultiSellElement(int index, MultiSellController* c,
                                   Inventory* i, QFont f,
                                   set<int>* selectedVeges,
                                   vector<int>& vegeIndexToActual):
  mForm(new QFormLayout()),
  controller(c),
  mInventory(i),
  font(f),
  first(1),
  selectedRemains(new set<int>()),
  mVegIndex(index),
  remainingDrops(new vector<QComboBox*>()),
  comboIndexToActual(new vector<int>[5]),
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
    if( mInventory->getVegetableByIndex(i)->getTotalVeges() &&
                    selectedVeges->find(i) == selectedVeges->end() ){
      vegeDrop->addItem(mInventory->getVegetableByIndex(i)->getVegetablename().c_str());
      vegeIndexToActual.push_back(i);
    }
  }
  vegeDrop->setFont(font);
  mForm->addRow("Vegetable", vegeDrop);

  /* Line */
  QFrame* line2 = new QFrame();
  line2->setGeometry(QRect(/* ... */));
  line2->setFrameShape(QFrame::HLine); // Replace by VLine for vertical line
  line2->setFrameShadow(QFrame::Sunken);
  mForm->addRow(line2);

  /* Amount */
  amount = new QLineEdit();
  QString label5 = QString("Amounts");
  mForm->addRow(label5, amount);

  /* In Stock */
  QComboBox* remainingDrop = new QComboBox();
  remainingDrop->installEventFilter(scrollEat);
  connect(remainingDrop, SIGNAL(activated(int)), this, SLOT(remainingDropChanged(int)));
  comboIndexToActual[0] = vector<int>(mInventory->getVegetableByIndex(index)->
                                                getRemainingNum());

  for(int i = 0; i < mInventory->getVegetableByIndex(index)-> getRemainingNum(); i++){
    remainingDrop->addItem(mInventory->getVegetableByIndex(index)->formatRemaining(i).c_str());
    comboIndexToActual[0][i] = i;
  }
  remainingDrops->push_back(remainingDrop);
  selectedRemains->insert(0);


  remainingDrop->setFont(font);

  QToolButton *tb = new QToolButton();
  tb->setText("+");
  QObject::connect(tb, SIGNAL(clicked()),this, SLOT(addRemaining()));

  hLay = new QHBoxLayout();
  hLay->addWidget(remainingDrop);
  hLay->addWidget(tb);

  mForm->addRow("In Stock", hLay);

  /* Price */
  price = new QLineEdit();

  QString label6 = QString("Price");
  mForm->addRow(label6, price);
}


MultiSellElement::~MultiSellElement()
{

  QLayoutItem* item;
  while ( ( item = hLay->takeAt( 0 ) ) != NULL )
  {
      delete item->widget();
      delete item;
  }
  int i = 0;
  while ( ( item = mForm->takeAt( 0 ) ) != NULL )
  {

     if(i != 2 ){
        delete item->widget();
     }
     delete item;
     i++;
  }

  mForm->deleteLater();
  vegeDrop->deleteLater();

  delete remainingDrops;
  delete selectedRemains;
  delete[] comboIndexToActual;
  delete scrollEat;
}

QFormLayout* MultiSellElement::getElement(){
  return mForm;
}

void MultiSellElement::addRemaining(){
  if(selectedRemains->size() < mInventory->getVegetableByIndex(mVegIndex)->getRemainingNum()
     && selectedRemains->size() < 5){

    int comboBoxNum = selectedRemains->size();
    comboIndexToActual[comboBoxNum] =
                        vector<int>(mInventory->getVegetableByIndex(mVegIndex)
                                                  ->getRemainingNum());

    QComboBox* remainingDrop = new QComboBox();
    remainingDrop->installEventFilter(scrollEat);
    connect(remainingDrop, SIGNAL(activated(int)), this, SLOT(remainingDropChanged(int)));

    /* First index will be the one that is shown on the combobox  */
    int firstIndex = -1;

    for(int i = 0; i < mInventory->getVegetableByIndex(mVegIndex)-> getRemainingNum(); i++){
      if(selectedRemains->find(i) == selectedRemains->end()){
        if(firstIndex == -1)
          firstIndex = i;
        remainingDrop->addItem(mInventory->getVegetableByIndex(mVegIndex)->formatRemaining(i).c_str());
        comboIndexToActual[comboBoxNum][remainingDrop->count() - 1] = i;
      }
    }
    selectedRemains->insert(firstIndex);
    remainingDrop->setFont(font);

    /* Changes visible label to empty */
    if(first){
      ((QLabel*) mForm->itemAt(6)->widget())->setText("");
      first = 0;
    }else
      ((QLabel*) mForm->itemAt(mForm->count() - 2)->widget())->setText("");

    mForm->insertRow(4,"In Stock", remainingDrop);
    remainingDrops->push_back(remainingDrop);

    updateRemainDrops();
  }
}

/* connect change in remain drop and vegetable drop */

void MultiSellElement::updateRemainDrops(){
  for( int z = 0; z < remainingDrops->size(); z++){
    QComboBox* remainingDrop = (*remainingDrops)[z];

    /* Save first index */
    int firstSelectionIndex = comboIndexToActual[z][0];

    /* Clear back end and front end of combo box */
    comboIndexToActual->clear();
    remainingDrop->clear();

    /* Add the first one back */
    remainingDrop->addItem(mInventory->getVegetableByIndex(mVegIndex)
                           ->formatRemaining(firstSelectionIndex).c_str());
    comboIndexToActual[z][0] = firstSelectionIndex;

    /* Re-add only the ones that aren't selected */
    for(int i = 0; i <  mInventory->getVegetableByIndex(mVegIndex)->getRemainingNum(); i++){
      if(selectedRemains->find(i) == selectedRemains->end()){
        remainingDrop->addItem(mInventory->getVegetableByIndex(mVegIndex)
                               ->formatRemaining(i).c_str());
        comboIndexToActual[z][remainingDrop->count() - 1] = i;
      }
    }
  }
}

void MultiSellElement::remainingDropChanged(int newIndex){
  /* Find which remaining drop has been changed */
  QComboBox* remainingDrop = dynamic_cast<QComboBox*>(QObject::sender());

  int comboBoxNum = -1;
  for(int i = 0; i < remainingDrops->size(); i++){
    if((*remainingDrops)[i] == remainingDrop)
      comboBoxNum = i;
  }
  /* Remove from set */
  selectedRemains->erase(comboIndexToActual[comboBoxNum][0]);

  /* Change the first element of the comboIndexToActual*/
  comboIndexToActual[comboBoxNum][0] =
                    comboIndexToActual[comboBoxNum][newIndex];
   /* Update the Set*/
   selectedRemains->insert(comboIndexToActual[comboBoxNum][0]);

   updateRemainDrops();
}

QComboBox* MultiSellElement:: getVegeDrop(){
  return vegeDrop;
}

void MultiSellElement:: updateVegeDrops( set<int> selectedVeges,
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
    if(mInventory->getVegetableByIndex(i)->getTotalVeges() &&
       selectedVeges.find(i) == selectedVeges.end()){
       vegeDrop->addItem(mInventory->getVegetableByIndex(i)
                             ->getVegetablename().c_str());
      actualIndexes.push_back(i);
    }
  }
}

string MultiSellElement:: getAmounts(){
  return amount->text().toStdString();
}

vector<int> MultiSellElement:: getRemainingIndexes(){
  vector<int> returnThis;
  for( int i = 0; i < selectedRemains->size(); i++){
    returnThis.push_back(comboIndexToActual[i][0]);
  }
  return returnThis;
}

string MultiSellElement:: getPrice(){
  return price->text().toStdString();
}


