#include "abbreviation.h"

Abbreviation::Abbreviation():
  abbreviations(map<string,string>())
{

}

Abbreviation::~Abbreviation()
{

}

string Abbreviation::shrink(string input){
  return abbreviations.find(input)->second;
}

void Abbreviation::add(string key, string value){
  abbreviations.insert(pair<string, string>(key, value));
}

void Abbreviation::remove(string input){
  abbreviations.erase(input);
}

int Abbreviation::keyCount(){
  return abbreviations.size();
}

map<string, string> Abbreviation:: getAbbs(){
  return abbreviations;
}
