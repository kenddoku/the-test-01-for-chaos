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

    void saveK();
    
    void saveAllKc();
    
    void saveM();
                    
    void savePQ();            

                    
    // static void saveK(const AnalysisResult& ares,
    //                 const std::string& filename);

};

#endif