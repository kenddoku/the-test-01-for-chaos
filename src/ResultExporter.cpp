#include "ResultExporter.h"

#include <fstream>
#include <iostream>

void ResultExporter::add_result(const AnalysisResult* ares) {
    ares_ptr_vec.push_back(ares);
    tseries_num++;
}

void ResultExporter::saveM(const std::string& filename) {
    std::ofstream file(filename);

    if(!file) {
        std::cerr << "ERROR: could not open " << filename << std::endl;
        return;
    }

    for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
        /* PROBLEM: need to know for which intermediate_result to save M values
           POSSIBLE FIX: when finding final K value via median() method figure out 
           the index that's associated with this particular value 
         */
/*         size_t N0 = ares.M.size(); <---- currently commented so program does not break
        for(size_t i=0; i<N0; i++) {
            file << acor.M[i] << '\n';
        } */
       const TimeSeries* ts_local_ptr = ares_ptr_vec[ts_index]->ts_ptr;  
       std::cout << "TimeSeries ("<<ts_index<<") | nu : " << ts_local_ptr->nu << '\n';
    }
    
}//ResultExporter::saveM()

void ResultExporter::savePQ(const std::string& filename) {
    std::ofstream file(filename);
/*  SAME PROBLEM AS ABOVE in saveM method
    size_t N = acor.p.size();
    for(size_t i=0; i<N; i++) {
        file << acor.p[i] << " " << acor.q[i] << '\n';
    } */
}//ResultExporter::savePQ()

void ResultExporter::saveEveryK(const std::string& filename) {
    std::ofstream file(filename);
/*  SAME PROBLEM AS ABOVE....
    size_t N = ares.intermediate_results.size();
    for(size_t i=0; i<N; i++) {
        file << ares.intermediate_results[i].c << " "
            << ares.intermediate_results[i].K_corr << "\n";
    } */
}// ResultExporter::saveEveryK()