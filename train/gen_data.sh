#!/bin/sh

TRAIN="scw.train"
TEST="scw.test"

#
data_path="data"
if [ ! -d "${data_path}" ]; then
    mkdir ${data_path}
fi

echo "convert segmented query to POC-tag..."
python ./seg2tag.py  $TRAIN > data/scw.train.tag
python ./seg2tag.py  $TEST > data/scw.test.tag
