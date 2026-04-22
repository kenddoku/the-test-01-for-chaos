#include "ResultExporter.h"

#include <fstream>

void ResultExporter::saveM(const AnalysisControlResult& acor,
                    const std::string& filename) {
    std::ofstream file(filename);

    size_t N0 = acor.M.size();
    for(size_t i=0; i<N0; i++) {
        file << acor.M[i] << '\n';
    }
}//ResultExporter::saveM()

void ResultExporter::savePQ(const AnalysisControlResult& acor,
    const std::string& filename) {
    std::ofstream file(filename);

    size_t N = acor.p.size();
    for(size_t i=0; i<N; i++) {
        file << acor.p[i] << " " << acor.q[i] << '\n';
    }
}//ResultExporter::savePQ()

void ResultExporter::saveEveryK(const AnalysisResult& ares,
    const std::string& filename) {
    std::ofstream file(filename);

    size_t N = ares.intermediate_results.size();
    for(size_t i=0; i<N; i++) {
        file << ares.intermediate_results[i].c << " "
            << ares.intermediate_results[i].K_corr << "\n";
    }
}// ResultExporter::saveEveryK()