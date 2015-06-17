#ifndef LABEL_HPP
#define LABEL_HPP

/// class Label -
template <typename DERIVATION>
class Label {
public:
  Label()
  {
    static_assert(std::is_base_of<Label, DERIVATION>::value, "Type parameter LABEL_TYPE must derive from Label.");
  }

  virtual std::string resolve_label_name() const = 0;

  virtual bool operator<(const DERIVATION& label) const = 0;

};

#endif