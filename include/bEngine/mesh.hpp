#ifndef BENGINE_MESH
#define BENGINE_MESH

#include <vector>
#include <fstream>
#include <sstream>
#include "bMath/bMath.hpp"

namespace bEngine {
    struct Mesh {
        std::vector<bMath::float3> vertices;
        std::vector<int> indices;

        
    };

    std::string filterString(std::string str, const char* breakpoint) {
        std::string newStr;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == *breakpoint) break;
            newStr += str[i];
        }
        return newStr;
    }

    Mesh ParseMeshFromFile(const char *filename) {
        std::ifstream file(filename);
        std::string line;
        Mesh newMesh;

        while(std::getline(file, line)) {
            std::string type = line.substr(0,2);
            
            if (type == "v ") {
                std::istringstream v(line.substr(2));
                float x,y,z;
                v>>x;v>>y;v>>z;
                newMesh.vertices.push_back(bMath::float3(x,y,z));
                std::cout << line << std::endl;
                std::cout << std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) << std::endl;
            }

            if (type == "f ") {
                std::istringstream f(line.substr(2));
                std::string sa,sb,sc;
                int a, b, c;
                f>>sa;f>>sb;f>>sc;

                sa = filterString(sa, "/");
                sb = filterString(sb, "/");
                sc = filterString(sc, "/");
                std::cout << line << std::endl;
                std::cout << sa + ", " + sb +", "+sc << std::endl;
                a = std::stoi(sa);
                b = std::stoi(sb);
                c = std::stoi(sc);
                // int a, b, c;
                // int A, B, C;
                // const char* chh=line.c_str();
                // sscanf(chh, "f %i/%i %i/%i %i/%i", &a,&A,&b,&B,&c,&C);
            
                newMesh.indices.push_back(a); 
                newMesh.indices.push_back(b);
                newMesh.indices.push_back(c);
                // std::cout << line << std::endl;
                // std::cout << std::to_string(a) + ", " + std::to_string(b) + ", " + std::to_string(c) << std::endl;
            }
        }
        return newMesh;
    }
}

#endif