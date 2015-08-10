#! /usr/bin/python2

import matplotlib.pyplot as plt
import numpy as np
import pylab

dot_file = "bin/log.txt"


def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass
 
    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass
    
    return False

if __name__ == "__main__":
#def user_max():
    histogram = {
        'CenterPixelNode': 10000000,
        'GradientNode': 100000000,
        'TwoPixelNode': 100000000,
        'HaarWaveletNode': 100000000,
        'SURFFilterNode': 100000000,
        'TwoRegionsNode': 100000000,
        'SumNode': 100000000,
        'TwoPixelGradientNode': 100000000,
        'TwoRegionsGradientNode': 100000000,
        'CannyEdgeNode':100000000,
        'Haar4WaveletNode':100000000,
        'HoGNode':100000000,
        'CannyEdgeMoment00Node':100000000
    }
    
    names = []
    counts = []
    for name, count in histogram.iteritems():
        names.append(name)
        counts.append(count)

    with open(dot_file) as f:
        for line in f:
            d = line.split(';')
            if d[0] in histogram:
                if is_number(d[1]):
                    histogram[d[0]] = max(histogram[d[0]], d[1])
        print histogram


