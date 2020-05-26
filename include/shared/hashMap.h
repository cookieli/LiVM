//
// Created by lzx on 5/14/20.
//

#ifndef LIVM_HASHMAP_H
#define LIVM_HASHMAP_H
#include <sparsepp/spp.h>
namespace LiVM {
    template<typename K, typename V>
    using HashMap = spp::sparse_hash_map<K, V>;
}
#endif //LIVM_HASHMAP_H
