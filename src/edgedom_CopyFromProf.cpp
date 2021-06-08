#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std; 

// dominance of edges simplest version   //
// TESTED VERSION                        //
 
const int nn=6500; 
const long int mm=6000000; 

string drive;        // drive flag c: d: f:     //
bool displ;          // display flag            //
                      
bool inc[nn][nn];    // the incidence matrix    //
int gsize;           // size of the graph       // 

long int edgen;      // number of edges (list)  //
int e[mm],f[mm];     // auxiliary nodes         //
bool g[mm];          // not deleted edge        //

fstream file1, file2, file3, file4;
           
//-----------------------------------//
// routines for the incidence matrix //
//-----------------------------------//
  
void readinc()
{ // Reads the incidence matrix from the disc. //
  int i,j,x;
   
  // if ( drive=="f" ) 
  // { file1.open("f:\\sharp\\data.dat", ios::in); }  
  // if ( drive=="d" ) 
  // { file1.open("d:\\coloring\\data.dat", ios::in); } 
  // if ( drive=="c" )  
  // { file1.open("c:\\sharp\\data.dat", ios::in); } 

  file1.open("data.dat", ios::in);

  i=1; j=1;
  while(!file1.eof())
  { file1 >> x;
    inc[i][j]=x;
    j++;
    if (j==gsize+1) { j=1; i++; }
  }
  file1.close(); 
}

//---------------------------//
// routines for the edges    //
//---------------------------//

void displayef()
{ // displays the edges on the screen in z columns //
  long int i,j,k,x,y,z;
  
  z=7; x=(edgen/z)+1;
  for (i=edgen+1; i<=edgen+z; i++) { e[i]=0; f[i]=0; g[i]=0; }  
  cout << "The edges." << endl;
  for (i=1; i<=x; i++)
  { cout << endl;
    for (k=1; k<=z; k++)
    { y=(k-1)*x;
      cout << setw(4) << i+y;
      cout << setw(4) << e[i+y];
      cout << setw(4) << f[i+y];
      cout << setw(4) << g[i+y];
      cout << "     ";
    }   
  }
  cout << endl;
  cout << "edgen=" << edgen << endl;
  cout << "Press 9 to continue" << endl;
  cin >> x;  
}

void fillef()
{ // fills in the edges //
  int i,j;
  long int t;
  
  t=0;
  for (i=1; i<=gsize-1; i++) 
  { for (j=i+1; j<=gsize; j++) 
    { if ( inc[i][j]==1 ) 
	  { e[++t]=i; f[t]=j; g[t]=1; }
    } 
  }
  edgen=t;  
  displayef(); 
}

//---------------------------//
// routines for the fillings //
//---------------------------//

bool decide(long int p,long int q)
{ // decides if edges p and q matching  //
  int i,ep,fp,eq,fq,u,a,v;
  bool w;
  
  // edges p and q matching --> w=1 //
  // identifies u,a,v //
  w=0; 
  if ((p!=q)&&(g[p]!=0)&&(g[q]!=0))
  { ep=e[p]; fp=f[p]; eq=e[q]; fq=f[q];
    if ((w==0)&&(ep==eq)) { u=fp; a=ep; v=fq; w=1; }
    if ((w==0)&&(ep==fq)) { u=fp; a=ep; v=eq; w=1; }
	if ((w==0)&&(fp==eq)) { u=ep; a=fp; v=fq; w=1; }
	if ((w==0)&&(fp==fq)) { u=ep; a=fp; v=eq; w=1; }
  }
  // node u dominates node v with respect to a //
  if (w==1)
  { for (i=1; i<=gsize; i++)
    { if ((inc[i][a]==1)&&(inc[i][u]==0)&&(inc[i][v]==1)) { w=0; }
    }
  }
  if (w==1)
  { g[q]=0; inc[eq][fq]=0; inc[fq][eq]=0; }
  return w;
}

void work()
{ // Removes dominated edges. //
  long int i,j;
  bool change;

  change=1;
  while (change) 
  { change=0;
    for (i=1; i<=edgen; i++)
    { for (j=1; j<=edgen; j++)
      { change=decide(i,j); }
    }
    displayef();
  }  
}


int main()
{ // Setting the initial values. //
  //===========================//
  gsize=135;  displ=1; drive="d";
  //===========================//
  readinc();
  fillef();

  work();
  
 
}




