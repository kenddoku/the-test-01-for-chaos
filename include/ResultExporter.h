#ifndef RESULTEXPORTER_H
#define RESULTEXPORTER_H

#include "ChaosAnalyzer.h"
#include "ProjectPaths.h"
#include <string>

class ResultExporter {
private:
    std::string base_path = ProjectPaths::chaos01_out_path;
public:

    static void saveM(const AnalysisControlResult& acor,
                    const std::string& filename);

    static void savePQ(const AnalysisControlResult& acor,
                    const std::string& filename);

    static void saveEveryK(const AnalysisResult& ares,
                    const std::string& filename);

    // static void saveK(const AnalysisResult& ares,
    //                 const std::string& filename);

};

#endif