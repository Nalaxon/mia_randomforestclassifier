#ifndef BASENODE_HPP
#define	BASENODE_HPP

#include "base/Node.hpp"
#include "Label.hpp"

#include <opencv/cv.h>

class BaseNode : public Node<Label, cv::Mat>
{
public:

  virtual std::string resolve_label_name(const Label& label) const
  {
    switch (label)
    {
    case Label::BORDER:
      return "border";
    case Label::CELL:
      return "cell";
    default:
      return "unknown label";
    }
  }
} ;

#endif	/* BASENODE_HPP */

