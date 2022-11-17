//
//  ECCommand.h
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#ifndef ECCommand_h
#define ECCommand_h

#include <vector>
#include <string>
#include <iostream>

using namespace std;
class ECTextDocumentCtrl;
// ******************************************************
// Implement command design pattern

class ECCommand
{
public:
    virtual ~ECCommand() {}
    virtual void Execute() = 0;
    virtual void UnExecute() = 0;

    ECTextDocumentCtrl* _docCtrl;

    // For debugging, easier to print commands with an attribute for their type
    std::string _name;

};

class ECInsertCommand : public ECCommand {
    public :
        ECInsertCommand(int pos, std::vector<char> chars, ECTextDocumentCtrl* doc) : _pos(pos), _chars(chars) {
            _docCtrl = doc;
            _name = "INSERT";
        }
        void Execute();
        void UnExecute();
    
    private :
        int _pos;
        std::vector<char> _chars;
};

class ECRemoveCommand : public ECCommand {
    public :
        ECRemoveCommand(int pos, int lenToRemove, std::vector<char> charsToRemove, ECTextDocumentCtrl* doc) : _pos(pos), _lenToRemove(lenToRemove) {
            _charsRemoved = charsToRemove;
            _docCtrl = doc;
            _name = "REMOVE";
        }
        
        void Execute();
        void UnExecute();

    private :
        int _pos;
        int _lenToRemove;
        std::vector<char> _charsRemoved;

};

class ECCapCommand : public ECCommand {
    public :
        ECCapCommand(int pos, int lenToCap, ECTextDocumentCtrl* doc, vector<int> unModifiedChars) : _pos(pos), _lenToCap(lenToCap) {
            _docCtrl = doc;
            _name = "CAP";
            _unModifiedChars = unModifiedChars;
        }

        void Execute();
        void UnExecute();
    
    private :
        int _pos;
        int _lenToCap;
        vector<int> _unModifiedChars;

};

class ECLowerCommand : public ECCommand {
    public :
        ECLowerCommand(int pos, int lenToLower, ECTextDocumentCtrl* doc, vector<int> unmodifiedChars) : _pos(pos), _lenToLower(lenToLower) {
            _docCtrl = doc;
            _name = "LOWER";
            _unModifiedChars = unmodifiedChars;
        }

        void Execute();
        void UnExecute();

    private :
        int _pos;
        int _lenToLower;
        vector<int> _unModifiedChars;

};

// ******************************************************
// Implement command history

class ECCommandHistory
{
public:
    ECCommandHistory();
    virtual ~ECCommandHistory();
    
    bool Undo();
    bool Redo();
    void ExecuteCmd( ECCommand *pCmd );
    void AddCmd(ECCommand* cmd);

    ECCommand* GetCurrentCommand();
    ECCommand* GetNextCommand();
    
    int GetSize();
    void CommandUndone();
    void CommandRedone();

private:
    std::vector<ECCommand*>* _commands;
    int _currentCommand;
};

#endif /* ECCommand_h */
