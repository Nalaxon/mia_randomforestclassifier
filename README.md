# mia_randomforestclassifier
Random Forest Classifier for detecting Cell Membrans

*** GETTING STARTED ***

** Requirements **
	* Installed OpenCV and the environment variable 'OpenCV_DIR' pointing to the particular directory e.g. 'D:\libraries\cpp\opencv\build\install'
		-> If OpenCV was build with shared libs, the DLL's destination must be added to the system path.
	* Installed Boost (tested with 1.58.0) and the environment variable 'BOOST_ROOT' pointing to the particular directory.
		-> Since Boost.Filesystem (is a separately-compiled Boost libraries) is used, you'll need to acquire the library binaries. 
		(Section 5 at http://www.boost.org/doc/libs/1_56_0/more/getting_started/windows.html)
		Probably you have to set BOOST_INCLUDEDIR (e.g. 'D:\libraries\cpp\boost_1_58_0\include') and BOOST_LIBRARYDIR (e.g. 'D:\libraries\cpp\boost_1_58_0\lib\x64') as well.
		

** Run **
1) You already extracted this archiv. 
	 Now the folder hierarchy should look like this:
	
	<root>
	  +- vs2013_c11
	    +- code
	      +- CMakeLists.txt
	      +- ...
	  +- vs2012
	    +- code
	      +- CMakeLists.txt
	      +- ...
	  +- README.txt
				
3) Choose a platform (VS2012 or VS2013).

4) Use the respective CMakeList file to create the Visual Studio solution. 

5) Build and run the with the command line argument '--dataset <path_to_data_folder>'

6) Use the following command line arguments to run the respective modes:
  --help                              display this help message
  --dataset arg                       path to the directory of the dataset
  --num_samples arg (=900)            the number of samples that will be extracted
                                      from each training image
  --sample_size arg (=30)             the size of the samples. (Each sample will
                                      be (sample_size x sample_size)
  --enable_bagging                    enable bagging of the input samples (-> use
                                      only randomly drawn 50% of the samples for each tree)
  --num_trees arg (=10)               the number of trees to train
  --max_depth arg (=7)                the maximal depth of each tree
  --min_samples_per_node arg (=10)    the minimum number of samples per node at
                                      which a node will be splitted again.
  --num_feature_tests arg (=150)      the number of feature tests to try at each
                                      split at each split.
  --print_trees arg                   enable printing of the trees after training
                                      in dot format to the standard output.
  --use_xvalidation                   Use cross validation.
  --num_xvalidations arg (=10)        the number of validation sets for
                                      xvalidation.
  --log arg (=.)                      define output folder to log performance and
                                      images, must not empty
