#!/bin/sh
export LD_LIBRARY_PATH=../nseg_model/ncrfpp/lib/:$LD_LIBRARY_PATH
../nseg_model/ncrfpp/bin/crf_test -m nseg.model data/scw.test.tag -o data/scw.test.tag_res.txt 
