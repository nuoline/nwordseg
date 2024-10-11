/**
 * @file src/nwordseg_model.cpp
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2023/08/09 12:49:30
 * @brief 
 *  
 **/
#include <math.h>

#include "nwordseg_model.h"

namespace nwordseg {

    WordsegModel::WordsegModel() {
        _crf_model = NULL;
        _tagger = NULL;
        _debug = false;
    }

    WordsegModel::~WordsegModel() {
       if (NULL != _tagger) {
           delete _tagger;
           _tagger = NULL;
       }    
    
       if (NULL != _crf_model) {
            delete _crf_model;
            _crf_model = NULL;
        }

    }
    
    bool WordsegModel::init_model(std::string& model_path) {
        //init conf
        if (model_path.length() < 1) {
            return false;
        }

        //init model
        //std::cerr << "1:model path:" << model_path << "\n";
        std::string input_path = "-m " + model_path + " -v 3";
        //std::cerr << "2:input_path:" << input_path << "3:start createModel...\n";
        _crf_model = CRFPP::createModel(input_path.c_str());
        //std::cerr << "4:end createModel\n";
        if (_crf_model == NULL) {
            //std::cerr << "5:_crf_model init_model is null\n";
            return false;
        }
        _tagger = _crf_model->createTagger();
        if (_tagger == NULL) {
            return false;
        }
        return true;
    }
    
    bool WordsegModel::bvec_to_seg_result(const std::vector<bool>* border_vec,
        const std::vector<std::string>* word_list, std::vector<std::string>* seg_res) {
         if (NULL == border_vec || NULL == seg_res || NULL == word_list) {
            return false;
        }
        int pos_i = 0;
        for(unsigned int i = 0; i < border_vec->size(); i++) {
            bool border = border_vec->at(i);
            if (border) {
                std::string term;
                join_words(word_list, pos_i, i + 1, &term);
                if (!is_all_spaces(term)) {
                    seg_res->push_back(term);
                }
                pos_i = i + 1;
            }
        }
        return true;
    }
 
    int WordsegModel::predict(const std::vector<std::string>& query_words, std::vector<bool>* border_res, int& wnum) {
        if (NULL == _tagger) {
            return -1;
        }
        if (query_words.size() < 1 || NULL == border_res) {
            return -2;
        }
        _tagger->clear();

        std::vector<std::string>::const_iterator it = query_words.begin();
        for ( ; it != query_words.end(); it++){
            _tagger->add((*it).c_str());
        }
        /**
        for (size_t i = 0; i < query_words.size(); i++){
            _tagger->add((query_words[i]).c_str());
        }*/
        if (!_tagger->parse()){
            return -3;
        }

        StrVec res_array;
        //std::cerr << "_tagger->size:" << _tagger->size() << "\n";
        for (size_t i = 0; i < _tagger->size(); i++){
            res_array.push_back(std::string(_tagger->y2(i)));
        }

        //for debug
        if (_debug) { 
            std::cerr << res_array.size() << ":";
            for (size_t i = 0; i < res_array.size(); i++) {
                std::cerr << res_array[i] << ":";
            }
        }

        bool res = array_to_border_vector(res_array, border_res, wnum);

        return 0;
    
    }
    bool WordsegModel::segment(std::string& query, std::vector<std::string>* scw_out) {
        if (NULL == scw_out) {
            return false;
        }
        //clear scw_out
        scw_out->clear();

        //query is empty
        if (query.size() < 1) {
            //std::cerr << "query size < 1\n";
            return false;
        }
        //single word segment
        StrVec word_list;
        bool res1 = single_character_seg(query, &word_list);
        if (!res1) {
            //std::cerr << "%s\tsingle_character_seg failed\n", query.c_str();
            return false;
        }
        debug_wordlist(word_list, _debug);
        //debug query
        //std::cerr << "debug wordseg_model:" << query << "\n";
        BVec basic_border;
        int basic_wordcount = 0;
        //if (_crf_model == NULL) {std::cerr << "7:before predict:_crf_model is null\n";}
        int res_basic = predict(word_list, &basic_border, basic_wordcount);
        if (res_basic < 0) {
            //std::cerr << "model predict failed flag:" << res_basic << "\n";
            return false;
        }
        debug_border_ids(&basic_border, _debug, std::string("basic_border"));

        //basic segment result
        bool res3 = bvec_to_seg_result(&basic_border, &word_list, scw_out);
        if (!res3) {
            //std::cerr << "%s\tbasic bvec_to_seg_result failed\n", query.c_str();
            return false;
        }

        return true;
    }
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
