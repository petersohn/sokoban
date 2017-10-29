#ifndef INDENTEDOUTPUT_H_
#define INDENTEDOUTPUT_H_

#include <ostream>
#include "Dumper/DumperFunctions.hpp"
#include "Status/PseudoStatus.hpp"
#include "Status/Status.hpp"

namespace sokoban {

template <class T>
inline
void indentedOutput(std::ostream& os, const T& t, int level)
{
    for (int i = 0; i < level; ++i) {
        os << ' ';
    }
    os << t << std::endl;
}

template <>
inline
void indentedOutput(std::ostream& os, const Status& status, int level)
{
    dumpStatus(os, status, "",
            Matrix<Highlight>(status.reachableMatrix()), level);
}

template <>
inline
void indentedOutput(std::ostream& os, const PseudoStatus& status, int level)
{
    dumpStatus(os, status, "",
            Matrix<Highlight>(status.reachableMatrix()), level);
}

} // namespace sokoban

#endif /* INDENTEDOUTPUT_H_ */
