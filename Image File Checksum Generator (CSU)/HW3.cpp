//doccument beginning
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <typeinfo>
#include <stdlib.h>
#include <vector>
#include <stdexcept>
using namespace std;
int numRows;
int numCols;
int maxPixel;
//print functions
void println(string str) {
    std::cout << str << std::endl;
}

void printError(string str2) {
    std::cerr << str2 << std::endl;
}

int retrieveHeader(string img) {
    ifstream inputFile(img);
    int currVal;
    string line;
    char header[2];
    inputFile.read(header, 2);
    //check value of header
    if (!(header[0] == 'P' && header[1] == '6')) {
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
    //clean up/reset pointer
    inputFile.close();
    return 0;
}

int checkNumItems(string img) {
    int numItems = 0;
    ifstream checkFile(img);
    char dummy[100];
    while (checkFile >> dummy) {
        numItems++;
    }
    numItems -=4;
    if (numItems != (numCols*3 * numRows)) {  
        println("Items Needed: ");
        std::cout << numCols*3 * numRows << std::endl;
        println("Items Counted: ");
        std::cout << numItems << std::endl;
        printError("Incorrect number of items");
        return -1;
    }
    return 0;
    checkFile.close();
}

int verifyPixelNums(string img) {
    ifstream readF(img);
    char dummy[100];
    //skip header
    for (int i = 0; i < 4; i++) {
        readF >> dummy;
    }
    for (int j = 0; j < numRows * numCols*3; j++) {
    char readValue[20];
    readF >> readValue;
    int readInt = std::stoi(readValue);
    if (readInt > maxPixel) {
        printError("Pixel value out of range");
        println("Pixel OOR value: ");
        std::cout << readInt << std::endl;
        println("max pixel value: ");
        std::cout << maxPixel << std::endl;
        return -1;
    }
    }
    return 0;


    //clean up
    readF.close();
}

void readFile(string img, string rowsC, string colsC) {
    ifstream readF(img);
    char dummy[100];
    //skip header
    for (int k = 0; k < 4; k++) {
        readF >> dummy;
    }

    std::vector<std::vector<int>> data(numRows, std::vector<int>(numCols*3));
    int currVal;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols*3; j++) {
            readF >> currVal;
            data[i][j] = currVal;
            //data is listed row, then column
        }
    }
    readF.close();
    //begin writing portion
    ofstream writeRows(rowsC);
    ofstream writeCols(colsC);
    char whitespace = ' ';

    //calculate and write checks for each row
    for (int ii = 0; ii < numRows; ii++) {
        //inner loop calculates total values for each row
        int totalR = 0;
        for (int jj = 0; jj < numCols*3; jj++) {
            totalR += data[ii][jj];
        }
        writeRows << totalR;
        writeRows << whitespace;
    }
    writeRows.close();

    //calculate and write checks for all columns
    for (int iii = 0; iii < numCols; iii++) {
        int totalC = 0;
        for (int jjj = 0; jjj < numRows; jjj++) {
            int rowTotal = 0;
            int minValue = iii*3;
            int maxValue = iii*3 + 3;
            for (int x = minValue; x < maxValue; x++) {
                rowTotal += data[jjj][x];
            }
            totalC += rowTotal;

        }
        writeCols << totalC;
        writeCols << whitespace;
    }
    writeCols.close();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printError("Incorrect number of arguments");
        return -1;
    }
    string image = argv[1];
    //row wise then columnwise
    string rowTXT = argv[2];
    string colTXT = argv[3];
    //retrieve file header
    retrieveHeader(image);
    if (typeid(image) != typeid(string) && typeid(rowTXT) != typeid(string) && typeid(colTXT) != typeid(string)) {
        printError("Arguments not of correct type");
        return -1;
    }
    //check number of items in file
    int checkItems = checkNumItems(image);
    int verifyPixels = verifyPixelNums(image);

    if (checkItems != -1 && verifyPixels != -1) {
        //if correct, read the file
        readFile(image, rowTXT, colTXT);
    } else {
        return -1;
    }
    return 0;

}