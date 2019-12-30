#include <iostream>
#include <string>
#include <omp.h>
#include <vector>
#pragma omp declare reduction (merge : std::vector<size_t> : omp_out.insert(omp_out.end(), omp_in.begin(), omp_in.end()))
int main()
{
    std::cout << "Enter a string and a substring: " ;

    std::string str, substr;
    std::vector<size_t> ad={};

    std::cin >> str >> substr;
    // omp_get_wtime();
    bool substring_was_found = false ;
    int cnt_char = 0;
    int cnt = 0;
    double runtime;
    int i=1;
    do{
        substring_was_found = false ;
        ad={};
        cnt_char = 0;
        cnt = 0;
        runtime = omp_get_wtime();
        #pragma omp parallel for reduction(merge: ad) num_threads(i)
        for( std::size_t pos = 0 ; pos < str.size() ; ++pos )
        {
            if( str.find( substr, pos ) == pos )
            {
                if( !substring_was_found )
                {
                    //std::cout << "String '" << substr << "' found, position(s): " ;
                    substring_was_found = true ;
                }
                //std::cout <<' '<< pos << ' ' ;
                ad.push_back(pos);
                #pragma omp atomic
                cnt=cnt+1;
                
            }
        }
        std::cout<<omp_get_num_threads()<<"      threads"<<std::endl;
        std::cout<<cnt<<"   num of results"<<std::endl;
        for(int i=0;i<cnt;++i){
            std::cout<<ad[i]<<std::endl;
        }
        runtime=omp_get_wtime()-runtime;
        std::cout<<i<<" threads, runtime is "<<runtime<<std::endl;
    }while(i<omp_get_thread_num());
    
    if( !substring_was_found ) std::cout << "String '" << substr << "' not found" ;
}