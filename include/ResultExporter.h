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
    
    void saveM(const std::string& filename);
                    
    void savePQ(const std::string& filename);            

    void saveEveryK(const std::string& filename);
                    
    // static void saveK(const AnalysisResult& ares,
    //                 const std::string& filename);

};

#endif