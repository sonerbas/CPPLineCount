#include "cpp_line_count.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>
#include <cassert>

bool display_fnames = false;

namespace fs = std::filesystem;
typedef std::string::iterator Str_iter;

File_lcount_info count_cpp_lines_file(const std::string& fname);
std::pair<File_lcount_info, int> count_cpp_lines_folder(const fs::path& dir);

std::pair<File_lcount_info, int> count_cpp_lines(const std::filesystem::path& a_path)
{
    if (fs::exists(a_path)==false) 
        return std::pair<File_lcount_info,int>();
    if (fs::is_directory(a_path) == false)
        return std::make_pair(count_cpp_lines_file(a_path.string()), 1);
    return count_cpp_lines_folder(a_path);
}

std::pair<File_lcount_info, int> count_cpp_lines_folder(const std::filesystem::path& dir)
{
    File_lcount_info total;
    int num_files = 0;
    for (auto& i : fs::directory_iterator(dir)) {
        if (fs::is_directory(i.path())) {
            auto lcount_info = count_cpp_lines_folder(i.path());
            total += lcount_info.first;
            num_files += lcount_info.second;
        }
        else if (i.path().extension()==".cpp" || i.path().extension()==".h") {
            if (display_fnames)
                std::cout << "Processing: " << i.path() << "\n";
            total += count_cpp_lines_file(i.path().string());
            num_files++;
        }
    }
    return std::make_pair(total, num_files);
}
//------------------------------------------------------------------------------

inline Str_iter src_line_start_pos(Str_iter first, Str_iter last)
{
    for ( ; first!=last; ++first) {
        if (*first!=' ' && *first!='\t')
            break;
    }
    return first;
}

inline void process_empty(File_lcount_info& finfo)
{
    ++finfo.empty;
}

inline void process_single_char(File_lcount_info& finfo, bool& in_comment_block)
{
    if (in_comment_block) ++finfo.block_comment;
    else                  ++finfo.src;
}

inline Str_iter find_block_start(Str_iter f, Str_iter l)
{
    char blk_end_seq[2] = { '/', '*' };
    auto s = std::search(f, l, std::begin(blk_end_seq), std::end(blk_end_seq));
    if (s != l) {
        if (s==f || (s!=f && *(s - 1)!='/'))
            return s;
    }
    return l;
}

inline Str_iter find_block_end(Str_iter f, Str_iter l)
{
    char blk_end_seq[2] = { '*', '/' };
    return std::search(f, l, std::begin(blk_end_seq), std::end(blk_end_seq));
}

inline bool is_line_comment(Str_iter f, Str_iter l)
{
    assert(std::distance(f, l) >= 2);
    return (std::distance(f, l)>=2 && *f=='/' && *(f + 1)=='/');
}

inline Str_iter process_line(Str_iter f, Str_iter l, bool& in_comment_block, 
    int& src_ln, int& comment_ln, int& blk_comment_ln)
{
    if (f == l) return l;
    if (in_comment_block) {
        blk_comment_ln = 1;
        f = find_block_end(f, l);
        if (f != l) {
            in_comment_block = false;
            assert(std::distance(f, l) >= 2);
            return f + 2;
        }
    }
    else {
        if (is_line_comment(f, l)) {
            if (comment_ln == 0) comment_ln = 1;
            return l;
        }
        auto bs = find_block_start(f, l);
        if (bs != l) {
            if (bs != f) src_ln = 1;
            blk_comment_ln = 1;
            in_comment_block = true;
            assert(std::distance(bs, l) >= 2);
            return bs + 2;
        }
        else {
            src_ln = 1;
        }
    }
    return l;
}

void process_whole_line(Str_iter f, Str_iter l, File_lcount_info& finfo, bool& in_comment_blk)
{
    int src_ln = 0;
    int comment_ln = (in_comment_blk) ? -1 : 0;
    int blk_comment_ln = 0;

    while (f != l)
        f = process_line(f, l, in_comment_blk, src_ln, comment_ln, blk_comment_ln);

    if (src_ln == 1) ++finfo.src;
    if (comment_ln == 1) ++finfo.comment;
    if (blk_comment_ln == 1) ++finfo.block_comment;
}

void process_src_line(Str_iter f, Str_iter l, File_lcount_info& finfo, bool& in_comment_blk)
{
    ++finfo.total;
    if      (std::distance(f, l) == 0) process_empty(finfo);
    else if (std::distance(f, l) == 1) process_single_char(finfo, in_comment_blk);
    else                               process_whole_line(f, l, finfo, in_comment_blk);
}

File_lcount_info count_cpp_lines_file(const std::string& fname)
{
    File_lcount_info finfo;

    std::ifstream fin(fname.c_str());
    if (!fin.is_open()) {
        std::cout << "Could not open file \"" << fname << "\" for input\n";
    }
    else {
        bool in_comment_block = false;
        for (std::string line; std::getline(fin, line); )
            process_src_line(src_line_start_pos(line.begin(), line.end()), 
                line.end(), finfo, in_comment_block);
    }

    return finfo;
}
