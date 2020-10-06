#include "MultiSellController.h"
#include "MultiSellElement.h"
#include "translator.h"

MultiSellController::MultiSellController(Inventory* inventory,
                                         QFormLayout* fo,
                              QFont f, QScrollArea* sa, QDialog* d,
                                         Translator* t):
  mInventory(inventory),
  font(f),
  comboIndexToActual(new vector<vector<int> >()),
  form(fo),
  selectedVeges(new set<int>()),
  scrollArea(sa),
  dialog(d),
  formArray(new vector<MultiSellElement*>),
  mTranslator(t)
{
  vector<int> tempv;
  comboIndexToActual->push_back(tempv);

  /* Add first vegetable in stock */
  int temp = 1;
  for(int i = 0; i < temp; i++){
    if( inventory->getVegetableByIndex(i)->getTotalVeges() ){
      selectedVeges->insert(i);
      (*comboIndexToActual)[0].push_back(i);
      formArray->push_back(new MultiSellElement(i, this, inventory, font,
                                           selectedVeges, (*comboIndexToActual)[0],
                           mTranslator));
      form->addRow((*formArray)[0]->getElement());
    }else
      temp++;
  }
}

MultiSellController::~MultiSellController()
{

}

void MultiSellController::vegeDropChanged( int newIndex ){
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

  (*formArray)[vegeBoxNum] = new MultiSellElement(
        (*comboIndexToActual)[vegeBoxNum][0], this, mInventory,
        font, selectedVeges, (*comboIndexToActual)[vegeBoxNum], mTranslator);
  form->insertRow(2 + vegeBoxNum, (*formArray)[vegeBoxNum]->getElement());
  updateVegeDrops();
}


void MultiSellController::updateVegeDrops(){

  for( int z = 0; z < selectedVeges->size(); z++){
    (*formArray)[z]->updateVegeDrops(*selectedVeges, (*comboIndexToActual)[z]);
  }

}

void MultiSellController::addElement(){

  if(selectedVeges->size() < mInventory->numberOfNonEmptyVeges()){

    vector<int> tempv;
    comboIndexToActual->push_back(tempv);
    QComboBox* temp = (*formArray)[0]->getVegeDrop();

    int comboBoxNum = selectedVeges->size();
    comboIndexToActual->push_back(vector<int>());

    for(int i = 0; i < mInventory->getVegNum(); i++){
      if(selectedVeges->find(i) == selectedVeges->end() &&
          mInventory->getVegetableByIndex(i)->getTotalVeges()){
        formArray->push_back(new MultiSellElement(i, this, mInventory, font,
                             selectedVeges, (*comboIndexToActual)[comboBoxNum],
                                                  mTranslator));
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

void MultiSellController::subtractElement(){
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

int MultiSellController::getActualVegeIndex(int i){
  return (*comboIndexToActual)[i][0];
}

string MultiSellController::getAmounts(int i){
  return (*formArray)[i]->getAmounts();
}

vector<int> MultiSellController::getActualRemainingIndexes(int i){
  return (*formArray)[i]->getRemainingIndexes();
}

string MultiSellController:: getPrice(int i){
  return (*formArray)[i]->getPrice();
}

int MultiSellController:: getElementNum(){
  return selectedVeges->size();
}
