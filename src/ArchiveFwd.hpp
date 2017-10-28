#ifndef ARCHIVEFWD_HPP
#define ARCHIVEFWD_HPP

namespace boost { namespace archive {

class binary_iarchive;
class binary_oarchive;

}} // namespace boost::archive

namespace sokoban {

using InputArchive = boost::archive::binary_iarchive;
using OutputArchive = boost::archive::binary_oarchive;

} // namespace sokoban

#endif // ARCHIVEFWD_HPP
