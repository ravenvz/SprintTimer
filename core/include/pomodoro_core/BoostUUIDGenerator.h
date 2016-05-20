#ifndef BOOSTUUIDGENERATOR_H_OF6HXUYB
#define BOOSTUUIDGENERATOR_H_OF6HXUYB

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class BoostUUIDGenerator {
public:
    std::string generateUUID() { return to_string(generator()); }

private:
    boost::uuids::random_generator generator;
};

#endif /* end of include guard: BOOSTUUIDGENERATOR_H_OF6HXUYB */
