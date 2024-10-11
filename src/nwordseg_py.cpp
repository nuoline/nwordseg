/**
 * @file src/nwordseg_py.cpp
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2024/10/6 14:49:30
 * @brief 
 *  
 **/
#include <iostream>
#include "nwordseg_model.h"

extern "C" {

nwordseg::WordsegModel* seg = NULL;

bool init_wordseg(const char* root_pchar) {
    if (NULL == root_pchar) {
        return false;
    }
    seg = new nwordseg::WordsegModel();
    std::string root(root_pchar);
    bool res = seg->init_model(root);
    return res;
}

void destroy_wordseg() {
    if (seg != NULL) {
        delete seg;
    }
    seg = NULL;
}

int segment(const char* query_pchar, char* basic_pchar) {
    if (seg == NULL) {
        std::cerr << "seg is NULL\n";
        return -1;
    }
    if (query_pchar == NULL || basic_pchar == NULL) {
        return -2;
    }

    //char* to string
    std::string query(query_pchar);
    
    //seg
    std::vector<std::string> seg_out;
    seg->segment(query, &seg_out);
     
    //get seg result
    std::string basic_seg;
    std::vector<std::string>::iterator it;
    for (size_t i = 0; i < seg_out.size(); ++i) {
        if ( i < seg_out.size() - 1 ) {
            basic_seg.append(seg_out[i] + " ");
        } else {
            basic_seg.append(seg_out[i]);
        }
    }
    //string to char*
    strcpy(basic_pchar, basic_seg.c_str());
    return 0;
}

}
