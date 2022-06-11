#include "HashCode.hpp"

HashCode Boolean::GetHashCode() const { return m_value ? 1U : 0U; }