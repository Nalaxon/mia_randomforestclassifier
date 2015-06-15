#ifndef RTPARAMETER_HPP
#define RTPARAMETER_HPP

struct RTParameter {

  // maximum depth of the trees
  unsigned int m_max_depth;
  
  // minimum number of samples in each node
  unsigned int m_min_samples;
  
  // number of tests to try at each split
  unsigned int m_num_tests_per_split;

};

#endif
