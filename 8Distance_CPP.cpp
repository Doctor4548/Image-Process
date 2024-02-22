#include <iostream>

#include <fstream>
using namespace std;

class imageProcessing {
public:
    int numRow;
    int numCol;
    int minVal;
    int maxVal;

    int newMin;
    int newMax;
    int **ZFAry;

    imageProcessing(int nr, int nc, int minv, int maxv){
        numRow=nr;
        numCol=nc;
        minVal=minv;
        maxVal=maxv;

        ZFAry= new int*[numRow+2];
        for(int i=0;i<numRow+2;i++){
            ZFAry[i]=new int[numCol+2];
        }
    }

    void setZero(int **ZFAry, int row, int col){
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                ZFAry[i][j]=0;
            }
        }
    }

    void loadImg (ifstream& imgFile, int **zeroFramedAry){
    for(int i=1;i<numRow+1;i++){
        for(int j=1;j<numCol+1;j++){
            imgFile>>zeroFramedAry[i][j];
        }
    }
}

    void imgReformat(int **inAry, ofstream& output){


        string str=to_string(maxVal);
        int width=str.length();
        int r=2;


        while(r<numRow+2){
            int c=2;


            while(c<numCol+2){
                output<<inAry[r][c]<<" ";
                str=to_string(inAry[r][c]);
                int WW=str.length();


                while(WW<width){
                    output<<" ";
                    WW++;
                }
                c++;
            }
            r++;
            output<<endl;
        }
         
    }

    int myMin(int a,int b, int c, int d){
        return min(min(min(a,b),c),d);
    }


    void firstPass8Distance(int **ZFAry, int row, int col){
        newMin=999;
        newMax=0;

        for(int i=1;i<row;i++){
            for(int j=1;j<col;j++){
                if(ZFAry[i][j]>0){
                    int a=ZFAry[i-1][j-1];
                    int b=ZFAry[i-1][j];
                    int c=ZFAry[i-1][j+1];
                    int d=ZFAry[i][j-1];
                    ZFAry[i][j]=1+myMin(a,b,c,d);
                }
                if(newMin>ZFAry[i][j]){
                    newMin=ZFAry[i][j];
                }
                if(newMax<ZFAry[i][j]){
                    newMax=ZFAry[i][j];
                }
            }
        }

    }

};

int main(int argc, char* argv[])
{
    ifstream input;
    ofstream output1;


    input.open(argv[1]);
    output1.open(argv[2]);


    int numRow;
    int numCol;
    int minVal;
    int maxVal;

    int newMin;
    int newMax;


    input>>numRow;
    input>>numCol;
    input>>minVal;
    input>>maxVal;

    imageProcessing process(numRow, numCol, minVal, maxVal);

    process.setZero(process.ZFAry, numRow+2, numCol+2);
    process.loadImg(input, process.ZFAry);
    output1<<numRow<<" "<<numCol<<" "<<minVal<<" "<<maxVal<<endl;

    output1<<"input image"<<endl;
    process.imgReformat(process.ZFAry, output1);
    process.firstPass8Distance(process.ZFAry,numRow+1,numCol+1);
    output1<<numRow<<" "<<numCol<<" "<<process.newMin<<" "<<process.newMax<<endl;
    output1<<"1st pass distance transform image"<<endl;
    process.imgReformat(process.ZFAry, output1);

    input.close();
    output1.close();
}