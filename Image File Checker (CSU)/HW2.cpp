#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <stdlib.h>
using namespace std;
int numCols;
int numRows;
int maxPixel;


//print functions because iostream is annoying
void println(string str) {
    std::cout << str << std::endl;
}

void printError(string str2) {
    std::cerr << str2 << std::endl;
}

int checkImageFormatting(string img) {
    //read through first line to find P3, rows, cols
    ifstream inputFile(img);
    int currVal;
    string line;
    char header[2];
    inputFile.read(header, 2);
    //check value of header
    if (!(header[0] == 'P' && header[1] == '3')) {
        return -1;
    }
    for (int i = 0; i < 3; i++) {
    inputFile >> currVal;
    if (i == 0) {
        numCols = currVal;
    }
    if (i == 1) {
        numRows = currVal;
    }
    if (i == 2) {
        maxPixel = currVal;
    }
    }
    //count number of rows and columns
    char checkLine[100];
    int dataCount = 0;
    int rowCount;
    //read total # of values
    while(inputFile >> checkLine) {
        dataCount++;
    }
    rowCount = dataCount/(numCols*3);
    //check correct number of rows
    if (rowCount != numRows) {
        printError("Incorrect number of rows");
        return -1;
    }
    inputFile.close(); //
    ifstream inputFile2(img); //reset file pointer to beginning
    int totalValuesCount = 0;
    char dummyCounter[3];
    bool checkValidRange = true;
    while(inputFile2 >> dummyCounter) {
        int checkInt;
        std::istringstream (dummyCounter) >> checkInt;
        if (checkInt > maxPixel) {
            printError("Pixel Value Out of Range");
            checkValidRange = false;
        }
        totalValuesCount++;
    }
    totalValuesCount = totalValuesCount - 4;
    if (!checkValidRange) {
        inputFile2.close();
        return -1;
    }
    if (totalValuesCount != numCols * numRows * 3) {
        printError("Incorrect Number of Columns");
        inputFile2.close();
        return -1;
    }
    inputFile2.close();
    return 0;

}
int checkChecksums (string check) {
    ifstream checkFile(check);
    int numChecks = 0;
    char dummy[10];
    while (checkFile >> dummy) {
        numChecks++;
    }
    if (numChecks != numRows) {
        checkFile.close();
        printError("Incorrect number of checksums");
        return -1;
    }
    return 0;
    checkFile.close();

}

int readFile(string img, string check) {
    //reminder that you have #cols and #rows
    ifstream imageFile(img);
    ifstream checkFile(check);
    //skips header
    char dummy[100];
    for (int k = 0; k < 4; k++) {
        imageFile >> dummy;
    }
    //iterates through rows, minus header
    for (int i = 0; i < numRows; i++) {
        int checkVal = 0;
        int checkSum;
        checkFile >> checkSum;
        //goes through and adds all values in row
        for (int j = 0; j < numCols*3; j++) {
            int currVal;
            imageFile >> currVal;
            checkVal += currVal;
        }
        if (checkVal != checkSum) {
            printError("Checksum Incorrect, Row: ");
            std::cout << -(i+2) << std::endl;
            return -(i+2);
        }

    }
    //some kind of outer loop for iterating rows, set max i value to #rows
    //some kind of inner loop for iterating columns, set max j value to #cols
    imageFile.close();
    checkFile.close();
    return 0;
}
//return 0 for success
//return -1 for format errors
//return -(R+2) for invalid sum where R is the row with an incorrect checksum
int main(int argc, char* argv[]) {
    int returnValue = 0;
    println("Program Start");
    string fileName = argv[1];
    string checksum = argv[2];

    //formatting checks
    if (argc > 3) {
        printError("Incorrect Number of Arguments");
        return -1;
    }
    if (typeid(fileName) != typeid(string) && typeid(checksum) != typeid(string)) {
        printError("Arguments not of correct type");
        return -1;
    }
    
    int validNumReturn = checkImageFormatting(fileName);
    if (validNumReturn != 0) {
        return validNumReturn;
    }
    int validChecksum = checkChecksums(checksum);
    if (validChecksum != 0) {
        return validChecksum;
    }
    int validReadFile = readFile(fileName, checksum);
    if (validReadFile != 0) {
        return validReadFile;
    }
    println("File Correct");
    return 0;
}