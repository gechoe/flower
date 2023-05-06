// Bryn Mawr College, gchoe, 2023

#include <cmath>
#include <string>
#include <vector>
#include "agl/plymesh.h"
#include "agl/window.h"
#include "agl/osutils.h"

using namespace std;
using namespace glm;
using namespace agl;

struct rgb {
  float red, green, blue, alpha;
};

class drawCircle {
  public:
    double size = 0.1;
    rgb rgbColors;
    float xCoord, yCoord;
};

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  bool isDone;
  float weight;
  glm::vec3 prevVel;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(600, 600);
    createWaterDroplets(70);

    renderer.loadTexture("cube", "../textures/cube.png", 0);
    renderer.loadTexture("vase", "../textures/vase.png", 0);

    // clean-up repitition later
    renderer.loadShader("purple-toon", "../shaders/purple-toon.vs", "../shaders/purple-toon.fs");
    renderer.loadShader("blue-toon", "../shaders/blue-toon.vs", "../shaders/blue-toon.fs");
    renderer.loadShader("background", "../shaders/background.vs", "../shaders/background.fs");
    renderer.loadShader("darkblue-solid", "../shaders/darkblue-solid.vs", "../shaders/darkblue-solid.fs");
    renderer.loadShader("ground", "../shaders/ground.vs", "../shaders/ground.fs");
    renderer.loadShader("vase", "../shaders/vase.vs", "../shaders/vase.fs");
    renderer.loadShader("soil", "../shaders/soil.vs", "../shaders/soil.fs");
    renderer.loadShader("logo", "../shaders/logo.vs", "../shaders/logo.fs");
    renderer.loadShader("water", "../shaders/water.vs", "../shaders/water.fs");

    renderer.loadTexture("waterDropletParticle", "../textures/cube.png", 0);

    wateringCanMesh = PLYMesh("../models/waterCan.ply");
    // renderer.setUniform("numTriangles", wateringCanMesh.numTriangles());
    flowersMesh = PLYMesh("../models/flower.ply");
    // renderer.setUniform("numTriangles", flowersMesh.numTriangles());
    groundMesh = PLYMesh("../models/cube.ply");
    // renderer.setUniform("numTriangles", groundMesh.numTriangles());

    vaseMesh = PLYMesh("../models/vase.ply");
    logoMesh = PLYMesh("../models/flowerWords.ply");
    drawerMesh = PLYMesh("../models/drawer.ply");
  }

  int width() {
    return 600;
  }

  int height() {
    return 600;
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    if (mouseIsDown(GLFW_MOUSE_BUTTON_LEFT)) {
      // todo: store a circle with the current color, size, x, y
      if (y > 70) { // Prevents user from drawing underneath palette
        drawCircle newCircle = circles;
        newCircle.xCoord = ((x / (float)width()) * halfWidth * 2) - halfWidth;
        float newY = height() - y;
        newCircle.yCoord = ((newY / (float)height()) * halfHeight * 2) - halfHeight;//y;
        newCircle.size = 25;
        // std::cout << newCircle.xCoord << "     " << newCircle.yCoord << std::endl;
        circleVector.push_back(newCircle);
      }
    }
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  // keyDown()
  // Changes brush size, brush transparency, and clears screen if certain keys
  // are pressed
  void keyDown(int key, int mods) override {
    if (key == GLFW_KEY_C) { // 'C' key, clears teh user's drawn path
      // Clear vector of circles
      circleVector.clear();
    } else if (key == GLFW_KEY_R) { // 'R' key, resets the scene/water level
      // Clear vector of circles
      reset = true;
    }
  }

  void scroll(float dx, float dy) {
  }

  float randomize(float min, float max) {
    float randNum = (float) rand() / RAND_MAX;
    float randNumInRange = (max - min) * (randNum) + (min);
    return randNumInRange;
  }

  void drawBackground() {
    // Drawing the background:
    renderer.beginShader("background");

    renderer.push();
    renderer.skybox();

    renderer.pop();
    renderer.endShader();
  }

  void drawGround() {
    // Drawing the ground:
    renderer.beginShader("ground");

    renderer.push();
    renderer.scale(vec3(10, 3.0, 1.0));
    renderer.translate(vec3(0.0, -1.8, 0));
    renderer.cube();

    renderer.pop();
    renderer.endShader();
  }

  void drawWateringCan() {
    // Drawing the watering can:
    renderer.beginShader("blue-toon");
    
    renderer.push();
    vec3 scale = wateringCanMesh.scaleVal();
    renderer.scale(scale * vec3(0.30));
    renderer.translate(vec3(-23, 20.5, 0));
    renderer.rotate(vec3(0.0, 0.0, -0.5));

    renderer.mesh(wateringCanMesh);
    renderer.pop();
    renderer.endShader();
  }

  void drawFlowers() {
    // Drawing the flower:
    renderer.beginShader("blue-toon");
    renderer.push();

    vec3 scale = flowersMesh.scaleVal();
    renderer.scale(scale * vec3(0.5));
    renderer.translate(vec3(22.0, -13.0, 0.0));

    renderer.mesh(flowersMesh);
    renderer.pop();
    renderer.endShader();

    // Drawing the soil:
    renderer.beginShader("soil");
    renderer.push();
    renderer.scale(vec3(1.5, 1.0, 1.5));
    renderer.translate(vec3(2.0, -3.4, 2.0));
    renderer.cube();
    renderer.pop();
    renderer.endShader();
  }

  void drawWaterLevel() {
    renderer.beginShader("water");
    renderer.push();
    renderer.scale(vec3(1.5, 1.0, 1.5));

    // Sets liimit of having at least a certain amount of particles fall in vase
    // for water level to increase
    if (((particlesInVase % 100 == 10) || (particlesInVase % 100 == 50)) && (waterLevel <= -2.5)) {
      moveWaterUp = 0.1;
      waterLevel += moveWaterUp;
    }

    if (reset == true) {
      waterLevel = -3.4;
      particlesInVase = 0;
      reset = false;
    }

    renderer.translate(vec3(2.0, waterLevel, 2.0));
    renderer.cube();
    renderer.pop();
    renderer.endShader();
  }

  void drawVase() {
    renderer.beginShader("vase");
    // Drawing the  vase sides:
    renderer.push();
    renderer.scale(vec3(0.1, 2.0, 2.0));
    renderer.translate(vec3(23.0, -1.45, 2.0));
    renderer.cube();
    renderer.pop();

    renderer.push();
    renderer.scale(vec3(1.5, 0.1, 2.0));
    renderer.translate(vec3(2.0, -38.5, 2.0));
    renderer.cube();
    renderer.pop();

    renderer.push();
    renderer.scale(vec3(0.1, 2.0, 2.0));
    renderer.translate(vec3(38.0, -1.45, 2.0));
    renderer.cube();
    renderer.pop();
  }

  void drawLogoObstruction() {
    // Drawing the obstruction block
    renderer.beginShader("logo");
    renderer.push();
    renderer.scale(vec3(4.5, 1.75, 1.0));
    renderer.translate(vec3(-0.5, -1.32, 1.8));
    renderer.cube();
    renderer.pop();
    renderer.endShader();

    // Drawing the logo
    renderer.beginShader("background");
    renderer.push();
    renderer.scale(vec3(1.0, 1.5, 1.2));
    renderer.translate(vec3(-4.2, -1.85, 2.0));
    renderer.mesh(logoMesh);
    renderer.pop();
    renderer.endShader();
  }

  void createWaterDroplets(int amount) {
    for (int i = 0; i < amount; i++) {
      Particle particle;
      particle.color = vec4(0.4, 0.7, 1, 1);
      particle.size = 0.1;

      particle.pos = spoutPos;
        
      float rVelY = randomize(-0.1, 0.1);
      particle.vel = vec3(0, rVelY, 0);
      particle.weight = weight;
        
      waterDropletParticles.push_back(particle);
    }
  }

  void updateWaterDroplets(float dt) {
    for (int i = 0; i < waterDropletParticles.size(); i++) {
      Particle particle = waterDropletParticles[i];

      int j = 0;
      while (j < 20) {
        vec3 force = vec3(0, -gravity, 0); // Creates force
        int numCircles = circleVector.size();

        // Loops through the circles in the user's drawn path (to see if there
        //is any collision)
        for (int k = 0; k < numCircles - 1; k++) {
          drawCircle circ = circleVector[k];
          vec3 circPos = vec3(circ.xCoord, circ.yCoord, 0);

          if (glm::distance(circPos, particle.pos) < 0.3) { // collision distance 0.3
            drawCircle nextCirc = circleVector[k + 1];
            vec3 nextCircPos = vec3(nextCirc.xCoord, nextCirc.yCoord, 0);

            float lengthCirc = glm::length(nextCircPos - circPos);

            if (lengthCirc > 0.0001) {
              vec3 direction = normalize(nextCircPos - circPos);
              force += direction * 0.01f;// 0.1 sideways force
            }

            float lengthCircPart = glm::length(particle.pos - circPos);

            if (lengthCircPart > 0.000001) {
              vec3 direction = vec3(0, gravity, 0) / vec3(5);
              force += direction;
            }
          }
        }

        particle.vel += force * dt;
        particle.pos += dt * particle.vel;

        if ((particle.pos.y <= bottom) || (particle.pos.x >= halfWidth)) {
          particle.pos = spoutPos;
          float rVelY = randomize(-0.4, 0.2);
          particle.vel = vec3(0, rVelY, 0);
        }

        // Checks if particle is inside the vase and if it hits the water level
        if ((particle.pos.x >= 2.26 && particle.pos.x <= 3.78) && 
          (particle.pos.y <= waterLevel)) {
          particlesInVase += 1;
          particle.pos = spoutPos;
          float rVelY = randomize(-0.4, 0.2);
          particle.vel = vec3(0, rVelY, 0);
        }

        // If it hits the inside of the vase's right wall, the water particle drops down,
        // instead of continuing to shoot outside the vase
        if (particle.pos.x <= 3.78 && particle.pos.x >= 3.75 && particle.pos.y <= -1.93) {
          float rVelY = randomize(-0.4, 0.2);
          particle.vel = vec3(0, rVelY, 0);
        }

        // If it hits outside the vase's left wall, the water particle drops down,
        // instead of continuing to shoot into the vase
        if (particle.pos.x <= 2.26 && particle.pos.x >= 2.23 && particle.pos.y <= -1.93) {
          float rVelY = randomize(-0.4, 0.2);
          particle.vel = vec3(0, rVelY, 0);
        }

        waterDropletParticles[i] = particle;

        if (particle.pos.y < 3.3) {
          j++;
        }
      }
    }
  }

  void drawWaterDroplets() {
    renderer.texture("WaterImage", "waterDropletParticle");

    for (int i = 0; i < waterDropletParticles.size(); i++) {
      Particle particle = waterDropletParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size);
    }
  }

  void draw() {
    halfWidth = 5;
    halfHeight = 5 * (width() / height());
    float aspect = ((float)width()) / height();
    renderer.ortho(-(halfWidth), halfWidth, -(halfHeight), halfHeight, 0, 100);
    renderer.lookAt(eyePos, lookPos, up);

    // draws the user drawn path
    for (drawCircle c : circleVector) {
      renderer.beginShader("ground");
      renderer.push();
      float findX = c.xCoord;
      float findY = c.yCoord;

      renderer.translate(vec3(findX, findY, 2));
      renderer.mesh(drawerMesh);
      renderer.pop();
      renderer.endShader();
    }

    // draws all objects in the scene
    drawBackground();
    drawGround();
    drawWateringCan();
    drawFlowers();
    drawVase();
    drawLogoObstruction();
    drawWaterLevel();

    // draws the water droplet particles
    renderer.beginShader("sprite");
    updateWaterDroplets(dt());
    drawWaterDroplets();
    renderer.endShader();
  }

protected:
  PLYMesh wateringCanMesh, groundMesh, flowersMesh, vaseMesh, logoMesh, drawerMesh;
  vec3 eyePos = vec3(0, 0, 5); // vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  float hamHandleX = width() - 40, hamHandleY = 35, hamHandleWidth = 35, hamHandleHeight = 45;
  float hammerX = width() - 40, hammerY = 42, hammerWidth = 35, hammerHeight = 25;
  float pointHandleX = hammerX - 60, pointHandleY = 35 - hamHandleHeight / 4, pointHandleWidth = hamHandleWidth / 3, pointHandleHeight = hamHandleHeight / 2;
  float pointerX = hammerX - 60, pointerY = 42, pointerWidth = 35, pointerHeight = 35;
  float speed = -0.2f;
  std::vector<Particle> waterDropletParticles;
  vec3 spoutPos = vec3(-2.4, 3.0, 0.0);
  float bottom = -4.0;
  float gravity = 0.1, weight = 100, moveWaterUp = 0.0;
  int particleGroupings = 0, particlesInVase = -1;
  float waterLevel = -3.4;
  drawCircle circles;
  vector<drawCircle> circleVector;
  float currSize = 10, alpha = 1.0;
  float halfWidth, halfHeight;
  bool reset = false, isFull = false;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}