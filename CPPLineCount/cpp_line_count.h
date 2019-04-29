#ifndef CPP_LINE_COUNT_H_SB
#define CPP_LINE_COUNT_H_SB

#include <utility>
#include <filesystem>

struct File_lcount_info
{
    int total;
    int src;
    int empty;
    int comment;
    int block_comment;

    File_lcount_info() 
        : total(0), src(0), empty(0), comment(0), block_comment(0) 
        {}

    File_lcount_info& operator+=(const File_lcount_info& rhs)
    {
        total += rhs.total;
        src += rhs.src;
        empty += rhs.empty;
        comment += rhs.comment;
        block_comment += rhs.block_comment;
        return *this;
    }
};

std::pair<File_lcount_info, int> count_cpp_lines(const std::filesystem::path& a_path);

extern bool display_fnames;

#endif  // !CPP_LINE_COUNT_H_SB
