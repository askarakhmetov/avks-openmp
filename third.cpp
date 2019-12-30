#include <omp.h>
#include <iostream>
using namespace std;

int main(){
    int row1, row2, col1;
    double** a, * b, * cs, *cc, *cb;
    
    cout << "Введите количество строк первой матрицы: ";
    cin >> row1;
    cout << "Введите количество столбцов первой матрицы: ";
    cin >> col1;
    cout << "Введите количество строк второй матрицы: ";
    cin >> row2;
    
    if (col1 != row2)
    {
        cout << "Умножение невозможно!";
        cin.get(); cin.get();
        return 0;
    }
    // Ввод элементов первой матрицы
    a = new double* [row1];
    cout << "Введите элементы первой матрицы" << endl;
    for (int i = 0; i < row1; i++)
    {
        a[i] = new double[col1];
        for (int j = 0; j < col1; j++)
        {
        cout << "a[" << i << "][" << j << "]= ";
        cin >> a[i][j];
        }
    }
    
    // Ввод элементов второй матрицы
    b = new double [row2];
    cout << "Введите элементы второй матрицы" << endl;
    for (int i = 0; i < row2; i++)
    {
        cout << "b[" << i << "]";
        cin >> b[i];
    }
    cs = new double[row2];
    cc=new double [row2];
    cb=new double[row2];
    double runtime;
    #pragma omp parallel for
        for(int i=0;i<row2;++i){
            cc[i]=0;
            cb[i]=0;
            cs[i]=0;
        }
    int count=1;
    do{
        runtime = omp_get_wtime();
        #pragma omp parallel for shared (a,b,cs) num_threads(count)
        for(int i=0;i<row1;++i){
            for(int j=0;j<col1;++j){
                #pragma omp atomic
                cs[i]=cs[i] + a[i][j]*b[j];
            }
        }
        runtime = omp_get_wtime()-runtime;   
        cout<<count<<" threads, lines time is "<<runtime<<endl;     

        runtime = omp_get_wtime();
        #pragma omp parallel for shared (a,b,cc) num_threads(count)
        for(int j=0;j<col1;++j){
            for(int i=0;i<row2;++i){
                #pragma omp atomic
                cc[i]=cc[i] + a[i][j]*b[j];
            }
        }
        runtime = omp_get_wtime()-runtime;
        cout<<count<<" threads, columns time is "<<runtime<<endl;

        runtime = omp_get_wtime();
        int bh = row2 / count;
        int bw = col1 / count;
        #pragma omp parallel shared(a, b, cb) num_threads(count)
        for(int blocks = 0; blocks < count*count; ++blocks)
        {
            int i = blocks / count;
            int j = blocks % count;
            for(int ii = i*bh; ii < (i+1) * bh; ++ii)
                for(int jj = j*bw; jj < (j+1) * bw; ++jj)
                    #pragma omp atomic
                    cb[ii] += a[ii][jj] * b[jj];
        }
        runtime = omp_get_wtime()-runtime;
        cout<<count<<" threads, blocks time is "<<runtime<<endl;

    }while(count<omp_get_thread_num());
    return 0;
}