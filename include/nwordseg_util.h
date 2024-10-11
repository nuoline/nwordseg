/**
 * @file nwordseg_util.h
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2023/08/9 15:26:49
 * @brief 
 *  
 **/

#ifndef  NWORDSEG_INCLUDE_WORDSEG_UTIL_H
#define  NWORDSEG_INCLUDE_WORDSEG_UTIL_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

namespace nwordseg {

typedef std::vector<std::string> StrVec;
typedef std::map<std::string, int> FDict;
typedef std::vector<bool> BVec;
typedef std::vector<int> IntVec;

const std::string BASIC = "basic";
const std::string BASIC_MODEL = "basic_model";

enum SegTag {
    SEG_B = 0,
    SEG_M,
    SEG_E,
    SEG_S
};

enum EncodeType {
    UTF8_ENCODE = 0,
    GB18030_ENCODE
};

//Define the util function

class Timer {
    public:
        Timer();
        Timer(int task_num);
        void start();
        void stop();
        double elapsed() const;

    private:
        double _elapsed;
        struct timespec _start;
        struct timespec _end;
};

class TimerSetter {
    public:
        TimerSetter(Timer* timer, bool enable) : _timer(timer), _enable(enable) {
            if (_enable) {
                _timer->start();
            }
        }
        ~TimerSetter() {
            if (_enable) {
                _timer->stop();
            }
        }

    private:
        Timer* _timer;
        int    _enable;
};

//to string
template < typename T > std::string to_string( const T& n );

//trim space str
bool is_all_spaces(const std::string& str);

//get next utf-8
int ul_ccode_next_utf8(const char* str, int szStr);

//get next gb18030 char
int ul_ccode_next_gb18030(const char* str, int szStr);

//Single Character segment
bool single_character_seg(const std::string& query, StrVec* out);

//split string by char delimiter
void split_str(const char* text, int len, char delim, StrVec* out);

//is boundary or not
bool is_seg_boundary(const std::string& res);

//join words list
bool join_words(const StrVec* word_list, int i, int j, std::string* term);

//load seg feature dict
bool load_feature_dict(const std::string& dict_path, FDict* feature_dict);

//transfer res_ids vector to border vector
bool ids_to_border_vector(const IntVec& ids_vec, BVec* border_vec, int* wnum);

//Boundary align post processing
int border_align(const BVec* basic_vec, BVec* comp_vec, int& wnum);

//int array in bool border vector
bool array_to_border_vector(const StrVec& array, BVec* border_vec, int& wnum);

//int ot SegTag type
SegTag int_to_segtag(int id);

//int to EncodeType
EncodeType int_to_encodetype(int ctype);

//debug function
void debug_ids(int* ids, int size, bool debug, std::string msg);
void debug_border_ids(BVec* bvec, bool debug, std::string msg);
void debug_wordlist(const StrVec& word_list, bool debug);

//
std::string clear_all_space(std::string& str);
}
#endif  //NWORDSEG_INCLUDE_WORDSEG_UTIL_H

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
