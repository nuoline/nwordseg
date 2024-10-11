/**
 * @file seg_demo.cpp
 * @author zhaizhouwei, email:nuoline@qq.com
 * @date 2023/08/06 10:27:03
 * @brief 
 *  
 **/
#include <time.h>
#include <iostream>
#include <fstream>
#include "nwordseg_model.h"

void print_res(std::string& query, std::vector<std::string>* out) {
    if (NULL == out) {
        return;
    }
    std::string basic_seg = "";
    std::vector<std::string>::iterator it;
    std::cout << query << "\t";
    for (it = out->begin(); it != out->end(); it++) {
        std::cout << *it << "|";
    }
    std::cout << "\n";
}

void segment_demo(const char* filename, nwordseg::WordsegModel* seg) {
    if (NULL == filename || NULL == seg) {
        std::cerr << "filename is NULL or seg is NULL\n";
        return;
    }
    seg->set_debug(false);
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in);
    if (!ifs.is_open()) {
        std::cerr << "open file fail\n";
        return;
    }
    std::string query;
    while (getline(ifs, query)) {
        std::vector<std::string> seg_out;
        seg->segment(query, &seg_out);
        print_res(query, &seg_out);
    }
    ifs.close();
}

void segment_demo_performance(const char* filename, nwordseg::WordsegModel* seg) {
    if (NULL == filename || NULL == seg) {
        std::cerr << "filename is NULL or seg is NULL\n";
        return;
    }
    seg->set_debug(false);
    //read input file
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::in);
    std::vector<std::string> query_vec;
    if (!ifs.is_open()) {
        std::cerr << "open file fail\n";
        return ;
    }
    std::string query;
    while (getline(ifs, query)) {
        query_vec.push_back(query);
    }
    ifs.close();
    //start segment
    nwordseg::Timer timer;
    std::vector< std::vector<std::string> > scw_vec;

    timer.start();
    for (std::vector<std::string>::iterator it = query_vec.begin(); it != query_vec.end(); it++) {
        std::string q = *it;
        std::vector<std::string> scw;
        //std::cerr << q << "\n";
        seg->segment(q, &scw);
        scw_vec.push_back(scw);
    }
    timer.stop();
    double speed = timer.elapsed() / scw_vec.size();
    std::cerr << "************Performance statistics**************" << "\n";
    std::cerr << "count:" << scw_vec.size() << "\t";
    std::cerr << "time_sum:" << timer.elapsed() << "\t";
    std::cerr << "speed:" << speed << "s/query\n";
    //print scw_out
    //print_res_out(&scwout_vec);
}

int main(int argc, char* args[]) {
    if (argc != 4) {
        std::cerr << "need args:inputfile model_path is_performance<yes:1 no:0>\n"; 
        return -1;
    }
   
    const char* filename = args[1]; 
    std::string root = std::string(args[2]);
    nwordseg::WordsegModel* seg = new nwordseg::WordsegModel();
    bool res = seg->init_model(root);
    if (!res) {
        std::cerr << "init model dict failed\n";
        delete seg;
        return -1;
    }

    //performance is 1; seg demo is 0
    int is_performance = atoi(args[3]);
    if (is_performance > 0) {
        segment_demo_performance(filename, seg);
    } else {
        segment_demo(filename, seg);
    }
    //clear
    delete seg;
}
/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
