#include "MultiSellController.h"
#include "MultiSellElement.h"

MultiSellController::MultiSellController(int queryNum,
                              Inventory* inventory, QFormLayout* fo,
                              QFont f, QScrollArea* sa, QDialog* d):
  mInventory(inventory),
  font(f),
  comboIndexToActual(new vector<int>[queryNum]),
  form(fo),
  selectedVeges(new set<int>()),
  scrollArea(sa),
  dialog(d)
{
  /* Make array for individual forms */
  formArray = new MultiSellElement* [queryNum] ;

  /* Add all selected into the set and set all indexes to actual */
  int index = 0; //index of combo box num
  int temp = queryNum;
  for(int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){
      selectedVeges->insert(i);
      comboIndexToActual[index].push_back(i);
      index++;
    }else
      temp++;
  }

  index = 0;
  /* Make and add individual forms to big form */
  for( int i = 0; i < queryNum; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){

      formArray[index] = new MultiSellElement(i, this, inventory, font,
                                           selectedVeges, comboIndexToActual[index]);
      form->addRow(formArray[index]->getElement());
      index++;
    }else{
      queryNum++;
    }
  }
}

MultiSellController::~MultiSellController()
{

}

void MultiSellController::vegeDropChanged( int newIndex ){
  int vegeBoxNum = -1;
  for( int i = 0; i < selectedVeges->size(); i++){
    if (formArray[i]->getVegeDrop() ==
        dynamic_cast <QComboBox*>(QObject::sender()))
      vegeBoxNum = i;
  }

  /* Remove from Set */
  selectedVeges->erase(comboIndexToActual[vegeBoxNum][0]);

  /* Change the first element of the comboIndexToActual*/
  comboIndexToActual[vegeBoxNum][0] =
      comboIndexToActual[vegeBoxNum][newIndex];

  /* Update the Set */
  selectedVeges->insert(comboIndexToActual[vegeBoxNum][0]);

  /* Remake the individual element */
  form->update();
  delete formArray[vegeBoxNum];

  formArray[vegeBoxNum] = new MultiSellElement(
        comboIndexToActual[vegeBoxNum][0], this, mInventory,
        font, selectedVeges, comboIndexToActual[vegeBoxNum]);
  form->insertRow(2 + vegeBoxNum, formArray[vegeBoxNum]->getElement());
  updateVegeDrops();
}


void MultiSellController::updateVegeDrops(){

  for( int z = 0; z < selectedVeges->size(); z++){
    formArray[z]->updateVegeDrops(*selectedVeges, comboIndexToActual[z]);
  }

}

int MultiSellController::getActualVegeIndex(int i){
  return comboIndexToActual[i][0];
}

string MultiSellController::getAmounts(int i){
  return formArray[i]->getAmounts();
}

vector<int> MultiSellController::getActualRemainingIndexes(int i){
  return formArray[i]->getRemainingIndexes();
}

string MultiSellController:: getPrice(int i){
  return formArray[i]->getPrice();
}
