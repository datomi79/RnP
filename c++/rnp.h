#pragma once

#include <Eigen/Eigen>
#include <vector>
#include "utils.h"

#define ERR_NO_SOLUTION 1
#define WARN_NO_CONVERGENCE 2

struct RSCameraPose{
    Eigen::Matrix3d R; // Camera orientation as rotation matrix
    Eigen::Vector3d C; // Camera center
    Eigen::Vector3d w; // Camera rotational velocity (rotation axis where ||w|| is the angular velocity)
    Eigen::Vector3d t; // Camera translational velocity
    double f; // focal length (only R7Pf and R7Pfr compute focal lenght, otherwise 1)
    double rd; // radial distortion coefficient (only R7Pfr computes it, otherwise 0)
};

struct RSDoublelinCameraPose{
    Eigen::Vector3d v; // Camera orientation as I + [v]_x
    Eigen::Vector3d C; // Camera center
    Eigen::Vector3d w; // Camera rotational velocity (rotation axis where ||w|| is the angular velocity)
    Eigen::Vector3d t; // Camera translational velocity
    double f; // focal length (only R7Pf and R7Pfr compute focal lenght, otherwise 1)
    double rd; // radial distortion coefficient (only R7Pfr computes it, otherwise 0)
};

struct RSSinglelinCameraPose{
    Eigen::Vector3d v; // Camera orientation in Cayley parameterization
    Eigen::Vector3d C; // Camera center
    Eigen::Vector3d w; // Camera rotational velocity (rotation axis where ||w|| is the angular velocity)
    Eigen::Vector3d t; // Camera translational velocity
    double f; // focal length (only R7Pf and R7Pfr compute focal lenght, otherwise 1)
    double rd; // radial distortion coefficient (only R7Pfr computes it, otherwise 0)
};

typedef std::vector<RSCameraPose> RSCameraPoseVector;
typedef std::vector<RSDoublelinCameraPose> RSDoublelinCameraPoseVector;
typedef std::vector<RSSinglelinCameraPose> RSSinglelinCameraPoseVector;

template<typename T>
bool isPoseApproxEqual(T pose1, T pose2);

int rs2LinProjection(const Eigen::Vector3d &X, Eigen::Vector2d &u, const Eigen::Vector3d &v, const Eigen::Vector3d &C, const Eigen::Vector3d &w, const Eigen::Vector3d &t, double f, double rd, double r0, int direction);

int rs1LinProjection(const Eigen::Vector3d &X, Eigen::Vector2d &u, const Eigen::Vector3d &v, const Eigen::Vector3d &C, const Eigen::Vector3d &w, const Eigen::Vector3d &t, double f, double rd, double r0, int direction);

int R7PfIter(const Eigen::MatrixXd &X, const Eigen::MatrixXd &u, const Eigen::Vector3d &vk, double r0, RSDoublelinCameraPoseVector * results);

int R7PfrIter(const Eigen::MatrixXd &X, const Eigen::MatrixXd &u, const Eigen::Vector3d &vk, double r0, RSDoublelinCameraPoseVector * results);

template<typename Model, int (*Solver)(const Eigen::MatrixXd &X, const Eigen::MatrixXd &u, const Eigen::Vector3d & vk, double r0, std::vector<Model> * results)>
int iterativeRnP(Eigen::MatrixXd &X, Eigen::MatrixXd &u, Eigen::Vector3d vk, int sampleSize, double r0, int direction, int maxIter, Model & result);

int R6P1Lin(const Eigen::MatrixXd & X, const Eigen::MatrixXd & u, int direction, double r0, int maxpow, RSSinglelinCameraPoseVector * results);

int R6P2Lin(const Eigen::MatrixXd & X, const Eigen::MatrixXd & u, int direction, double r0, RSDoublelinCameraPoseVector * results);

int R6PIter(const Eigen::MatrixXd & X, const Eigen::MatrixXd & u, const Eigen::Vector3d & vk, double r0, RSDoublelinCameraPoseVector * results);


double calcErrAlgebraicRnPFocalRadialDoubleLin(Eigen::Vector3d vr, Eigen::Vector3d Cr, Eigen::Vector3d wr, Eigen::Vector3d tr, double f, double rd, double r0, Eigen::MatrixXd X, Eigen::MatrixXd u, int direction);
double calcErrAlgebraicRnPFocalRadialSingleLin(Eigen::Vector3d vr, Eigen::Vector3d Cr, Eigen::Vector3d wr, Eigen::Vector3d tr, double f, double rd, double r0, Eigen::MatrixXd X, Eigen::MatrixXd u, int direction);

