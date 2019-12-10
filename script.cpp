#include <bits/stdc++.h>
using namespace std;
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <stdlib.h>   
#include <time.h> 

int main(void){
srand(time(NULL));
ofstream file;
file.open("treeSpacing.txt");
double x=0,y=0,z=0;
//drawing outer ring of trees

//drawing trees on edges with road
for( x=-500;x<500;x+=(rand()%5)+10){
    while(x>-10&&x<10){
        x++;
    }
    file<<x<<" 0 -500 "<<rand()%5+10<<" "<<endl;
}
for( x=-500;x<500;x+=(rand()%5)+10){
    while(x>-10&&x<10){
        x++;
    }
    file<<x<<" 0 500 "<<rand()%5+10<<" "<<endl;
}

//drawing trees on edges without road
for( z=-490;z<490;z+=(rand()%5)+10){
    file<<"500 0 "<<z<<" "<<rand()%5+10<<" "<<endl;
}
for( z=-490;z<490;z+=(rand()%5)+10){
    file<<"-500 0 "<<z<<" "<<rand()%5+10<<" "<<endl;
}

//drawing inner ring of trees
//drawing trees on edges with road
for( x=-485;x<485;x+=(rand()%5)+10){
    while(x>-10&&x<10){
        x++;
    }
    file<<x<<" 0 -485 "<<rand()%5+10<<" "<<endl;
}
for( x=-485;x<485;x+=(rand()%5)+10){
    while(x>-10&&x<10){
        x++;
    }
    file<<x<<" 0 485 "<<rand()%5+10<<" "<<endl;
}

//drawing trees on edges without road
for( z=-475;z<475;z+=(rand()%5)+10){
    file<<"485 0 "<<z<<" "<<rand()%5+10<<" "<<endl;
}
for( z=-475;z<475;z+=(rand()%5)+10){
    file<<"-485 0 "<<z<<" "<<rand()%5+10<<" "<<endl;
}

//drawing 450*450 square with some trees
int some=250;
int h=0;
for(int i=0;i<some;i++){
	x=0;
	while(x>-10&&x<10){
		x=rand()%900-450;
	}

	z=rand()%900-450;
	h=rand()%5+10;
	file<<x<<" "<<y<<" "<<z<<" "<<h<<" ";
}




return 0;
}
