#include "MultiSellController.h"

MultiSellController::MultiSellController(int queryNum,
                              Inventory* inventory, QFormLayout& form,
                              QFont font)
{

  /* Make array for individual forms */
  MultiSellElement** formArray = new MultiSellElement* [queryNum] ;

  /* Make and add individual forms to big form */
  for( int i = 0; i < queryNum; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){
      formArray[i] = new MultiSellElement(i, this, inventory, font);
      form.addRow(formArray[i]->getElement());
    }else{
      queryNum++;
    }
  }

}

MultiSellController::~MultiSellController()
{

}

void MultiSellController::vegeDropChanged( int newIndex ){
  qDebug()<<newIndex<<"hi";
  /* Find which vege drop has been changed */
  QComboBox* vegeDrop = dynamic_cast <QComboBox*>(QObject::sender());

  int comboboxNum = -1;
  //for( int i = 0; i < vegeD)

}
