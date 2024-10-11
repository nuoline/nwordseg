/**
 * @file nwordseg_model.h
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2023/08/9 17:26:49
 * @brief 
 *  
 **/

#ifndef  NWORDSEG_INCLUDE_WORDSEG_MODEL_H
#define  NWORDSEG_INCLUDE_WORDSEG_MODEL_H

#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include "crfpp.h"
#include "nwordseg_util.h"

namespace nwordseg {

class WordsegModel {

public:
    WordsegModel();
    ~WordsegModel();

    /*
     * @parameter root path
     * @return bool
     * @brief load wordseg model file
     */
    bool init_model(std::string& model_path);

    //set debug
    void set_debug(bool debug) { _debug = debug; }

    /*
     *wordseg api
    */ 
    bool segment(std::string& query, std::vector<std::string>* segs);

private:
    int predict(const std::vector<std::string>& query_words, std::vector<bool>* border_res, int& wnum);
    bool bvec_to_seg_result(const std::vector<bool>* border_vec,
        const std::vector<std::string>* word_list, std::vector<std::string>* seg_res);    

    CRFPP::Model* _crf_model;
    CRFPP::Tagger* _tagger;
    bool _debug;
};

}
#endif  //NWORDSEG_MODEL_H

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
