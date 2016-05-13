#ifndef UUIDUTILS_H_AYB1IZ28
#define UUIDUTILS_H_AYB1IZ28

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class BoostUUUIDGenerator {
public:
    std::string generateUUID() { return to_string(generator()); }

private:
    boost::uuids::random_generator generator;
};

#endif /* end of include guard: UUIDUTILS_H_AYB1IZ28 */
