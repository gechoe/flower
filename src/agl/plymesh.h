//--------------------------------------------------
// Author: Grace Choe
// Date: 2/28/2023
// Description: Loads PLY files in ASCII format
//--------------------------------------------------

#ifndef plymeshmodel_H_
#define plymeshmodel_H_

#include "agl/aglm.h"
#include "agl/mesh/triangle_mesh.h"

namespace agl {
   // class vertexPos {
   //    public:
   //       float xLoc;
   //       float yLoc;
   //       float zLoc;
   // };

   // class facePos {
   //    public:
   //       int vert1;
   //       int vert2;
   //       int vert3;
   // };

   class PLYMesh : public TriangleMesh
   {
   public:

      PLYMesh(const std::string& filename);
      PLYMesh();

      virtual ~PLYMesh();

      // Method that helps with splitting a line by spaces.
      void splitString(std::string strLine, std::string splitBySpaces);

      long stoNumber(const std::string str); // Converts string to a number

      // Initialize this object with the given file
      // Returns true if successfull. false otherwise.
      bool load(const std::string& filename);

      // Return the minimum point of the axis-aligned bounding box
      glm::vec3 minBounds() const;

      // Return the maximum point of the axis-aligned bounding box
      glm::vec3 maxBounds() const;

      // Gets the vec3 for the renderer.translate() value
      glm::vec3 translateVal();

      // Gets the vec3 for the renderer.scale() value
      glm::vec3 scaleVal();

      // Return number of vertices in this model
      int numVertices() const;

      // Positions in this model
      const std::vector<GLfloat>& positions() const;

      // Positions in this model
      const std::vector<GLfloat>& normals() const;

      // Return number of faces in this model
      int numTriangles() const;

      // face indices in this model
      const std::vector<GLuint>& indices() const;

   protected:
      void init();

   protected:
      std::vector<std::string> wordsVec; // contains words from text file
      // std::vector<vertexPos> vertices_vec; // contains vertices from text file
      // std::vector<facePos> faces_vec; // contains faces from text file

      std::vector<GLfloat> _positions; // vertices
      std::vector<GLfloat> _normals; // normals
      std::vector<GLuint> _faces; // faces
   };
}

#endif
