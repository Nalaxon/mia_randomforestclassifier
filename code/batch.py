#! /usr/bin/python2

from subprocess import *

program_path = "./bin/miaKU"
data_path = "../data/MIA_KU_2015_DataSet/"
log_path = "../output/"
params_num_trees = [10, 20, 30, 40, 50]
params_num_tests = [100, 150]
params_max_depth = [10, 11, 12, 13, 14, 15]

if __name__ == "__main__":
	num_total = len(params_num_trees) * len(params_num_tests) * len(params_max_depth)
	num_processed = 0
	print("Running %d tests..." % (num_total))
	for num_trees in params_num_trees:
		for num_tests in params_num_tests:
			for max_depth in params_max_depth:
				try:
					subproc = Popen([program_path, "--dataset", data_path, "--log", log_path, "--use_xvalidation", "--enable_bagging", "--num_trees", str(num_trees), "--max_depth", str(max_depth), "--num_feature_tests", str(num_tests)],
						stdout=PIPE)
					subproc.communicate()
				except:
					print("Some error in the subprocess occured...") # just make sure to keep on running!
				num_processed += 1
				print("%d/%d" % (num_processed, num_total))