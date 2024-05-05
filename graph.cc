
#include <vector>
#include "./storage.cc"


template<typename USE_NUMBER_TYPE>
class Graph {

public:

    struct Relationship {
        USE_NUMBER_TYPE connectedNodeId;
        float cost;
    };


    struct BASIC_NODE {
        std::vector<Relationship> relationShips;


        void serialize(std::ostream &os) const {
            USE_NUMBER_TYPE vectorSize = relationShips.size();
            os.write(reinterpret_cast<const char *>(&vectorSize), sizeof(vectorSize));


            const Relationship *data = relationShips.data();
            os.write(reinterpret_cast<const char *>(data), sizeof(Relationship) * vectorSize);
        }

        void deserialize(std::istream &is) {
            USE_NUMBER_TYPE vectorSize;
            is.read(reinterpret_cast<char *>(&vectorSize), sizeof(vectorSize));

            relationShips.resize(vectorSize);

            Relationship *data = relationShips.data();
            is.read(reinterpret_cast<char *>(data), sizeof(Relationship) * vectorSize);
        }
    };

    struct NODE : public BASIC_NODE {
        USE_NUMBER_TYPE id;
    };

    Storage<BASIC_NODE, USE_NUMBER_TYPE> graphStorage;

//    struct NODE : public Storage<BASIC_Node, USE_NUMBER_TYPE>::DOC_TYPE {
//    };

//    typename Storage<Node, USE_NUMBER_TYPE>::DOC_TYPE NODE_WITH_ID;

    explicit Graph(const std::string &filename) : graphStorage(filename) {

    }


//    Node setNode(const Node &doc) {
    auto setNode(const NODE &doc) {

//        std::cout << "setNode:doc---> : " << doc.id << "  END" << std::endl;
        graphStorage.set({doc.relationShips, doc.id});
//        std::cout << "doc ---> : " << doc.id << "  END" << std::endl;
        return doc;
    }

    auto findById(const USE_NUMBER_TYPE &id) {
        return graphStorage.findById(id);
    }

    void addEdge(const USE_NUMBER_TYPE &fromId, const USE_NUMBER_TYPE &toId, const float &cost) {
        auto from = graphStorage.findById(fromId);

        const Relationship rel = {toId, cost};

        from.relationShips.push_back(rel);

        graphStorage.set({from.relationShips, from.id});
    }

    void findShortestPath(const USE_NUMBER_TYPE &fromId, const USE_NUMBER_TYPE &toId) {

    }
};
