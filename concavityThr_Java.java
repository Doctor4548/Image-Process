



import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


class concavityThr{
	int numRows;
	int numCols;
	int minVal;
	int maxVal;
	
	int x1;
	int y1;
	int x2;
	int y2;
	
	double m;
	double b;
	
	int [] histAry;
	int maxHeight;
	char[][] histGraph;
	int bestThr;
	
	
	
	concavityThr(int nr,int nc, int minV, int maxV, int firstx, int firsty, int secondx, int secondy){
		numRows=nr;
		numCols=nc;
		minVal=minV;
		maxVal=maxV;
		
		x1=firstx;
		y1=firsty;
		x2=secondx;
		y2=secondy;
		
		
		
		histAry=new int[maxVal+1];
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
	
	void plotHistGraph(char [][]histGraph, int [] histAry) {
		
		for(int i=0;i<maxVal+1;i++) {
			histGraph[i][histAry[i]]='*';
		}
	}
	
	void lineEQ(int x1, int y1, int x2, int y2) {
		m=(double)(y2-y1)/(double)(x2-x1);
		b=(double)y1-(m*(double)x1);
		
	}
	
	int deepestConcavity(int []histAry, int x1, int x2, double m, double b, char [][] histGraph) {
		int max=0;
		int first=x1;
		int second=x2;
		int x=first;
		bestThr=first;
		
		while(x<=second) {
			int y=(int)(m*x +b);
			histGraph[x][y]='+';
			
			int gap=Math.abs(histAry[x]-y);
			if(gap>max) {
				max=gap;
				bestThr=x;
			}
			x++;
		}
		
		return bestThr;
		
	}
	
	void printHistGraph(char[][] graph, BufferedWriter output) throws IOException {
		
		for(int i=0;i<graph.length;i++) {
			for(int j=0;j<graph[0].length;j++) {
				output.write(graph[i][j]+" ");
			}
			output.write("\n");
		}
		
	}
	
	
	
}

public class Project_Q1_Java {

	public static void main(String[] args) throws IOException {
        Scanner input=new Scanner(new FileReader(args[0]));
        Scanner input2=new Scanner(new FileReader(args[1]));
        BufferedWriter output= new BufferedWriter(new FileWriter(args[2]));

		
        
		int numRows=input.nextInt();
		int numCols=input.nextInt();
		int minVal=input.nextInt();
		int maxVal=input.nextInt();
		
		int x1=input2.nextInt();
		int y1=input2.nextInt();
		int x2=input2.nextInt();
		int y2=input2.nextInt();
		
		concavityThr thr=new concavityThr(numRows, numCols, minVal, maxVal, x1,y1,x2,y2);
		
		thr.maxHeight=thr.loadHist(thr.histAry, input);
		
		thr.histGraph=new char[maxVal+1][thr.maxHeight+10];
		for(int i=0;i<thr.histGraph.length;i++) {
			for(int j=0;j<thr.histGraph[0].length;j++) {
				thr.histGraph[i][j]=' ';
			}
		}
		
		thr.plotHistGraph(thr.histGraph,thr.histAry);
		
		thr.dispHist(output);
		
		thr.lineEQ(x1, y1, x2, y2);
		
		int bestThr=thr.deepestConcavity(thr.histAry, x1, x2, thr.m, thr.b,thr.histGraph);
		//thr.bestThr=thr.deepestConcavity(thr.histAry, x1, x2, thr.m, thr.b,thr.histGraph);
		output.write("The bestThrVal is: "+bestThr+"\n");
		thr.printHistGraph(thr.histGraph,output);
        
		
		//output.write(numRows+" "+numCols+" "+minVal+" "+maxVal);
		
		input.close();
		output.close();
		input2.close();
	}

}
