//
//  ECCommand.cpp
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#include "ECCommand.h"
#include <iostream>
// ******************************************************
// Implement command history

int ECCommandHistory::GetSize() {
  return _commands->size();
}

void ECCommandHistory::CommandUndone() {
  _currentCommand--;
}

void ECCommandHistory::CommandRedone() {
  _currentCommand++;
}

void ECCommandHistory::AddCmd(ECCommand* cmd) {
  _commands->push_back(cmd);
  _currentCommand = _commands->size() - 1;
}

ECCommand* ECCommandHistory::GetCurrentCommand() {
  if (_commands->size() == 0) { return NULL; }
  return (*_commands)[_currentCommand];
}

ECCommand* ECCommandHistory::GetNextCommand() {
  if (_commands->size() == 0) { return NULL; }
  return (*_commands)[_currentCommand + 1];
}

ECCommandHistory :: ECCommandHistory() 
{
  _commands = new std::vector<ECCommand*>;
}

ECCommandHistory :: ~ECCommandHistory()
{
}


