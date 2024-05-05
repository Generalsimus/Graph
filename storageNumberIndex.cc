#include <vector>
#include <fstream>

// // template <typename DATA_TYPE>
// class StorageIndex
// {
//     // storageIndex
//     // private:
//     // std::fstream fileStream;
//     // std::streampos size;
//     // std::streamsize itemSizeOf = sizeof(DATA_TYPE);

//     // public:s
// }
template<typename USE_NUMBER_TYPE>
class StorageNumberIndex {
private:
    std::fstream idxFileStream;
public:
    StorageNumberIndex(const std::string &filename) : idxFileStream(filename,
                                                                    std::ios::in | std::ios::out | std::ios::binary) {
        if (!idxFileStream.is_open()) {
            idxFileStream.open(filename, std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
            std::cerr << "Failed to open the file: " << filename << std::endl;
            return;
        } else {
            readCurrentIndexes();
        }
    }

    struct Position {
        USE_NUMBER_TYPE id;
        USE_NUMBER_TYPE position;
    };

    std::vector<Position> elementVector;

public:
    USE_NUMBER_TYPE position(const USE_NUMBER_TYPE &id) {
        USE_NUMBER_TYPE index = findClosesLowerIndex(id);

        std::cout << "position: id: " << id << " index: " << index << std::endl;
        return elementVector[index].position;
    }

    void readCurrentIndexes() {
        idxFileStream.seekg(0, std::ios::end);
        USE_NUMBER_TYPE fileSize = idxFileStream.tellg();
        USE_NUMBER_TYPE allocSize = sizeof(Position);

        for (USE_NUMBER_TYPE index = 0; index < (fileSize / allocSize); ++index) {
            Position value;
            idxFileStream.seekg(index * allocSize, std::ios::beg);
            idxFileStream.read(reinterpret_cast<char *>(&value), allocSize);
            elementVector.push_back(value);
        }
//        log();
    }

    void remove(const USE_NUMBER_TYPE &id) {
        USE_NUMBER_TYPE index = findClosesLowerIndex(id);
        log();
        if (index < elementVector.size()) {
            elementVector.erase(elementVector.begin() + index);
        }
    }

    void add(const Position &pos) {
        USE_NUMBER_TYPE index = findClosesLowerIndex(pos.id);


        elementVector.insert(elementVector.begin() + index, pos);

        USE_NUMBER_TYPE allocSize = sizeof(pos);
        idxFileStream.seekg(pos.id * allocSize);
        idxFileStream.write(reinterpret_cast<const char *>(&pos), allocSize);

    }

    USE_NUMBER_TYPE size() {
        return elementVector.size();
    }

    USE_NUMBER_TYPE findClosesLowerIndex(const USE_NUMBER_TYPE &id) {
        USE_NUMBER_TYPE leftIndex = 0;
        USE_NUMBER_TYPE rightIndex = elementVector.size();

        while (leftIndex < rightIndex) {
            USE_NUMBER_TYPE mid = leftIndex + (rightIndex - leftIndex) / 2;
//            std::cout << "mid: " << mid << std::endl;
            if (id <= elementVector[mid].id) {
                rightIndex = mid;
            } else {
                leftIndex = mid + 1;
            }
        }

        return leftIndex;
    }

    void log() {

        std::cout << "[";
        for (uint64_t index = 0; index < elementVector.size(); index++) {
            std::cout << (index == 0 ? "" : ", ") << "{ id: " << elementVector[index].id << ", position: "
                      << elementVector[index].position << " }";
        }
        std::cout << "]" << std::endl;
        return;
    }
};
