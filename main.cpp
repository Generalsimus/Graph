#include <utility>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include <vector>

//#include "./storage.cc"
#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include "./graph.cc"


template<typename USE_NUMBER_TYPE>
struct TestStruct {
    USE_NUMBER_TYPE testNum;
    std::vector<USE_NUMBER_TYPE> testVector;

    void serialize(std::ostream &os) const {
        os.write(reinterpret_cast<const char *>(&testNum), sizeof(testNum));
        USE_NUMBER_TYPE vectorSize = testVector.size();
        os.write(reinterpret_cast<const char *>(&vectorSize), sizeof(vectorSize));

        const USE_NUMBER_TYPE *data = testVector.data();
        os.write(reinterpret_cast<const char *>(data), sizeof(USE_NUMBER_TYPE) * vectorSize);
    }

    void deserialize(std::istream &is) {
        is.read(reinterpret_cast<char *>(&testNum), sizeof(testNum));

        USE_NUMBER_TYPE vectorSize;
        is.read(reinterpret_cast<char *>(&vectorSize), sizeof(vectorSize));

        testVector.resize(vectorSize);

        USE_NUMBER_TYPE *data = testVector.data();
        is.read(reinterpret_cast<char *>(data), sizeof(USE_NUMBER_TYPE) * vectorSize);
    }
};

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    Graph<uint64_t> graph("./graph_storage.db");

//    Storage<TestStruct<uint64_t>, uint64_t> reader("./test_storage.db");


//    graph.
    const auto node0 = graph.setNode({{}, 0});
    const auto node1 = graph.setNode({{}, 1});
    const auto node2 = graph.setNode({{}, 2});
//    std::cout << "const auto node0 = graph.setNode({0, {}}) ---> : " << node2.id << "  END" << std::endl;
    graph.addEdge(node0.id, node1.id, 0.1f);
    graph.addEdge(node1.id, node2.id, 0.2f);
    graph.addEdge(node2.id, node0.id, 0.3f);

//    node1.re
    std::cout << "RESPONSE ---> : " << graph.findById(0).relationShips[0].cost << "  END" << std::endl;
//    reader.add({5, {0}});
//    reader.add({5, {1}});
//    reader.add({5, {2}});
//    auto rrr = reader.add({5, {1, 2}});
//    reader.add({5, {4}});
//    reader.add({5, {5}});
//    reader.add({5, {6}});
//
//
//    rrr.testVector.push_back(2222222);
//    rrr.testVector.push_back(3333333);
//    rrr.testVector.push_back(444444);
//    reader.set(rrr);

//    auto v1 = reader.findById(3);

//    std::cout << "RESPONSE ---> : " << v1.testVector[4] << "  END" << std::endl;


    //   std::cout << "size:   memo: " << std::endl;
//    setInterval([&reader, &rrr]() {
////                    reader.set(rrr);
//                    auto v4 = reader.findById(3);
//                    std::cout << " ---> 21312312: " << v4.testVector[3] << "  EEMD" << std::endl;
//                },
//                1000);


    // int end = clock();

    auto endTime = std::chrono::high_resolution_clock::now();
    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    // Output the duration
    std::cout << "Function execution time: " << ((float) duration.count() / 1000) << " seconds" << std::endl;
    return 0;
}
