#ifndef CELLLABEL_HPP
#define CELLLABEL_HPP

#include <iostream>
#include <type_traits>
#include "base/Label.hpp"

class CellLabel : public Label<CellLabel>{

public:

  static CellLabel Border();

  static CellLabel Cell();

  CellLabel();

  CellLabel(const CellLabel& other);

  CellLabel& operator=(CellLabel other);

  virtual std::string resolve_label_name() const;

  virtual bool operator<(const CellLabel& label) const;

  bool operator==(const CellLabel& label) const;

  enum class cellLabel_t {
    BORDER,
    CELL,
    UNDEFINED
  };

private:
  CellLabel(const cellLabel_t label);

  friend void swap(CellLabel& first, CellLabel& second)
  {
    std::swap(first.m_label, second.m_label);
  }

  cellLabel_t m_label;

};

#endif