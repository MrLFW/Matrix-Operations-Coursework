// MyMatLb.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
using namespace std;

// matlib.cpp : A simple matrix library program.
// Written by Prof Richard Mitchell    7/1/22
//

struct myMat
{                // allows for matrices up to size 4*4
    int numRows; // number of rows
    int numCols;
    int data[16]; // data are stored in row order
};

myMat zeroMat(int r, int c)
{
    // create a matrix with r rows and c columns, filled with zeros
    myMat m;       // define matrix
    m.numRows = r; // set size
    m.numCols = c;
    for (int ct = 0; ct < 16; ct++)
        m.data[ct] = 0; // set elems to 0
    return m;           // return matrix
}

int getElem(myMat m, int r, int c)
{
    // return the item at m[r,c]   where r is 0..m.numRows-1 etc
    return m.data[r * m.numCols + c];
}

void setElem(myMat &m, int r, int c, int val)
{
    // set element m[r,c] to val
    m.data[r * m.numCols + c] = val;
}

myMat mFromStr(string s)
{
    // create a matrix from string s
    // string of form "1,2;3,4;5,6"   ; separates rows and , separates columns ... No error check
    int ms;
    if (s.length() > 0)
        ms = 1;
    else
        ms = 0;
    myMat m = zeroMat(ms, ms); // is s empty create 0*0 matrix, else start with 1*1 matrix
    int mndx = 0;              // used to index into array
    string sub = "";           // sub string - numbers between , or ; set empty
    for (int ct = 0; ct < s.length(); ct++)
    { // each char in turn
        if ((s[ct] == ',') || (s[ct] == ';'))
        {                               // if , or ; then sub contains number
            m.data[mndx++] = stoi(sub); // convert number to integer, put in data array
            sub = "";                   // clear sub string
            if (s[ct] == ';')
                m.numRows++; // if found ; indicates an extra row
            else if (m.numRows == 1)
                m.numCols++; // if , then (if in row 1) increase count of columns
        }
        else
            sub = sub + s[ct]; // add character to sub string
    }
    if (sub.length() > 0)
        m.data[mndx++] = stoi(sub); // add last sub string
    return m;
}

myMat mGetRow(myMat m, int row)
{
    // create a matrix from m, having one row
    myMat res = zeroMat(1, m.numCols);        // create a matrix of 1 row
    for (int col = 0; col < m.numCols; col++) // for each element in row
        res.data[col] = getElem(m, row, col); // copy col element to res
    return res;
}

myMat mGetCol(myMat m, int col)
{
    // create a matrix from m, having one col
    myMat res = zeroMat(m.numRows, 1);        // create a matrix of 1 col
    for (int row = 0; row < m.numRows; row++) // for each element in col
        res.data[row] = getElem(m, row, col); // copy row element to res
    return res;
}

int dotProd(myMat v1, myMat v2)
{
    int product = 0;
    for (int i = 0; i < v1.numRows; i++)             // loops through the rows
        product = product + v1.data[i] * v2.data[i]; // product of v1 and v1 row found then added to total
    return product;                                  // returns total
}

myMat mTranspose(myMat m)
{
    // return a matrix which is m transposed
    myMat res;
    res.numRows = m.numCols;
    res.numCols = m.numRows; // rows and columns flip in transpose

    for (int r = 0; r < m.numRows; r++)           // do each row
        for (int c = 0; c < m.numCols; c++)       // do each column
            setElem(res, c, r, getElem(m, r, c)); // flips each element in m's rows and columns
    return res;
}

myMat mAdd(myMat m1, myMat m2)
{
    myMat res;
    res.numRows = m1.numRows;
    res.numCols = m1.numCols; // creating result matrix

    for (int r = 0; r < m1.numRows; r++)     // do each row
        for (int c = 0; c < m1.numCols; c++) // do each column
            setElem(res, r, c, (getElem(m1, r, c) + getElem(m2, r, c)));
    /*
            get each element from m1 and m2
            adds them together
            then puts it in the corresponding place in result matrix
            */
    return res;
}

myMat mScal(myMat m1, double s)
{
    myMat res;
    res.numRows = m1.numRows;
    res.numCols = m1.numCols; // creating result matrix

    for (int r = 0; r < m1.numRows; r++)                 // do each row
        for (int c = 0; c < m1.numCols; c++)             // do each column
            setElem(res, r, c, (getElem(m1, r, c) * s)); // takes each element and multiplies it by scalar value
    return res;
}

myMat mMult(myMat m1, myMat m2)
{
    myMat res; // result matrix
    // the number of columns in the first matrix must match the number of rows in the second matrix
    res.numRows = m1.numRows;                // the result matrix has to have matrix m1's amount of rows
    res.numCols = m2.numCols;                // the result matrix has to have matrix m2's amount of columns
    res = zeroMat(res.numRows, res.numCols); // empty result matrix
    for (int i = 0; i < m1.numRows; i++)     // loops through m1's rows
    {
        for (int j = 0; j < m2.numCols; j++) // loops through m2's columns
        {
            setElem(res, i, j, dotProd(mTranspose(mGetRow(m1, i)), mGetCol(m2, j)));
            /*
            each row of m1 is transposed
            then the dot product of this new vector and m2 is found 
            then set as the element in position (i, j) in the new matrix
            */
        }
    }
    return res;
}

myMat mEquat(myMat mA, myMat mB)
{
    myMat mx, iA; // creating result and inverse matrix
    double s;     // scalar var
    mx = zeroMat(1, 2);
    mx.numCols = 1;
    mx.numRows = 2;
    iA = zeroMat(2, 2);
    iA.numCols = 2;
    iA.numRows = 2; // setting attributes for matrices

    float a, b, c, d;
    a = getElem(mA, 0, 0);
    b = getElem(mA, 0, 1);
    c = getElem(mA, 1, 0);
    d = getElem(mA, 1, 1); // getting values in mA
    setElem(iA, 0, 0, d);
    setElem(iA, 0, 1, -b);
    setElem(iA, 1, 0, -c);
    setElem(iA, 1, 1, a);    // creating inverse matrix
    s = 1 / (a * d - b * c); // finding scalar
    mx = mMult(iA, mB);      // multiplying inverse A and B
    mx = mScal(mx, s);       // applying scalar
    return mx;
}

int mDet(myMat m)
{
    int a, b, c, d;
    a = getElem(m, 0, 0);
    b = getElem(m, 0, 1);
    c = getElem(m, 1, 0);
    d = getElem(m, 1, 1); // getting values in mA
    return a * d - b * c;
}

myMat mCramer(myMat m1, myMat m2)
{
    // Ax=B
    // m1 = A
    // m2 = B
    // res = x
    int D, Dx, Dy;       // integers for determinants
    myMat mDx, mDy, res; // matrices for calculations
    res.numCols = 1;
    res.numRows = 2;
    mDx.numCols = 2;
    mDx.numRows = 2;
    mDy.numCols = 2;
    mDy.numRows = 2; // setting matrix values

    setElem(mDx, 0, 0, m2.data[0]); // giving values to matrix x to calculate determinant
    setElem(mDx, 0, 1, m1.data[1]);
    setElem(mDx, 1, 0, m2.data[1]);
    setElem(mDx, 1, 1, m1.data[3]);
    setElem(mDy, 0, 0, m1.data[0]); // giving values to matrix y to calculate determinant
    setElem(mDy, 0, 1, m2.data[0]);
    setElem(mDy, 1, 0, m1.data[2]);
    setElem(mDy, 1, 1, m2.data[1]);

    Dx = mDet(mDx);       // determinant of mDx
    Dy = mDet(mDy);       // determinant of mDy
    D = mDet(m1);         // determinant of input matrix
    res.data[0] = Dx / D; // dividing both deteminants to get result
    res.data[1] = Dy / D; // dividing both deteminants to get result
    return res;           // returns result
}
void printMat(const char *mess, myMat m)
{
    // mess is string to be printed, followed by matrix m
    cout << mess << " = "
         << "\n"; // print message
    for (int r = 0; r < m.numRows; r++)
    {                                         // do each row
        for (int c = 0; c < m.numCols; c++)   // do each column
            cout << getElem(m, r, c) << "\t"; // outputing the element then tab
        cout << "\n";                         // output new line at end of row
    }
    cout << "\n"; // and end of Matrix
}

int main()
{
    myMat A, C, A2, B2, A3, B3, x;

    A = mFromStr("7,6,7;8,3,6");
    C = mFromStr("10,7;10,10;7,8");
    printMat("A", A);
    printMat("C", C);
    printMat("A + C’", mAdd(A, mTranspose(C)));
    printMat("A * C", mMult(A, C));
    printMat("C * A", mMult(C, A));
    printMat("5A – 4C’", mAdd(mScal(A, 5), mScal(mTranspose(C), -4)));

    // two variable matrix equation using a ‘magic’ matrix
    A2 = mFromStr("3,6;3,10");
    B2 = mFromStr("84; 124");
    printMat("Ax=B\nx", mEquat(A2, B2));

    // Cramer function, question 8.
    A3 = mFromStr("8,10;10,6");
    B3 = mFromStr("140;136");
    printMat("Cramers rule\nx", mCramer(A3, B3));
    return 0;
}