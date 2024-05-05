// #include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdint>
#include <string>
#include <limits>
#include <time.h>
#include "./storageNumberIndex.cc"
#include <cstring> // for strerror
// template <typename DATA_TYPE>

template<typename BASIC_DOC_TYPE, typename USE_NUMBER_TYPE>
class Storage {
private:

    std::fstream fileStream;
    StorageNumberIndex<USE_NUMBER_TYPE> indexes;

public:
    struct DOC_TYPE : public BASIC_DOC_TYPE {
        USE_NUMBER_TYPE id;
    };

    std::string filename;

    Storage(const std::string &filename) : filename(filename),
                                           fileStream(filename, std::ios::in | std::ios::out | std::ios::binary),
                                           indexes(filename + ".indx") {
        if (!fileStream.is_open()) {
            fileStream.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
            std::cerr << "Failed to open the file: " << filename << std::endl;
            return;
        }
    };

    void remove(uint64_t id) {
        indexes.remove(id);
    }

    DOC_TYPE add(const BASIC_DOC_TYPE &doc) {
        DOC_TYPE newDoc;
        static_cast<BASIC_DOC_TYPE &>(newDoc) = doc;
        USE_NUMBER_TYPE id = indexes.size();
        newDoc.id = id;

        fileStream.seekp(0, std::ios::end);
        USE_NUMBER_TYPE fileSize = fileStream.tellg();

        fileStream.write(reinterpret_cast<const char *>(&id), sizeof(id));
        newDoc.serialize(fileStream);

        indexes.add({id, fileSize});
        return newDoc;
    }

    void removeById(const USE_NUMBER_TYPE &id) {
        indexes.remove(id);
    }


    void set(const DOC_TYPE &doc) {
        indexes.remove(doc.id);

        fileStream.seekp(0, std::ios::end);
        USE_NUMBER_TYPE fileSize = fileStream.tellg();

        indexes.add({doc.id, fileSize});

        fileStream.write(reinterpret_cast<const char *>(&doc.id), sizeof(doc.id));
        doc.serialize(fileStream);
    }

    DOC_TYPE findById(const USE_NUMBER_TYPE &id) {
        USE_NUMBER_TYPE pos = indexes.position(id);
        DOC_TYPE value;

        fileStream.seekg(pos, std::ios::beg);
        fileStream.read(reinterpret_cast<char *>(&value.id), sizeof(value.id));
        value.deserialize(fileStream);

        return value;
    }

    USE_NUMBER_TYPE size() {
        return indexes.size();
    }
};
