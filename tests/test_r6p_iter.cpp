#include "rnp.h"
#include "utils.h"
#include <math.h>
#include <iostream>
#include <random>

bool testR6PIter(){
    Eigen::MatrixXd X(3,6);
    Eigen::MatrixXd u(2,6);
    double r0 = 0;
    int direction = 0;
    int n_points = 6;
    int maxIter = 5;
    
    bool passed = true;

    //  test with random reasonable values

    RSDoublelinCameraPose gt, model;

    gt.f = 1;
    gt.C << Eigen::Vector3d::Random();
    gt.v << Eigen::Vector3d::Random()/10;
    gt.w << Eigen::Vector3d::Random()/gt.f/10;
    gt.t << Eigen::Vector3d::Random()/gt.f/10;
    gt.rd = 0;
    X << Eigen::Matrix<double,3,6>::Random() + Eigen::Vector3d(0,0,5).replicate(1,6);
    
    // create exact 2D projections
    for (int i = 0; i < n_points; i++)
    {
        Eigen::Vector2d temp;
        if(rs2LinProjection(X.col(i), temp, gt.v, gt.C, gt.w, gt.t,  gt.f,  gt.rd,  r0,  direction)){
            std::cout << "R6PIter test failed due to projection function\n";
            return 0;
        }
        u.col(i) = temp;
    }

    // run R6PIter
    int res =  iterativeRnP<RSDoublelinCameraPose, R6PIter>(X, u, gt.v, n_points, r0, direction, maxIter, model);

    

    if(res == ERR_NO_SOLUTION){
        std::cout << "R6PIter, direction 0, returned no solution\n";
        passed = false;
    }else if(res == WARN_NO_CONVERGENCE){
        std::cout << "R6PIter, direction 0,  did not converge\n";
        passed = false;
    }else{
        if(!isPoseApproxEqual(gt,model)){
            std::cout << "R6PIter, direction 0, returned bad pose\n";
        passed = false;
        }
    }

    // Now test with other RS direction
    direction = 1;

    // create exact 2D projections
    for (int i = 0; i < n_points; i++)
    {
        Eigen::Vector2d temp;
        if(rs2LinProjection(X.col(i), temp, gt.v, gt.C, gt.w, gt.t,  gt.f,  gt.rd,  r0,  direction)){
            std::cout << "R6PIter test failed due to projection function\n";
            return 0;
        }
        u.col(i) = temp;
    }

    // run R6PIter
    res =  iterativeRnP<RSDoublelinCameraPose, R6PIter>(X, u, gt.v, n_points, r0, direction, maxIter, model);


    if(res == ERR_NO_SOLUTION){
        std::cout << "R6PIter , direction 1, returned no solution\n";
        passed = false;
    }else if(res == WARN_NO_CONVERGENCE){
        std::cout << "R6PIter, direction 1,  did not converge\n";
        passed = false;
    }else{
        if(!isPoseApproxEqual(gt,model)){
            std::cout << "R6PIter, direction 1, returned bad pose\n";
        passed = false;
        }
    }

    return passed;

}

int main(int argc, char ** argv){


if(testR6PIter()){
    std::cout << "R6PIter test passed\n";
}else{
    std::cerr << "R6PIter test did not pass\n";
    return 1;
};

return 0;
}