#include<vector>
#include<unordered_map>

using std::vector, std::unordered_map;
using label = int;

// union by size
class UnionFindStructure {
    private:
    unordered_map<label, label> parent; // element -> parent/representant of element
    unordered_map<label, size_t> sizes; // element -> size of its partition if element is representant, rubish otherwise

    public:
    // construct from universe of elements
    UnionFindStructure(const vector<label>& universe){
        for(const auto& el : universe){
            parent[el] = el;
            sizes[el] = 1;
        }
    }
    // construct from number of labels label {0...(n-1)}
    UnionFindStructure(const int n){
        for(int i = 0; i < n; i++){
            parent[i] = i;
            sizes[i] = 1;
        }
    }

    label findRepresentative(label x){
        if(parent[x] != x){
            parent[x] = findRepresentative(parent[x]); // path compression
        }
        return parent[x];
    }

    void setUnion(label x, label y){
        label rootX = findRepresentative(x);
        label rootY = findRepresentative(y);

        if(rootX == rootY){
            return; // already in the same partition
        }

        // ensure rootX represents the bigger set
        if(sizes[rootX] < sizes[rootY]){
            std::swap(rootX, rootY);
        }

        // merge smaller set into the larger one
        parent[rootY] = rootX;
        sizes[rootX] += sizes[rootY]; // note that only the representatnt of the subset holds corrent subset size
    }

    bool inTheSameSubset(label x, label y){
        return findRepresentative(x) == findRepresentative(y);
    }
};

// weighted edges, possible simple representations, std::sort compatible
using Edge = std::pair<label, label>; // from, to
using WeightedEdge1 = std::tuple<int, label, label>; // weight, from, to
using WeightedEdge2 = std::pair<int, Edge>; // weight, (from, to)

struct weigthedEdge3{
    int weight;
    label from;
    label to;

    // comparison operator for std::sort, does not work with std::set or std::map
    bool operator<(const weigthedEdge3& other) const {
        return weight < other.weight;
    }
};

struct weigthedEdge4{
    int weight;
    label from;
    label to;

    // constructor with normalized parameter order
    weigthedEdge4(int w, label f, label t){
        weight = w;
        if (f > t){
            std::swap(f, t);
        }
        from = f;
        to = t;
    }

    // comparison operator that will work with std::set or std::map in expected way
    bool operator<(const weigthedEdge4& other) const {
        if(weight != other.weight){
            return weight < other.weight;
        }
        if(from != other.from){
            return from < other.from;
        }
        return to < other.to;
    }
};

// simple graph representation as list of edges, pick your best representation
using Graph = vector<WeightedEdge1>;

int main(){
    // Cpp constuction and initialization is sadly quite tricky but these tend to work fine
    WeightedEdge1 we1{10, 1, 2};
    WeightedEdge2 we2{20, {2, 3}};
    weigthedEdge3 we3{15, 3, 4};
    weigthedEdge4 we4{25, 4, 5};
    return 0;
}