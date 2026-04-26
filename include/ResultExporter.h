#ifndef RESULTEXPORTER_H
#define RESULTEXPORTER_H

#include "ChaosAnalyzer.h"
#include "ProjectPaths.h"
#include <string>

class ResultExporter {
private:
    std::string base_path = ProjectPaths::chaos01_out_path;
public:
    std::vector<const AnalysisResult*> ares_ptr_vec;
    size_t tseries_num = 0;

    void add_result(const AnalysisResult*);

    void saveK();
    
    void saveM();
                    
    void savePQ();            

    void saveEveryK();
                    
    // static void saveK(const AnalysisResult& ares,
    //                 const std::string& filename);

};

#endif