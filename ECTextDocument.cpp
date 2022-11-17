//
//  ECTextDocument.cpp
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#include "ECTextDocument.h"
#include <cctype>
#include <algorithm>

using namespace std;

// **********************************************************
// Commands

// Insert Command
void ECInsertCommand::Execute() {
  _docCtrl->InsertTextAt(_pos, _chars);
}
void ECInsertCommand::UnExecute() {
  _docCtrl->RemoveTextAt(_pos, _chars.size());
}

// Remove Command
void ECRemoveCommand::Execute() {
  _docCtrl->RemoveTextAt(_pos, _lenToRemove); 
}
void ECRemoveCommand::UnExecute() {
  _docCtrl->InsertTextAt(_pos, _charsRemoved);
}

// Cap Command
void ECCapCommand::Execute() {
  _docCtrl->CapTextAt(_pos, _lenToCap);
}

void ECCapCommand::UnExecute() {
  // If all chars in range were lowercase to begin with, then they were all capitalized, and no chars were unmodified
  // So, lowercase all of them to undo
  if (_unModifiedChars.size() == 0) {
      _docCtrl->LowerTextAt(_pos, _lenToCap);
      return;
  }
  // If there were some chars that were capital to begin with, loop through the chars in range, and if they were modified
  // by being capitalized, then lowercase them. If not, leave them capital
  for (int i = _pos; i < (_lenToCap + _pos); i++) {
    for (auto c : _unModifiedChars) {
      if (find(_unModifiedChars.begin(), _unModifiedChars.end(), c) == _unModifiedChars.end()) {
        _docCtrl->LowerTextAt(i, 1);
      }
    }
  }
}

// Lower Command
void ECLowerCommand::Execute() {
  _docCtrl->LowerTextAt(_pos, _lenToLower);
}
void ECLowerCommand::UnExecute() {
  // Just like capitalize unexecute: capitalize all in range if they were all lowercased
  if (_unModifiedChars.size() == 0) {
      _docCtrl->CapTextAt(_pos, _lenToLower);
      return;
  }

  // As before, loop through chars in range, capitalize if they were lowercased, leave lowercased
  // if they were lowercase to begin with
  for (int i = _pos; i < (_lenToLower + _pos); i++) {
    for (auto c : _unModifiedChars) {
      if (find(_unModifiedChars.begin(), _unModifiedChars.end(), c) == _unModifiedChars.end()) {
        _docCtrl->CapTextAt(i, 1);
      }
    }
  }
}

// **********************************************************
// Controller for text document

ECTextDocumentCtrl :: ECTextDocumentCtrl(ECTextDocument &docIn) 
{
  _document = &docIn;
  _history = new ECCommandHistory();
  executingUndoRedo = false;
}

ECTextDocumentCtrl :: ~ECTextDocumentCtrl() {}

void ECTextDocumentCtrl :: InsertTextAt(int pos, const std::vector<char> &listCharsToIns)
{
  // If inserting is part of an undo or a redo, it isn't a new command simply repeating an old one, so no need to add it to command history
  // If it isn't add this command to history
  if (!executingUndoRedo) {
  ECInsertCommand* command = new ECInsertCommand(pos, listCharsToIns, this);
  _history->AddCmd(command);
  }

  // Insert specified chars in specfiied range
  for (int i = pos, j = 0; j < listCharsToIns.size(); i++, j++) {
    _document->InsertCharAt(i, listCharsToIns[j]);
  }
}

void ECTextDocumentCtrl :: RemoveTextAt(int pos, int lenToRemove)
{
  // Check for out of bounds
  if (lenToRemove <= 0) { std::cout << "##Cannot remove with length lte 0##" << endl; return;}

  // Keep track of chars being removed: if this command is undone, they will need to be inserted
  vector<char> charsToRemove = _document->GetCharsFrom(pos, pos + lenToRemove - 1);
  
  // If removing is part of an undo or redo, it isn't a new command simply repeating an old one, so no need to add it to command history
  if (!executingUndoRedo) {
  ECRemoveCommand* command = new ECRemoveCommand(pos, lenToRemove, charsToRemove, this);
  _history->AddCmd(command);
  }

  // Remove chars from specified range
  for (int i = pos; i < pos + lenToRemove; i++) {
    _document->RemoveCharAt(pos);
  }
}

void ECTextDocumentCtrl :: CapTextAt(int pos, int lenToCap)
{

  // Keep track of chars that are already capitalized: if undone these chars need to stay capitalized
  vector<int> unModifiedChars;

  // Loop through chars in range, if they are already capitalized, add them to vector
  // If they are not, capitalize them
  for (int i = pos, j = 0; j < lenToCap; j++) {
    char currentChar = _document->GetCharAt(i+j);
    if (isupper(currentChar)) {
      unModifiedChars.push_back(currentChar);
    } else {
      _document->CapCharAt(i + j);
    }
  }

  // If capitalizing is part of an undo or redo command, it isn't a new command simply repeating an old one, so no need to add it to command history
  // If it isn't add this command to history
  if (!executingUndoRedo) {
  ECCapCommand* command = new ECCapCommand(pos, lenToCap, this, unModifiedChars);
  _history->AddCmd(command);
  }
}

void ECTextDocumentCtrl :: LowerTextAt(int pos, int lenToLower)
{

  // Keep track of chars that are already lowercase: if undone these chars need to stay lowercase
  vector<int> unModifiedChars;

  // Loop through chars in range, if they are already lowercase, add them to vector
  // If they are not, lower them
  for (int i = pos, j = 0; j < lenToLower; j++) {
    char currentChar = _document->GetCharAt(i+j);
    if (islower(currentChar)) {
      unModifiedChars.push_back(currentChar);
    } else {
      _document->LowerCharAt(i + j);
    }
  }

  // If lowering is part of an undo or redo command, it isn't a new command simply repeating an old one, so no need to add it to command history
  // If it isn't add this command to history
  if (!executingUndoRedo) {
  ECLowerCommand* command = new ECLowerCommand(pos, lenToLower, this, unModifiedChars);
  _history->AddCmd(command);
  }
}

bool ECTextDocumentCtrl :: Undo()
{
  // Check for empty command set
  if (_history->GetSize() <= 0) {
    return false;
  }

  // Set to true so commands executed to undo are not added to command history
  executingUndoRedo = true;

  // Get the last executed command and unexecute it
  ECCommand* command = _history->GetCurrentCommand();
  command->UnExecute();

  // Done executing, future commands need to be added to history
  executingUndoRedo = false;

  // Adjust the currentCommand value so it points to the now last executed command
  _history->CommandUndone();
  return true;
}

bool ECTextDocumentCtrl :: Redo()
{
  // Check for empty command set
  if (_history->GetSize() <= 0) {
    return false;
  }

  // Set to true so commands executed to undo are not added to command history
  executingUndoRedo = true;
  
  // Get the next command to be executed (the last one undone) and execute it
  ECCommand* command = _history->GetNextCommand();
  command->Execute();
  
  // Done executing, future commands need to be added to history
  executingUndoRedo = false;

  // Adjust currentCommand value so that it points to the next command to execute
  _history->CommandRedone();
  return true;
}

char ECTextDocumentCtrl :: GetCharAt(int pos) {
  return _document->GetCharAt(pos);
}

// **********************************************************
// Document for text document
ECTextDocument :: ECTextDocument() 
{
  listChars = new vector<char>;
  docCtrl = new ECTextDocumentCtrl(*this);

}

ECTextDocument :: ~ECTextDocument() {}

ECTextDocumentCtrl & ECTextDocument :: GetCtrl() { return *docCtrl; }

void ECTextDocumentCtrl :: PrintDocument() { _document->Print(); }

char ECTextDocument :: GetCharAt(int pos) const { return (*listChars)[pos]; }

void ECTextDocument :: InsertCharAt(int pos, char ch) { listChars->insert(listChars->begin() + pos, ch); }

void ECTextDocument :: RemoveCharAt(int pos) { listChars->erase(listChars->begin() + pos); }

void ECTextDocument :: CapCharAt(int pos) { (*listChars)[pos] = toupper((*listChars)[pos]); }

void ECTextDocument :: LowerCharAt(int pos) { (*listChars)[pos] = tolower((*listChars)[pos]); }

std::vector<char> ECTextDocument :: GetCharsFrom (int pos1, int pos2) { return std::vector<char>(listChars->begin() + pos1, listChars->begin() + pos2); }

void ECTextDocument :: Print() {
  std::cout << "= = = = = = = = = = = = = = = = = = = =" << endl;
  std::cout << "\t";
  for (auto c : *listChars) {
    if (c == '\n') {
      std::cout << "\t";
    }
    std::cout << c;
  }
  std::cout << endl;
}
