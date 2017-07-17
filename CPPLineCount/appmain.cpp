#include <iostream>
#include <iomanip>
#include <filesystem>
#include <chrono>
#include <cassert>
#include <vector>
#include "cpp_line_count.h"

int main()
{
    namespace fs = std::tr2::sys;  //filesystem library
    using namespace std;

    //std::vector<fs::path> source_paths = {fs::path("D:\\dev\\progs\\Console_VS2015\\Console_VS2015\\data_race.cpp")};
    //std::vector<fs::path> source_paths = {fs::path("C:\\Users\\sbas.PROKON\\Desktop\\Test.cpp")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\source\\comosys")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\plugin source")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\deleted modules\\source\\comosys")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\deleted modules\\source")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\source\\engineering drawing module")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\software_development\\libraries\\comosys\\plugins")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\dev\\progs\\OpenSees.2.2.2\\OpenSees\\SRC")};
    //std::vector<fs::path> source_paths = {fs::path("C:\\Users\\sbas.PROKON\\Desktop\\CALWinExp")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\dev\\progs\\NLEng\\src")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\dev\\progs\\NLEng\\src\\NLEng")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\software_development\\libraries\\analysis & design\\PryStructuralModel\\src\\StructuralModelLib")};
    //std::vector<fs::path> source_paths = {fs::path("C:\\PRY_Paylasim\\Rebar 1")};
    //std::vector<fs::path> source_paths = {fs::path("C:\\PRY_Paylasim\\Rebar 1")};
    //std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\deleted modules\\source\\comosys")};
    std::vector<fs::path> source_paths = {fs::path("D:\\Comosys 3.0\\source\\connection design")};
    /*std::vector<fs::path> source_paths = {
        //Baris Irhan 
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\Rebar 1"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\UI"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\irhan"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\commonmethods_bi.h"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\commonmethods_bi.cpp"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\commonmethods_rfcd.h"),
        fs::path("D:\\software_development\\libraries\\comosys\\plugins\\commonmethods\\commonmethods_rfcd.cpp")
    };*/
    /*std::vector<fs::path> source_paths = {
        fs::path("D:\\dev\\progs\\CPPLineCount\\CPPLineCount\\cpp_line_count.h"),
        fs::path("D:\\dev\\progs\\CPPLineCount\\CPPLineCount\\cpp_line_count.cpp")
    };*/

    char c = 0;
    std::cout << "display file names? (Y/N):";
    std::cin >> c;
    if (c == 'y' || c == 'Y')
        display_fnames = true;

    std::pair<File_lcount_info, int> whole_stats;
    std::int64_t elapsed_whole = 0;

    for (auto& pth : source_paths)
    {
        //Start timer
        auto start_tp = std::chrono::high_resolution_clock::now();

        //Count lines
        auto total = count_cpp_lines(pth);

        //End timer and calculate duration
        auto end_tp = std::chrono::high_resolution_clock::now();
        auto elapsed = static_cast<std::int64_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                end_tp - start_tp).count());

        //Display data
        cout << "\n";
        cout << "Path                = " << pth << endl;
        cout << "Number of files     = " << total.second << endl;
        cout << "Total lines         = " << total.first.total << endl;
        cout << "Empty lines         = " << total.first.empty << endl;
        cout << "Source lines        = " << total.first.src << endl;
        cout << "Comment lines       = " << total.first.comment << endl;
        cout << "Block comment lines = " << total.first.block_comment << endl;
        cout << "Elapsed Time        = " << elapsed << " milliseconds\n";

        //Cumulative
        whole_stats.first += total.first;
        whole_stats.second += total.second;
        elapsed_whole += elapsed;
    }

    if (source_paths.size() > 1) {
        cout << "\n\n";
        cout << "=========================================\n";
        cout << "===            GRAND TOTAL            ===\n";
        cout << "=========================================\n";
        cout << "Number of files     = " << whole_stats.second << endl;
        cout << "Total lines         = " << whole_stats.first.total << endl;
        cout << "Empty lines         = " << whole_stats.first.empty << endl;
        cout << "Source lines        = " << whole_stats.first.src << endl;
        cout << "Comment lines       = " << whole_stats.first.comment << endl;
        cout << "Block comment lines = " << whole_stats.first.block_comment << endl;
        cout << "Elapsed Time        = " << elapsed_whole << " milliseconds\n\n";
    }
}