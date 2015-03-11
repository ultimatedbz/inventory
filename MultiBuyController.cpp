#include "MultiBuyController.h"
#include "MultiBuyElement.h"

MultiBuyController::MultiBuyController(Inventory* inventory,
                                       QFormLayout* fo, QFont f,
                                       QScrollArea* sa,
                                       QDialog* d):
  mInventory(inventory),
  font(f),
  form(fo),
  comboIndexToActual(new vector<vector<int> >()),
  selectedVeges(new set<int>()),
  formArray(new vector<MultiBuyElement*>),
  scrollArea(sa),
  dialog(d)

{

  vector<int> tempv;
  comboIndexToActual->push_back(tempv);
  /* Add all selected into the set and set all indexes to actual */
  int index = 0; //index of combo box num
  int temp = 1;
  for(int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){
      selectedVeges->insert(i);
      (*comboIndexToActual)[index].push_back(i);
      index++;
    }else
      temp++;
  }
  index = 0;
  temp = 1;
  /* Make and add individual forms to big form */
  for( int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){

      formArray->push_back(new MultiBuyElement(i, this, inventory, font,
                                   selectedVeges, (*comboIndexToActual)[index]));
      form->addRow((*formArray)[index]->getElement());
      index++;
    }else{
      temp++;
    }
  }
}

MultiBuyController::~MultiBuyController()
{

}

void MultiBuyController::vegeDropChanged( int newIndex ){
  int vegeBoxNum = -1;
  for( int i = 0; i < selectedVeges->size(); i++){
    if ((*formArray)[i]->getVegeDrop() ==
        dynamic_cast <QComboBox*>(QObject::sender()))
      vegeBoxNum = i;
  }

  /* Remove from Set */
  selectedVeges->erase((*comboIndexToActual)[vegeBoxNum][0]);

  /* Change the first element of the comboIndexToActual*/
  (*comboIndexToActual)[vegeBoxNum][0] =
      (*comboIndexToActual)[vegeBoxNum][newIndex];

  /* Update the Set */
  selectedVeges->insert((*comboIndexToActual)[vegeBoxNum][0]);

  /* Remake the individual element */
  form->update();

  delete (*formArray)[vegeBoxNum];

  (*formArray)[vegeBoxNum] = new MultiBuyElement(
        (*comboIndexToActual)[vegeBoxNum][0],
        this, mInventory,
        font, selectedVeges, (*comboIndexToActual)[vegeBoxNum]);

  form->insertRow(2 + vegeBoxNum, (*formArray)[vegeBoxNum]->getElement());
  updateVegeDrops();
}


void MultiBuyController::updateVegeDrops(){

  for( int z = 0; z < selectedVeges->size(); z++){
    (*formArray)[z]->updateVegeDrops(*selectedVeges, (*comboIndexToActual)[z]);
  }

}

int MultiBuyController::getActualVegeIndex(int i){
  return (*comboIndexToActual)[i][0];
}

string MultiBuyController::getAmount(int i){
  return (*formArray)[i]->getAmount();
}

string MultiBuyController:: getPrice(int i){
  return (*formArray)[i]->getPrice();
}

int MultiBuyController:: getElementNum(){
  return selectedVeges->size();
}

void MultiBuyController:: addElement(){
  qDebug()<<"add";

  vector<int> tempv;
  comboIndexToActual->push_back(tempv);
  if(selectedVeges->size() < mInventory->getVegNum()){

    QComboBox* temp = (*formArray)[0]->getVegeDrop();

    int comboBoxNum = selectedVeges->size();
    comboIndexToActual->push_back(vector<int>());

    for(int i = 0; i < mInventory->getVegNum(); i++){
      if(selectedVeges->find(i) == selectedVeges->end()){
        formArray->push_back(new MultiBuyElement(i, this, mInventory, font,
                             selectedVeges, (*comboIndexToActual)[comboBoxNum]));
        form->addRow((*formArray)[selectedVeges->size()]->getElement());
        selectedVeges->insert(i);
        break;
      }
    }
    updateVegeDrops();

    int h = (selectedVeges->size() > 5)?
          scrollArea->sizeHint().height() + 4
                     * ((*formArray)[0]->getElement()->sizeHint().height() +
                     .5 * temp->sizeHint().height())
        :
        form->sizeHint().height()+ 2.5 *
        temp->sizeHint().height();
    scrollArea->setMinimumHeight(h);
    dialog->adjustSize();
  }
}

void MultiBuyController:: subtractElement(){
  qDebug()<<"subtract";


  if(selectedVeges->size() > 1){
    int vegeBoxNum = selectedVeges->size() - 1;

    /* Remove from Set */
    selectedVeges->erase((*comboIndexToActual)[vegeBoxNum][0]);

    /* Remove individual element */
    delete (*formArray)[vegeBoxNum];
    formArray->pop_back();
    comboIndexToActual->pop_back();
    updateVegeDrops();

    form->update();
    QComboBox* temp = (*formArray)[0]->getVegeDrop();
    int h = (selectedVeges->size()>5)?
          scrollArea->sizeHint().height() + 4
                   * ((*formArray)[0]->getElement()->sizeHint().height() +
                    .5 * temp->sizeHint().height())
        :
          scrollArea->sizeHint().height()
                   + (selectedVeges->size() - 1)
                    * ((*formArray)[0]->getElement()->sizeHint().height() +
                      .5 * temp->sizeHint().height());
    scrollArea->setMinimumHeight(h);
     dialog->adjustSize();
  }
}

