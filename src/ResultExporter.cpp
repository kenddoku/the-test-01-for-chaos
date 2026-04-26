#include "ResultExporter.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

void ResultExporter::add_result(const AnalysisResult* ares) {
    ares_ptr_vec.push_back(ares);
    tseries_num++;
}

void ResultExporter::saveK() {
    if(tseries_num == 0) {
        std::cerr << "ERROR: saveK(), No results to save (tseries_num == 0)" << std::endl;
        return;
    }
    // Code below assumes every time series has the same values of nu_int, cp_val and cp_name
    // Otherwise the behaviour is not valid
    int nu_int          = ares_ptr_vec[0]->ts_ptr->nu_int;
    int cp_val_int      = ares_ptr_vec[0]->ts_ptr->cp_val_int;
    std::string cp_name = ares_ptr_vec[0]->ts_ptr->cp_name;
    std::ostringstream oss;
    oss << base_path << "/K/" << cp_name << "/nu_" << nu_int;  
    fs::path K_out_dir = oss.str();

    if(!fs::exists(K_out_dir)) {
        fs::create_directories(K_out_dir);
    }

    oss << "/" << cp_name << "_" << cp_val_int << ".txt";
    fs::path K_out_path = oss.str();

    std::ofstream file(K_out_path);

    for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
        KWithIndex KWI_corr = ares_ptr_vec[ts_index]->KWI_corr;
        size_t K_index = KWI_corr.index;
        double c = ares_ptr_vec[ts_index]->intermediate_results[K_index].c;

        file << std::fixed << std::setprecision(6) << c << " " << KWI_corr.K << " ";
    }

    file.close();
}

void ResultExporter::saveM() {
    // std::ofstream file(filename);

    // if(!file) {
    //     std::cerr << "ERROR: could not open " << filename << std::endl;
    //     return;
    // }

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
       std::cout << "TimeSeries ("<<ts_index<<") | nu : " << ts_local_ptr->nu_int << '\n';
    }
    
}//ResultExporter::saveM()

void ResultExporter::savePQ() {
    //std::ofstream file(filename);
/*  SAME PROBLEM AS ABOVE in saveM method
    size_t N = acor.p.size();
    for(size_t i=0; i<N; i++) {
        file << acor.p[i] << " " << acor.q[i] << '\n';
    } */
}//ResultExporter::savePQ()

void ResultExporter::saveEveryK() {
    //std::ofstream file(filename);
/*  SAME PROBLEM AS ABOVE....
    size_t N = ares.intermediate_results.size();
    for(size_t i=0; i<N; i++) {
        file << ares.intermediate_results[i].c << " "
            << ares.intermediate_results[i].K_corr << "\n";
    } */
}// ResultExporter::saveEveryK()