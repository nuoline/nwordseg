#!/usr/bin/env python
# -*- coding: utf-8 -*

"""
File: duer_wordseg.py
Author: zhaizhouwei@baidu.com
Date: 2019/11/21 16:03:39
"""

import sys
import os
os.environ['LD_LIBRARY_PATH'] = './ncrfpp/lib:' + os.environ.get('LD_LIBRARY_PATH', '')
import string
import ctypes

SEG_PY_SO_PATH = "./nseg_model/libnseg.so"
STRING_BUFFER_SIZE = 2048

cstr_basic_res = ctypes.create_string_buffer(STRING_BUFFER_SIZE)
cstr_wp_res = ctypes.create_string_buffer(STRING_BUFFER_SIZE)

seger = ctypes.cdll.LoadLibrary(SEG_PY_SO_PATH)

def init(root):
    root_pchar = ctypes.c_char_p(root)

    seger.init_wordseg(root_pchar)

def destroy():
    seger.destroy_wordseg()

def segment(query):
    query_pchar = ctypes.c_char_p(query)

    #segment
    seger.segment(query_pchar, cstr_basic_res)

    #get result
    basic_words = cstr_basic_res.value.split(' ')
    
    #return basic and wp result list
    return basic_words

if __name__ == "__main__":
    root = sys.argv[1]
    qfile = sys.argv[2] 
    #init
    init(root)
    
    for line in open(qfile, 'r'):
        query = line.strip()
        basic_words= segment(query)

        basic_words_str = " ".join(basic_words)
        print '\t'.join([query, basic_words_str])

    #destroy
    destroy()
