#pragma once
/*
*Author: Turan Mert Duran
*Date: 23.05.2020
*CS 202- Section 1
* Homework 5
* 21601418
*/
#include <stdio.h>
#include <string>
using namespace std;
class FriendNet {

public:
    FriendNet(const string fileName); // Default Constructor
    FriendNet(const FriendNet& aNet); // Copy-Constructor
    ~FriendNet(); // Destructor

    void listFriends(const string personName, const int hopNo); // Prints priends of Person with hopNo distance
    void displayAverageDegrees(); // Displays average degrees of each components
    void displayDiameters(); // Displays longest distance of shortest paths of each components
    void search(int hNo, int pId, bool*& array, int arraySize); // searches friends of person with given distance
    void compSelector(int id, int count, int compId);  // Arranges Components of people
    void printFriends(const int pId, const int hNo, const string personName); // Prints Friend ( Recursive used in listFriends)
    void componentFind();// Finds which component for people

private:

    struct Node {
        int id = -1; // Holds id numbers of people
        string name = "UNKNOWN";    // holds name of people
        int friendCount = 0; // holds friend counts of people
        Node* next = NULL; // Next pointer that points friends
        int* distances; // Holds shortest distance to other people in Components
    };

    Node **head; // That point the beginning of people
    int size; // Holds people count 
    int* compList; // Holds which people from which component
};