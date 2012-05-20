/* 
 * File:   Model.h
 * Author: dbf
 *
 * Created on 20. Mai 2012, 13:43
 */

#ifndef MODEL_H
#define	MODEL_H

namespace iNumerics {

    class Model {
    public:
        
        virtual void rhs(const DVec &y, DVec &dydt, const double t) = 0;
        
        
        virtual ~Model() {
            //
        };
        
    private:

    };

}

#endif	/* MODEL_H */

