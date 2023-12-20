#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
void readData(const string& fileName, vector<double>& pathAngleVec, vector<double>& coeffVec) {
    ifstream inData(fileName);
    if(!inData.is_open()) {
        cout << "Error opening " << fileName << endl;
        exit(1);
    }
    double pathAngle, coeff;
    while (inData >> pathAngle >> coeff) {
        pathAngleVec.push_back(pathAngle);
        coeffVec.push_back(coeff);
    }
    inData.close();
}
double interpolation(double inputAngle, const vector<double> & pathAngleVec, const vector<double> & coeffVec) {
    if (inputAngle < pathAngleVec.at(0) || inputAngle > pathAngleVec.at(pathAngleVec.size() - 1)) {
        cout << "Error";
        return 1;
    }
    double angleA;
    double angleC;
    double angleB;
    double fa;
    double fc;
    double outputCoeff;
    for (unsigned int i = 0; i < coeffVec.size(); i++) {
        if (inputAngle == pathAngleVec.at(i)) {
            outputCoeff = coeffVec.at(i);
            return outputCoeff;
        }
    }
    unsigned int i = 0;
    for (i = 0; i < pathAngleVec.size(); i++) {
        if (pathAngleVec.at(i) > inputAngle) {
            break;
        }
    }
    angleA = pathAngleVec.at(i-1);
    angleC = pathAngleVec.at(i);
    angleB = inputAngle;
    fa = coeffVec.at(i-1);
    fc = coeffVec.at(i);

    outputCoeff = fa + ((angleB - angleA) / (angleC - angleA)) * (fc - fa);
    return outputCoeff;
}
bool isOrdered(const vector<double> & anyVect) {
    bool ordered = true;
    for (unsigned int i = 1; i < anyVect.size(); i++) {
        if (anyVect.at(i) < anyVect.at(i - 1)) {
            ordered = false;
            break;
        } 
    }
    return ordered;
}

void reorder(vector<double> &pathAngleVec, vector<double> &coeffVec) {
    for (int i = 0; i < (int) pathAngleVec.size() - 1; i++) {
        int indexSmallest = i;
        for (int j = i + 1; j < (int) pathAngleVec.size(); j++) {
            if (pathAngleVec.at(j) < pathAngleVec.at(indexSmallest)) {
                indexSmallest = j;
            }
        }
        double temp = pathAngleVec.at(i);
        double temp2 = coeffVec.at(i);
        pathAngleVec.at(i) = pathAngleVec.at(indexSmallest);
        coeffVec.at(i) = coeffVec.at(indexSmallest);
        pathAngleVec.at(indexSmallest) = temp;
        coeffVec.at(indexSmallest) = temp2;
    }
}

int main(int argc, char *argv[]) {
    string fileName;
    if (argc !=2) {
        cout << "Error" << endl;
        return 1;
    }
    fileName = argv[1];
    vector <double> pathAngleVec;
    vector <double> coeffVec;
    readData(fileName, pathAngleVec, coeffVec);
    if (isOrdered(pathAngleVec) == false) {
        reorder (pathAngleVec, coeffVec);
    }
    
    double inputAngle;
    string response;
    do {
        cout << "Enter an input angle: ";
        cin >> inputAngle;
        double finalVal = interpolation(inputAngle, pathAngleVec, coeffVec);
        cout << finalVal << endl;
        cout << "Do you want to enter in another value?" << endl;
        cin >> response;
    } while (response == "YES" || response == "Yes" || response == "yes");


    
    return 0;
}