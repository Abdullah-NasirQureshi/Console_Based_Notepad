#include <iostream>
#include<fstream>
#include <Windows.h>
using namespace std;

class ptrStack;
class LinkedList;


template<class T>
class Node
{
    T data;
    //COORD pos;
    Node* right;
    Node* left;
    Node* upper;
    Node* lower;

public:
    int xp = 0;
    int yp = 0;
    Node()
    {
        //data = 0;
        //pos = { 2, 2 };
        right = NULL;
        left = NULL;
        upper = NULL;
        lower = NULL;
    }
    Node(T dat)
    {
        //pos = { 2, 2 };
        data = dat;
        right = NULL;
        left = NULL;
        upper = NULL;
        lower = NULL;
    }

    T getData()
    {
        return data;
    }
    void setData(T dat)
    {
        data = dat;
    }
    Node* getleft()
    {
        return this->left;
    }
    Node* getupper()
    {
        return this->upper;
    }
    Node* getlower()
    {
        return this->lower;
    }
    Node* getright()
    {
        return this->right;
    }
    void setright(Node* r)
    {
        right = r;
    }
    void setleft(Node* r)
    {
        left = r;
    }
    void setupper(Node* r)
    {
        upper = r;
    }
    void setlower(Node* r)
    {
        lower = r;
    }
};


struct lineSize
{
    int size;
    lineSize* next;
    lineSize* prev;
    Node<char>* lHead;

    lineSize()
    {
        size = 0;
        next = NULL;
        prev = NULL;
        lHead = NULL;
    }



};


template<typename T>
class sNode {
public:
    T data;
    sNode* next;

    // Node constructor
    sNode(T dat) : data(dat), next(NULL) {}
};


class WordNode {
public:
    sNode<char>* head;
    int x;
    int y;
    WordNode* next;

    WordNode(sNode<char>* wordHead, int xpos, int ypos)
        : head(wordHead), x(xpos), y(ypos), next(NULL) {}
};

class ptrStack {
    int count;
    sNode<char>* tail;
    sNode<char>* head;
    int wordCount;
    WordNode* wordListHead;
    WordNode* wordListTail;
public:
    Node<int>* xpos;
    Node<int>* ypos;

    ptrStack() : tail(NULL), head(NULL), count(0), wordCount(0), wordListHead(NULL), wordListTail(NULL) {
        xpos = new Node<int>(1);
        ypos = new Node<int>(1);
    }

    void pushChar(char ch, int x, int y, bool cMoved);
    void popWord(int& xv, int& yv, ptrStack*& otherStack, bool rem, bool cMoved);
    void spacePressed(int x, int y);
    void removeFirstWord();
    void removeLastWord();
    void printWords();
    bool isEmpty();

    void clear();
    void popLast(LinkedList& stdat, int& x, int& y, Node<char>*& cursor, bool cMoved);

};

/////////////////////////////////////////// FOR CHILLI MILLI TREE/////////////////////////
struct TNode
{
    char* word;
    TNode** TCon;   // Array of connections to next words
    int TCon_count; // Number of connections

    TNode(const char* w)
    {
        // Manual string copy (instead of strcpy)
        int len = 0;
        while (w[len] != '\0')
        {
            len++;
        }

        word = new char[len + 1];
        for (int i = 0; i < len; ++i)
        {
            word[i] = w[i];
        }
        word[len] = '\0'; // null-terminate the string

        TCon = nullptr;
        TCon_count = 0;
    }

    ~TNode()
    {
        delete[] word;
        if (TCon)
        {
            delete[] TCon;
        }
    }
};
bool now = false;
char* sto;

void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

}


class SentenceTree
{
private:
    TNode* lastNode; // Keeps track of the last inserted node

public:
    TNode* root;

    SentenceTree() : root(nullptr), lastNode(nullptr) {}

    // Insert a word into the tree
    void insert(const char* word)
    {
        if (!root)
        {
            root = new TNode(word);
            lastNode = root; // Update lastNode
            return;
        }


        TNode* node = findNode(root, word);
        if (node)
        {
           
            lastNode = node;
        }
        else
        {
          
            join(lastNode, word);
            lastNode = findNode(root, word); // Update lastNode to the current word
        }
    }

    // Find a node by word
    TNode* findNode(TNode* node, const char* word)
    {
        if (!node)
            return nullptr;

        if (Wcompare(node->word, word) == 0)
        {
            return node;
        }

        for (int i = 0; i < node->TCon_count; ++i)
        {
            TNode* result = findNode(node->TCon[i], word);
            if (result)
                return result;
        }

        return nullptr;
    }

    // Add a connection to a node
    void join(TNode* node, const char* word)
    {
        // Check if the word is already a connection
        for (int i = 0; i < node->TCon_count; ++i)
        {
            if (Wcompare(node->TCon[i]->word, word) == 0)
            {
                return; // Already connected
            }
        }

        // Add a new connection
        TNode** new_TCon = new TNode * [node->TCon_count + 1];
        for (int i = 0; i < node->TCon_count; ++i)
        {
            new_TCon[i] = node->TCon[i];
        }
        new_TCon[node->TCon_count] = new TNode(word);
        if (node->TCon)
        {
            delete[] node->TCon;
        }
        node->TCon = new_TCon;
        node->TCon_count++;
    }

    // Compare two words
    int Wcompare(const char* c1, const char* c2)
    {
        int i = 0;
        while (c1[i] != '\0' && c2[i] != '\0')
        {
            if (c1[i] != c2[i])
            {
                return c1[i] - c2[i];
            }
            i++;
        }
        return c1[i] - c2[i];
    }

    // Manual string concatenation (instead of strcat)
    void concatenate(char* from, const char* to, bool addSpace = true)
    {
        int fromLen = 0;
        while (from[fromLen] != '\0')
        {
            fromLen++;
        }

        int toLen = 0;
        while (to[toLen] != '\0')
        {
            toLen++;
        }

        // If adding space and the fromination is not empty, add a space before concatenating
        if (addSpace && fromLen > 0)
        {
            from[fromLen] = ' ';
            fromLen++;
        }

        for (int i = 0; i < toLen; ++i)
        {
            from[fromLen + i] = to[i];
        }

        from[fromLen + toLen] = '\0';
    }

    // Print the tree in a hierarchical format
    void printTree(TNode* node, const char* initialVal = "")
    {
        if (!node)
            return;

        std::cout << initialVal << node->word << std::endl;

        // Add '->' to the initialVal for the next level of recursion
        char* newinitialVal = new char[100]; // Make sure this is large enough
        int i = 0;
        while (initialVal[i] != '\0')
        {
            newinitialVal[i] = initialVal[i];
            i++;
        }
        newinitialVal[i] = '\0'; // null-terminate the string

        concatenate(newinitialVal, "->");

        for (int i = 0; i < node->TCon_count; ++i)
        {
            printTree(node->TCon[i], newinitialVal);
        }

        delete[] newinitialVal;
    }

    void print()
    {
        printTree(root);
    }

    int vv = 31;
    // Suggest possible sentence completions from the current word
    void suggestSentence(TNode* node, const char* initialVal = "")
    {
        if (!node)
            return;

        // Print the current sentence suggestion
        char* suggestion = new char[1000]; // Allocate space for a suggestion
        int i = 0;
        while (initialVal[i] != '\0')
        {
            suggestion[i] = initialVal[i];
            i++;
        }
        suggestion[i] = '\0'; // null-terminate the string

        concatenate(suggestion, node->word);
        gotoxy(60,vv++);
        std::cout << suggestion << std::endl;

        // Recursively suggest further completions
        for (int i = 0; i < node->TCon_count; ++i)
        {
            suggestSentence(node->TCon[i], suggestion); // Pass the current suggestion
        }

        delete[] suggestion;
    }

    // Function to initiate suggestions based on the word entered by the user
    void suggest(const char* word)
    {
        vv = 34;
        TNode* node = findNode(root, word);
        if (node)
        {
            // Start suggesting from this point in the tree
            suggestSentence(node);
        }
        else
        {
            gotoxy(60, vv);
            cout << "No suggestions available!" << endl;
        }
    }
};
/////////////////////////////////////////// FOR THE TREE /////////////////////////////////

void setTextColorBrightGreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
}


void resetTextColor() {
    // Get the console handle
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Reset text color to the default (light gray)
    SetConsoleTextAttribute(hConsole, 7);
}



class twoDNode
{
public:
    int* x; // Dynamic array to store x positions
    int* y; // Dynamic array to store y positions
    int line;
    char data;
    twoDNode** ParNode;         // Array of pointers to children nodes
    int childCount;             // Current count of children
    const int maxChildren = 26; // Maximum limit for children
    int posCount;               // Count of positions stored in x and y arrays

    twoDNode(int initialX = 0, int initialY = 0, int line = 0, char data = '\0')
        : line(line), data(data), childCount(0), posCount(1)
    {
        // Initialize x and y arrays with one element initially
        x = new int[posCount];
        y = new int[posCount];
        x[0] = initialX;
        y[0] = initialY;

        // Allocate ParNode array up to maxChildren
        ParNode = new twoDNode * [maxChildren];
        for (int i = 0; i < maxChildren; ++i)
        {
            ParNode[i] = nullptr;
        }
    }

    ~twoDNode()
    {
        delete[] x;
        delete[] y;
        for (int i = 0; i < childCount; ++i)
        {
            delete ParNode[i];
        }
        delete[] ParNode;
    }

    // Resize the x and y arrays to store additional positions
    void addPosition(int newX, int newY)
    {
        int* newXArr = new int[posCount + 1];
        int* newYArr = new int[posCount + 1];

        for (int i = 0; i < posCount; ++i)
        {
            newXArr[i] = x[i];
            newYArr[i] = y[i];
        }

        newXArr[posCount] = newX;
        newYArr[posCount] = newY;
        ++posCount;

        delete[] x;
        delete[] y;
        x = newXArr;
        y = newYArr;
    }

    // Add a child node to ParNode, resizing if necessary
    bool addChild(int childX, int childY, int childLine, Node<char>* childData)
    {
        if (childCount >= maxChildren)
        {
            std::cout << "Maximum children reached, cannot add more.\n";
            return false;
        }

        // Add new child node at the next available position
        ParNode[childCount++] = new twoDNode(childX, childY, childLine, childData->getData());
        return true;
    }

    // Check if a child node exists with the specified data character
    // If it exists, recursively find or add to its children
    void findOrAddChild(Node<char>* targetData)
    {
        if (targetData == nullptr)
            return;

        twoDNode* current = this; // Start with the root node (this)

        // Traverse the linked list and build or find the corresponding path in the tree
        while (targetData != nullptr)
        {
            bool found = false;

            // Search for a matching child with the same data
            for (int i = 0; i < current->childCount; ++i)
            {
                if (current->ParNode[i]->data == targetData->getData())
                {
                    // Add new occurrence of position to x and y arrays
                    current->ParNode[i]->addPosition(targetData->xp, targetData->yp);

                    // Match found, move to this child
                    current = current->ParNode[i];
                    found = true;
                    break;
                }
            }

            // If no match found, add a new child with the target data
            if (!found)
            {
                current->addChild(targetData->xp, targetData->yp, targetData->yp, targetData); // Adjust x, y, line as needed
                current = current->ParNode[current->childCount - 1];                           // Move to the newly added child
            }

            // Move to the next character in the linked list
            targetData = targetData->getright();
        }
    }
};

void displayTree(twoDNode* node, char* initialVal)
{
    if (node == nullptr)
        return;

    // Display current node data
    std::cout << initialVal;
    cout<< node->data << "\n";

    // Prepare the new initialVal for children
    int len = 0;
    while (initialVal[len] != '\0')
        len++;                           // Find the current initialVal length
    char* newinitialVal = new char[len + 3]; // Add space for "->" and '\0'
    for (int i = 0; i < len; ++i)
    {
        newinitialVal[i] = initialVal[i];
    }
    newinitialVal[len] = '-';
    newinitialVal[len + 1] = '>';
    newinitialVal[len + 2] = '\0';

    // Display all children
    for (int i = 0; i < node->childCount; ++i)
    {
        displayTree(node->ParNode[i], newinitialVal);
    }

    delete[] newinitialVal; // Clean up dynamically allocated memory
}
//class twoDNode
//{
//public:
//    int* x; // Dynamic array to store x positions
//    int* y; // Dynamic array to store y positions
//    int line;
//    char data;
//    twoDNode** ParNode;         // Array of pointers to children nodes
//    int childCount;             // Current count of children
//    const int maxChildren = 26; // Maximum limit for children
//    int posCount;               // Count of positions stored in x and y arrays
//
//    twoDNode(int initialX = 0, int initialY = 0, int line = 0, char data = '\0')
//        : line(line), data(data), childCount(0), ParNode(nullptr), posCount(1)
//    {
//        // Initialize x and y arrays with one element initially
//        x = new int[posCount];
//        y = new int[posCount];
//        x[0] = initialX;
//        y[0] = initialY;
//    }
//
//    ~twoDNode()
//    {
//        delete[] x;
//        delete[] y;
//        for (int i = 0; i < childCount; ++i)
//        {
//            delete ParNode[i];
//        }
//        delete[] ParNode;
//    }
//
//    // Resize function to expand the array if needed
//    void resize()
//    {
//        int newSize = childCount + 1;
//        twoDNode** newParNode = new twoDNode * [newSize];
//        for (int i = 0; i < childCount; ++i)
//        {
//            newParNode[i] = ParNode[i];
//        }
//        delete[] ParNode;
//        ParNode = newParNode;
//    }
//
//    // Resize the x and y arrays to store additional positions
//    void addPosition(int newX, int newY)
//    {
//        int* newXArr = new int[posCount + 1];
//        int* newYArr = new int[posCount + 1];
//
//        for (int i = 0; i < posCount; ++i)
//        {
//            newXArr[i] = x[i];
//            newYArr[i] = y[i];
//        }
//
//        newXArr[posCount] = newX;
//        newYArr[posCount] = newY;
//        ++posCount;
//
//        delete[] x;
//        delete[] y;
//        x = newXArr;
//        y = newYArr;
//    }
//
//    // Setters and Getters
//    void setLine(int newLine) { line = newLine; }
//    void setData(char newData) { data = newData; }
//    int getLine() const { return line; }
//    char getData() const { return data; }
//
//    // Add a child node to ParNode, resizing if necessary
//    bool addChild(int childX, int childY, int childLine, Node<char>* childData)
//    {
//        if (childCount >= maxChildren)
//        {
//            std::cout << "Maximum children reached, cannot add more.\n";
//            return false;
//        }
//
//        // Resize if needed
//        resize();
//
//        // Add new child node
//        ParNode[childCount++] = new twoDNode(childX, childY, childLine, childData->getData());
//        return true;
//    }
//
//    // Check if a child node exists with the specified data character
//    // If it exists, recursively find or add to its children
//    void findOrAddChild(Node<char>* targetData)
//    {
//        if (targetData == nullptr)
//            return;
//
//        twoDNode* current = this; // Start with the root node (this)
//
//        // Traverse the linked list and build or find the corresponding path in the tree
//        while (targetData != nullptr)
//        {
//            bool found = false;
//
//            // Search for a matching child with the same data
//            for (int i = 0; i < current->childCount; ++i)
//            {
//                if (current->ParNode[i]->data == targetData->getData())
//                {
//                   //  Add new occurrence of position to x and y arrays
//                ParNode[i]->addPosition(targetData->xp, targetData->yp);
//                
//                    // Match found, move to this child
//                    current = current->ParNode[i];
//                    found = true;
//                    break;
//                }
//            }
//
//            // If no match found, add a new child with the target data
//            if (!found)
//            {
//                current->addChild(targetData->xp, targetData->yp, targetData->yp, targetData);              // Adjust x, y, line as needed
//                current = current->ParNode[current->childCount - 1]; // Move to the newly added child
//            }
//
//            // Move to the next character in the linked list
//            targetData = targetData->getright();
//        }
//    }
//};
//
//void displayTree(twoDNode* node, char* initialVal)
//{
//    if (node == nullptr)
//        return;
//
//    // Display current node data
//    std::cout << initialVal << node->data << "\n";
//
//    // Prepare the new initialVal for children
//    int len = 0;
//    while (initialVal[len] != '\0')
//        len++;                           // Find the current initialVal length
//    char* newinitialVal = new char[len + 3]; // Add space for "->" and '\0'
//    for (int i = 0; i < len; ++i)
//    {
//        newinitialVal[i] = initialVal[i];
//    }
//    newinitialVal[len] = '-';
//    newinitialVal[len + 1] = '>';
//    newinitialVal[len + 2] = '\0';
//
//    // Display all children
//    for (int i = 0; i < node->childCount; ++i)
//    {
//        displayTree(node->ParNode[i], newinitialVal);
//    }
//
//    delete[] newinitialVal; // Clean up dynamically allocated memory
//}


//class twoDNode {
//public:
//    int* x;          // Dynamic array to store x positions
//    int* y;          // Dynamic array to store y positions
//    int line;
//    char data;
//    twoDNode** ParNode;   // Array of pointers to children nodes
//    int childCount;       // Current count of children
//    const int maxChildren = 26;  // Maximum limit for children
//    int posCount;         // Count of positions stored in x and y arrays
//
//    twoDNode(int initialX = 0, int initialY = 0, int line = 0, char data = '\0')
//        : line(line), data(data), childCount(0), ParNode(nullptr), posCount(1) {
//        // Initialize x and y arrays with one element initially
//        x = new int[posCount];
//        y = new int[posCount];
//        x[0] = initialX;
//        y[0] = initialY;
//    }
//
//    ~twoDNode() {
//        delete[] x;
//        delete[] y;
//        for (int i = 0; i < childCount; ++i) {
//            delete ParNode[i];
//        }
//        delete[] ParNode;
//    }
//
//    // Resize function to expand the array if needed
//    void resize() {
//        int newSize = childCount + 1;
//        twoDNode** newParNode = new twoDNode * [newSize];
//        for (int i = 0; i < childCount; ++i) {
//            newParNode[i] = ParNode[i];
//        }
//        delete[] ParNode;
//        ParNode = newParNode;
//    }
//
//    // Resize the x and y arrays to store additional positions
//    void addPosition(int newX, int newY) {
//        int* newXArr = new int[posCount + 1];
//        int* newYArr = new int[posCount + 1];
//
//        for (int i = 0; i < posCount; ++i) {
//            newXArr[i] = x[i];
//            newYArr[i] = y[i];
//        }
//
//        newXArr[posCount] = newX;
//        newYArr[posCount] = newY;
//        ++posCount;
//
//        delete[] x;
//        delete[] y;
//        x = newXArr;
//        y = newYArr;
//    }
//
//    // Setters and Getters
//    void setLine(int newLine) { line = newLine; }
//    void setData(char newData) { data = newData; }
//    int getLine() const { return line; }
//    char getData() const { return data; }
//
//    // Add a child node to ParNode, resizing if necessary
//    bool addChild(int childX, int childY, int childLine, Node<char>* childData) {
//        if (childCount >= maxChildren) {
//            std::cout << "Maximum children reached, cannot add more.\n";
//            return false;
//        }
//
//        // Resize if needed
//        resize();
//
//        // Add new child node
//        //int initialX = 0, int initialY = 0, int line = 0, char data = '\0';
//        ParNode[childCount++] = new twoDNode(childX, childY, childLine, childData->getData());
//        return true;
//    }
//
//    // Check if a child node exists with the specified data character
//    // If it exists, recursively find or add to its children
//    void findOrAddChild(Node<char>* targetData) {
//        if (targetData == NULL)
//            return;
//
//        for (int i = 0; i < childCount; ++i) {
//            if (ParNode[i]->data == targetData->getData()) {
//                // Add new occurrence of position to x and y arrays
//                ParNode[i]->addPosition(targetData->xp, targetData->yp);
//                ParNode[i]->findOrAddChild(targetData->getright());
//                return;
//            }
//        }
//
//        std::cout << "Child with data " << targetData << " not found, adding as a new child.\n";
//        addChild(targetData->xp, targetData->yp, targetData->yp, targetData);
//    }
//
//    // Display function to print the tree structure
//    void displayTree(twoDNode* node, int level = 0) {
//        if (node == nullptr) return;
//
//        // Print current node's data, line, and positions
//        std::cout << std::string(level * 2, ' ') << "Node: " << node->getData()
//            << ", Line: " << node->getLine()
//            << ", Positions: ";
//        for (int i = 0; i < node->posCount; ++i) {
//            std::cout << "(" << node->x[i] << ", " << node->y[i] << ") ";
//        }
//        std::cout << "\n";
//
//        // Recursively display each child node at the next level
//        for (int i = 0; i < node->childCount; ++i) {
//            displayTree(node->ParNode[i], level + 1);
//        }
//    }
//};
//
//// Display function to print the tree structure
//void displayTree(twoDNode* node, int level = 0)
//{
//    if (node == nullptr)
//        return;
//
//    // Print current node's data, line, and positions
//    std::cout << std::string(level * 2, ' ') << "Node: " << node->getData()
//        << ", Line: " << node->getLine()
//        << ", Positions: ";
//    for (int i = 0; i < node->posCount; ++i)
//    {
//        std::cout << "(" << node->x[i] << ", " << node->y[i] << ") ";
//    }
//    std::cout << "\n";
//
//    // Recursively display each child node at the next level
//    for (int i = 0; i < node->childCount; ++i)
//    {
//        displayTree(node->ParNode[i], level + 1);
//    }
//}

//////////////////////////////////////////////////////////////////////////////////////////


class LinkedList
{

    Node<char>* head;
    lineSize* lines;

public:
    char* tobeWord;
    twoDNode* TheTree = new twoDNode;
    SentenceTree* STree = new SentenceTree;
    // Initialize currWord as an empty string
      LinkedList()
    {
          tobeWord = new char[1];
    tobeWord[0] = '\0';
        head = NULL;
        lines = NULL;
        
    }
   
    // Linked list to represent currWord
    Node<char>* currWordHead = nullptr;
    Node<char>* currWordTail = nullptr;
    int currLength = 0;  // Track the current length of the word

    // Function to add a character to the end of the list
    void addCharacter(int x, int y, char c) {
        Node<char>* newNode = new  Node<char>(c);
        newNode->xp = x;
        newNode->yp = y;
        if (currWordHead == nullptr) {
            // Initialize head and tail if the list is empty
            currWordHead = newNode;
            currWordTail = newNode;
        }
        else {
            // Add new node to the end of the list
            currWordTail->setright(newNode);
            currWordTail = newNode;
        }
        currLength++;
    }

    // Function to clear the entire list (called when spacebar is entered)
    void clearWord() {
        Node<char>* temp;
        while (currWordHead != nullptr) {
            temp = currWordHead;
            currWordHead = currWordHead->getright();
            delete temp;
        }
        currWordTail = nullptr;
        currLength = 0;
    }

    // Function to remove the last character from the list
    void removeLastCharacter() {
        if (currWordHead == nullptr) return;  // List is already empty

        if (currWordHead == currWordTail) {
            // Only one node in the list
            delete currWordHead;
            currWordHead = nullptr;
            currWordTail = nullptr;
        }
        else {
            // Traverse to the second-to-last node
            Node<char>* temp = currWordHead;
            while (temp->getright() != currWordTail) {
                temp = temp->getright();
            }
            delete currWordTail;
            currWordTail = temp;
            currWordTail->setright(NULL);
        }
        currLength--;
    }


    Node<char>* getHead();
    int sizeOfLine(int y);
    Node<char>* lineHead(int y);
    void incLineSize(int y);
    void decLineSize(int y);
    lineSize* getStartingOfSize();
    void insertAt(int& x, int& y, Node<char>*& cursor, char dat, ptrStack* pStack, bool bl, bool cMoved);
    bool firstCharDeleted = false;

    void removeAt(int& x, int& y, Node<char>*& cursor);


};


bool Cur = false;
Node<char>* LinkedList::getHead()
{
    return head;
}

int LinkedList::sizeOfLine(int y)
{
    lineSize* t = lines;
    for (int i = 0; i < y; i++)
    {
        t = t->next;
    }

    return t->size;
}

Node<char>* LinkedList::lineHead(int y)
{
    lineSize* t = lines;
    for (int i = 1; i < y; i++)
    {
        t = t->next;
    }

    return t->lHead;

}

void LinkedList::incLineSize(int y)
{
    lineSize* t = lines;
    for (int i = 1; i < y; i++)
    {
        t = t->next;
    }

    t->size = (t->size) + 1;
}

void LinkedList::decLineSize(int y)
{
    lineSize* t = lines;
    for (int i = 1; i < y; i++)
    {
        t = t->next;
    }

    t->size = (t->size) - 1;
}

lineSize* LinkedList::getStartingOfSize()
{
    return lines;
}

void putChar(char*& ptr, char c) {
    int len = 0;
    while (ptr[len] != '\0') {
        len++;
    }

    char* temp = new char[len + 2];
    for (int i = 0; i < len; ++i) {
        temp[i] = ptr[i];
    }
    temp[len] = c;
    temp[len + 1] = '\0';

    delete[] ptr;
    ptr = temp;
}

void emptyString(char*& ptr) {
    delete[] ptr;
    ptr = new char[1];
    ptr[0] = '\0';
}


void copyCharArray(const char* to, char*& from) {
    int len = 0;
    while (to[len] != '\0') {
        len++;
    }

    from = new char[len + 1];

    for (int i = 0; i < len; i++) {
        from[i] = to[i];
    }

    from[len] = '\0';
}

void LinkedList::insertAt(int& x, int& y, Node<char>*& cursor, char dat, ptrStack* pStack, bool bl, bool cMoved)
{
    Node<char>* newNode = new Node<char>(dat);
    now = false;
    if ((dat >= 'A' && dat <= 'Z') || (dat >= 'a' && dat <= 'z')||dat==' ')
        addCharacter(x,y,dat);
    if (dat == ' ')
        clearWord();
    if(dat!=' ')
    TheTree->findOrAddChild(currWordHead);


    ///////////////////Adding in sentence tree
    
    

    if (dat == ' ')
    {
        now = true;
        STree->insert(tobeWord);
        copyCharArray(tobeWord, sto);
        emptyString(tobeWord);
       
    }
    else
        putChar(tobeWord, dat);
   

    /////////////////////////

    if (head == NULL)
    {
        head = newNode;
        cursor = newNode;
        lines = new lineSize;
        lines->lHead = head;
        incLineSize(y);
        if (bl)
            pStack->pushChar(dat, x, y, cMoved);
        cMoved = false;
        return;

    }
    int nodeY = 1;
    Node<char>* last = cursor;
    lineSize* tt = lines;
    Node<char>* tr = cursor;
    bool fulWord = false;

    for (int p = 1; p < y; p++)
    {
        tt = tt->next;
        nodeY++;
    }
    //last = head;
    int c = 1;


    if (bl)
        pStack->pushChar(dat, x, y, cMoved);
    cMoved = false;


    if (x == 1 && cursor->getData() == ' ')
    {
        cursor->setData(dat);
        return;
    }

    if (dat == '\n')
    {
        int p = 118 - x;
        for (int i = 0; i < p; i++)
        {
            insertAt(x, y, cursor, ' ', pStack, 1, cMoved);
        }

        tr = tt->lHead;
        while (tr->getright() != NULL)
        {
            tr = tr->getright();
        }
        while (tr->getData() == ' ' && tr->getleft() != NULL)
        {
            if (tr->getlower())
                tr->getlower()->setupper(NULL);
            if (tr->getupper())
                tr->getupper()->setlower(NULL);

            tr = tr->getleft();
            delete tr->getright();
            tr->setright(NULL);
            decLineSize(y - 1);
        }
        //removeAt(x, y, cursor);
        cursor = tt->next->lHead;
        //x = 0;
        //y++;
        //
        return;
    }


    int addSpace = 1;
    int xx = x;
    int yy = y;

    while (tt->next && tt->size >= 117)
    {
        tt = tt->next;
        nodeY++;
    }

    if (tt->size >= 117 && tt->next == NULL)
    {

        last = tt->lHead;
        last->setlower(newNode);
        newNode->setupper(last);
        nodeY++;
        x++;
        if (x >= 117)
        {
            x = 1;
            y++;
        }
        /*x = 1;
        y++;*/

        //y = nodeY;

        lineSize* add = new lineSize;
        tt->next = add;
        add->lHead = newNode;
        add->prev = tt;

        while (last->getright())
        {
            last = last->getright();
        }

        //x = 1;
        //incLineSize(y);
        //cursor = newNode;
        //return;

    }
    else
    {

        last = tt->lHead;

        while (last->getright() != NULL)
        {
            last = last->getright();
        }

        //if (last)
        {
            last->setright(newNode);
            newNode->setleft(last);

            if (last->getupper() && last->getupper()->getright())
            {
                newNode->setupper(last->getupper()->getright());
                last->getupper()->getright()->setlower(newNode);
            }
            x++;
            if (x >= 118)
            {
                x = 1;
                y++;
            }
        }
    }

    // it traverses to connect the bottom line with newly added node in upper line
    if (tt->next)
    {
        Node<char>* up = tt->lHead;
        Node<char>* down = tt->next->lHead;
        while (up && down)
        {
            up->setlower(down);
            down->setupper(up);

            up = up->getright();
            down = down->getright();
        }
    }




    Node<char>* prev = last;
    last = newNode;

    int use = nodeY;

    // now reusing last to traverse back 
    // while last isnt back to cursor
    while (prev != cursor)
    {
        last->setData(prev->getData());
        last = prev;

        if (last->getleft())
            prev = last->getleft();
        else if (lineHead(use)->getupper())
        {
            prev = lineHead(use--)->getupper();
            while (prev->getright() != NULL)
            {
                prev = prev->getright();
            }
        }
    }

    last->setData(dat);
    cursor = last;

    incLineSize(nodeY);
    cursor = last;


    //cursor = newNode;

}

bool firstCharDeleted = false;

void LinkedList::removeAt(int& x, int& y, Node<char>*& cursor)
{
    if (head == NULL)
        return;

    // Case: Single node in the list
    if (head->getright() == NULL && head->getlower() == NULL)
    {
        delete head;
        head = NULL;
        cursor = NULL;
        decLineSize(1);
        x = 1;
        y = 1;

        return;
    }

    int nodeY = 1;
    lineSize* curLine = lines;

    for (int p = 1; p < y; p++)
    {
        curLine = curLine->next;
        //nodeY++;
    }


    //;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    {

        lineSize* delLine = lines;
        lineSize* tempLine = curLine;
        for (int p = 1; p < y; p++)
        {
            delLine = delLine->next;
            nodeY++;
        }
        while (delLine->size == 117 && delLine->next)
        {
            delLine = delLine->next;
            nodeY++;
        }
        /*Node* del = delLine->lHead;
        while (del->getright() != NULL)
        {
            del = del->getright();
        }*/

        Node<char>* nextNode = cursor;
        if (cursor->getright())
            nextNode = cursor->getright();
        else if (cursor->getright() == NULL && curLine->next && curLine->size == 117)
        {
            nextNode = curLine->next->lHead;
            curLine = curLine->next;
        }
        else
        {
            Node<char>* del = cursor;
            if (cursor->getleft())
            {
                cursor = cursor->getleft();
                cursor->setright(NULL);
                x--;
                decLineSize(y);
                if (del->getupper())
                    del->getupper()->setlower(NULL);
                if (del->getlower())
                    del->getlower()->setupper(NULL);
                delete del;
                del = NULL;
                return;
            }
            else
            {
                if (curLine->prev)
                {
                    curLine = curLine->prev;
                    curLine->lHead->setlower(NULL);
                    cursor = curLine->lHead;
                    y--;
                    x = curLine->size;
                    delete curLine->next;
                    curLine->next = NULL;
                    //decLineSize(nodeY);

                    while (cursor->getright() != NULL)
                    {
                        cursor = cursor->getright();
                    }

                    if (del->getupper())
                        del->getupper()->setlower(NULL);
                    if (del->getlower())
                        del->getlower()->setupper(NULL);
                    delete del;
                    del = NULL;
                    return;
                }
            }
        }


        Node<char>* retC = cursor;

        //deleting first node once only


        if (x == 1 && y == 1 && firstCharDeleted)
        {
            return;
        }
        if (x == 1 && y == 1 && firstCharDeleted == false)
        {
            firstCharDeleted = true;
        }


        // Case: Removing from the middle of the line
        while (nextNode != NULL)
        {
            cursor->setData(nextNode->getData());
            if (nextNode->getright())
            {
                cursor = nextNode;
                nextNode = nextNode->getright();
            }
            else if (tempLine->next && tempLine->size == 117)
            {
                cursor = nextNode;
                nextNode = tempLine->next->lHead;
                tempLine = tempLine->next;
            }
            else
            {
                cursor = nextNode;
                nextNode = NULL;
            }

        }

        if (cursor->getleft())
        {
            cursor->getleft()->setright(NULL);
        }
        else if (cursor == head) // If cursor is the head
        {
            head = cursor->getright(); // Move head to the next node
        }

        delete cursor; // Delete the cursor node
        cursor = NULL; // Reset cursor to NULL
        cursor = retC;
        if (cursor->getleft())
        {
            x--;
            cursor = cursor->getleft();
        }
        else if (curLine->prev)
        {
            cursor = curLine->prev->lHead;
            while (cursor->getright())
                cursor = cursor->getright();

            y--;
            x = curLine->prev->size;
        }
        // Decrease line size if needed
        decLineSize(nodeY);
    }

}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ptrStack::pushChar(char ch, int x, int y, bool cMoved) {
    if (ch == ' ' || ch == '\n' || Cur) {
        spacePressed(x, y);
        //Cur = false;
    }
    else {
        sNode<char>* newNode = new sNode<char>(ch);

        if (tail == NULL) {
            head = newNode;
        }
        else {
            tail->next = newNode; // Link to the current word
        }
        tail = newNode;
    }
}

// Function to push the most recent word into another stack and remove it from this stack
void ptrStack::popWord(int& xv, int& yv, ptrStack*& otherStack, bool rem, bool cMoved) {
    if (wordCount == 0) return;

    WordNode* lastWord = wordListTail;
    xv = lastWord->x;
    yv = lastWord->y;

    sNode<char>* temp = lastWord->head;
    while (temp != NULL) {
        otherStack->pushChar(temp->data, xv, yv, cMoved);
        temp = temp->next;
    }
    otherStack->pushChar(' ', xv, yv, cMoved);  // Add a space to indicate end of word

    if (rem)
        removeLastWord();
}

void ptrStack::spacePressed(int x, int y) {
    if (tail == NULL) return;  // No word added yet

    WordNode* newWord = new WordNode(head, x, y);

    if (wordCount == 5) {
        removeFirstWord();
    }

    if (wordListTail == NULL) {
        wordListHead = wordListTail = newWord;
    }
    else {
        wordListTail->next = newWord;
        wordListTail = newWord;
    }

    wordCount++;
    count++;

    head = tail = NULL;
}

void ptrStack::removeFirstWord() {
    if (wordListHead == NULL) return;

    WordNode* temp = wordListHead;
    sNode<char>* node = temp->head;

    while (node != NULL) {
        sNode<char>* nextNode = node->next;
        delete node;
        node = nextNode;
    }

    wordListHead = wordListHead->next;
    delete temp;

    if (wordListHead == NULL) {
        wordListTail = NULL;
    }

    wordCount--;
}

void ptrStack::removeLastWord() {
    if (wordCount == 0) return;

    WordNode* current = wordListHead;
    WordNode* previous = NULL;

    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }

    sNode<char>* temp = current->head;
    while (temp != NULL) {
        sNode<char>* nextNode = temp->next;
        delete temp;
        temp = nextNode;
    }

    delete current;

    if (previous == NULL) {
        wordListHead = wordListTail = NULL;
    }
    else {
        previous->next = NULL;
        wordListTail = previous;
    }

    wordCount--;
}

void ptrStack::printWords() {
    WordNode* current = wordListHead;

    cout << "word count: " << wordCount << endl;
    while (current != NULL) {
        std::cout << '(' << current->x << ',' << current->y << ')';

        sNode<char>* temp = current->head;
        while (temp != NULL) {
            std::cout << temp->data;
            temp = temp->next;
        }
        std::cout << " ";

        current = current->next;
    }
    std::cout << "\n";
}

bool ptrStack::isEmpty() {
    return wordCount == 0 && head == NULL && tail == NULL;
}

void ptrStack::clear() {
    while (wordListHead != NULL) {
        WordNode* wordTemp = wordListHead;
        sNode<char>* charTemp = wordTemp->head;
        while (charTemp != NULL) {
            sNode<char>* nextChar = charTemp->next;
            delete charTemp;
            charTemp = nextChar;
        }

        wordListHead = wordListHead->next;
        delete wordTemp;
    }

    wordListHead = wordListTail = NULL;
    head = tail = NULL;
    wordCount = 0;
    count = 0;

    delete xpos;
    delete ypos;
    xpos = new Node<int>(1);
    ypos = new Node<int>(1);
}

void ptrStack::popLast(LinkedList& stdat, int& x, int& y, Node<char>*& cursor, bool cMoved) {
    if (wordCount == 0) {
        return;
    }

    // Get the last word node
    WordNode* lastWordNode = wordListTail;

    sNode<char>* temp = lastWordNode->head;
    if (cursor == NULL)
    {
        x = 1;
        y = 1;

        while (temp != NULL) {
            stdat.insertAt(x, y, cursor, temp->data, this, 0, cMoved);

            // Add each character to the string
            temp = temp->next;
        }
        stdat.insertAt(x, y, cursor, ' ', this, 0, cMoved);


    }
    else
    {

        x = lastWordNode->x;
        y = lastWordNode->y;

        for (int i = 1; i < y; i++)
        {
            if (cursor->getlower())
            {
                cursor = cursor->getlower();
                //y++;
            }
        }
        for (int i = 1; i < x; i++)
        {
            if (cursor->getright())
            {
                cursor = cursor->getright();
                //x++;
            }
        }

        while (temp != NULL) {
            stdat.insertAt(x, y, cursor, temp->data, this, 0, cMoved);

            temp = temp->next;
        }
        stdat.insertAt(x, y, cursor, ' ', this, 0, cMoved);



    }
    if (wordListHead == wordListTail) {
        delete lastWordNode;
        wordListHead = wordListTail = NULL;
    }
    else {
        WordNode* current = wordListHead;
        while (current->next != wordListTail) {
            current = current->next;
        }

        current->next = NULL;
        delete wordListTail;
        wordListTail = current;
    }

    // Decrease word count
    wordCount--;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void SaveGame(Node<char>* head)
//{
//    ofstream savedGame;
//    savedGame.open("E:\\Uni Content\\Semester_03\\DSA\\Assignment\\Assignment02\\file\\TheText.txt");
//
//    Node<char>* down = head;
//
//    while (down != NULL)
//    {
//        Node<char>* trav = down;
//        while (trav != NULL)
//        {
//            savedGame << trav->getData();
//            trav = trav->getright();
//        }
//        savedGame << endl;
//        down = down->getlower();
//    }
//
//    savedGame.close();
//}
//
//void LoadGame(LinkedList& storedData, Node<char>* cursor, int&x,int&y) {
//    ifstream savedGame;
//    savedGame.open("E:\\Uni Content\\Semester_03\\DSA\\Assignment\\Assignment02\\file\\TheText.txt");
//
//    if (!savedGame) {
//        cout << "Error: Could not open file!" << endl;
//        return;
//    }
//
//    //int y = 1; // Start from line 1
//    //int x = 1; // Start from position 1
//    ////Node<char>* cursor = nullptr;
//
//    char ch;
//    for(int i=0; i<10; i++){
//        savedGame.get(ch);
//        if (ch == '\n') {
//            // If a newline is encountered, move to the next line
//            y++;
//            x = 1; // Reset x position for the new line
//        }
//        else {
//            // Insert each character into the LinkedList at position (x, y)
//            storedData.insertAt(x, y, cursor, ch, nullptr, false, false);
//            x++; // Move to the next position in the line
//        }
//    }
//
//    savedGame.close();
//}

void saveGame(Node<char>* head)
{
    ofstream savedGame;
    savedGame.open("E:\\Uni Content\\Semester_03\\DSA\\Assignment\\Assignment02\\file\\TheText.txt");

    Node<char>* temp = head;

    while (temp != NULL)
    {
        Node<char>* trav = temp;
        while (trav != NULL)
        {
            savedGame << trav->getData();
            trav = trav->getright();

        }

        savedGame << '\n';
        //savedGame << endl;
        temp = temp->getlower();
    }

    savedGame.close();
}

//#include<string>
void LoadGame(LinkedList& storedData, int& x, int& y, Node<char>* cursor, ptrStack*& pStack) {
    ifstream inputFile("E:\\Uni Content\\Semester_03\\DSA\\Assignment\\Assignment02\\file\\TheText.txt");

    if (!inputFile.is_open()) {
        cout << "Failed to open the file." << endl;
        return;
    }

    //int x = 1, y = 1;
    char ch;

    while (inputFile.get(ch)) {
        storedData.insertAt(x, y, cursor, ch, pStack, 1, 0);
        //if (ch == '\0') {
        //    /*y++;
        //    x = 1;*/
        //    storedData.insertAt(x, y, cursor, '\n',pStack, 1, 0);
        //}
        //else {
        //    storedData.insertAt(x, y, cursor, ch, pStack, 1, 0);
        //    //x++;
        //}
    }

    if (cursor->getupper())
    {
        cursor = cursor->getupper();
        y--;
        x = 1;
        while (cursor->getright())
        {
            cursor = cursor->getright();
            x++;
        }
    }
    pStack->clear();
    inputFile.close();
    // cout << "Notepad read successfully." << endl;
}


void drawBox(int width, int height)
{
    // Draw top border
    gotoxy(1, 0);
    for (int i = 1; i < width - 1; ++i) {
        cout << "_";
    }

    // Draw sides
    for (int i = 1; i < height - 1; ++i) {
        gotoxy(0, i);
        cout << "|";
        gotoxy(width - 1, i);
        cout << "|";
    }

    // Draw bottom border
    gotoxy(1, height - 2);
    for (int i = 1; i < width - 1; ++i) {
        cout << "_";
    }

    gotoxy(126, 1);
    cout << "SEARCH WORDS";

    gotoxy(20, 32);
    cout << "SUGGESTION WORDS";

    gotoxy(60, 32);
    cout << "SUGGESTION SENTENCE";


}
int big = 34;

void suggestWords(twoDNode* node, char* initialVal, int initialValLength) {
    if (!node) return;

    // Append current node's character to the initialVal
    initialVal[initialValLength] = node->data;
    initialVal[initialValLength + 1] = '\0';

    // If this node could be the end of a word, display it as a suggestion
    if (node->childCount == 0) { // Assuming no children implies end of a word
        gotoxy(10, big++);
        std::cout << initialVal << std::endl;
    }

    // Recursively explore each child to continue forming words
    for (int i = 0; i < node->childCount; ++i) {
        suggestWords(node->ParNode[i], initialVal, initialValLength + 1);
    }

    // Reset the character for this position after returning
    initialVal[initialValLength] = '\0';
}

char toLowercase(char c) {
    // Convert uppercase letters to lowercase
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

bool isAlphabet(char c) {
    // Check if the character is a letter (case-insensitive)
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void findAndSuggest(twoDNode* root, Node<char>* partialWordHead) {
    twoDNode* current = root;
    Node<char>* temp = partialWordHead;
    char initialVal[100] = {}; // Adjust size as needed

    big = 34;

    // Check for invalid characters in the partial word
    Node<char>* validationNode = partialWordHead;
    while (validationNode) {
        if (!isAlphabet(validationNode->getData())) {
            gotoxy(10, big++);
            std::cout << "Invalid input: Suggestions cannot be made for spaces or special characters." << std::endl;
            return;
        }
        validationNode = validationNode->getright();
    }

    // Navigate to the node representing the last character of the partial word
    while (temp && current) {
        bool found = false;
        for (int i = 0; i < current->childCount; ++i) {
            // Perform a case-insensitive comparison
            if (toLowercase(current->ParNode[i]->data) == toLowercase(temp->getData())) {
                current = current->ParNode[i];
                found = true;
                break;
            }
        }

        if (!found) {
            gotoxy(10, big);
            std::cout << "No suggestions available." << std::endl;
            return;
        }

        temp = temp->getright();
    }

    // Start collecting suggestions from the last matched node
    int initialValLength = 0;
    while (partialWordHead) {
        initialVal[initialValLength++] = partialWordHead->getData();
        partialWordHead = partialWordHead->getright();
    }
    initialVal[initialValLength] = '\0';

    // Recursively find all words from this node
    suggestWords(current, initialVal, initialValLength);
}

void print(Node<char>* h, lineSize* line, ptrStack* pStack, ptrStack* zStack, Node<char>* search, twoDNode* trav, LinkedList TT)
{
    system("cls");
    drawBox(120, 32);
    if (h == NULL)
        return;
    Node<char>* temp = h;
    int x = 1, y = 1;
    gotoxy(x, y);

    Node<char>* sHead = search;
    
    //resetTextColor();

    while (temp != NULL)
    {
        h = temp;
        while (h != NULL)
        {
            /*if (x == 1 && h->getData() == ' ')
            {

                h = h->getright();
                x++;
                continue;
            }*/
            
            cout << h->getData();
            h = h->getright();
            x++;
        }
       
        x = 1;
        y++;
        gotoxy(x, y);
        temp = temp->getlower();
    }

    x = 122;
    y = 3;
    gotoxy(x, y);

    while (search != NULL)
    {
        cout << search->getData();
        search = search->getright();
        x++;
        gotoxy(x, y);
    }

    findAndSuggest(trav, TT.currWordHead);
    if (now)
    {
        //cout << "STO: " << sto << endl;
        TT.STree->suggest(sto);
    }
    /////////////////////////////////////
    /*
    // Reset search to start of search list for comparison
    Node<char>* current = sHead;  // Ensure sHead is set to start of search list
    trav = TT.TheTree;    // Start from root of twoDNode tree
   
    // Traverse through each character in search
    while (current) {
        bool matchFound = false;

        // Check all children of current tree node (trav) for a match
        for (int i = 0; i < trav->childCount; i++) {
            char dataChar = current->getData();
            char treeDataChar = trav->ParNode[i]->data;
       
            // Check if current character in search matches any child node's data (case-insensitive)
            if (treeDataChar == dataChar || treeDataChar == dataChar + 32 || treeDataChar == dataChar - 32) {
                // If a match is found, print '#' at all positions stored in matching node

                for (int posIdx = 0; posIdx < trav->ParNode[i]->posCount; posIdx++) {
                   
                    gotoxy(trav->ParNode[i]->x[posIdx] + 1, trav->ParNode[i]->y[posIdx]);
                    setTextColorBrightGreen();
                    cout << dataChar;
                    resetTextColor();
                }

                // Move to this child node for next character
                trav = trav->ParNode[i];
                current = current->getright();
                matchFound = true;
                break;
            }
        }

        if (!matchFound) {
            gotoxy(125, 8);
            cout << "----NO MATCH----";
            break;
        }
    }
    */


    /*cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
    while (line != NULL)
    {
        cout << line->lHead->getData()<<"  "<<line->size<<"------->";
        line = line->next;
    }
    cout << "\n\npstack:\n";
    pStack->printWords();
    cout << "\n\n zStack:\n";
    zStack->printWords();*/


}

void printDIFF(Node<char>* h, lineSize* line, ptrStack* pStack, ptrStack* zStack, Node<char>* search, twoDNode* trav, LinkedList TT) {
    system("cls");
    drawBox(120, 32);
    if (h == nullptr) return;

    Node<char>* temp = h;
    Node<char>* sHead = search;
    int x = 1, y = 1;
    gotoxy(x, y);

    // Print all characters from linked list h (the main content)
    while (temp != nullptr) {
        Node<char>* lineNode = temp;
        while (lineNode != nullptr) {
            cout << lineNode->getData();
            lineNode = lineNode->getright();
            x++;
        }
        x = 1;
        y++;
        gotoxy(x, y);
        temp = temp->getlower();
    }

    // Print the search term
    x = 122;
    y = 3;
    gotoxy(x, y);
    while (search != nullptr) {
        cout << search->getData();
        search = search->getright();
        x++;
        gotoxy(x, y);
    }
   
    // Reset search to start of search list for comparison
    
    Node<char>* current = sHead;  // Ensure sHead is set to start of search list
    trav = TT.TheTree;    // Start from root of twoDNode tree
    gotoxy(130, 5);
    int ly = 5;
    int ky = -1;
    //cout << "ON LINE: ";
    // Traverse through each character in search
    while (current) {
        bool matchFound = false;

        // Check all children of current tree node (trav) for a match
        for (int i = 0; i < trav->childCount; i++) {
            char dataChar = current->getData();
            char treeDataChar = trav->ParNode[i]->data;
            ly = 5;
            // Check if current character in search matches any child node's data (case-insensitive)
            if (treeDataChar == dataChar || treeDataChar == dataChar + 32 || treeDataChar == dataChar - 32) {
                // If a match is found, print '#' at all positions stored in matching node
               
                for (int posIdx = 0; posIdx < trav->ParNode[i]->posCount; posIdx++) {
                    gotoxy(130, ly);
                    if (trav->ParNode[i]->y[posIdx] != ky)
                    {
                        cout << "ON LINE: ";
                        ky = trav->ParNode[i]->y[posIdx];
                        cout << ky << "  ";
                        ly++;
                    }
                    gotoxy(trav->ParNode[i]->x[posIdx]+1, trav->ParNode[i]->y[posIdx]);
                    setTextColorBrightGreen();
                    cout << dataChar;
                    resetTextColor();
                }

                // Move to this child node for next character
                trav = trav->ParNode[i];
                current = current->getright();
                matchFound = true;
                break;
            }
        }

        if (!matchFound) {
            gotoxy(125, 8);
            cout << "----NO MATCH----";
            break;
        }
    }
}


void setConsoleSize(int width, int height)
{
    // Get the handle to the console window
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Set the screen buffer size
    COORD bufferSize = { static_cast<SHORT>(width), static_cast<SHORT>(height) };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    // Set the window size
    SMALL_RECT windowSize = { 0, 0, static_cast<SHORT>(width - 1), static_cast<SHORT>(height - 1) };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
}

void showWelcomeMessage() {
    // Clear the screen (platform-dependent)
    // system("cls"); // For Windows
    // system("clear"); // For Unix-like systems

    drawBox(119, 15); // Draw a box of width 60 and height 15 for better aesthetics

    gotoxy(40, 2);
    cout << "***** WELCOME TO MY NOTEPAD *****";

    gotoxy(40, 4);
    cout << "&-------------------------------&";
    gotoxy(40, 5);
    cout << "| 1. Create a New File          |";
    gotoxy(40, 6);
    cout << "| 2. Load an Existing File      |";
    gotoxy(40, 7);
    cout << "| 3. Exit the Program           |";
    gotoxy(40, 8);
    cout << "&-------------------------------&";

    gotoxy(40, 11);
    cout << "Please select an option (1-3): ";
}

void clearWord(Node<char>* ww) {
    if (ww == NULL)
        return;
    Node<char>* temp;
    while (ww != nullptr) {
        temp = ww;
        ww = ww->getright();
        delete temp;
    }

}

int main(int argc, char* argv[]) {

    system("cls");
    showWelcomeMessage();

    int choice;
    cin >> choice;

    bool cMoved = false;
    char clicked;
    // Handle user input accordingly


    //system("cls");


    LinkedList StoredData;
    Node<char>* current = NULL;
    current = StoredData.getHead();
    HANDLE  rhnd = GetStdHandle(STD_INPUT_HANDLE);  // handle to read console

    DWORD Events = 0;     // Event count
    DWORD EventsRead = 0; // Events read from console

    ptrStack* pStack = new ptrStack;
    ptrStack* zStack = new ptrStack;

    int x = 1, y = 1;

    bool Running = true;
    int rightLimit = 117;
    int leftLimit = 2;
    int upperLimit = 2;
    int lowerLimit = 28;

    //bool PrintRequired = false;
    int xFilled = 0;
    int yFilled = 0;
    bool cursorMoved = false;

    Node<char>* cursor = NULL;
    Node<char>* currentLine = NULL;

    //Regarding search
    Node<char>* search = NULL;
    Node<char>* searchHead = NULL;
    int tempx = 0, tempy = 0;
    bool searching=false;

    bool czPressed = false;

    switch (choice) {
    case 1:
        cout << "\n*** Creating a new file... ***";
        system("cls");

        // Logic for creating a new file
        break;
    case 2:
        cout << "\n*** Loading an existing file... ***";
        // Logic for loading an existing file
        LoadGame(StoredData, x, y, cursor, pStack);
        //LinkedList idk;
        //print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, NULL,NULL,idk);
        gotoxy(x, y);
        currentLine = StoredData.lineHead(y);

        break;

    case 3:
        cout << "\n*** Exiting the program... ***";
        Running = false;
        // Logic for exiting
        break;
    default:
        cout << "\n!!! Invalid option. Please try again. !!!";
        break;
    }


    //system("cls");
    // Fix the console size to 80x25 (for example)
    setConsoleSize(150, 40);
    //80% of 150 is 120
    //80% of 150 is 32
    drawBox(120, 32);

    gotoxy(x, y);
    //programs main loop
    while (Running) {
        //cout << "in\n";

        // gets the systems current "event" count
        GetNumberOfConsoleInputEvents(rhnd, &Events);

        if (Events != 0) { // if something happened we will handle the events we want

            //cout<<"in\n";
            // create event buffer the size of how many Events
            INPUT_RECORD eventBuffer[200];

            // fills the event buffer with the events and saves count in EventsRead
            ReadConsoleInput(rhnd, eventBuffer, Events, &EventsRead);

            // loop through the event buffer using the saved count
            for (DWORD i = 0; i < EventsRead; ++i) {
                //PrintRequired = false;
                // check if event[i] is a key event && if so is a press not a release
                if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown) {

                    char key = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;

                    // Check if the key is an alphabet (small or capital), space, backspace, or enter
                    if ((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || key == ' ' || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_BACK || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_UP || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_DOWN || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RIGHT || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_LEFT || eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_CONTROL) || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_DELETE) || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_INSERT) || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_CAPITAL) || (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE)) // enter key 
                    {
                        // Adjusted part for search handling and reset
                        if (searching) {
                            if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_RETURN) {
                                Node<char>* current = searchHead;
                                while (current != nullptr) {
                                    Node<char>* nextNode = current->getright();
                                    delete current;
                                    current = nextNode;
                                }
                                searchHead = nullptr;
                                searching = false;
                                break;
                            }
                            else if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_BACK) {
                                // Handle BACKSPACE deletion logic
                            }

                            // Add new character to search list
                            clicked = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                            if (clicked == ' ')
                            {
                                clearWord(searchHead);
                                searchHead = NULL;
                                break;
                            }
                            Node<char>* com = new Node<char>(clicked);

                            if (searchHead == nullptr) {
                                searchHead = com;
                            }
                            else {
                                Node<char>* temp = searchHead;
                                while (temp->getright() != nullptr) {
                                    temp = temp->getright();
                                }
                                temp->setright(com);
                            }
                            twoDNode* trav = StoredData.TheTree;
                            // **Reset trav and current to the start for each new search**
                            current = searchHead;
                            trav = StoredData.TheTree; // Reset to root node of the tree for each search
                            printDIFF(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, trav, StoredData);
                            continue;
                        }

                        
                        // check if the key press was an arrow key
                        switch (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode) {

                        case VK_UP: //up
                            if (currentLine && currentLine->getupper())
                            {
                                cMoved = true;
                                Cur = true;
                                if (cursor && cursor->getupper())
                                {
                                    cursor = cursor->getupper();
                                }
                                else
                                {
                                    cursor = currentLine->getupper();
                                    x = 1;
                                    while (cursor->getright() != NULL)
                                    {
                                        cursor = cursor->getright();
                                        x++;
                                    }

                                }
                                y--;
                                gotoxy(x, y);
                                currentLine = StoredData.lineHead(y);
                                //updateCurrent(x, y, current);

                            }
                            StoredData.firstCharDeleted = false;
                            break;


                        case VK_DOWN: //down
                            if (currentLine && currentLine->getlower() != NULL)
                            {
                                cMoved = true;
                                //Cur = true;
                                if (cursor && cursor->getlower())
                                {
                                    cursor = cursor->getlower();
                                }
                                else
                                {
                                    cursor = currentLine->getlower();
                                    x = 1;
                                    while (cursor->getright() != NULL)
                                    {
                                        cursor = cursor->getright();
                                        x++;
                                    }

                                }
                                y++;
                                gotoxy(x, y);
                                currentLine = StoredData.lineHead(y);
                                //updateCurrent(x, y, current);
                            }
                            StoredData.firstCharDeleted = false;
                            break;
                        case VK_RIGHT: //right
                            if (cursor)
                            {
                                cMoved = true;
                                Cur = true;
                                if (cursor->getright() != NULL)
                                {
                                    x++;
                                    cursor = cursor->getright();
                                    gotoxy(x, y);
                                }
                                else if (currentLine->getlower() != NULL)
                                {
                                    cursor = currentLine->getlower();
                                    y++;
                                    x = 1;
                                    gotoxy(x, y);
                                    currentLine = StoredData.lineHead(y);
                                }
                            }
                            StoredData.firstCharDeleted = false;
                            break;
                        case VK_LEFT: //left
                            if (cursor)
                            {
                                cMoved = true;
                                Cur = true;
                                if (cursor->getleft() != NULL)
                                {
                                    x--;
                                    cursor = cursor->getleft();
                                    gotoxy(x, y);
                                }
                                else if (currentLine && currentLine->getupper())
                                {
                                    cursor = currentLine->getupper();
                                    y--;
                                    x = 1;
                                    while (cursor->getright() != NULL)
                                    {
                                        cursor = cursor->getright();
                                        x++;
                                    }

                                    //x = StoredData.sizeOfLine(y);
                                    gotoxy(x, y);
                                    currentLine = StoredData.lineHead(y);
                                }
                            }
                            StoredData.firstCharDeleted = false;
                            break;

                        case VK_BACK: //backspace
                        {

                            StoredData.removeAt(x, y, cursor);
                            //PrintRequired = true;
                           //x--;
                           // gotoxy(x, y);
                            print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, StoredData.TheTree, StoredData);
                            gotoxy(x, y);
                            currentLine = StoredData.lineHead(y);
                            break;
                        }

                        case VK_RETURN:
                        {
                            if (y < 29)
                            {
                                StoredData.insertAt(x, y, cursor, '\n', pStack, 1, cMoved);
                                print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, StoredData.TheTree, StoredData);
                                gotoxy(x, y);
                                currentLine = StoredData.lineHead(y);

                            }
                            break;
                        }
                        
                        case VK_DELETE:
                        {
                            if (pStack->isEmpty() == false)
                            {
                                czPressed = true;
                                int cx = 0, cy = 0;
                                pStack->popWord(cx, cy, zStack, 1, cMoved);
                                print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead,StoredData.TheTree, StoredData);
                                cursor = StoredData.getHead();
                                /*x = cx;
                                y = cy;*/
                                x = 1;
                                y = 1;

                                cout << "cx:" << cx << endl;
                                cout << "cy:" << cy << endl;

                                for (int i = 1; i < cx; i++)
                                {
                                    if (cursor->getright())
                                    {
                                        cursor = cursor->getright();
                                        x++;
                                    }
                                }

                                for (int i = 1; i < cy; i++)
                                {
                                    if (cursor->getlower())
                                    {
                                        cursor = cursor->getlower();
                                        y++;
                                    }
                                }

                                int len = 0;
                                while (cursor->getData() != ' ' || (y == 1 && cursor == NULL))
                                {
                                    if (cursor->getleft() == NULL)
                                    {
                                        if (cursor->getupper())
                                        {
                                            cursor = cursor->getupper();

                                            while (cursor->getright())
                                                cursor = cursor->getright();
                                        }
                                        else
                                        {
                                            StoredData.removeAt(x, y, cursor);
                                            break;
                                        }
                                    }
                                    StoredData.removeAt(x, y, cursor);
                                }
                                if (cursor)
                                    StoredData.removeAt(x, y, cursor);


                                /*for (int i = 0; i < len; i++)
                                {
                                    StoredData.removeAt(x, y, cursor);
                                }*/

                                print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, StoredData.TheTree, StoredData);
                                gotoxy(x, y);
                                currentLine = StoredData.lineHead(y);


                            }
                            break;


                        }
                        case VK_INSERT:
                        {
                            //dummy variables
                            int o, m;
                            cursor = StoredData.getHead();
                            zStack->popWord(o, m, pStack, 0, cMoved);
                            zStack->popLast(StoredData, x, y, cursor, cMoved);
                            x = 1;
                            cursor = StoredData.lineHead(y);
                            while (cursor->getright())
                            {
                                cursor = cursor->getright();
                                x++;
                            }
                            print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, StoredData.TheTree, StoredData);
                            gotoxy(x, y);
                            currentLine = StoredData.lineHead(y);
                            break;
                        }

                        case VK_ESCAPE:
                        {
                            Running = false;
                            saveGame(StoredData.getHead());
                            break;
                        }
                        case VK_CAPITAL:
                        {
                            //cout << "\n\nsaving\n\n";
                            char* pp = new char;
                            *pp = '\n';
                            ;displayTree(StoredData.TheTree, pp);
                            saveGame(StoredData.getHead());

                            ;cout << "\n\n\n\n";
                            ;StoredData.STree->print();
                            break;
                        }
                        case VK_CONTROL:
                        {
                            tempx = x;
                            tempy = y;

                            searching = true;

                           /* char* pp = new char;
                            *pp = ' ';
                            displayTree(StoredData.TheTree, pp);*/
                            break;
                           /* system("cls");
                            StoredData.TheTree->displayChildren();
                            Sleep(15000);*/

                        }
                        default:
                            clicked = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;

                            if (y < 30)
                            {
                                StoredData.insertAt(x, y, cursor, clicked, pStack, 1, cMoved);

                                if (czPressed)
                                {
                                    zStack->clear();
                                    czPressed = false;
                                }

                                currentLine = StoredData.lineHead(y);
                                /*x++;
                                if (x > rightLimit)
                                {
                                   y++;
                                   x = leftLimit - 2;
                                }*/

                                print(StoredData.getHead(), StoredData.getStartingOfSize(), pStack, zStack, searchHead, StoredData.TheTree, StoredData);
                                gotoxy(x, y);

                            }
                            //cMoved = false;
                            break;
                        }
                    }
                }
            } // end EventsRead loop
        }
    }
    return 0;
}
