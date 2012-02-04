/* 
 * File:   Trajectory.h
 * Author: dbf
 *
 * Created on 4. Februar 2012, 16:58
 */

#ifndef TRAJECTORY_H
#define	TRAJECTORY_H

#include <vector>
#include "Types.h"

namespace iNumerics {

    class Trajectory {
    public:
        Trajectory();
        Trajectory(const Trajectory& orig);
        virtual ~Trajectory();

        void operator()(const DVec& x, const double t);
        
        double getTime(std::size_t i);
        const DVec& getState(std::size_t i);
        size_t size() const;

    private:
        std::vector< DVec > _states;
        std::vector< double > _times;
    };

}

#endif	/* TRAJECTORY_H */

