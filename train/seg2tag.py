import sys

def split_chinese_utf8(s):
    char = []
    try:
        uni  = unicode(s, 'UTF-8')
        char = [w.encode('UTF-8') for w in uni]
    except:
        pass

    return char

def split_chinese_gb(s):
    char = []
    try:
        uni  = unicode(s, 'gbk')
        char = [w.encode('gbk') for w in uni]
    except:
        pass

    return char

def seg_to_tag(seg_file) :
    for line in open(seg_file, 'r') :
        cols = line.strip().split('/')
        for i in range(len(cols)) :
            wordstr = cols[i]
            wlist = split_chinese_utf8(wordstr)
            if len(wlist) == 1 :
                print '\t'.join([wlist[0], 'S'])
                continue
            if len(wlist) == 2 :
                print '\t'.join([wlist[0], 'B'])
                print '\t'.join([wlist[1], 'E'])
                continue
            if len(wlist) > 2 :
                print '\t'.join([wlist[0], 'B'])
                for j in range(1, len(wlist) - 1) :
                    print '\t'.join([wlist[j], 'M'])
                print '\t'.join([wlist[-1], 'E'])
                continue
        print ""

if __name__ == "__main__" :
    seg_file = sys.argv[1]
    seg_to_tag(seg_file)
