#include "ResultExporter.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

// ****** ParamereStorage constructor ******
ParameterStorage::ParameterStorage(const AnalysisResult* ares_) {
    nu_int      = ares_->ts_ptr->nu_int;
    cp_val_int  = ares_->ts_ptr->cp_val_int;
    cp_name     = ares_->ts_ptr->cp_name;
    c_num       = ares_->intermediate_results.size();
}

// ****** ResultExporter constructor ******
ResultExporter::ResultExporter() : params(nullptr) {}

void ResultExporter::add_result(const AnalysisResult* ares) {
    ares_ptr_vec.push_back(ares);
    tseries_num++;
}//ResultExporter::add_result()

void ResultExporter::saveK() {
    if(tseries_num == 0) {
        std::cerr << "ERROR: saveK(), No results to save (tseries_num == 0)" << std::endl;
        return;
    }
    // Code below assumes every time series has the same values of nu_int, cp_val and cp_name
    // Otherwise the behaviour is not valid
    ParameterStorage params(ares_ptr_vec[0]);
    std::ostringstream oss;
    oss << base_path << "/K/" << params.cp_name << "/nu_" << params.nu_int;  
    fs::path K_out_dir = oss.str();

    if(!fs::exists(K_out_dir)) {
        fs::create_directories(K_out_dir);
    }

    oss << "/K_" << params.cp_name << "_" << params.cp_val_int << ".txt";
    fs::path K_out_path = oss.str();
    std::ofstream file(K_out_path);

    for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
        KWithIndex KWI_corr     = ares_ptr_vec[ts_index]->KWI_corr;
        KWithIndex KWI_linreg   = ares_ptr_vec[ts_index]->KWI_linreg;
        size_t K_corr_index     = KWI_corr.index;
        size_t K_linreg_index   = KWI_linreg.index;
        double c_corr   = ares_ptr_vec[ts_index]->intermediate_results[K_corr_index].c;
        double c_linreg = ares_ptr_vec[ts_index]->intermediate_results[K_linreg_index].c;

        file << std::fixed << std::setprecision(6) 
            << c_corr << " " << KWI_corr.K << " "
            << c_linreg << " " << KWI_linreg.K << " ";
    }

    file.close();
}//ResultExporter::saveK()

void ResultExporter::saveAllKc() {
    if(tseries_num == 0) {
        std::cerr << "ERROR: saveAllKc(), No results to save (tseries_num == 0)" << std::endl;
        return;
    }

    ParameterStorage params(ares_ptr_vec[0]);
    std::ostringstream oss;
    oss << base_path << "/AllKc/" << params.cp_name << "/nu_" << params.nu_int;
    fs::path allKcDir = oss.str();

    if(!fs::exists(allKcDir)) {
        fs::create_directories(allKcDir);
    }

    oss << "/AllKc_" << params.cp_name << "_" << params.cp_val_int << ".txt";
    fs::path AllKc_out_path = oss.str();
    std::ofstream file(AllKc_out_path);

    for(size_t c_index=0; c_index<params.c_num; c_index++) {
        for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
            double c   = ares_ptr_vec[ts_index]->intermediate_results[c_index].c; // Shared c since we save all of these results
            double Kc_corr  = ares_ptr_vec[ts_index]->intermediate_results[c_index].KWI_corr.K;
            double Kc_linreg = ares_ptr_vec[ts_index]->intermediate_results[c_index].KWI_linreg.K;

            file << std::fixed << std::setprecision(6) << c << " " << Kc_corr << " " << Kc_linreg << " ";
        }
        file << '\n';
    }
}// ResultExporter::saveAllKc()

void ResultExporter::saveM() {
    if(tseries_num == 0) {
        std::cerr << "ERROR: saveM(), No results to save (tseries_num == 0)" << std::endl;
        return;
    }
    
    ParameterStorage params(ares_ptr_vec[0]);
    std::ostringstream oss;
    oss << base_path << "/M/" << params.cp_name << "/nu_" << params.nu_int;
    fs::path MDir = oss.str();

    if(!fs::exists(MDir)) {
        fs::create_directories(MDir);
    }

    oss << "/M_" << params.cp_name << "_" << params.cp_val_int << ".txt";
    fs::path M_out_path = oss.str();
    std::ofstream file(M_out_path);

    // Collecting pointers to M vectors for every time series so that saving the data
    // in colums is made possible without copying original M vectors
    std::vector<const double*> M_corr_ptrs;
    std::vector<const double*> M_linreg_ptrs;
    const size_t M_size = ares_ptr_vec[0]->intermediate_results[0].M.size(); // Every M vector has the same size determined by AnalysisParameters

    for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
        size_t K_corr_index         = ares_ptr_vec[ts_index]->KWI_corr.index;
        const double *M_corr_ptr    = &( ares_ptr_vec[ts_index]->intermediate_results[K_corr_index].M[0] );
        M_corr_ptrs.push_back(M_corr_ptr);

        size_t K_linreg_index       = ares_ptr_vec[ts_index]->KWI_linreg.index;
        const double *M_linreg_ptr  = &( ares_ptr_vec[ts_index]->intermediate_results[K_linreg_index].M[0] );
        M_linreg_ptrs.push_back(M_linreg_ptr);
    }

    // Saving the results to a file
    for(size_t i=0; i<M_size; i++) {
        for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
            file << std::fixed << std::setprecision(6)
                << *(M_corr_ptrs[ts_index] + i) << " " << *(M_linreg_ptrs[ts_index] + i) << " ";
        }
        file << '\n';
    }

}//ResultExporter::saveM()

void ResultExporter::savePQ() {
    if(tseries_num == 0) {
        std::cerr << "ERROR: savePQ(), No results to save (tseries_num == 0)" << std::endl;
        return;
    }
    
    ParameterStorage params(ares_ptr_vec[0]);
    std::ostringstream oss;
    oss << base_path << "/pq/" << params.cp_name << "/nu_" << params.nu_int;
    fs::path pqDir = oss.str();

    if(!fs::exists(pqDir)) {
        fs::create_directories(pqDir);
    }

    oss << "/pq_" << params.cp_name << "_" << params.cp_val_int << ".txt";
    fs::path pq_out_path = oss.str();
    std::ofstream file(pq_out_path);

    // Collecting pointers to p/q vectors (first elements) for every time series so that saving the data
    // in colums is made possible without copying original p/q vectors
    std::vector<const double*> p_corr_ptrs;
    std::vector<const double*> p_linreg_ptrs;
    std::vector<const double*> q_corr_ptrs;
    std::vector<const double*> q_linreg_ptrs;
    const size_t pq_size = ares_ptr_vec[0]->intermediate_results[0].p.size(); // All p/q vectors have the same size

    for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
        size_t K_corr_index = ares_ptr_vec[ts_index]->KWI_corr.index;
        const double* p_corr_ptr = &( ares_ptr_vec[ts_index]->intermediate_results[K_corr_index].p[0] );
        const double* q_corr_ptr = &( ares_ptr_vec[ts_index]->intermediate_results[K_corr_index].q[0] );
        p_corr_ptrs.push_back(p_corr_ptr);
        q_corr_ptrs.push_back(q_corr_ptr);

        size_t K_linreg_index = ares_ptr_vec[ts_index]->KWI_linreg.index;
        const double* p_linreg_ptr = &( ares_ptr_vec[ts_index]->intermediate_results[K_linreg_index].p[0] );
        const double* q_linreg_ptr = &( ares_ptr_vec[ts_index]->intermediate_results[K_linreg_index].q[0] );
        p_linreg_ptrs.push_back(p_linreg_ptr);
        q_linreg_ptrs.push_back(q_linreg_ptr);
    }

    // Saving the results to a file
    for(size_t i=0; i<pq_size; i++) {
        for(size_t ts_index=0; ts_index<tseries_num; ts_index++) {
            file << std::fixed << std::setprecision(6)
                << *(p_corr_ptrs[ts_index] + i) << " " << *(q_corr_ptrs[ts_index] + i) << " "
                << *(p_linreg_ptrs[ts_index] + i) << " " << *(q_linreg_ptrs[ts_index] + i) << " ";
        }
        file << '\n';
    }
}//ResultExporter::savePQ()
