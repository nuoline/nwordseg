/**
 * @file nwordseg_util.cpp
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2023/08/09 17:17:02
 * @brief 
 *  
 **/
#include "nwordseg_util.h"

namespace nwordseg {

    Timer::Timer() : _elapsed(0) { }

    void Timer::start() {
        clock_gettime(CLOCK_MONOTONIC, &_start);
    }

    void Timer::stop()  {
        clock_gettime(CLOCK_MONOTONIC, &_end);
        _elapsed += (_end.tv_sec - _start.tv_sec)*1e9 + (_end.tv_nsec - _start.tv_nsec);
    }

    double Timer::elapsed() const {
        return _elapsed / 1e9;
    }

    bool is_all_spaces(const std::string& str) {
        for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
            if (!std::isspace(*it)) {
                return false; 
            }
        }
        return true; 
    }

    int ul_ccode_next_utf8(const char* str, int szStr){
        unsigned char * strIn = (unsigned char *)str;
    
        if(szStr > 0 && strIn[0] < 0x80) {
            return 1;
        }
        if(szStr >= 2 && strIn[0] >= 0xC2 && strIn[0] < 0xE0 && strIn[1] >> 6 == 2) {
            return 2;
        }
        if(szStr >= 3 && strIn[0] >> 4 == 14 && strIn[1] >> 6 == 2 && strIn[2] >> 6 == 2
            && (strIn[0] > 0xE0 || strIn[1] >= 0xA0)) {
            return 3;
        }
        if(szStr >= 4 && strIn[0] >> 3 == 30 && strIn[1] >> 6 == 2 && strIn [2] >> 6 == 2 &&
            strIn[3] >> 6 == 2 && strIn[0] <= 0xF4 && (strIn[0] > 0xF0 || strIn[1] >= 0x90) ) {
            return 4;
        }
        return 0;
    }    

    int ul_ccode_next_gb18030(const char* str, int szStr){
        unsigned char * strIn = (unsigned char *)str;
    
        if(szStr > 0 && strIn[0] < 0x80) {
            return 1;
        }
        if(szStr >= 2 && strIn[0] >= 0x81 && strIn[0] <= 0xfe && strIn[1] >= 0x40 && strIn[1] <= 0xFE && strIn[1] != 0x7F) {
            return 2;
        }
        if(szStr >= 4 && strIn[0] >= 0x81 && strIn[0] <= 0xfe && strIn[1] >= 0x30 && strIn[1] <= 0x39
            && strIn[2] >= 0x81 && strIn[2] <= 0xfe && strIn[3] >= 0x30 && strIn[3] <= 0x39 ) {
            return 4;
        }
        return 0;
    }

    bool single_character_seg(const std::string& text, StrVec* out) {
        if (NULL == out) {
            return false;
        }
        const char* s = text.c_str();
        const char* e = s + text.size();

        out->reserve(text.size());
        while (s < e) {
            int len = 0;
            len = ul_ccode_next_utf8(s, e-s);
            std::string buf = std::string(s, len);
            if (len == 0) {
                break;
            }
            out->push_back(buf);
            s += len;
        }
        return true;
    }

    void split_str(const char* text, int len, char delim, StrVec* out) {
        if (NULL == text || len < 1 || NULL == out) {
            return;
        }
        const char* p1  = text;
        const char* end = text + len;

        while (p1 < end) {
            const char* p2 = strchr(p1, delim);
            if (p2 == NULL) {
                out->push_back(std::string(p1, end - p1));
                break;
            } else {
                out->push_back(std::string(p1, p2 - p1));
                p1 = p2 + 1;
            }
        }
        return;
    }

    bool is_seg_boundary(const std::string& res) {
        if (res == "E" || res == "S") {
            return true;
        } else {
            return false;
        }
    }

    bool join_words(const StrVec* word_list, int i, int j, std::string* term) {
        if (NULL == word_list || NULL == term) {
            return false;
        }
        for (int p = i; p < j; p++) {
            term->append(word_list->at(p));
        }
        return true;
    }

    int border_align(const BVec* basic_vec, BVec* comp_vec, int& wnum) {
        if (NULL == basic_vec || NULL == comp_vec) {
            return -1;
        }
        if (basic_vec->size() != comp_vec->size()) {
            return -2;
        }
        int border_tag = 1;//default border is align
        for (unsigned int i = 0; i < comp_vec->size(); i++) {
            bool merge_border = basic_vec->at(i) && comp_vec->at(i);
            comp_vec->at(i) = merge_border;
            if (!merge_border) {
                border_tag = 0;//border is not align
            } else {
                wnum += 1; //border is align and word count
            }
        }
        return border_tag;
    }

    bool array_to_border_vector(const StrVec& array, BVec* border_vec, int& wnum) {
        if (NULL == border_vec) {
            return false;
        }
        for (unsigned int i = 0; i < array.size(); i++) {
            bool is_border = is_seg_boundary(array[i]);
            border_vec->push_back(is_border);
            if (is_border) {
                wnum += 1;
            }
        }
        return true;
    }

    SegTag int_to_segtag(int id) {
        if (id == 0) {
            return SEG_B;
        } else if (id == 1) {
            return SEG_M;
        } else if (id == 2) {
            return SEG_E;
        } else if (id == 3) {
            return SEG_S;
        } else {
            return SEG_S;
        }
    }

    EncodeType int_to_encodetype(int ctype) {
        if (ctype == 0) {
            return UTF8_ENCODE;
        } else {
            return GB18030_ENCODE;
        }
    }

    void debug_ids(int* ids, int size, bool debug, std::string msg) {
        if (!debug) {
            return;
        }
        std::string debug_str = msg;
        std::cerr << msg;
        for (int i = 0; i < size; i++) {
            std::cerr << ids[i] << ":";
        }
        std::cerr <<"\n";
        return;
    }

    void debug_border_ids(BVec* bvec, bool debug, std::string msg) {
        if (!debug) {
            return;
        }
        std::string debug_str = msg;
        std::cerr << msg;
        for (unsigned int i = 0; i < bvec->size(); i++) {
            std::cerr << (int)bvec->at(i) << ":";
        }
        std::cerr << "\n";
        return;
    }

    void debug_wordlist(const StrVec& word_list, bool debug) {
        if (!debug) {
            return;
        }
        std::cerr << "word_list size:" << word_list.size() << "\n";
        std::string debug_str("");
        for (unsigned int i = 0; i < word_list.size(); i++) {
            //std::cerr << word_list[i] << "\n";
            debug_str += word_list[i] + "|";
        }
        std::cerr << "debug_wordlist:" << debug_str.c_str() << "\n";
    }

    std::string clear_all_space(std::string& str) {
        int index = 0;
        if (!str.empty()) {
            while((index = str.find(' ', index)) != std::string::npos) {
                str.erase(index, 1);
            }
        }
        return str;
    }
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
