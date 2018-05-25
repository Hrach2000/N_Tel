#include "GLBall.hpp"
#include "NBodyScene.hpp"
#include <cmath>
#include <iostream>
#include <vector>

const double G = 10;

class Body : public GLBall {

  double m;
  double r;
  double x;
  double y;
  double v_x;
  double v_y;

public:
  Body(){};
  Body(double m, double r, double x, double y, double v_x, double v_y)
      : m(m), r(r), x(x), y(y), v_x(v_x), v_y(v_y) {}

  void move(double a_x, double a_y, double d_t) {
    v_x += a_x * d_t;
    v_y += a_y * d_t;
    x += v_x * d_t;
    y += v_y * d_t;
  }

  double getX() const override { return x; }

  double getY() const override { return y; }

  double getR() const override { return r; }

  double getM() const { return m; }

  friend class BodyScene;
  friend std::istream &operator>>(std::istream &is, Body &bd);
};

std::istream &operator>>(std::istream &is, Body &bd) {
  is >> bd.m >> bd.r >> bd.x >> bd.y >> bd.v_x >> bd.v_y;
  return is;
}
class BodyScene : public NBodyScene {
  std::vector<Body> bodies;
  double d_t;

public:
  BodyScene() : d_t(0.1) {}
  /*
  BodyScene() {
      d_t = 0.1;
  }
  */
  BodyScene(double d_t) : d_t(d_t) {}

  void setBodies(const std::vector<Body> &otherBodies) { bodies = otherBodies; }

  unsigned int getNumberOfBodies() const override { return bodies.size(); }

  const GLBall &getBody(unsigned int number) const override {
    return bodies.at(number);
  }

  void doTimeStep() override {
    // считаем для каждого тела ускорение как сумму ускорений по x и y
    std::vector<double> a_xVec;
    std::vector<double> a_yVec;
    for (Body &body : bodies) {
      double a_x = 0, a_y = 0;

      for (Body &second : bodies) {
        double delta_x = second.getX() - body.getX();
        double delta_y = second.getY() - body.getY();
        double distance = sqrt(delta_x * delta_x + delta_y * delta_y);

        if (distance == 0)
          continue;

        double a = G * second.getM() / (distance * distance);
        a_x += a * delta_x / distance;
        a_y += a * delta_y / distance;
      }

      a_xVec.push_back(a_x);
      a_yVec.push_back(a_y);
    }

    for (unsigned int i = 0; i < bodies.size(); i++)
      bodies.at(i).move(a_xVec[i], a_yVec[i], d_t);
  }
  void initScene(std::vector<Body> bodies) { this->bodies = bodies; }
};

NBodyScene *getScene() {
  BodyScene *s = new BodyScene();

  std::vector<Body> bodies;
  unsigned int num;

  std::cin >> num;

  for (unsigned int i = 0; i < num; i++) {
    Body cur;
    std::cin >> cur;

    bodies.push_back(cur);
  }

  s->initScene(bodies);
  return s;
}
int main() {
  NBodyScene *bodyScene = getScene();
  /*
std::vector<Body> myBodies;
myBodies.push_back(Body(20000, 1, 0, 0, 0, 0));
myBodies.push_back(Body(0.00001, 10, 10, 0, 0, 200));
//    myBodies.push_back(Body(100, 10, 0, 500, 10, 0));

BodyScene bodyScene;
bodyScene.setBodies(myBodies);

std::cout << bodyScene.getBody(0).getX() << "    "
      << bodyScene.getBody(0).getY() << "  |  ";
std::cout << bodyScene.getBody(1).getX() << "    "
      << bodyScene.getBody(1).getY() << std::endl;
//    std::cout << bodyScene.getBody(2).getX() << " " <<
//    bodyScene.getBody(2).getY() << std::endl;
  */
  for (int i = 0; i < 500; i++) {
    bodyScene->doTimeStep();
    std::cout << bodyScene->getBody(0).getX() << "    "
              << bodyScene->getBody(0).getY() << "  |  ";
    std::cout << bodyScene->getBody(1).getX() << "    "
              << bodyScene->getBody(1).getY() << std::endl;
  }
  return 0;
}
