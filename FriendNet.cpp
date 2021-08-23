#include "FriendNet.h"
#include <fstream>
#include <iostream>
/*
*Author: Turan Mert Duran
*Date: 23.05.2020
*CS 202- Section 1
* Homework 5
* 21601418
*/
// Default Constructor
FriendNet::FriendNet(const string fileName)
{
    ifstream input;
    input.open(fileName);
    if (!input) {
        head = NULL;
        size = 0;
    }
    else {
           input >> size;
           head = new Node*[size];
          for (int i = 0; i < size; i++)
             head[i] = new Node();
          for (int i = 0; i < size; i++) {
            int pid; 
            string pName;
            int fCount;
            int fId;  
            input >> pid;
            input >> pName;
            input >> fCount;
            head[i]->friendCount = fCount;
            head[i]->name = pName;
            head[i]->id = pid;
            Node* cur;
            cur = head[i];
           for (int j = 0; j < fCount; j++) {
                cur->next = new Node;
                cur = cur->next;
                input >> fId;
                cur->id = fId;
                cur->friendCount = -1;
                cur->name = "unknown ( friend )";
                cur->next = NULL;
            }
            cur->next = NULL;
            
        }
       

    }
    input.close();
}
// Copy-Constructor
FriendNet::FriendNet(const FriendNet& aNet)
{
    if (aNet.size == 0) {
        size = 0;
        head = NULL;
    }else{
        head = new Node * [aNet.size];
        size = aNet.size;
        for (int i = 0; i < size; i++)
            head[i] = new Node();
        for (int i = 0; i < size; i++) {
            head[i]->name = aNet.head[i]->name;
            head[i]->friendCount = aNet.head[i]->friendCount;
            head[i]->id = aNet.head[i]->id;
            Node* cur;
            cur = head[i];
            Node* copy;
            copy = aNet.head[i];
            for (int j = 0; j < head[i]->friendCount; j++) {
                cur->next = new Node;
                cur = cur->next;
                copy = copy->next;
                cur->id = copy->id;
                cur->friendCount = aNet.head[copy->id]->friendCount;
                cur->name = aNet.head[copy->id]->name;
                cur->next = NULL;
            }
        }
    }
}
// Destructor
FriendNet::~FriendNet() {
    if (head != NULL) {
        for (int i = 0; i < size; i++) {
            Node* cur;
            cur = head[i];
            delete[] cur->distances;
            while (cur != NULL) {
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
        }
    }
   
    delete[] head;
    delete[] compList;
}
// Prints priends of Person with hopNo distance
void FriendNet::listFriends(const string personName, const int hopNo)
{

    if (hopNo < 1) {
        cout << "People accessible from " << personName << " within " << hopNo << " hops: NOBODY" << endl;
        return;
    }
    bool found = false;
    int index;
    for (int i = 0; i < size; i++) {
        
        if (head[i]->name == personName) {
            found = true;
            index = i;
        }
    }
    if (found != true) {
        cout << personName << " does not exist in the network." << endl;
        return;
    }
    else {
        cout << "People accessible from " << personName << " within " << hopNo << " hops: ";

        int nHopNo;
        if (hopNo > size) {
            nHopNo = size;
        }
        else {
            nHopNo = hopNo;
        }
        bool* arr = new bool[size];
        for (int i = 0; i < size; i++)
            arr[i] = false;
        search(nHopNo, index, arr, size);
        for (int i = 0; i < size; i++) {
            if (arr[i] != false && i != index) {
                cout << head[i]->name << " ";
            }
        }
        delete[] arr;
    }
    cout << endl;

}
// Displays average degrees of each components
void FriendNet::displayAverageDegrees()
{
    componentFind();
    int compSize = -1;
    for (int i = 0; i < size; i++) {
        if (compList[i] > compSize)
            compSize = compList[i];
    }
    cout << "There are " << compSize << " connected components. The average degrees are: " << endl;
    int* comp = new int[compSize];
    int* fSize = new int[compSize];
    for (int i = 0; i < compSize; i++){
        comp[i] = 0;
        fSize[i] = 0;
    }
   for (int i = 0; i < size; i++) {
       int index = compList[i] - 1;
       comp[index] = comp[index] + head[i]->friendCount;
       fSize[index]++;
   }
   for (int i = 0; i < compSize; i++) {
       double average = (double)comp[i] / (double)fSize[i];
       cout << "For component " << i << " : " << average << endl;
   }
   delete[] comp;
   delete[] fSize;
}
// Displays longest distance of shortest paths of each components
void FriendNet::displayDiameters()
{
    componentFind();
    int compSize = -1;
    for (int i = 0; i < size; i++) {
        if (compList[i] > compSize)
            compSize = compList[i];
    }

    int* comp = new int[compSize];
    int* fSize = new int[compSize];
    for (int i = 0; i < compSize; i++) {
        comp[i] = 0;
        fSize[i] = 0;
    }
    for (int i = 0; i < size; i++) {
        int index = compList[i] - 1;
        comp[index] = comp[index] + head[i]->friendCount;
        fSize[index]++;
    }

    for (int i = 0; i < size; i++) {
        head[i]->distances = new int[size];
        for (int j = 0; j < size; j++) {
            head[i]->distances[j] = -1;
        }
    }


    bool* arr;
    for (int i = 0; i < size; i++) {
        arr = new bool[size];
        int index = compList[i] - 1;
        int newSize = fSize[index];
        for (int j = 0; j < newSize; j++) {
            search(j, i, arr, size);
            for (int k = 0; k < size; k++) {
                if (arr[k] == true && head[i]->distances[k] == -1) {
                    head[i]->distances[k] = j;
                }
            }
        }
        delete[] arr;
    }
    cout << "There are " << compSize << " connected components. The diameters are: " << endl;
    for (int i = 0; i < compSize; i++) {
        int whichComp = i + 1;
        int total = 0;
        for (int j = 0; j < size; j++) {
            if (compList[j] == whichComp) {
                for (int k = 0; k < size; k++) {
                    if (head[j]->distances[k] > total && j != k)
                        total = head[j]->distances[k];
                }
            }
        }
        cout <<  "For component " << i << " : " << total << endl;
    }
    
    delete[] comp;
    delete[] fSize;
}
// Prints Friend ( Recursive used in listFriends)
void FriendNet::printFriends(const int pId, const int hNo, const string personName)
{
    if (hNo <1 )
        return;
    Node* cur;
    cur = head[pId];
    while (cur->next != NULL) {
        cur = cur->next;
        int id = cur->id;
        int hnumber = hNo - 1;
        printFriends(id, hnumber,  personName);
        int index = cur->id;
    }
  }
// searches friends of person with given distance
void FriendNet::search(int hNo, int pId, bool*& array, int arraySize)
{
    if(hNo < 1)
        return;
    if (hNo > size)
        hNo = size;
    Node* cur = head[pId];
    while (cur->next != NULL) {
        cur = cur->next;
        int id = cur->id;
        array[id] = true;
        int hnumber = hNo - 1;
        search(hnumber, id, array, arraySize);
    }
}
// Arranges Components of people
void FriendNet::compSelector(int id, int count, int compId) {
    if (count < 1)
        return;
    Node* cur;
    cur = head[id];
    compList[id] = compId;
    while (cur->next != NULL) {
        cur = cur->next;
        id = cur->id;
        compList[id] = compId;
        count--;
        compSelector(id, count, compId);
    }
}
// Finds which component for people
void FriendNet::componentFind()
{
    int compCounter = 1;
    compList = new int[size];
    for (int i = 0; i < size; i++)
        compList[i] = 0;
    for (int i = 0; i < size; i++) {
        if (compList[i] == 0) {
            compList[i] = compCounter;
            compSelector(i, size, compCounter);
            compCounter++;
        }
    }
}


