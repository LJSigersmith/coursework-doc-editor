//
//  ECTextDocument.h
//  
//
//  Created by Yufeng Wu on 2/26/20.
//
//

#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECCommand.h"
#include <vector>
#include <iostream>

class ECTextDocument;

// **********************************************************
// Implements Commands for editing 


// your code goes here


// **********************************************************
// Controller for text document

class ECTextDocumentCtrl
{
public:
    ECTextDocumentCtrl(ECTextDocument &docIn);                              // conroller constructor takes the document as input
    ~ECTextDocumentCtrl();                                                  // destructor
    void InsertTextAt(int pos, const std::vector<char> &listCharsToIns);    // insert a list of characters starting at position
    void RemoveTextAt(int pos, int lenToRemove);                            // remove a segment of characters  of lenToRemove starting from pos
    void CapTextAt(int pos, int lenToCap);                                  // Capitalize the text of lenToCap long starting from pos
    void LowerTextAt(int pos, int lenToLower);                              // Lowercase the text of lenToLoer starting from pos
    bool Undo();                                                            // undo any change you did to the text
    bool Redo();                                                            // redo the change to the text
    void PrintDocument();
    char GetCharAt(int pos);

private:
    ECTextDocument* _document;
    ECCommandHistory* _history;
    bool executingUndoRedo;
};

// **********************************************************
// Document for text document

class ECTextDocument
{
public:
    ECTextDocument();
    virtual ~ECTextDocument();
    ECTextDocumentCtrl &GetCtrl();          // return document controller
    int GetDocLen() const { return listChars->size(); }
    char GetCharAt(int pos) const;          // get char at current position
    void InsertCharAt(int pos, char ch);    // insert a single char at position
    void RemoveCharAt(int pos);             // erase a single char at position
    void CapCharAt(int pos);                // capitalize the char at position
    void LowerCharAt(int pos);              // lowercase the char at position
    void Print();
    std::vector<char> GetCharsFrom (int pos1, int pos2);
private:
    std::vector<char> *listChars;
    ECTextDocumentCtrl* docCtrl;
};


#endif /* ECTextDocument_h */
