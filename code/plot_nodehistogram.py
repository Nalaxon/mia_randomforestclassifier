#! /usr/bin/python2

import matplotlib.pyplot as plt
import numpy as np
import pylab

dot_file = "bin/trees.txt"


if __name__ == "__main__":
	histogram = {
		'CenterPixelNode': 0,
		'GradientNode': 0,
		'TwoPixelNode': 0,
		'HaarWaveletNode': 0,
		'SURFFilterNode': 0,
		'TwoRegionsNode': 0,
		'SumNode': 0,
		'TwoPixelGradientNode': 0,
                'TwoRegionsGradientNode': 0,
                'CannyEdgeNode':0,
                'Haar4WaveletNode':0
	}

	with open(dot_file) as f:
		file_data = "".join([line for line in f])
		count_total = 0
		for key in histogram:
			count = file_data.count(key)
			count_total += count
			histogram[key] = count

		fig, ax = plt.subplots()
		plt.subplots_adjust(left=0.25, right=0.88)
		pos = np.arange(len(histogram)) + 0.5
		names = []
		counts = []
		for name, count in histogram.iteritems():
			names.append(name)
			counts.append(count)
		counts = np.array(counts, dtype=np.float) / count_total
		ax.barh(pos, counts, align='center', color='m')
		plt.yticks(pos, names)
		plt.title('Distribution of chosen weak learners')
		plt.xlabel('%')
		plt.draw()
		plt.show()
