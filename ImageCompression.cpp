#include <iostream>
#include <fstream>


using namespace std;


class ImageCompression{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int** ZFAry;
    int** skeletonAry;

    ImageCompression(int nR, int nC, int minV, int maxV){
        numRows=nR;
        numCols=nC;
        minVal=minV;
        maxVal=maxV;


        ZFAry = new int*[numRows+2];
        skeletonAry = new int*[numRows+2];
        for (int i = 0; i < numRows+2; i++){
            ZFAry[i] = new int[numCols+2]; 
            skeletonAry[i] = new int[numCols+2]; 
        }
    }

    void setZero(int **array, int row, int col){
        for(int i=0;i<row;i++){
            for(int j=0;j<col;j++){
                array[i][j]=0;
            }
        }
    }

    void loadImg(ifstream& input, int** array){
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                input >> array[i][j];
                }
            }
    }

    void distance8Pass1(int** array, ofstream& debug){
        minVal=9999;
        maxVal=-1;
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                int p=array[i][j];
                if(p>0){
                    int a=array[i-1][j-1];
                    int b=array[i-1][j];
                    int c=array[i-1][j+1];
                    int d=array[i][j-1];

                    int minimum=min(a, min(b, min(c, d)))+1;
                    array[i][j]=minimum;

                    if(array[i][j]>maxVal){
                        maxVal=array[i][j];
                    }
                    if(array[i][j]<minVal){
                        minVal=array[i][j];
                    }
                }
            }
        }
    }

    void imgReformat (int **array, ofstream& output){
        output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
       for (int i = 0; i < numRows+2; i++) {
            for (int j = 0; j < numCols+2; j++) {
                output<<array[i][j]<<" ";
            }
            output<<"\n";
       }
    }



    void distance8Pass2 (int **array, ofstream& debug){
        minVal=9999;
        maxVal=-1;

        for(int i=numRows;i>0;i--){
            for(int j=numCols;j>0;j--){
                if(array[i][j]>0){
                    int p=array[i][j];

                    int e=array[i][j+1];
                    int f=array[i+1][j-1];
                    int g=array[i+1][j];
                    int h=array[i+1][j+1];

                    array[i][j]=min(e+1,min(f+1,min(g+1,min(h+1,p))));

                    if(array[i][j]>maxVal){
                        maxVal=array[i][j];
                    }
                    if(array[i][j]<minVal){
                        minVal=array[i][j];
                    }
                }
            }
        }

    }


    void Distance8(int** array, ofstream& output, ofstream& debug){
        debug<<"Entering Distance8() method. \n";
        distance8Pass1(array, debug);
        output<<"Distance Transform First Pass \n";
        imgReformat (array, output);
        distance8Pass2 (array, debug); 
        output<<"\n Distance Transform Second Pass \n";     
        imgReformat (array, output);
        debug<<"“Leaving Distance8() method. \n";
    }

    bool isLocalMaxima(int i, int j, int **array){
        int p=array[i][j];

        int a=array[i-1][j-1];
        int b=array[i-1][j];
        int c=array[i-1][j+1];
        int d=array[i][j-1];

        int e=array[i][j+1];
        int f=array[i+1][j-1];
        int g=array[i+1][j];
        int h=array[i+1][j+1];

        if(p>=max(a, max(b, max(c, max(d, max(e, max(f, max(g, max(h, p))))))))){
            return true;
        }
        else{
            return false;
        }
    }

    void computeLocalMaxima (int **ZFAry, int **skeletonAry, ofstream& debug){
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(isLocalMaxima(i,j, ZFAry)){
                    skeletonAry[i][j]=ZFAry[i][j];
                }
                else{
                    skeletonAry[i][j]=0;
                }
            }
        }
    }

    void extractSkeleton(int **skeletonAry, ofstream& skeletonFile, ofstream& debug){
        skeletonFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<" \n";
        for(int i=1;i<=numRows;i++){
            for(int j=1;j<=numCols;j++){
                if(skeletonAry[i][j]>0){
                    skeletonFile<<i<<"   "<<j<<"   "<<skeletonAry[i][j]<<"\n";
                }
            }
        }
    }

    void imageCompression (int **array, int ** skeletonAry, ofstream& skeletonFile, ofstream& output, ofstream& debug){
        debug<<"Entering imageCompression () method, \n";
        computeLocalMaxima (array, skeletonAry, debug);
        output<<"\n Local maxima, skeletonAry \n";
        imgReformat (skeletonAry, output);
        extractSkeleton (skeletonAry, skeletonFile, debug);
        debug<<"Leaving imageCompression () method. \n";
    }

    void loadSkeleton(ifstream& skeletonFile, int** array, ofstream& debug){//
        int row, col, minV, maxV;
        skeletonFile>>row;
        skeletonFile>>col;
        skeletonFile>>minV;
        skeletonFile>>maxV;

        int i,j,val;

        while(skeletonFile >> i >> j >> val){
            array[i][j] = val;
        }
    }

    void expensionPass1 (int **array, ofstream& debug){
        for(int i=1;i<=numRows;i++){
            for(int j=1;j<=numCols;j++){
                if(array[i][j]==0){
                    int p=array[i][j];

                    int a=array[i-1][j-1];
                    int b=array[i-1][j];
                    int c=array[i-1][j+1];
                    int d=array[i][j-1];

                    int e=array[i][j+1];
                    int f=array[i+1][j-1];
                    int g=array[i+1][j];
                    int h=array[i+1][j+1];


                    int maximum=max(a-1, max(b-1, max(c-1, max(d-1, max(e-1, max(f-1, max(g-1, max(h-1, array[i][j]))))))));
                    array[i][j]=maximum;
                }
            }
        }

    }

        void expensionPass2 (int **array, ofstream& debug){
        for(int i=numRows;i>0;i--){
            for(int j=numCols;j>0;j--){
                
                int p=array[i][j];

                int a=array[i-1][j-1];
                int b=array[i-1][j];
                int c=array[i-1][j+1];
                int d=array[i][j-1];

                int e=array[i][j+1];
                int f=array[i+1][j-1];
                int g=array[i+1][j];
                int h=array[i+1][j+1];


                int maximum=max(a-1, max(b-1, max(c-1, max(d-1, max(e-1, max(f-1, max(g-1, max(h-1, array[i][j]))))))));
                if(array[i][j]<maximum){
                    array[i][j]=maximum;
                }

            
            }
        }

    }

    void imageDeCompression(int **array, ofstream& output, ofstream& debug){
        debug<<"Entering imageDeCompression () method. \n";
        expensionPass1 (array, debug);
        output<<"\n first pass Expansion \n";
        imgReformat (array, output);
        expensionPass2 (array, debug);
        output<<"\n second pass Expansion \n";
        imgReformat (array, output);
        debug<<"Leaving imageDeCompression () method. \n";
    }

    void threshold(int **array, ofstream& output, int threshold){
        minVal=9999;
        maxVal=-1;
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                if(array[i][j]>=threshold){
                    array[i][j]=1;
                }
                else{
                    array[i][j]=0;
                }

                if(array[i][j]>maxVal){
                    maxVal=array[i][j];
                }
                if(array[i][j]<minVal){
                    minVal=array[i][j];
                }
            }
        }
    }


    void print(int** array, ofstream& output){
        output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";
        for(int i=1;i<numRows+1;i++){
            for(int j=1;j<numCols+1;j++){
                output<<array[i][j]<<" ";

            }
            output<<"\n";

        }
    }

};


int main(int argc, char * argv[]) {
    ifstream input;
    ofstream output;
    ofstream debug;

    input.open(argv[1]);
    output.open(argv[2]);
    debug.open(argv[3]);

    int numRows;
    int numCols;
    int MinVal;
    int MaxVal;

    input>>numRows;
    input>>numCols;
    input>>MinVal;
    input>>MaxVal;



    ImageCompression com(numRows, numCols, MinVal, MaxVal);

    ofstream skeletonFile(string(argv[1])+"_skeleton.txt");
    ofstream deCompressFile(string(argv[1])+"_deCompressed.txt");
    

    com.setZero(com.ZFAry, numRows+2, numCols+2);
    com.setZero(com.skeletonAry, numRows+2, numCols+2);

    com.loadImg(input, com.ZFAry);
    com.Distance8 (com.ZFAry, output, debug); 
    

    com.imageCompression (com.ZFAry, com.skeletonAry, skeletonFile, output, debug);
    skeletonFile.close();

    ifstream skeletonFile2(string(argv[1])+"_skeleton.txt");

    com.setZero(com.ZFAry, numRows+2, numCols+2);
    com.loadSkeleton (skeletonFile2, com.ZFAry, debug);//

    com.imageDeCompression (com.ZFAry, output, debug); 
    com.threshold(com.ZFAry, deCompressFile, 1);
    com.print(com.ZFAry, deCompressFile);



    //com.print(com.ZFAry, output);



    input.close();
    output.close();
    debug.close();
    skeletonFile2.close();
    deCompressFile.close();
    
}