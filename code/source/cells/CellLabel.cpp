#include "CellLabel.hpp"


  CellLabel CellLabel::Border()
  {
    return CellLabel(cellLabel_t::BORDER);
  }

  CellLabel CellLabel::Cell()
  {
    return CellLabel(cellLabel_t::CELL);
  }

  CellLabel::CellLabel() : m_label(cellLabel_t::UNDEFINED)
  {
  }
  
  CellLabel::CellLabel(const CellLabel& other)
    : m_label(other.m_label)
  {
  }

  CellLabel& CellLabel::operator=(CellLabel other)
  {
    swap(*this, other);
    return *this;
  }

  std::string CellLabel::resolve_label_name() const
  {
    if (m_label == cellLabel_t::BORDER)
      return "BORDER";
    else if (m_label == cellLabel_t::CELL)
      return "CELL";
    else
      return "UNDEFINED";
  }

  bool CellLabel::operator<(const CellLabel& label) const
  {
    return m_label < label.m_label;
  }

  bool CellLabel::operator==(const CellLabel& label) const
  {
	  return m_label == label.m_label;
  }

  CellLabel::CellLabel(const cellLabel_t label) : m_label(label)
  {
  }