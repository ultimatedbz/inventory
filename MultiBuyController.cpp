#include "MultiBuyController.h"
#include "MultiBuyElement.h"

MultiBuyController::MultiBuyController(Inventory* inventory,
                                       QFormLayout* fo, QFont f):
  mInventory(inventory),
  font(f),
  form(fo),
  comboIndexToActual(new vector<vector<int> >()),
  selectedVeges(new set<int>()),
  elementNum(1)

{
  /* Make array for individual forms */
  vector<MultiBuyElement*> formArray;

  vector<int> tempv;
  comboIndexToActual->push_back(tempv);
  /* Add all selected into the set and set all indexes to actual */
  int index = 0; //index of combo box num
  int temp = elementNum;
  for(int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){
      selectedVeges->insert(i);
      (*comboIndexToActual)[index].push_back(i);
      index++;
    }else
      temp++;
  }
qDebug()<<27;
  index = 0;
  temp = elementNum;
  /* Make and add individual forms to big form */
  for( int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){

      formArray.push_back(new MultiBuyElement(i, this, inventory, font,
                                   selectedVeges, (*comboIndexToActual)[index]));
      form->addRow(formArray[index]->getElement());
      index++;
      qDebug()<<41;
    }else{
      temp++;
    }
  }
  qDebug()<<45;
}

MultiBuyController::~MultiBuyController()
{

}

void MultiBuyController::vegeDropChanged( int newIndex ){
  int vegeBoxNum = -1;
  for( int i = 0; i < selectedVeges->size(); i++){
    if (formArray[i]->getVegeDrop() ==
        dynamic_cast <QComboBox*>(QObject::sender()))
      vegeBoxNum = i;
  }

  /* Remove from Set */
  selectedVeges->erase((*comboIndexToActual)[vegeBoxNum][0]);

  /* Change the first element of the comboIndexToActual*/
  comboIndexToActual[vegeBoxNum][0] =
      comboIndexToActual[vegeBoxNum][newIndex];

  /* Update the Set */
  selectedVeges->insert((*comboIndexToActual)[vegeBoxNum][0]);

  /* Remake the individual element */
  form->update();
  delete formArray[vegeBoxNum];

  formArray[vegeBoxNum] = new MultiBuyElement(
        (*comboIndexToActual)[vegeBoxNum][0],
        this, mInventory,
        font, selectedVeges, (*comboIndexToActual)[vegeBoxNum]);
  form->insertRow(2 + vegeBoxNum, formArray[vegeBoxNum]->getElement());
  updateVegeDrops();
}


void MultiBuyController::updateVegeDrops(){

  for( int z = 0; z < selectedVeges->size(); z++){
    formArray[z]->updateVegeDrops(*selectedVeges, (*comboIndexToActual)[z]);
  }

}

int MultiBuyController::getActualVegeIndex(int i){
  return (*comboIndexToActual)[i][0];
}

string MultiBuyController::getAmount(int i){
  return formArray[i]->getAmount();
}

string MultiBuyController:: getPrice(int i){
  return formArray[i]->getPrice();
}
