#include <iostream>
#include <fstream>


using namespace std;


class Point {
public:
    int row;
    int col;

    Point(){}

    Point(int r, int c) {
        row=r;
        col=c;
    }

    bool operator==(const Point& other) const {
        return (row == other.row) && (col == other.col);
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};


class ChainCode{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int label;

    int** ZFAry;
    int** reConstructAry;
    Point coordOffset[8];
    int zeroTable[8] = {6, 0, 0, 2, 2, 4, 4, 6};

    Point startP;
    Point currentP;
    Point nextP;
    int lastQ;
    int chainDir;

    ChainCode(int rows, int cols, int minV, int maxV) {
        numRows=rows;
        numCols=cols;
        minVal=minV;
        maxVal=maxV;


        ZFAry = new int*[numRows + 2];
        reConstructAry = new int*[numRows + 2];
        for (int i = 0; i < numRows + 2; ++i) {
            ZFAry[i] = new int[numCols + 2];
            reConstructAry[i] = new int[numCols + 2];
        }

        // Dynamically allocate reConstructAry



        for(int i =0;i<numRows+2;i++){
            for(int j=0;j<numCols+2;j++){
                ZFAry[i][j]=0;
                reConstructAry[i][j]=0;
            }
        }

        coordOffset[0] = Point(0, 1);
        coordOffset[1] = Point(-1, 1);
        coordOffset[2] = Point(-1, 0);
        coordOffset[3] = Point(-1, -1);
        coordOffset[4] = Point(0, -1);
        coordOffset[5] = Point(1, -1);
        coordOffset[6] = Point(1, 0);
        coordOffset[7] = Point(1, 1);
    
    }

    void loadImg(ifstream& input, int** array){
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                input >> array[i][j];
                }
            }
    }

    void imgReformat (int **inAry, ofstream& output){

        output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        string str=to_string(maxVal);
        int width=str.length();
        int r=0;


        while(r<numRows+2){
            int c=0;


            while(c<numCols+2){
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

    void reformatPrettyPrint(int **inAry, ofstream& output){
        output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

       for (int i = 0; i < numRows+2; i++) {
            for (int j = 0; j < numCols+2; j++) {
                if (inAry[i][j] == 0) {
                    output << ". ";
                } else {
                    output << inAry[i][j]<<" ";
                }
            }
            output<<endl;
        }
        output<<endl;
    }

    /*void reformatPrettyPrint2(int **inAry, ofstream& output){
        output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

       for (int i = 0; i < numRows+2; i++) {
            for (int j = 0; j < numCols+2; j++) {
                if (inAry[i][j] == 0) {
                    output << ". ";
                } else {
                    output << inAry[i][j]-1<<" ";
                }
            }
            output<<endl;
        }
        output<<endl;
    }*/

    void getChainCode(int **ZFAry, ofstream& chainCodeFile, ofstream& debug){
        debug<<"Entering getChainCode method: \n";
        chainCodeFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(ZFAry[i][j]>0){
                    label=ZFAry[i][j];
                    chainCodeFile<<label<<" "<<i<<" "<<j<<" ";
                    startP.row=i;
                    startP.col=j;
                    currentP.row=i;
                    currentP.col=j;
                    lastQ=4;
                    goto next;
                }
             }
        }
        next:

        do{
            lastQ=(lastQ++)%8;
            chainDir=findNextP(currentP,lastQ,label,debug);
            
            chainCodeFile<<chainDir<<" ";
            nextP.row=currentP.row+coordOffset[chainDir].row;
            nextP.col=currentP.col+coordOffset[chainDir].col;
            ZFAry[nextP.row][nextP.col]=(label+4);
            //cout<<ZFAry[nextP.row][nextP.col];

            currentP=nextP;

            if(chainDir==0){
                lastQ=zeroTable[7];
            }
            else{
                lastQ=zeroTable[chainDir-1];
            }
            debug<<"lastQ = "<<lastQ<<"; currentP.row = "<<currentP.row<<"; currentP.col = "<<currentP.col<<"; nextP.row = "<<nextP.row<<"; nextP.col = "<<nextP.col<<"; \n";
        }while(currentP.row != startP.row || currentP.col != startP.col);

        if(label+4>maxVal){
            maxVal=label+4;
        }

        debug<<"leaving getChianCode \n";

    }

    int findNextP(Point currentP, int lastQ, int label, ofstream& debug){
        debug<<"Entering findNextP method \n";
        int index=lastQ;
        bool found=false;

        while(found!=true){
            int iRow=currentP.row+coordOffset[index].row;

            int jCol=currentP.col+coordOffset[index].col;

            if(ZFAry[iRow][jCol]==label||ZFAry[iRow][jCol]==label+4){
                chainDir=index;
                found=true;
            }
            else{
                index=(index+1)%8;
            }
    }
        debug<<"Leaving findNextP method \n";
        debug<<"chainDir = "<<chainDir<<" \n";
        return chainDir;
}

    /*void constructBoundary(int **array, ifstream& input){
        int numRows, numCols, minVal, maxVal;
        input>>numRows;
        input>>numCols;
        input>>minVal;
        input>>maxVal;

        int label, startR, startC; 
        input>>label;
        input>>startR;
        input>>startC;

        bool pass=false;
        bool stop=false;
        int dir;

        int i=startR;
        int j=startC;
        do{
            //array[i][j]=1;
            input>>dir;
            if(dir==0){
                j++;
            }
            else if(dir==1){
                i--;
                j++;
            }
            else if(dir==2){
                i--;
            }
            else if(dir==3){
                i--;
                j--;
            }
            else if(dir==4){
                j--;
            }
            else if(dir==5){
                i++;
                j--;
            }
            else if(dir==6){
                i++;
            }
            else if(dir==7){
                i++;
                j++;
            }
            array[i][j]=1;

            if(i==startR&&j==startC){
                stop = true;
            }
            
        }while(!stop);

}*/

    void constructBoundary(int **array, ifstream& input){
        int numRows, numCols, minV, maxV;
        input>>numRows;
        input>>numCols;
        input>>minV;
        input>>maxV;

        int label, startR, startC; 
        input>>label;
        input>>startR;
        input>>startC;

        int dir;
        maxVal=label;

        do{

            input>>dir;
            currentP.row=currentP.row+coordOffset[dir].row;
            currentP.col=currentP.col+coordOffset[dir].col;
            array[currentP.row][currentP.col]=label;

        }while(startP.row!=currentP.row||startP.col!=currentP.col);

        

    }



};
int main(int argc, char* argv[]) {

    ifstream input;
    ofstream output;
    ofstream debug;


    input.open(argv[1]);
    output.open(argv[2]);
    debug.open(argv[3]);


    int numRows,numCols,minVal,maxVal;
    input>>numRows;
    input>>numCols;
    input>>minVal;
    input>>maxVal;

    ofstream chainCodeFile(string(argv[0])+"_chainCode.txt");
    ofstream boundaryFile(string(argv[0])+"_boundary.txt");


    ChainCode code(numRows,numCols,minVal,maxVal);
    code.loadImg(input, code.ZFAry);
    debug<<"After loadImage; ZFAry as below: \n";
    code.reformatPrettyPrint(code.ZFAry,output);

    code.getChainCode(code.ZFAry, chainCodeFile, debug);
    debug<<"After getChainCode; ZFAry as below: \n";
    code.reformatPrettyPrint(code.ZFAry, output);


    chainCodeFile.close();
    ifstream chainCodeFile2(string(argv[0])+"_chainCode.txt");


    code.constructBoundary(code.reConstructAry,chainCodeFile2);
    code.reformatPrettyPrint(code.reConstructAry,output);
    code.imgReformat(code.reConstructAry,boundaryFile);



    input.close();
    output.close();
    debug.close();
    chainCodeFile2.close();
    boundaryFile.close();

}