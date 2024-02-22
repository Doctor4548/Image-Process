
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class Thinning{
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	
	int changeCount;
	int cycleCount;
	int [][]arrayOne;
	int [][]arrayTwo;
	int []neighborAry;
	
	Thinning(int nR, int nC, int min, int max){
		
		numRows=nR;
		numCols=nC;
		minVal=min;
		maxVal=max;
		
		arrayOne = new int[numRows+2][numCols+2];
		arrayTwo = new int[numRows+2][numCols+2];
		neighborAry = new int[9];
		
		changeCount=1;
		cycleCount=0;
		
		for(int i=0;i<numRows+2;i++) {
			for(int j=0;j<numCols+2;j++) {
				arrayOne[i][j]=0;
				arrayTwo[i][j]=0;
			}
		}
		
		for(int i=0;i<9;i++) {
			neighborAry[i]=0;
		}
		
	}
	
	void loadImage(Scanner input, int[][] array) {
		while(input.hasNext()) {
			for (int i = 1; i < (numRows + 1); i++) {
				for (int j = 1; j < (numCols + 1); j++) {
					array[i][j] = input.nextInt();
				}
			}
		}
	}
	
	int loadNeighborAry(int []array, int[][] arrayOne, int i, int j) {
		int index = 0;
		int counter = 0; 
		
		for(int row = i - 1; row < i + 2; row++) {
			for(int col = j - 1; col < j + 2; col++) {
				array[index] = arrayOne[row][col];
				if(array[index] > 0) {
					counter++;
				}
				index++;
			}			
		}
		return counter-1;		
	}
	
	void copyArys(int[][] two, int[][] one) {
		for(int i = 0; i < numRows + 2; i++) {
			for(int j = 0; j < numCols + 2; j++) {
				one[i][j] = two[i][j];
			}
		}
	}
	
	boolean checkConnector(int [] neighbors) {//
		int a = neighbors[0]; 
		int b = neighbors[1]; 
		int c = neighbors[2];
		int d = neighbors[3]; 
		int x = neighbors[4]; 
		int e = neighbors[5];
		int f = neighbors[6]; 
		int g = neighbors[7]; 
		int h = neighbors[8];
		
		if(b==0&&g==0) {
			return true;
		}
		else if(d==0&&e==0) {
			return true;
		}
		else if(a==1&&b==0&&d==0) {
			return true;
		}
		else if(b==0&&c==1&&e==0) {
			return true;
		}
		else if(d==0&&f==1&&g==0) {
			return true;
		}
		else if(e==0&&g==0&&h==1) {
			return true;
		}
		else {
			return false;	
		}
		
			
	}
	
	void northThinning(int [][]aryOne, int[][] aryTwo, BufferedWriter debug) throws IOException {
		debug.write("Entering northThinning (); cycleCount = "+ cycleCount+" ; "+ "changeCount = "+ changeCount+"\n");
		int i=1;
		
		while(i<=numRows+1) {
			int j=1;
			
			while(j<=numCols+1) {
				aryTwo[i][j]=aryOne[i][j];//
				
				
				if(aryOne[i][j]>0&&aryOne[i-1][j]<=0) {
					int nonZeroCount=loadNeighborAry(neighborAry, aryOne, i , j);
					boolean flag=checkConnector(neighborAry);
					debug.write("In northThinning, i = "+ i +"; j = "+j+"; nonZeroCount = "+nonZeroCount+"; Flag = "+flag+"; \n");
					if(nonZeroCount>=4&&flag==false) {
						aryTwo[i][j]=0;
						changeCount++;
					}
					else {
						aryTwo[i][j]=aryOne[i][j];
					}					
			}
				j++;
		}
			i++;	
		}
		debug.write("Leaving northThinning(); cycleCount = "+cycleCount+"; changeCount = "+changeCount+"; \n");
	}
	
	void southThinning(int [][]aryOne, int[][] aryTwo, BufferedWriter debug) throws IOException {
		debug.write("Entering southThinning (); cycleCount = "+ cycleCount+" ; "+ "changeCount = "+ changeCount+"\n");
		
		int i=1;
		
		while(i<=numRows+1) {
			int j=1;
			
			while(j<=numCols+1) {
				aryTwo[i][j]=aryOne[i][j];//
				
				if(aryOne[i][j]>0&&aryOne[i+1][j]<=0) {
					int nonZeroCount=loadNeighborAry(neighborAry, aryOne, i , j);
					boolean flag=checkConnector(neighborAry);
					debug.write("In southThinning, i = "+ i +"; j = "+j+"; nonZeroCount = "+nonZeroCount+"; Flag = "+flag+"; \n");
					if(nonZeroCount>=4&&flag==false) {
						aryTwo[i][j]=0;
						changeCount++;
					}
					else {
						aryTwo[i][j]=aryOne[i][j];
					}
			}
				j++;
		}
			i++;
		
		}
		debug.write("Leaving southThinning(); cycleCount = "+cycleCount+"; changeCount = "+changeCount+"; \n");
	}
	
	void westThinning(int [][]aryOne, int[][] aryTwo, BufferedWriter debug) throws IOException {
		debug.write("Entering westThinning (); cycleCount = "+ cycleCount+" ; "+ "changeCount = "+ changeCount+"\n");
		
		int i=1;
		
		while(i<=numRows+1) {
			int j=1;
			
			while(j<=numCols+1) {
				aryTwo[i][j]=aryOne[i][j];//
				
				if(aryOne[i][j]>0&&aryOne[i][j-1]==0) {
					int nonZeroCount=loadNeighborAry(neighborAry, aryOne, i , j);
					boolean flag=checkConnector(neighborAry);
					debug.write("In westThinning, i = "+ i +"; j = "+j+"; nonZeroCount = "+nonZeroCount+"; Flag = "+flag+"; \n");
					if(nonZeroCount>=4&&flag==false) {
						aryTwo[i][j]=0;
						changeCount++;
					}
					else {
						aryTwo[i][j]=aryOne[i][j];
					}
			}
				j++;
		}
			i++;
		}
		debug.write("Leaving westThinning() ; cycleCount = "+cycleCount+"; changeCount = "+changeCount+"; \n");
	}
	
	void eastThinning(int [][]aryOne, int[][] aryTwo, BufferedWriter debug) throws IOException {
		debug.write("Entering eastThinning (); cycleCount = "+ cycleCount+" ; "+ "changeCount = "+ changeCount+"\n");
		
		int i=1;
		
		while(i<=numRows+1) {
			int j=1;
			
			while(j<=numCols+1) {
				
				aryTwo[i][j]=aryOne[i][j];//
				
				if(aryOne[i][j]>0&&aryOne[i][j+1]==0) {
					int nonZeroCount=loadNeighborAry(neighborAry, aryOne, i , j);
					boolean flag=checkConnector(neighborAry);
					debug.write("In eastThinning, i = "+ i +"; j = "+j+"; nonZeroCount = "+nonZeroCount+"; Flag = "+flag+"; \n");
					if(nonZeroCount>=4&&flag==false) {
						aryTwo[i][j]=0;
						changeCount++;
					}
					else {
						aryTwo[i][j]=aryOne[i][j];
					}
			}
				j++;
		}
			i++;
			
		}
		debug.write("Leaving eastThinning(); cycleCount = "+cycleCount+"; changeCount = "+changeCount+"; \n");
	}
	
	void reformatPrettyPrint(int [][]array, BufferedWriter debug) throws IOException {
		debug.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
		
		for(int i=0;i<array.length;i++) {
			for(int j=0;j<array[0].length;j++) {
				if(array[i][j]>0) {
					debug.write(array[i][j]+" ");
				}
				else {
					debug.write(". ");
				}
			}
			debug.write("\n");
		};
		
	}
	
	void thinning(int [][] aryOne, int[][] aryTwo, BufferedWriter debug) throws IOException {
		debug.write("Entering thinning () before thinning 4 sides, aryOne is below: \n");
		reformatPrettyPrint(aryOne, debug);
		changeCount=0;
		
		northThinning(aryOne,aryTwo,debug);
		debug.write("After northThinning; aryTwo is below: \n");
		reformatPrettyPrint(aryTwo,debug);
		copyArys(aryTwo, aryOne);
		
		southThinning(aryOne, aryTwo, debug);
		debug.write("In thinning, after SouthThinning aryTwo is below: \n");
		reformatPrettyPrint(aryTwo, debug);
		copyArys(aryTwo, aryOne);
		
		westThinning(aryOne,aryTwo,debug);
		debug.write("after WestThinning aryTwo is below: \n");
		reformatPrettyPrint(aryTwo,debug);
		copyArys(aryTwo, aryOne);
		
		eastThinning(aryOne,aryTwo,debug);
		debug.write("after EastThinning aryTwo is below: \n");
		reformatPrettyPrint(aryTwo,debug);
		copyArys(aryTwo, aryOne);		
		
		debug.write("Leaving thinning (); cycleCount = "+ cycleCount+"; changeCount = "+changeCount+"; \n");
		
	}
	
	
	void imgReformat(int[][] array, BufferedWriter output) throws IOException {
		output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
		
		for(int i=0;i<array.length;i++) {
			for(int j=0;j<array[0].length;j++) {
				if(array[i][j]>0) {
					output.write(array[i][j]+" ");
				}
				else {
					output.write(array[i][j]+" ");
				}
			}
			output.write("\n");
		}		
	}
	
	
	
	
}


public class thinningImage {
	public static void main(String[] args) throws IOException {
        Scanner input=new Scanner(new FileReader(args[0]));
        BufferedWriter output= new BufferedWriter(new FileWriter(args[1]));
        BufferedWriter debug = new BufferedWriter(new FileWriter(args[2]));

		
		int numRows=input.nextInt();
		int numCols=input.nextInt();
		int minVal=input.nextInt();
		int maxVal=input.nextInt();
		
		output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
		Thinning thin=new Thinning(numRows,numCols,minVal,maxVal);
		thin.loadImage(input, thin.arrayOne);
		output.write("In main(), before thinning, changeCount = "+thin.changeCount+"; cycleCount = "+thin.cycleCount+"; \n");
		thin.reformatPrettyPrint(thin.arrayOne, output);
		
		while(thin.changeCount>0) {
			thin.thinning(thin.arrayOne,thin.arrayTwo, debug);
			thin.cycleCount++;
			output.write("In main(), inside iteration, changeCount = "+thin.changeCount+"; cycleCount = "+thin.cycleCount+"; \n");
			thin.reformatPrettyPrint(thin.arrayOne, output);
		}
		output.write("In main(), the final skeleton, changeCount = "+thin.changeCount+"; cycleCount = "+thin.cycleCount+"; \n");
		thin.imgReformat(thin.arrayOne, output);
		
		input.close();
		output.close();
		debug.close();
		
	}
}


