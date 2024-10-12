#!/bin/sh
export LD_LIBRARY_PATH=../nseg_model/ncrfpp/lib/:$LD_LIBRARY_PATH
../nseg_model/ncrfpp/bin/crf_learn -f 3 -c 4.0 seg.template data/scw.train.tag nseg.model
