#include <iostream>
#include <fstream>

using namespace std;

class Enhancement{
public:
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int maskRows;
    int maskCols;
    int maskMin;
    int maskMax;
    int** mirrorFramedAry;
    int** avgAry;
    int** GaussAry;
    int neighborAry[25];
    int maskAry[25];
    int maskWeight;
    int* histAvgAry;
    int* histGaussAry;

    Enhancement(int nR,int nC,int minV,int maxV, int maskR,int maskC,int mMin, int mMax){
        numRows=nR;
        numCols=nC;
        minVal=minV;
        maxVal=maxV;

        maskRows=maskR;
        maskCols=maskC;
        maskMin=mMin;
        maskMax=mMax;

        mirrorFramedAry = new int*[numRows + 4];
        avgAry = new int*[numRows + 4];
        GaussAry = new int*[numRows + 4];

        for (int i = 0; i < numRows + 4; i++) {
            mirrorFramedAry[i] = new int[numCols + 4];
            avgAry[i] = new int[numCols + 4];
            GaussAry[i] = new int[numCols + 4];
        }

        histAvgAry = new int[maxVal + 1]();
        histGaussAry = new int[maxVal + 1]();

    // Initialize histAvgAry and histGaussAry to zero
        for (int i = 0; i <= maxVal; i++) {
            histAvgAry[i] = 0;
            histGaussAry[i] = 0;
        }
    }

    void loadImage(ifstream& inFile,int **mirrorFramedAry){

        for(int i=2;i<numRows+2;i++){
            for(int j=2;j<numCols+2;j++){
                inFile>>mirrorFramedAry[i][j];
                
            }
        }
    }

    void mirrorFraming(int **mirrorFramedAry){


        for(int i=2;i<numCols+2;i++){
            mirrorFramedAry[1][i]=mirrorFramedAry[2][i];
            mirrorFramedAry[0][i]=mirrorFramedAry[1][i];
            mirrorFramedAry[numRows+2][i]=mirrorFramedAry[numRows+1][i];
            mirrorFramedAry[numRows+3][i]=mirrorFramedAry[numRows+2][i];
        }

        for(int i=0;i<numRows+4;i++){
            mirrorFramedAry[i][1]=mirrorFramedAry[i][2];
            mirrorFramedAry[i][0]=mirrorFramedAry[i][1];
            mirrorFramedAry[i][numCols+2]=mirrorFramedAry[i][numCols+1];
            mirrorFramedAry[i][numCols+3]=mirrorFramedAry[i][numCols+2];
        }
    }

    void imgReformat(int **inAry, ofstream& output){
      output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;


        string str=to_string(maxVal);
        int width=str.length();
        int r=2;


        while(r<numRows+2){
            int c=2;


            while(c<numCols+2){
                output<<inAry[r][c]<<" ";
                str=to_string(inAry[r][c]);
                int WW=str.length();


                while(WW<width){
                    output<<"  ";
                    WW++;
                }
                c++;
            }
            r++;
            output<<endl;
        }
         
    }

    int avg5x5(int **mirrorFramedAry, int i, int j){

        int sum=0;
        for(int r=i-2;r<=i+2;r++){
            for(int c=j-2;c<=j+2;c++){
                sum+=mirrorFramedAry[r][c];
            }
        }
 
        return (int)(sum/25);
    }

    void computeAvg5x5(int **mirrorFramedAry, int **avgAry, ofstream& debug){
        debug<<"entering computeAvg5x5 method"<<endl;
        int i=2;


        while(i<numRows+2){
            int j=2;
            while(j<numCols+2){
                avgAry[i][j]=avg5x5(mirrorFramedAry,i,j);
                j++;
            }
            i++;
        }

        debug<<"Leaving computeAvg5x5 method"<<endl;
    }

    void computeHist(int **inAry, int *histAry, ofstream& debug){
        debug<<"entering computeHist method"<<endl;
        int i=2;

        while(i<numRows+2){
            int j=2;
            while(j<numCols + 2){
                histAry[inAry[i][j]]++;
                j++;
            }
            i++;
        }

        debug<<"Leaving computeHist method"<<endl;
    }

    void printHist(int *histAry, ofstream& histFile, int maxVal, ofstream& debug){
        debug<<"entering printHist method"<<endl;
        int index=0;
        histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;

        while(index<=maxVal){
            histFile<<index<<" "<<histAry[index]<<endl;
            index++;
        }
        debug<<"Leaving printHist method"<<endl;
    }

    int loadMaskAry(ifstream& mask, int *maskAry){
        maskWeight=0;
        int count=0;
        for(int i=0;i<maskRows;i++){
            for(int j=0;j<maskCols;j++){
                mask>>maskAry[count];
                maskWeight+=maskAry[count];
                count++;
            }
        }
        return maskWeight;
    }

    void loadNeighborAry(int **mirrorFramedAry, int i, int j, int *neighborAry){
        int count=0;
        for(int r=i-2;r<=i+2;r++){
            for(int c=j-2;c<=j+2;c++){
                neighborAry[count]=mirrorFramedAry[r][c];
                count++;
            }
        }

    }

    int convolution(int *neighborAry, int *maskAry, int weight, ofstream& debug){
        debug<<"entering convolution method"<<endl;
        int result=0;

        int i=0;

        while(i<25){
            result += neighborAry[i] * maskAry[i];
            i++;
        }

        debug<<"in convolution method, result is: "<<result<<endl;
        debug<<"Leaving convolution method"<<endl;
        return(result/weight);

    }

    void computeGauss5x5(int **mirrorFramedAry, int **GaussAry, int *maskAry, int *neighborAry, int weight, ofstream& debug){
        debug<<"entering computeGauss method"<<endl;
        int i=2;

        while(i<numRows+2){
            int j=2;

            while(j<numCols+2){
                loadNeighborAry(mirrorFramedAry, i, j, neighborAry);
                GaussAry[i][j]=convolution(neighborAry, maskAry, weight,debug);
                j++;
            }
            i++;
        }

        debug<<"Leaving computeGauss method"<<endl;
    }




    void print(int **inAry, ofstream& output){

      output<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<endl;


        string str=to_string(maxVal);
        int width=str.length();
        int r=2;


        while(r<numRows+2){
            int c=2;


            while(c<numCols+2){
                output<<inAry[r][c]<<" ";
                str=to_string(inAry[r][c]);
                int WW=str.length();


                while(WW<width){
                    output<<"  ";
                    WW++;
                }
                c++;
            }
            r++;
            output<<endl;
        }
    }

};


int main(int argc, char* argv[])
{
    ifstream inFile;
    ifstream maskFile;
    ofstream output;
    ofstream debug;
    int choose;

    inFile.open(argv[1]);
    maskFile.open(argv[2]);
    choose=stoi(argv[3]);
    output.open(argv[4]);
    debug.open(argv[5]);

    int numRows, numCols, minVal, maxVal;
    inFile>>numRows;
    inFile>>numCols;
    inFile>>minVal;
    inFile>>maxVal;


    int maskRows, maskCols, maskMin, maskMax;
    maskFile>>maskRows;
    maskFile>>maskCols;
    maskFile>>maskMin;
    maskFile>>maskMax;

    
    int** mirrorFramedAry = new int*[numRows + 4];
    int** avgAry = new int*[numRows + 4];
    int** GaussAry = new int*[numRows + 4];

    for (int i = 0; i < numRows + 4; i++) {
        mirrorFramedAry[i] = new int[numCols + 4];
        avgAry[i] = new int[numCols + 4];
        GaussAry[i] = new int[numCols + 4];
    }

    int* histAvgAry = new int[maxVal + 1]();
    int* histGaussAry = new int[maxVal + 1]();

    // Initialize histAvgAry and histGaussAry to zero
    for (int i = 0; i <= maxVal; i++) {
        histAvgAry[i] = 0;
        histGaussAry[i] = 0;
    }

    Enhancement enhance(numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax);

    enhance.loadImage(inFile, enhance.mirrorFramedAry);
    enhance.mirrorFraming(enhance.mirrorFramedAry);
    enhance.imgReformat(enhance.mirrorFramedAry,output);




    //enhance.print();

    if(choose ==1){
        enhance.computeAvg5x5(enhance.mirrorFramedAry, enhance.avgAry ,debug);
        enhance.computeHist(enhance.avgAry, enhance.histAvgAry,debug);

        string inputFileName = argv[1];
        string nameAvg = inputFileName + "_Avg5x5.txt";
        ofstream avgFile;
        avgFile.open(nameAvg);

        enhance.imgReformat(enhance.avgAry, output);


        enhance.print(enhance.avgAry, avgFile);

        string avgHist = inputFileName + "_Avg5x5_hist.txt";
        ofstream histAvgFile;
        histAvgFile.open(avgHist);

        enhance.printHist(enhance.histAvgAry, histAvgFile, maxVal, debug);

        histAvgFile.close();
        avgFile.close();
    }
    else if(choose==2){
        int maskWeight=enhance.loadMaskAry(maskFile, enhance.maskAry);
        enhance.computeGauss5x5(enhance.mirrorFramedAry, enhance.GaussAry, enhance.maskAry, enhance.neighborAry, maskWeight, debug);//
        enhance.computeHist(enhance.GaussAry, enhance.histGaussAry, debug);

        string inputFileName = argv[1];

        string nameGauss = inputFileName + "_Gauss5x5.txt";
        ofstream GaussFile;
        GaussFile.open(nameGauss);

        enhance.imgReformat(enhance.GaussAry,output);
        enhance.print(enhance.avgAry, GaussFile);

        string GaussHist = inputFileName + "_Gauss5x5_hist.txt";
        ofstream histGaussFile;
        histGaussFile.open(GaussHist);

        enhance.printHist(enhance.histGaussAry, histGaussFile, maxVal, debug);

        GaussFile.close();
        histGaussFile.close();

    }



    inFile.close();
    maskFile.close();
    output.close();
    debug.close();


    return 0;
}