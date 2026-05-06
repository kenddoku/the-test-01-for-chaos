#ifndef RESULTEXPORTER_H
#define RESULTEXPORTER_H

#include "ChaosAnalyzer.h"
#include "ProjectPaths.h"
#include <string>


/// !!!!! NOTE: Consider adding a method that would compare parameters between different time series
///             to ensure all of them reference the same result but different neurons. Would call it
///             from the level of ResultExporter::add_result method with diff beahviour depending on
///             time series number
class ParameterStorage {
public:
    size_t      N;
    size_t      N0;
    int         nu_int;
    int         cp_val_int;
    std::string cp_name;
    size_t      c_num; // Number of individual c values present in intermediate results stored in AnalysisResult
    ParameterStorage(const AnalysisResult* ares_);
};

class ResultExporter {
private:
    std::string base_path = ProjectPaths::chaos01_out_path;
    ParameterStorage *params;
public:
    ResultExporter();

    std::vector<const AnalysisResult*> ares_ptr_vec;
    size_t tseries_num = 0;

    void add_result(const AnalysisResult*);

    // SaveK() method saves the resulting K parameters (calculated using both correlation and regression method)
    // for every time series in a single file. File name's format is 'K_<cp_name>_<cp_val>_<cp_val_int>_N_<N>_N0_<N0>.txt'
    // and the data is stored in format shown below
    //
    // <c_corr0> <K_corr0> <c_linreg0> <K_linreg0> <c_corr1> <K_corr1> <c_linreg1> <K_linreg1> [...]
    //
    // where the indices are associated with time series indices
    void saveK();
    
    void saveAllKc();
    
    void saveM();
                    
    void savePQ();            
};

#endif