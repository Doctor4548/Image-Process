

import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class thresholdSelection{
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	
	int[] histAry;
	int[] GaussAry;
	int BiGaussThrVal;
	int maxHeight;
	
	thresholdSelection(int row, int col, int min,int max){
		numRows=row;
		numCols=col;
		minVal=min;
		maxVal=max;
		histAry=new int[maxVal+1];
		GaussAry=new int[maxVal+1];
		
		for(int i=0;i<histAry.length;i++) {
			histAry[i]=0;
		}
	}
	
	int loadHist(int[] hist,Scanner input) {
		maxHeight=0;
		
		int count=0;
		while(input.hasNext()) {
			hist[input.nextInt()]=input.nextInt();
			
			if(hist[count]>maxHeight) {
				maxHeight=hist[count];
			}
			count++;
		}
		histAry=hist;
		
		return maxHeight;
	}
	
	void dispHist(BufferedWriter output) throws IOException {
		output.write(numRows+" "+numCols+" "+minVal+" "+maxVal+"\n");
		for(int i=0;i<histAry.length;i++) {
			output.write(i+" ("+histAry[i]+"): ");
			for(int j=0;j<histAry[i];j++) {

				output.write("+");
			}
			output.write("\n");
		}
		
	}
	
	void setZero(int[] array) {
		for(int i=0;i<array.length;i++) {
			array[i]=0;
		}
	}
	
	double computeMean(int leftIndex, int rightIndex, int maxHeight, int[] hist, BufferedWriter debug) throws IOException {
		debug.write("Entering computeMean method \n");
		maxHeight=0;
		int sum=0;
		int numPixels=0;
		
		int index=leftIndex;
		
		while(index<rightIndex) {
			sum+=(hist[index]*index);
			numPixels+=hist[index];
			if(hist[index]>maxHeight) {
				maxHeight=hist[index];
			}
			index++;
		}
		double result=(double)sum/(double)numPixels;
		
		debug.write("Leaving computeMean method maxHeight is an result maxHeight"+maxHeight+", result: "+result+"\n" );
		return result;
		
	}
	
	double computeVar(int leftIndex, int rightIndex, double mean, int[] hist, BufferedWriter debug) throws IOException {
		debug.write("Entering computeVar method \n");
		double sum=0.0;
		double numPixels=0.0;
		int index=leftIndex;
		
		while(index<rightIndex) {
			sum+=(double)hist[index]*Math.pow((double)index-mean, 2);
			numPixels+=hist[index];
			index++;
		}
		
		double result=sum/(double)numPixels;
		debug.write("Leaving computeVar method returning result, result: "+result+"\n");
		return result;
	}
	
	double modifiedGauss(int x, double mean, double var, int maxHeight) {
		return (double) (maxHeight * Math.exp(-Math.pow(((double) x - mean), 2) / (2 * var)));
	}
	
	double fitGauss(int leftIndex, int rightIndex, int[]hist, int[] Gauss, BufferedWriter debug) throws IOException {
		debug.write("Entering fitGauss method \n");
		double mean;
		double var;
		double sum=0.0;
		double Gval;
		double maxGval;
		
		mean=computeMean(leftIndex, rightIndex,maxHeight, hist, debug);
		var=computeVar(leftIndex,rightIndex,mean,histAry,debug);
		
		int index=leftIndex;
		
		while(index<=rightIndex) {
			Gval=modifiedGauss(index,mean,var,maxHeight);
			sum+=Math.abs(Gval-(double)hist[index]);
			GaussAry[index]=(int)Gval;
			index++;
		}
		
		debug.write("leaving fitGauss method, sum is "+sum);
		return sum;
		
	}
	
	int biGaussian(int []histAry,int maxHeight, int minVal, int maxVal,BufferedWriter debug) throws IOException {
		debug.write("Entering biGaussian method \n");
		double sum1;
		double sum2;
		double total;
		double minSumDiff;
		int offset=(int)(maxVal-minVal)/10;
		int dividePt=offset;
		int bestThr=dividePt;
		minSumDiff=99999.0;
		
		while(dividePt<(maxVal-offset)) {
			setZero(GaussAry);
			sum1=fitGauss(0,dividePt,histAry,GaussAry,debug);
			sum2=fitGauss(dividePt,maxVal,histAry,GaussAry,debug);
			
			total=sum1+sum2;
			if(total<minSumDiff) {
				minSumDiff=total;
				bestThr=dividePt;
			}
			
			debug.write("dividePt="+dividePt+"; sum1="+sum1+"; sum2="+sum2+"; total="+total+"; minSumDiff="+minSumDiff+"; bestThr="+bestThr+"\n");
			dividePt++;
		
	}
		
		debug.write("leaving biGaussian method, minSumDiff = bestThr is "+bestThr);
		return bestThr;
	}
}

public class threshold {

	public static void main(String[] args) throws IOException {
		Scanner input=new Scanner(new FileReader(args[0]));
		BufferedWriter output= new BufferedWriter(new FileWriter(args[1]));
		BufferedWriter debug = new BufferedWriter(new FileWriter(args[2]));
		
		int numRows=input.nextInt();
		int numCols=input.nextInt();
		int minVal=input.nextInt();
		int maxVal=input.nextInt();
		
		
		thresholdSelection thre = new thresholdSelection(numRows,numCols,minVal,maxVal);
		
		int[] histAry=new int[maxVal+1];

		int maxHeight=thre.loadHist(histAry,input);
		
		thre.dispHist(output);
		
		int BiGaussThrVal = thre.biGaussian(histAry,maxHeight,minVal,maxVal,debug);
		output.write("\n The Bi-Gaussian auto-selected threshold value: "+BiGaussThrVal);


		
		input.close();
		output.close();
		debug.close();
	}

}
