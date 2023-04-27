//--------------------------------------------------
// Author: Grace Choe
// Date: 2/28/2023
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#include "plymesh.h"
#include "agl/window.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace glm;

namespace agl {

   PLYMesh::PLYMesh(const std::string& filename) {
      load(filename);
   }

   PLYMesh::PLYMesh() {
      load("../models/cube.ply");
   }

   void PLYMesh::init() {
      assert(_positions.size() != 0);
      initBuffers(&_faces, &_positions, &_normals);
   }

   PLYMesh::~PLYMesh() {
      _positions.clear();
      _faces.clear();
      _normals.clear();
   }

   void PLYMesh::splitString(string line, string splitVar = " ") {
      wordsVec.clear();
      int begin = 0;
      int end = line.find(splitVar);
      while (end != -1) {
        //cout << line.substr(begin, end - begin) << endl;
        wordsVec.push_back(line.substr(begin, end - begin));
        begin = end + splitVar.size();
        end = line.find(splitVar, begin);
      }
      
      wordsVec.push_back(line.substr(begin, end - begin));
   }

   long PLYMesh::stoNumber(string number) {
      long i;
      i = 0;
      
      for (char c : number) {
         if (c >= '0' && c <= '9') {
            i = i * 10 + (c - '0');
            c++;
         }
      }
     
      return i;
   }

   bool PLYMesh::load(const string& filename) {
      if (_positions.size() != 0) {
         std::cout << "WARNING: Cannot load different files with the same PLY mesh\n";
         return false;
      }

      // todo: your code here
      string fileText;

      // std::cout << filename << std::endl;
      ifstream FileReader("../models/" + filename);

      if (FileReader.is_open()) {
         getline(FileReader, fileText);

         if (fileText.compare("ply") != 0) {
            std::cout << "WARNING: Ply file is incorrect." << std::endl;
            return false;
         }

         getline(FileReader, fileText); // Skips "format ascii 1.0" line
         getline(FileReader, fileText); // Skips comment line
      
         getline(FileReader, fileText); // Reads "element vertex [number]"
         splitString(fileText); // Splits string by spaces
         // Gets only the number from the element vertex line
         string vAmount = wordsVec[wordsVec.size() - 1];
         int vertexAmount = stoNumber(vAmount);

         getline(FileReader, fileText);
         splitString(fileText);
         
         while (wordsVec[0].compare("element") != 0) {
            getline(FileReader, fileText);
            splitString(fileText);
         }

         splitString(fileText); // Splits string by spaces
         // Gets only the number from the element face line
         int faceAmount = stoNumber(wordsVec[wordsVec.size() - 1]);

         // Skips "property list uint8 int32 vertex_indices" line
         getline(FileReader, fileText);

         getline(FileReader, fileText); // Skips "end_header" line
         wordsVec.clear(); 

         for (int i = 0; i < vertexAmount; i++) {
            getline(FileReader, fileText); // Reads vertex information
            splitString(fileText); // Splits string by spaces
            // float x = 
            _positions.push_back(stof(wordsVec[0]));
            // float y = 
            _positions.push_back(stof(wordsVec[1]));
            // float z = 
            _positions.push_back(stof(wordsVec[2]));
            _normals.push_back(stof(wordsVec[3])); // for nx
            _normals.push_back(stof(wordsVec[4])); // for ny
            _normals.push_back(stof(wordsVec[5])); // for nz
            // vertexPos vert = {x, y, z};

            // vertices_vec.push_back(vert);
            wordsVec.clear();
         }

         for (int j = 0; j < faceAmount; j++) {
            getline(FileReader, fileText); // Reads face information
            splitString(fileText); // Splits string by spaces
            // int vertOne = 
            _faces.push_back(stoi(wordsVec[1]));
            // int vertTwo = 
            _faces.push_back(stoi(wordsVec[2]));
            // int vertThree = 
            _faces.push_back(stoi(wordsVec[3]));
            // facePos face = {vertOne, vertTwo, vertThree};

            // faces_vec.push_back(face);
            wordsVec.clear();
         }

         FileReader.close();
      } else {
         std::cout << "WARNING: Ply file cannot be opened." << std::endl;
      }

      return false;
   }

   glm::vec3 PLYMesh::minBounds() const {
      glm::vec3 minVert;

      minVert.x = _positions[0];
      minVert.y = _positions[1];
      minVert.z = _positions[2];

      for (int i = 3; i < _positions.size(); i += 3) {
         minVert.x = std::min(minVert.x, _positions[i]);
         minVert.y = std::min(minVert.y, _positions[i + 1]);
         minVert.z = std::min(minVert.z, _positions[i + 2]);
      }

      return minVert;
   }

   glm::vec3 PLYMesh::maxBounds() const {
      glm::vec3 maxVert;
      
      maxVert.x = _positions[0];
      maxVert.y = _positions[1];
      maxVert.z = _positions[2];

      for (int i = 3; i < _positions.size(); i += 3) {
         maxVert.x = std::max(maxVert.x, _positions[i]);
         maxVert.y = std::max(maxVert.y, _positions[i + 1]);
         maxVert.z = std::max(maxVert.z, _positions[i + 2]);
      }

      return maxVert;
   }

   glm::vec3 PLYMesh::translateVal() {
      glm::vec3 maxB = maxBounds();
      glm::vec3 minB = minBounds();
      glm::vec3 currCenter;

      currCenter.x = ((maxB.x - minB.x) / 2) + minB.x;
      currCenter.y = ((maxB.y - minB.y) / 2) + minB.y;
      currCenter.z = ((maxB.z - minB.z) / 2) + minB.z;

      glm::vec3 tVal = {(currCenter.x * -1), (currCenter.y * -1), (currCenter.z * -1)};
      
      return tVal;
   }

   glm::vec3 PLYMesh::scaleVal() {
      glm::vec3 sVal;
      float scaleValue;

      glm::vec3 maxB = maxBounds();
      glm::vec3 minB = minBounds();
      glm::vec3 size;

      size.x = maxB.x - minB.x;
      size.y = maxB.y - minB.y;
      size.z = maxB.z - minB.z;

      if ((size.x > 10) || (size.y > 10) || (size.z > 10)) {
         float largest = std::max(size.x, size.y);
         largest = std::max(largest, size.z);

         float smallest = std::min(size.x, size.y);
         smallest = std::min(smallest, size.z);

         scaleValue = 10 / (largest + (smallest / 2));
      } else if ((size.x < 5) || (size.y < 5) || (size.z < 5)) {
         float largest = std::max(size.x, size.y);
         largest = std::max(largest, size.z);

         float smallest = std::min(size.x, size.y);
         smallest = std::min(smallest, size.z);

         scaleValue = 10 / ((smallest * 2) + largest);
      } else {
         scaleValue = 1;
      }

      sVal = {scaleValue, scaleValue, scaleValue};
      return sVal;
   }

   int PLYMesh::numVertices() const {
      return _positions.size();
   }

   int PLYMesh::numTriangles() const {
      return _faces.size();
   }

   const std::vector<GLfloat>& PLYMesh::positions() const {
      return _positions;
   }

   const std::vector<GLfloat>& PLYMesh::normals() const {
      return _normals;
   }

   const std::vector<GLuint>& PLYMesh::indices() const {
      return _faces;
   }
}
