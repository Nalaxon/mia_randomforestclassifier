#! /usr/bin/python2

import os, os.path
import re
import numpy as np
import matplotlib.pyplot as plt


def parse_result_data(result_dir, logfile_regex, logfile_data_regex):
	"""Parses the result files and returns an 2 3D-arrays with axis (num_tests, num_trees, max_depth).
	The first one contains the cross correlation accuracy values and the second one the accuracy directly
	on the image.

	Returns:
	xacc (3D)
	iacc (3D)
	axis_num_tests (1D)
	axis_num_trees (1D)
	axis_max_depth (1D)
	"""
	filename_regex = re.compile(logfile_regex)
	data_regex = re.compile(logfile_data_regex)
	result_list = []
	axis_num_trees = set()
	axis_max_depth = set()
	axis_num_tests = set()
	for filename in os.listdir(result_dir):
		if filename_regex.match(filename):
			pathname = os.path.join(result_dir, filename)
			with open(pathname) as logfile:
				for line in logfile.readlines():
					data_match = data_regex.match(line)
					if data_match:
						result_item = {
							'cross_acc': float(data_match.group(1)),
							'image_acc': float(data_match.group(2)),
							'num_trees': int(data_match.group(3)),
							'max_depth': int(data_match.group(4)),
							'num_tests': int(data_match.group(5))	
						}
						result_list.append(result_item)
						axis_num_trees.update({result_item['num_trees']})
						axis_max_depth.update({result_item['max_depth']})
						axis_num_tests.update({result_item['num_tests']})

	axis_num_trees = sorted(axis_num_trees)
	axis_num_tests = sorted(axis_num_tests)
	axis_max_depth = sorted(axis_max_depth)

	xacc_array = np.zeros((len(axis_num_tests), len(axis_num_trees), len(axis_max_depth)), dtype=np.float)
	iacc_array = np.zeros((len(axis_num_tests), len(axis_num_trees), len(axis_max_depth)), dtype=np.float)
	for result_item in result_list:
		num_trees_idx = axis_num_trees.index(result_item['num_trees'])
		num_tests_idx = axis_num_tests.index(result_item['num_tests'])
		max_depth_idx = axis_max_depth.index(result_item['max_depth'])
		xacc_array[num_tests_idx][num_trees_idx][max_depth_idx] = result_item['cross_acc']
		iacc_array[num_tests_idx][num_trees_idx][max_depth_idx] = result_item['image_acc']

	return xacc_array, iacc_array, np.array(axis_num_tests), np.array(axis_num_trees), np.array(axis_max_depth)

def plot_acc_heatmap(acc, num_test_idx, axis_num_tests, axis_num_trees, axis_max_depth, title_str):
	_, ax = plt.subplots()
	plt.pcolormesh(acc[num_test_idx], cmap=plt.cm.afmhot, edgecolors='k')
	plt.title(title_str % axis_num_tests[num_test_idx])

	ax.set_xticks(np.arange(acc[num_test_idx].shape[1])+0.5, minor=False)
	ax.set_yticks(np.arange(acc[num_test_idx].shape[0])+0.5, minor=False)
	ax.set_xticklabels(axis_max_depth, minor=False)
	#we need to reverse the num_trees axis. Don't know why for sure... 
	ax.set_yticklabels(axis_num_trees[::-1], minor=False)
	ax.invert_yaxis()

	plt.xlabel("Max tree depth")
	plt.ylabel("Number of trees")
	plt.colorbar(ticks=np.arange(acc[num_test_idx].max(), acc[num_test_idx].min(), -0.003))

def plot_xacc_heatmap(acc, num_test_idx, axis_num_tests, axis_num_trees, axis_max_depth):
	title_str = "Cross validation accuracies with %d random feature tests per node."
	return plot_acc_heatmap(acc, num_test_idx, axis_num_tests, axis_num_trees, axis_max_depth, title_str)

def plot_iacc_heatmap(acc, num_test_idx, axis_num_tests, axis_num_trees, axis_max_depth):
	title_str = "Test image based accuracies with %d random feature tests per node."
	return plot_acc_heatmap(acc, num_test_idx, axis_num_tests, axis_num_trees, axis_max_depth, title_str)


#################################################################################################################
result_dir = '../output'
logfile_regex = '.+-log.txt'
logfile_data_regex = '([0-9]*\.[0-9]+|[0-9]+),([0-9]*\.[0-9]+|[0-9]+),(\d*),(\d*),(\d*),(\d*),(\d*)'

if __name__ == "__main__":
	xacc, iacc, axis_num_tests, axis_num_trees, axis_max_depth = parse_result_data(result_dir, logfile_regex, logfile_data_regex)
	figure_idx = 0
	for i in range(0, len(axis_num_tests)):
		plot_xacc_heatmap(xacc, i, axis_num_tests, axis_num_trees, axis_max_depth)
		plot_iacc_heatmap(iacc, i, axis_num_tests, axis_num_trees, axis_max_depth)

	plt.draw()
	plt.show()
	


