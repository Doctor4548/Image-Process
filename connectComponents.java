import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

class Property {
    int label;
    int numPixels;
    int minR;
    int minC;
    int maxR;
    int maxC;

    public Property() {
    }


}

class ccLabel{
    int numRows;
    int numCols;
    int minVal;
    int maxVal;
    int newLabel;
    int trueNumCC;
    int newMin;
    int newMax;
    int[][] zeroFramedAry;
    int[] NonZeroNeighborAry;
    int[] EQAry;
    char option;
    Property[] CCproperty;
	
    
    public ccLabel(int numRows, int numCols, int minVal, int maxVal, int connectness) {
    	this.newLabel = 0;
    	this.newMin=0;
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        //this.trueNumCC=connectness;//
        //this.newMax = trueNumCC;//


        this.zeroFramedAry = new int[numRows + 2][numCols + 2];
        this.NonZeroNeighborAry= new int[5];



        int EQArySize = (numRows * numCols) / 4;
        this.EQAry = new int[EQArySize];
        for (int i = 0; i < EQArySize; i++) {
            this.EQAry[i] = i;
        }

    }
    
    void zero2DAry(int [][]zeroFramedAry) {
    	for(int i=0;i<zeroFramedAry.length;i++) {
    		for(int j=0;j<zeroFramedAry[0].length;j++) {
    			zeroFramedAry[i][j]=0;
    		}
    	}
    }
    
    void loadImg(Scanner input, int[][] zeroFrameAry) {
    	for(int i=1;i<zeroFrameAry.length-1;i++) {
    		for(int j=1;j<zeroFrameAry[0].length-1;j++) {
    			zeroFramedAry[i][j]=input.nextInt();
    		}
    	}	
    }
    
    
    int connect4Pass1(int[][] zeroFramedAry, int newLabel, int [] EQAry) {//
    	for(int i=1;i<zeroFramedAry.length-1;i++) {
    		for(int j=1;j<zeroFramedAry[0].length-1;j++) {
    			int a=zeroFramedAry[i-1][j];
    			int b=zeroFramedAry[i][j-1];
    					
    			if(zeroFramedAry[i][j]>0) { 
    				if((a==0)&&(b==0)) {
    					newLabel++;
    					//EQAry[zeroFramedAry[i][j]]=newLabel;
    					zeroFramedAry[i][j]=newLabel;

    				}
    				else if(a>0&&b==0) {
    					zeroFramedAry[i][j]=a;
    				}
    				else if(b>0&&a==0) {
    					zeroFramedAry[i][j]=b;
    				}
    				else if(a!=b) {
    					if(a<=b) {
    						zeroFramedAry[i][j]=a;
        					EQAry[newLabel]=zeroFramedAry[i][j];
    						EQAry[b]=a;
    					}
    					else {
    						zeroFramedAry[i][j]=b;
        					EQAry[newLabel]=zeroFramedAry[i][j];
    						EQAry[a]=b;
    					}					
    					//zeroFramedAry[i][j]=Math.min(a, b);
    					//EQAry[newLabel]=zeroFramedAry[i][j];

    				}
    			}

    			
    		}
    	}
    	return newLabel;
    	
    }
    
    void imgReformat(int [][]zeroFramedAry, BufferedWriter output) throws IOException {
    	output.write("\n");
    	for(int i=1;i<zeroFramedAry.length-1;i++) {
    		for(int j=1;j<zeroFramedAry[0].length-1;j++) {
    			if(zeroFramedAry[i][j]>9) {
        			output.write(zeroFramedAry[i][j]+" "); 				
    			}
    			else if(zeroFramedAry[i][j]>0) {
    				output.write(zeroFramedAry[i][j]+"  "); 	
    			}
    			else {
    				output.write(".  ");
    			}

    		}
			output.write("\n");
    	}

    	
    }
    
    void printEQAry (int newLabel, BufferedWriter RFprettyPrintFile) throws IOException {
    	for(int i=1;i<=newLabel;i++) {
    		RFprettyPrintFile.write(EQAry[i]+" ");
    	}
    	RFprettyPrintFile.write("\n");
    }
    
    void Connect4Pass2(int[][] zeroFramedAry,int [] EQAry) {
    	for(int i=zeroFramedAry.length-2;i>0;i--) {
    		for(int j=zeroFramedAry[0].length-2;j>0;j--) {
    			int c=zeroFramedAry[i][j+1];
    			int d=zeroFramedAry[i+1][j];
    			if(zeroFramedAry[i][j]>0) {
    				if(c == 0&&d == 0) {
    					zeroFramedAry[i][j]=zeroFramedAry[i][j];
    				}
    				else if((c!=0&&c==d)||(d!=0&&d==c)||(c!=0&&c==zeroFramedAry[i][j])||(d!=0&&d==zeroFramedAry[i][j])) {

    				}
    				else if((c!=0&&d!=0&&c!=d)||(c!=0&&d!=0&&c!=zeroFramedAry[i][j])||(c!=0&&d!=0&&d!=zeroFramedAry[i][j])) {
    					int min=Math.min(c,d);
    					int minLabel=Math.min(min, zeroFramedAry[i][j]);
    					if(zeroFramedAry[i][j]>minLabel) {
    						zeroFramedAry[i][j]=minLabel;
    					}
    				}
					boolean pass=false;
					for(int z=1;z<=newLabel;z++) {
						if(EQAry[z]==zeroFramedAry[i][j]) {
							pass=true;
						}
					}
					if(pass==false) {
						zeroFramedAry[i][j]=EQAry[zeroFramedAry[i][j]];
					}    					
    					
    			}
    		}
    	}
    }
    
    int manageEQAry (int []EQAry, int newLabel) {
    	int readLabel=0;
    	int index=1;
    	while(index<=newLabel) {
	    	if(index!=EQAry[index]) {
	    		EQAry[index]=EQAry[EQAry[index]];
	    	}
	    	else {
	    		readLabel++;
	    		EQAry[index]=readLabel;
	    	}
	    	index++;
    	}
    	return readLabel;
    }
    
    void connectPass3(int [][]zeroFramedAry, int []EQAry, Property []CCproperty, int trueNumCC, BufferedWriter debug) throws IOException {
    	debug.write("entering connectPas3 method \n");
    	for(int i=1;i<=trueNumCC;i++) {
    		CCproperty[i].label=i;
    		CCproperty[i].numPixels=0;
    		CCproperty[i].minR=numRows;
    		CCproperty[i].maxR=0;
    		CCproperty[i].minC=numCols;
    		CCproperty[i].maxC=0;
    	}
    	
    	for(int r=1;r<zeroFramedAry.length-1;r++) {
    		for(int c=1;c<zeroFramedAry[0].length-1;c++) {
    			if(zeroFramedAry[r][c]>0) {
	    			zeroFramedAry[r][c]=EQAry[zeroFramedAry[r][c]];
	    			int k=zeroFramedAry[r][c];
	    			CCproperty[k].numPixels++;
	    			if(r < CCproperty[k].minR) {
	    				CCproperty[k].minR=r-1;//
	    			}
	    			if(r > CCproperty[k].maxR) {
	    				CCproperty[k].maxR=r-1;//
	    			}
	    			if(c < CCproperty[k].minC) {
	    				CCproperty[k].minC=c-1;//
	    			}
	    			if(c > CCproperty[k].maxC) {
	    				CCproperty[k].maxC=c-1;//
	    			}
	    			
    			}
    			
    		}
    	}
    	debug.write("leaving connectPas3 method\n");
    }
    
    void connected4(int [][]zeroFramedAry, int newLabel, int [] EQAry, BufferedWriter RFprettyPrintFile, BufferedWriter debug) throws IOException {
    	debug.write("entering connected4 method \n");
    	newLabel=connect4Pass1(zeroFramedAry, newLabel, EQAry);//
    	debug.write("After connected4 pass1, newLabel: "+newLabel+"\n");
    	RFprettyPrintFile.write("Result of: Pass 1 ");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\n Equivalency Table after: Pass 1 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);

    	 

    	Connect4Pass2(zeroFramedAry, EQAry);
    	debug.write("After connected4 pass2, newLabel: "+newLabel+"\n");  	
    	RFprettyPrintFile.write("\nResult of: Pass 2");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\nEquivalency Table after: Pass 2 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);    	
    	
    	
    	
    	trueNumCC=manageEQAry (EQAry, newLabel);
    	RFprettyPrintFile.write("\nEquivalency Table after: EQ Table Management (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);
    	newMin=0;
    	newMax=trueNumCC;
        this.CCproperty = new Property[trueNumCC + 1];//
        for (int i = 0; i <= trueNumCC; i++) {
            this.CCproperty[i] = new Property();
        }
    	debug.write("In connected4, after manage EQAry, trueNumCC = "+trueNumCC+"\n");
    	
    	connectPass3(zeroFramedAry, EQAry, CCproperty, trueNumCC, debug);
    	RFprettyPrintFile.write("\nResult of: Pass 3");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\nEquivalency Table after: Pass 3 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);
    	debug.write("Leaving connected4 method\n");
    }
    
    int connect8Pass1(int[][] zeroFramedAry, int newLabel, int [] EQAry) {//
    	for(int i=1;i<zeroFramedAry.length-1;i++) {
    		for(int j=1;j<zeroFramedAry[0].length-1;j++) {
  
    			if(zeroFramedAry[i][j]>0) { 
    				
        			int a=zeroFramedAry[i-1][j-1];
        			int b=zeroFramedAry[i-1][j];
        			int c=zeroFramedAry[i-1][j+1];
        			int d=zeroFramedAry[i][j-1];
        			
        			int [] array= {a,b,c,d};
        			int tem=0;
        			
        			boolean case1=false;
        			boolean case2=false;
        			boolean case3=false;
        			
        			if((a==0)&&(b==0)&&(c==0)&&(d==0)) {
        				case1=true;
        			}
        			
        			for(int z=0;z<4;z++) {
        				if(array[z]>0&&tem!=0) {
        					if(array[z]!=tem) {
        						case2=false;
        						case3=true;
        						break;
        					}
        					else {
        						case2=true;
        					}
        				}
        				else if(array[z]>0) {
        					case2=true;
        					tem=array[z];
        				}
        			}
/*        			
        			int tem2=0;
        			for(int z=0;z<4;z++) {
        				if(array[z]>0&&tem2!=0&&array[z]!=tem2) {
        					case3=true;
        					break;
        				}
        				else if(array[z]>0) {
        					case3=false;;
        					tem2=array[z];
        				}
        			}    	*/		
        			
        		
    				
    				if(case1) {
    					newLabel++;					
    					zeroFramedAry[i][j]=newLabel;
    					//EQAry[zeroFramedAry[i][j]]=newLabel;
    				}
    				else if(case2) {
    					int max1=Math.max(a, b);
    					int max2=Math.max(c, d);
    					zeroFramedAry[i][j]=Math.max(max1, max2);
    				}
    				else if(case3) {
    					/*int max1=Math.max(a, b);
    					int max2=Math.max(c, d);
    					int minlabel=Math.max(max1, max2);
    					for(int z=0;z<4;z++) {
    						if(array[z]>0) {
    							if(array[z]<minlabel) {
    								minlabel=array[z];
    							}
    						}
    					}		*/
    					if((c<b||b==0)&&(c<a||a==0)&&(c<d||d==0)&&(c>0)) {
    						if(c<b) {
        						zeroFramedAry[i][j]=c;
            					//EQAry[newLabel]=zeroFramedAry[i][j];

            					EQAry[b]=c;      						


    						}
    						else if(c<a) {
        						zeroFramedAry[i][j]=c;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[a]=c;
    						}
    						else if(c<d) {
        						zeroFramedAry[i][j]=c;
            					
            					//EQAry[newLabel]=zeroFramedAry[i][j];
            					EQAry[d]=c;



        						
        						}
    					}
    					else if((a<b||b==0)&&(a<c||c==0)&&(a<d||d==0)&&(a>0)) {
    						if(a<b) {
        						zeroFramedAry[i][j]=a;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[b]=a;
    						}
    						else if(a<c) {
        						zeroFramedAry[i][j]=a;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[c]=a;
    						}
    						else if(a<d) {
        						zeroFramedAry[i][j]=a;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[d]=a;
    						}
    					}
    					else if((b<a||a==0)&&(b<c||c==0)&&(b<d||d==0)&&(b>0)) {
    						if(b<a) {
        						zeroFramedAry[i][j]=b;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[a]=b;
    						}
    						else if(a<c) {
        						zeroFramedAry[i][j]=b;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[c]=b;
    						}
    						else if(a<d) {
        						zeroFramedAry[i][j]=b;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[d]=b;
    						}
    					}
    					else if((d<=b||b==0)&&(d<=c||c==0)&&(d<=a||a==0)&&(d>0)) {
    						if(d<b) {
        						zeroFramedAry[i][j]=d;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[b]=d;
    						}
    						else if(d<c) {
        						zeroFramedAry[i][j]=d;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[c]=d;
    						}
    						else if(d<a) {
        						zeroFramedAry[i][j]=d;
            					//EQAry[newLabel]=zeroFramedAry[i][j];
        						EQAry[a]=d;
    						}

    					}
    					
    					
       					//zeroFramedAry[i][j]=minlabel;
    					//EQAry[newLabel]=zeroFramedAry[i][j];
    					
    					/*if(a<=b) {
    						zeroFramedAry[i][j]=a;
        					EQAry[newLabel]=zeroFramedAry[i][j];
    						EQAry[b]=a;
    					}
    					else {
    						zeroFramedAry[i][j]=b;
        					EQAry[newLabel]=zeroFramedAry[i][j];
    						EQAry[a]=b;
    					}	*/
    				}

    			}
	
    		}
    	}
    	return newLabel;
    	
    }
    
    void Connect8Pass2(int[][] zeroFramedAry,int [] EQAry) {
    	for(int i=zeroFramedAry.length-2;i>0;i--) {
    		for(int j=zeroFramedAry[0].length-2;j>0;j--) {

    			if(zeroFramedAry[i][j]>0) {
    				int e=zeroFramedAry[i][j+1];
    				int f=zeroFramedAry[i+1][j-1];
    				int g=zeroFramedAry[i+1][j];
    				int h=zeroFramedAry[i+1][j+1];
    				
    				
    				if((e==0)&&(f==0)&&(g==0)&&(h==0)) {
    				 	zeroFramedAry[i][j]=zeroFramedAry[i][j];
    				}
    				else {
        				int[] array= {e,f,g,h};

    					int max1=Math.max(e, f);
    					int max2=Math.max(g, h);
    					int minlabel=Math.max(max1, max2);
    					
    					int tem=0;
    					for(int z=0;z<4;z++) {							
    						if(array[z]>0) {
    							if(tem>0&&array[z]!=tem) {
    							}
    							else{
    								tem=array[z];
    							}
    							if(array[z]<minlabel) {
    								minlabel=array[z];
    							}
    							

    						}
    					}
    					
    			
    					boolean pass=false;
    					for(int z=1;z<=newLabel;z++) {
    						if(EQAry[z]==zeroFramedAry[i][j]) {
    							pass=true;
    						}
    					}
    	
    					if(minlabel>0) {
    						EQAry[zeroFramedAry[i][j]]=minlabel;
    						zeroFramedAry[i][j]=minlabel;
    					}
    					
    					if(pass==false) {
    						zeroFramedAry[i][j]=EQAry[zeroFramedAry[i][j]];
    					}
    					



    					
    					  
    					
    					
    					
    					
    					/*
    					if(change) {
    						EQAry[zeroFramedAry[i][j]]=minlabel;
    						zeroFramedAry[i][j]=minlabel;
    					}
        				
        				
    					boolean pass=false;
    					for(int z=1;z<=newLabel;z++) {
    						if(EQAry[z]==zeroFramedAry[i][j]) {
    							pass=true;
    						}
    					}
    					if(pass==false) {

    						
    						
    						//zeroFramedAry[i][j]=EQAry[zeroFramedAry[i][j]];
    					} */
        				
    				}
    				


    				
    		   					
    					
    			}
    		}
    	}
    }
    
    void connected8(int [][]zeroFramedAry, int newLabel, int [] EQAry, BufferedWriter RFprettyPrintFile, BufferedWriter debug) throws IOException {
    	debug.write("entering connected8 method \n");
    	newLabel=connect8Pass1(zeroFramedAry, newLabel, EQAry);//
    	debug.write("After connected8 pass1, newLabel: "+newLabel+"\n");
    	RFprettyPrintFile.write("Result of: Pass 1 ");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\n Equivalency Table after: Pass 1 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);

    	 

    	Connect8Pass2(zeroFramedAry, EQAry);
    	debug.write("After connected8 pass2, newLabel: "+newLabel+"\n");  	
    	RFprettyPrintFile.write("\nResult of: Pass 2");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\nEquivalency Table after: Pass 2 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);    	
    	
    	
    	
    	trueNumCC=manageEQAry (EQAry, newLabel);
    	RFprettyPrintFile.write("\nEquivalency Table after: EQ Table Management (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);
    	newMin=0;
    	newMax=trueNumCC;
        this.CCproperty = new Property[trueNumCC + 1];//
        for (int i = 0; i <= trueNumCC; i++) {
            this.CCproperty[i] = new Property();
        }
    	debug.write("In connected8, after manage EQAry, trueNumCC = "+trueNumCC+"\n");
    	
    	connectPass3(zeroFramedAry, EQAry, CCproperty, trueNumCC, debug);
    	RFprettyPrintFile.write("\nResult of: Pass 3");
    	imgReformat(zeroFramedAry, RFprettyPrintFile);
    	RFprettyPrintFile.write("\nEquivalency Table after: Pass 3 (indexing starts from 1)\n");
    	printEQAry(newLabel, RFprettyPrintFile);
    	debug.write("Leaving connected8 method\n");
    }
    
    void printCCproperty(BufferedWriter property) throws IOException {
    	for(int i=1;i<CCproperty.length;i++) {
    		property.write(CCproperty[i].label+"\n");
    		property.write(CCproperty[i].numPixels+"\n");
    		property.write(CCproperty[i].minR+" "+CCproperty[i].minC+"\n");
    		property.write(CCproperty[i].maxR+" "+CCproperty[i].maxC+"\n");
    	}
    }
    
    void drawBoxes(int [][]zeroFramedAry, Property []CCproperty, int trueNumCC, BufferedWriter debug) throws IOException {
    	debug.write("entering drawBoxes method\n");
    	int index=1;
    	
    	while(index<=trueNumCC) {
	    	int minRow=CCproperty[index].minR + 1;
	    	int maxRow=CCproperty[index].maxR + 1;
	    	int minCol=CCproperty[index].minC + 1;
	    	int maxCol=CCproperty[index].maxC + 1;
	    	int label=CCproperty[index].label;
	    	
	        for (int i = minCol; i <= maxCol; i++) {
	            zeroFramedAry[minRow][i] = label;
	            zeroFramedAry[maxRow][i] = label;
	        }
	        for (int i = minRow; i <= maxRow; i++) {
	            zeroFramedAry[i][minCol] = label;
	            zeroFramedAry[i][maxCol] = label;
	        }
	    	
	        index++;
    	}
    	debug.write("leaving drawBoxes method\n");
    }

   void printImg(int [][]zeroFramedAry, BufferedWriter output) throws IOException {
    	output.write("\n");
    	for(int i=1;i<zeroFramedAry.length-1;i++) {
    		for(int j=1;j<zeroFramedAry[0].length-1;j++) {
    			if(zeroFramedAry[i][j]>9) {
        			output.write(zeroFramedAry[i][j]+" "); 				
    			}
    			else if(zeroFramedAry[i][j]>0) {
    				output.write(zeroFramedAry[i][j]+"  "); 	
    			}
    			else {
    				output.write("0  ");
    			}

    		}
			output.write("\n");
    	}

    	
    }
    
    void print() {
    	for(int i=0;i<zeroFramedAry.length;i++) {
    		for(int j=0;j<zeroFramedAry[0].length;j++) {
    			System.out.print(zeroFramedAry[i][j]+" ");
    		}
			System.out.println();
    	}
    }
    
    
    
	
}


public class connectComponents {

	public static void main(String[] args) throws IOException {
        Scanner input=new Scanner(new FileReader(args[0]));
        int connectness=Integer.parseInt(args[1]);
        BufferedWriter RFprettyPrintFile= new BufferedWriter(new FileWriter(args[2]));
        BufferedWriter labelFile = new BufferedWriter(new FileWriter(args[3]));
        BufferedWriter propertyFile = new BufferedWriter(new FileWriter(args[4]));
        BufferedWriter deBugFile = new BufferedWriter(new FileWriter(args[5]));
		
		int numRows=input.nextInt();
		int numCols=input.nextInt();
		int minVal=input.nextInt();
		int maxVal=input.nextInt();
		
		ccLabel label=new ccLabel(numRows,numCols,minVal,maxVal,connectness);
		
		label.zero2DAry(label.zeroFramedAry);
		label.loadImg(input, label.zeroFramedAry);
		
		if(connectness==4) {
			label.connected4(label.zeroFramedAry, label.newLabel, label.EQAry, RFprettyPrintFile, deBugFile);
		}
		else if(connectness==8) {
			label.connected8(label.zeroFramedAry, label.newLabel, label.EQAry, RFprettyPrintFile, deBugFile);
		}
	
		labelFile.write(label.numRows+" " +label.numCols+" "+ label.newMin+" "+ label.newMax);
		label.printImg(label.zeroFramedAry, labelFile);
		
		propertyFile.write("Prop. File: \n");
		propertyFile.write(label.numRows+" " +label.numCols+" "+ label.newMin+" "+ label.newMax+"\n"+label.trueNumCC+"\n");
		label.printCCproperty(propertyFile);
		label.drawBoxes(label.zeroFramedAry, label.CCproperty, label.trueNumCC, deBugFile);
		RFprettyPrintFile.write("\nBounding Boxes :");
		label.imgReformat(label.zeroFramedAry, RFprettyPrintFile);
		RFprettyPrintFile.write("\nNumber of Connected Components: "+label.trueNumCC+"\n");
		
		
		
		
		input.close();
		RFprettyPrintFile.close();
		labelFile.close();
		propertyFile.close();
		deBugFile.close();
	}

}



