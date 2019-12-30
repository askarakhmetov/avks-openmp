#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    int n,m;
    cout<<"enter n: "; cin>>n;
    cout<<endl<<"enter m: "; cin>>m;
    cout<<endl;
    int **mas = new int*[n];
    
    for(int i=0;i<n;++i){
        mas[i]=new int[m];
        for(int j=0;j<m;++j){
            cout<<"enter mas[i][j]: ";
            cin>>mas[i][j];
        }
    }
    int min[n];
    int nuofth = omp_get_num_threads();
    #pragma omp parallel for schedule(auto)
    {for(int i=0;i<n;++i) min[i]=mas[i][0];}

    #pragma omp parallel for schedule(auto)
    for(int i=0;i<n;++i){
        for(int j=0;j<m;++j){
           if(min[i]>mas[i][j])min[i]=mas[i][j];
        }
    }
    int minm=min[0];
    
    for(int i=0;i<n;++i){
        if(minm<min[i])minm=min[i];
    }

    cout<<"max of mins: "<<minm;
    return 0;
}
