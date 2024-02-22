#include <iostream>
#include <fstream>


using namespace std;


class EdgeDetector{
public:

    int numRows;
    int numCols;
    int minVal;
    int maxVal;

    int** mirrorFramedAry;
    int RobertVertical[2][2];
    int RobertHorizontal[2][2];
    int RobertRightDiag[2][2];
    int RobertLeftDiag[2][2];
    int** RobertEdgeAry;
    int SobelVertical[3][3];
    int SobelHorizontal[3][3];
    int SobelRightDiag[3][3];
    int SobelLeftDiag[3][3];
    int** SobelEdgeAry;
    int* histRobertAry;
    int* histSobelAry;



    EdgeDetector(int nR, int nC, int minV, int maxV){
        numRows=nR;
        numCols=nC;
        minVal=minV;
        maxVal=maxV;


        mirrorFramedAry = new int*[numRows + 2];
        RobertEdgeAry = new int*[numRows + 2];
        SobelEdgeAry = new int*[numRows + 2];


        for (int i = 0; i < numRows + 2; i++) {
            mirrorFramedAry[i] = new int[numCols + 2];
            RobertEdgeAry[i] = new int[numCols + 2];
            SobelEdgeAry[i] = new int[numCols + 2];
        }

        for(int i=0;i<numRows+2;i++){
            for(int j=0;j<numCols+2;j++){
                mirrorFramedAry[i][j]=0;
                RobertEdgeAry[i][j]=0;
                SobelEdgeAry[i][j]=0;
            }
        }

      


        RobertVertical[0][0] = 1; RobertVertical[0][1] = 1;
        RobertVertical[1][0] = -1;  RobertVertical[1][1] = -1;

        RobertHorizontal[0][0] = 1; RobertHorizontal[0][1] = -1;
        RobertHorizontal[1][0] = 1; RobertHorizontal[1][1] = -1;

        RobertRightDiag[0][0] = 1; RobertRightDiag[0][1] = -1;
        RobertRightDiag[1][0] = -1; RobertRightDiag[1][1] = 1;

        RobertLeftDiag[0][0] = -1; RobertLeftDiag[0][1] = 1;
        RobertLeftDiag[1][0] = 1;  RobertLeftDiag[1][1] = -1;

        SobelVertical[0][0] = 1;    SobelVertical[0][1] = 2;    SobelVertical[0][2] = 1;
        SobelVertical[1][0] = 0;    SobelVertical[1][1] = 0;    SobelVertical[1][2] = 0;
        SobelVertical[2][0] = -1;    SobelVertical[2][1] = -2;    SobelVertical[2][2] = -1;

        SobelHorizontal[0][0] = 1; SobelHorizontal[0][1] = 0; SobelHorizontal[0][2] = -1;
        SobelHorizontal[1][0] = 2; SobelHorizontal[1][1] = 0; SobelHorizontal[1][2] = -2;
        SobelHorizontal[2][0] = 1; SobelHorizontal[2][1] = 0; SobelHorizontal[2][2] = -1;

        SobelRightDiag[0][0] = 2; SobelRightDiag[0][1] = 1; SobelRightDiag[0][2] = 0;
        SobelRightDiag[1][0] = 1; SobelRightDiag[1][1] = 0; SobelRightDiag[1][2] = -1;
        SobelRightDiag[2][0] = 0; SobelRightDiag[2][1] = -1; SobelRightDiag[2][2] = -2;

        SobelLeftDiag[0][0] = 0; SobelLeftDiag[0][1] = 1; SobelLeftDiag[0][2] = 2;
        SobelLeftDiag[1][0] = -1; SobelLeftDiag[1][1] = 0; SobelLeftDiag[1][2] = 1;
        SobelLeftDiag[2][0] = -2; SobelLeftDiag[2][1] = -1; SobelLeftDiag[2][2] = 0;

    }


    void loadImg(ifstream& input, int** array){
        for (int i = 1; i <= numRows; i++) {
            for (int j = 1; j <= numCols; j++) {
                input >> array[i][j];
                }
            }
    }

    void mirrorFraming(int **mirrorFramedAry){

        for(int i=1;i<numCols+1;i++){
            mirrorFramedAry[0][i]=mirrorFramedAry[1][i];
            mirrorFramedAry[numRows+1][i]=mirrorFramedAry[numRows][i];
        }

        for(int i=0;i<numRows+2;i++){
            mirrorFramedAry[i][0]=mirrorFramedAry[i][1];
            mirrorFramedAry[i][numCols+1]=mirrorFramedAry[i][numCols];
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

    int computeRobert(int row, int col, int array[2][2]){
        int first=mirrorFramedAry[row][col]*array[0][0];
        int second=mirrorFramedAry[row][col+1]*array[0][1];
        int third=mirrorFramedAry[row+1][col]*array[1][0];
        int fouth=mirrorFramedAry[row+1][col+1]*array[1][1];

        return (first+second+third+fouth);
    }

    
    void RobertEdgeDetector (int **mirrorFramedAry, int **RobertEdgeAry, ofstream& debug){
        debug<<"entering RobertEdgeDetector method \n";
        minVal=99990;
        maxVal=0;

        int i=1;

        while(i<=numRows){
        int j=1;

            while(j<=numCols){
                int temV= abs(computeRobert(i,j, RobertVertical));
                int temH= abs(computeRobert(i,j, RobertHorizontal));
                int temR= abs(computeRobert(i,j, RobertRightDiag));
                int temL= abs(computeRobert(i,j, RobertLeftDiag));

                RobertEdgeAry[i][j]=temV+temH+temR+temL;
                if(RobertEdgeAry[i][j]>maxVal){
                    maxVal=RobertEdgeAry[i][j];
                }
                if(RobertEdgeAry[i][j]<minVal){
                    minVal=RobertEdgeAry[i][j];
                }

                j++;
            }
        i++;
        }

        histRobertAry = new int[maxVal+1];
        for(int i=0;i<maxVal+1;i++){
            histRobertAry[i]=0;
        }
        debug<<"Leaving RobertEdgeDetector method \n";

    }

    int computeSobel(int row, int col, int array[3][3]){
        int a = mirrorFramedAry[row-1][col-1]*array[0][0];
        int b = mirrorFramedAry[row-1][col]*array[0][1];
        int c = mirrorFramedAry[row-1][col+1]*array[0][2];

        int d = mirrorFramedAry[row][col-1]*array[1][0];
        int x = mirrorFramedAry[row][col]*array[1][1];
        int e = mirrorFramedAry[row][col+1]*array[1][2];

        int f = mirrorFramedAry[row+1][col-1]*array[2][0];
        int g = mirrorFramedAry[row+1][col]*array[2][1];
        int h = mirrorFramedAry[row+1][col+1]*array[2][2];

        return (a+b+c+d+e+f+g+h+x);
    }


    void SobelEdgeDetector(int **mirrorFramedAry, int **SobelEdgeAry, ofstream& debug){
        debug<<"entering SobelEdgeDetector method \n";
        minVal=99990;
        maxVal=0;

        int i=1;

        while(i<=numRows){//
        int j=1;

            while(j<=numCols){//
                int temV= abs(computeSobel(i,j, SobelVertical));
                int temH= abs(computeSobel(i,j, SobelHorizontal));
                int temR= abs(computeSobel(i,j, SobelRightDiag));
                int temL= abs(computeSobel(i,j, SobelLeftDiag));

                SobelEdgeAry[i][j]=temV+temH+temR+temL;
                if(SobelEdgeAry[i][j]>maxVal){
                    maxVal=SobelEdgeAry[i][j];
                }
                if(SobelEdgeAry[i][j]<minVal){
                    minVal=SobelEdgeAry[i][j];
                }

                j++;
            }
        i++;
        }
        histSobelAry=new int[maxVal+1];
        for(int i=0;i<maxVal+1;i++){
            histSobelAry[i]=0;
        }
        debug<<"Leaving SobelEdgeDetector method \n";
    }

    void computeHist (int **inAry, int *histAry, ofstream& debug){
        debug<<"entering computeHist method \n";
        int i=1;

        while(i<=numRows){
            int j=1;

            while(j<=numCols){
                histAry[inAry[i][j]]++;
                j++;
            }
            i++;
        }

        debug<<"Leaving computeHist method \n";
    }

    void printHist (int *histAry, ofstream& histFile, ofstream& debug){
        debug<<"entering printHist method \n";
        histFile<<numRows<<" "<<numCols<<" "<<minVal<<" "<<maxVal<<"\n";

        int index=0;

        while(index<=maxVal){
            histFile<<index<<" "<<histAry[index]<<"\n";
            index++;
        }

        debug<<"Leaving printHist method \n";
    } 




};


int main(int argc, char* argv[]) {

    ifstream input;
    ofstream output;
    ofstream debug;
    int choose;




    input.open(argv[1]);
    choose=stoi(argv[2]);
    output.open(argv[3]);
    debug.open(argv[4]);



    int numRows;
    int numCols;
    int MinVal;
    int MaxVal;

    input>>numRows;
    input>>numCols;
    input>>MinVal;
    input>>MaxVal;

    EdgeDetector ed(numRows, numCols, MinVal, MaxVal);


    ed.loadImg(input, ed.mirrorFramedAry);
    ed.mirrorFraming(ed.mirrorFramedAry);
    ed.imgReformat(ed.mirrorFramedAry, output);

    if(choose==1){
        ed.RobertEdgeDetector(ed.mirrorFramedAry, ed.RobertEdgeAry, debug);
        ed.computeHist(ed.RobertEdgeAry, ed.histRobertAry, debug);
        ofstream RobertEdgeFile(string(argv[1])+"_Robert_edge.txt");

        ed.imgReformat(ed.RobertEdgeAry,output);
        ed.imgReformat(ed.RobertEdgeAry, RobertEdgeFile);
        ofstream histRobertFile(string(argv[1])+"_Robert_hist.txt");

        ed.printHist(ed.histRobertAry, histRobertFile, debug);

        RobertEdgeFile.close();
        histRobertFile.close();
    }
    else if(choose ==2){
        ed.SobelEdgeDetector(ed.mirrorFramedAry, ed.SobelEdgeAry, debug);
        ed.computeHist(ed.SobelEdgeAry, ed.histSobelAry, debug);
        ofstream SobelEdgeFile(string(argv[1])+"_Sobel_edge.txt");

        ed.imgReformat(ed.SobelEdgeAry,output);
        ed.imgReformat(ed.SobelEdgeAry, SobelEdgeFile);
        ofstream histSobelFile(string(argv[1])+"_Sobel_hist.txt");

        ed.printHist(ed.histSobelAry, histSobelFile, debug);

        SobelEdgeFile.close();
        histSobelFile.close();
    }





    input.close();
    output.close();
    debug.close();


}