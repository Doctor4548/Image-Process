#include <iostream>
#include <fstream>


using namespace std;


class morphology{
public:
    int numImgRows;
    int numImgCols;
    int imgMin;
    int imgMax;


    int numStructRows;
    int numStructCols;
    int structMin;
    int structMax;


    int rowOrigin;
    int colOrigin;


    int rowFrameSize;
    int colFrameSize;
    int extraRows;
    int extraCols;


    int rowSize;
    int colSize;


    int **zeroFramedAry;
    int **morphAry;
    int **tempAry;
    int **structAry;






    morphology(int nir, int nic, int imin, int imax, int nsr, int nsc, int smi, int sma, int ro, int co){
        numImgRows=nir;
        numImgCols=nic;
        imgMin=imin;
        imgMax=imax;


        numStructRows=nsr;
        numStructCols=nsc;
        structMin=smi;
        structMax=sma;


        rowOrigin=ro;
        colOrigin=co;




        rowFrameSize=numStructRows/2;
        colFrameSize=numStructCols/2;
        extraRows=rowFrameSize*2;
        extraCols=colFrameSize*2;


        rowSize=numImgRows + extraRows;
        colSize=numImgCols + extraCols;


        zeroFramedAry=new int*[rowSize];
        morphAry = new int*[rowSize];
        tempAry = new int*[rowSize];


        for (int i = 0; i < rowSize; i++) {
            zeroFramedAry[i] = new int[colSize];
            tempAry[i] = new int[colSize];
            morphAry[i] = new int[colSize];
        }


        structAry = new int*[numStructRows];
        for (int i = 0; i < numStructRows; i++) {
            structAry[i] = new int[numStructCols];
        }
    }


    void zero2DAry(int** Ary, int nRows, int nCols) {
        for (int i = 0; i < nRows; i++) {
            for (int j = 0; j < nCols; j++) {
                Ary[i][j] = 0;
            }
        }
    }






    void loadImg (ifstream& imgFile, int **zeroFramedAry){
        for(int i=rowOrigin;i<numImgRows+rowOrigin;i++){
            for(int j=colOrigin;j<numImgCols+colOrigin;j++){
                imgFile>>zeroFramedAry[i][j];
            }
        }
    }




    void loadstruct(ifstream& structFile, int **structAry){
        for(int i=0;i<numStructRows;i++){
            for(int j=0;j<numStructCols;j++){
                structFile>>structAry[i][j];
            }
        }  
    }


    void prettyPrint(int **inAry, ofstream& output, int row, int col, int startRow, int startCol){


       for (int i = startRow; i < row+startRow; i++) {
            for (int j = startCol; j < col+startCol; j++) {
                if (inAry[i][j] == 0) {
                    output << ".  ";
                } else {
                    output << "1 ";
                }
            }
            output<<endl;


        }


        output<<endl;
    }


    void onePixelDilation (int row, int col, int **inAry, int **outAry){
        int iOffset=row-rowOrigin;
        int jOffset=col-colOrigin;


        int rIndex=0;
        while(rIndex < numStructRows){
            int cIndex=0;
            while(cIndex < numStructCols){
                if (structAry[rIndex][cIndex] > 0){
                    outAry[iOffset + rIndex][jOffset + cIndex] = 1;
                }
                cIndex++;
            }
            rIndex++;
        }
    }


    void ComputeDilation(int **inAry, int **outAry){
        int i=rowFrameSize;


        while(i<rowSize){
            int j=colFrameSize;
           
            while(j<colSize){
                if(inAry[i][j]>0){
                    onePixelDilation (i, j, inAry, outAry);
                }
                j++;
            }
            i++;
        }
    }


    void onePixelErosion (int row, int col, int **inAry, int **outAry){
        int iOffset=row-rowOrigin;
        int jOffset=col-colOrigin;


        bool matchFlag=true;


        int rIndex=0;
        while(rIndex < numStructRows){
            int cIndex=0;
            while(cIndex < numStructCols){
                if ((structAry[rIndex][cIndex] > 0) &&(inAry[iOffset + rIndex][jOffset + cIndex] <= 0)){
                    matchFlag=false;
                }
                cIndex++;
            }
            rIndex++;
        }


        if(matchFlag==true){
            outAry[row][col]=1;
        }
        else{
            outAry[row][col]=0;
        }
    }


    void ComputeErosion (int **inAry, int **outAry){
        int i=rowFrameSize;


        while(i<rowSize){
            int j=colFrameSize;
           
            while(j<colSize){
                if(inAry[i][j]>0){
                    onePixelErosion (i, j, inAry, outAry);
                }
                j++;
            }
            i++;
        }    
    }


    void ComputeOpening(int **inAry, int **outAry, int **tempAry){
        zero2DAry(tempAry, rowSize, colSize);//
        ComputeErosion(inAry, tempAry);
        ComputeDilation(tempAry, outAry);
    }


    void ComputeClosing(int **inAry, int **outAry, int **tempAry){
        ComputeDilation(inAry, tempAry);
        ComputeErosion(tempAry, outAry);
    }






    bool checkHeaderMatch(int **image, int pattern[5][5], int row, int col, int maxRow, int maxCol){
        for(int i=0;i<5&&i+row<maxRow;i++){
            for(int j=0;j<5&&j+col<maxCol;j++){
                if(pattern[i][j]!=image[i+row][j+col]){
                    return false;
                }
                else if(maxRow-row<5||maxCol-col<5){
                    return false;
                }
            }
        }


        return true;
    }


    bool checkArmMatch(int **image, int pattern[3][8], int row, int col, int maxRow, int maxCol){
        for(int i=0;i<3&&i+row<maxRow;i++){
            for(int j=0;j<8&&j+col<maxCol;j++){
                if(pattern[i][j]!=image[i+row][j+col]){
                    return false;
                }
                else if(maxRow-row<3||maxCol-col<8){
                    return false;
                }
            }
        }


        return true;
    }


    bool checkBodyMatch(int **image, int pattern[11][11], int row, int col, int maxRow, int maxCol){
        for(int i=0;i<11&&i+row<maxRow;i++){
            for(int j=0;j<11&&j+col<maxCol;j++){
                if(pattern[i][j]!=image[i+row][j+col]){
                    return false;
                }
                else if(maxRow-row<11||maxCol-col<11){
                    return false;
                }
            }
        }


        return true;
    }


    bool checkLegsMatch(int **image, int pattern[8][5], int row, int col, int maxRow, int maxCol){
        for(int i=0;i<8&&i+row<maxRow;i++){
            for(int j=0;j<5&&j+col<maxCol;j++){
                if(pattern[i][j]!=image[i+row][j+col]){
                    return false;
                }
                else if(maxCol-col<5||maxRow-row<6){
                    return false;
                }
            }
        }


        return true;
    }


    void printHeader(int **image, ofstream& output, int row, int col, int maxRow, int maxCol){
        int max=0;
        for(int i=row-1; i<maxRow;i++){
            int count=0;
            for(int j=col-1; j<maxCol;j++){
                if(image[i][j]==1){
                    count++;
                    output<<"1  ";
                }
                else{
                    output<<".  ";
                }
            }
            output<<endl;
            if(count>=max){
                max=count;
            }
            else{
                return ;
            }
        }
        output<<endl;
    }


    void printArms(int **image, ofstream& output, int row, int col, int maxRow, int maxCol){
        int count=0;
        for(int i=row; i<=row+maxRow;i++){
            for(int j=col; j<col+maxCol;j++){
                if(image[i][j]==1){
                    output<<"1  ";
                }
                else{
                    output<<".  ";
                }
        }
            output<<endl;


        }
        output<<endl;
    }






    void printBody(int **image, ofstream& output, int row, int col, int maxRow, int maxCol){
        int max=0;
        for(int i=row-1; i<=maxRow+1;i++){
            int count=0;
            for(int j=col-1; j<=maxCol+1;j++){
                if(image[i][j]==1){
                    count++;
                    output<<"1  ";
                }
                else{
                    output<<".  ";
                }
            }
            output<<endl;
            if(count>=max){
                max=count;
            }
            else if(max-count>2){
                return ;
            }
        }


        output<<endl;
    }


    void printLegs(int **image, ofstream& output, int row, int col, int maxRow, int maxCol){
        for(int i=row;i<maxRow;i++){
            for(int j=col;j<maxCol;j++){
                if(image[i][j]==1){
                    output<<"1  ";
                }
                else{
                    output<<".  ";
                }
            }
            output<<endl;
        }
        output<<endl;
    }


    void ObjectDecomp(ifstream& input, ofstream& output){
        int row, col, min, max;
        input>>row;
        input>>col;
        input>>min;
        input>>max;


        int **image;
        image=new int*[row];
        for(int i=0;i<row;i++){
            image[i]=new int[col];
        }


        int header[5][5]={
                    {0,1,1,1,0},
                    {1,1,1,1,1},
                    {1,1,1,1,1},
                    {1,1,1,1,1},
                    {1,1,1,1,1}};


        int headerRow=5;
        int headerCol=5;


        int arms[3][8]={
                    {0,0,0,0,0,0,0,0},
                    {1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1}};


        int armsRow=3;
        int armsCol=8;


        int body[11][11]={
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1},
                    {1,1,1,1,1,1,1,1,1,1,1}};


        int bodyRow=11;
        int bodyCol=11;


        int legs[8][5]={
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0},
                    {0,1,1,1,0}};


        int legsRow=8;
        int legsCol=5;
       
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                input>>image[i][j];
            }
        }


        int armCount=0;
        int legCount=0;
       
        output<<"Header: "<<endl;
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                if(checkHeaderMatch(image, header, i, j, row, col)){
                    printHeader(image, output, i, j, row, col);
                }
                else if(checkArmMatch(image, arms, i, j, row, col)){
                    if(armCount>=2){
                        continue;
                    }
                    else if(armCount==0){
                        output<<"Arms: "<<endl;
                    }
                    printArms(image, output, i, j, armsRow, armsCol);
                    armCount++;
                }
                else if(checkBodyMatch(image, body, i, j, row, col)){
                    output<<"Body: "<<endl;
                    printBody(image, output, i, j, i+bodyRow, j+bodyCol);
                }
                else if(checkLegsMatch(image, legs, i, j, row, col)){
                    if(legCount>=1){
                        continue;
                    }
                    else if(legCount==0){
                        output<<endl;
                        output<<"Legs: "<<endl;
                    }
                    printLegs(image, output, i, j, row, col);
                    legCount++;


                }
        }




    }


    }
















    void print(ofstream& output){
        for(int i=0;i<numImgRows;i++){
            for(int j=0;j<numImgCols;j++){
                output<<zeroFramedAry[i][j]<<" ";
            }
            output<<endl;
        }
    }










};


int main(int argc, char * argv[]) {


    ifstream imgFile;
    ifstream structFile;
    ifstream img1;
    ifstream img2;
    ofstream output1;
    ofstream output2;


    imgFile.open(argv[1]);
    structFile.open(argv[2]);
    img1.open(argv[3]);
    img2.open(argv[4]);
    output1.open(argv[5]);
    output2.open(argv[6]);


    int numImgRows;
    int numImgCols;
    int imgMin;
    int imgMax;


    imgFile>>numImgRows;
    imgFile>>numImgCols;
    imgFile>>imgMin;
    imgFile>>imgMax;


    int numStructRows, numStructCols, structMin, structMax;
    structFile>>numStructRows;
    structFile>>numStructCols;
    structFile>>structMin;
    structFile>>structMax;


    int rowOrigin, colOrigin;
    structFile>>rowOrigin;
    structFile>>colOrigin;


    morphology my(numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin);


    my.zero2DAry(my.zeroFramedAry,my.rowSize,my.colSize);
    my.loadImg(imgFile, my.zeroFramedAry);//
    output1<<"Original Image: "<<endl;
    output1<<my.numImgRows<<" "<<my.numImgCols<<" "<<imgMin<<" "<<imgMax<<endl;
    my.prettyPrint(my.zeroFramedAry, output1, my.numImgRows, my.numImgCols, my.rowOrigin, my.colOrigin);


    my.zero2DAry(my.structAry, my.numStructRows, my.numStructCols);
    my.loadstruct (structFile, my.structAry);
    output1<<"Structuring Element: "<<endl;
    output1<<my.numStructRows<<" "<<my.numStructCols<<" "<<structMin<<" "<<structMax<<endl;
    my.prettyPrint (my.structAry, output1, my.numStructRows,my.numStructCols, 0, 0);


    my.zero2DAry(my.morphAry, my.rowSize, my.colSize);
    my.ComputeDilation (my.zeroFramedAry, my.morphAry);
    output1<<"Dilation: "<<endl;
    output1<<my.rowSize<<" "<<my.colSize<<" "<<imgMin<<" "<<imgMax<<endl;
    my.prettyPrint (my.morphAry, output1, my.rowSize, my.colSize,0,0);
   
    my.zero2DAry(my.morphAry, my.rowSize, my.colSize);
    my.ComputeErosion (my.zeroFramedAry, my.morphAry);
    output1<<"Erosion: "<<endl;
    output1<<my.rowSize<<" "<<my.colSize<<" "<<imgMin<<" "<<imgMax<<endl;
    my.prettyPrint (my.morphAry, output1, my.rowSize, my.colSize,0,0);


    my.zero2DAry(my.morphAry, my.rowSize, my.colSize);
    my.ComputeOpening (my.zeroFramedAry, my.morphAry, my.tempAry);
    output1<<"Opening: "<<endl;
    output1<<my.rowSize<<" "<<my.colSize<<" "<<imgMin<<" "<<imgMax<<endl;
    my.prettyPrint (my.morphAry, output1, my.rowSize, my.colSize,0,0);


    my.zero2DAry(my.morphAry, my.rowSize, my.colSize);
    my.ComputeClosing (my.zeroFramedAry, my.morphAry, my.tempAry);
    output1<<"Closing: "<<endl;
    output1<<my.rowSize<<" "<<my.colSize<<" "<<imgMin<<" "<<imgMax<<endl;
    my.prettyPrint (my.morphAry, output1, my.rowSize, my.colSize,0,0);




    my.ObjectDecomp(img2, output2);




    imgFile.close();
    structFile.close();
    img1.close();
    img2.close();
    output1.close();
    output2.close();
   
}
