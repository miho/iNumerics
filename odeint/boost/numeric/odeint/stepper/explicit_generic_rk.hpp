/*
 [auto_generated]
 boost/numeric/odeint/stepper/explicit_generic_rk.hpp

 [begin_description]
 Implementation of the generic Runge-Kutta steppers. This is the base class for many Runge-Kutta steppers.
 [end_description]

 Copyright 2009-2011 Karsten Ahnert
 Copyright 2009-2011 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_GENERIC_RK_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_GENERIC_RK_HPP_INCLUDED


#include <boost/array.hpp>

#include <boost/ref.hpp>

#include <boost/numeric/odeint/stepper/base/explicit_stepper_base.hpp>
#include <boost/numeric/odeint/algebra/range_algebra.hpp>
#include <boost/numeric/odeint/algebra/default_operations.hpp>
#include <boost/numeric/odeint/stepper/detail/generic_rk_algorithm.hpp>

#include <boost/numeric/odeint/util/state_wrapper.hpp>
#include <boost/numeric/odeint/util/resizer.hpp>

#include <iostream>

namespace mpl = boost::mpl;
namespace fusion = boost::fusion;

namespace boost {
namespace numeric {
namespace odeint {

//forward declarations

template<
size_t StageCount,
size_t Order,
class State ,
class Value = double ,
class Deriv = State ,
class Time = Value ,
class Algebra = range_algebra ,
class Operations = default_operations ,
class Resizer = initially_resizer
>
class explicit_generic_rk;

struct stage_vector;

template< class T , class Constant >
struct array_wrapper
{
    typedef const typename boost::array< T , Constant::value > type;
};

template< class T , size_t i >
struct stage
{
    T c;
    boost::array< T , i > a;
};


template< class T , class Constant >
struct stage_wrapper
{
    typedef stage< T , Constant::value > type;
};

/*
template<
size_t StageCount,
size_t Order,
class State ,
class Value ,
class Deriv ,
class Time ,
class Algebra ,
class Operations ,
class Resizer
>
std::ostream& operator <<( std::ostream &os ,
        const explicit_generic_rk< StageCount , Order , State , Value , Deriv , Time , Algebra , Operations , Resizer > &rk )
{
    os << "Generic RK with " << rk.stage_count << " stages." << std::endl;
    os << "Butcher Tableau: " << std::endl;
    rk.m_stages.print( os );
    return os;
}
*/


template<
size_t StageCount,
size_t Order,
class State ,
class Value ,
class Deriv ,
class Time ,
class Algebra ,
class Operations ,
class Resizer
>
class explicit_generic_rk : public explicit_stepper_base<
explicit_generic_rk< StageCount , Order , State , Value , Deriv , Time , Algebra , Operations , Resizer > ,
Order , State , Value , Deriv , Time , Algebra , Operations , Resizer >
{

public:

    typedef explicit_stepper_base<
            explicit_generic_rk< StageCount , Order , State , Value , Deriv ,Time , Algebra , Operations , Resizer > ,
            Order , State , Value , Deriv , Time , Algebra ,
            Operations , Resizer > stepper_base_type;

    typedef typename stepper_base_type::state_type state_type;
    typedef typename stepper_base_type::wrapped_state_type wrapped_state_type;
    typedef typename stepper_base_type::value_type value_type;
    typedef typename stepper_base_type::deriv_type deriv_type;
    typedef typename stepper_base_type::wrapped_deriv_type wrapped_deriv_type;
    typedef typename stepper_base_type::time_type time_type;
    typedef typename stepper_base_type::algebra_type algebra_type;
    typedef typename stepper_base_type::operations_type operations_type;
    typedef typename stepper_base_type::resizer_type resizer_type;

    typedef explicit_generic_rk< StageCount , Order , State , Value , Deriv ,Time , Algebra , Operations , Resizer > stepper_type;

    typedef detail::generic_rk_algorithm< StageCount , Value , Algebra , Operations > rk_algorithm_type;

    typedef typename rk_algorithm_type::coef_a_type coef_a_type;
    typedef typename rk_algorithm_type::coef_b_type coef_b_type;
    typedef typename rk_algorithm_type::coef_c_type coef_c_type;

    static const size_t stage_count = StageCount;


private:

public:

    explicit_generic_rk( const coef_a_type &a , const coef_b_type &b , const coef_c_type &c ,
            const algebra_type &algebra = algebra_type() )
    : stepper_base_type( algebra ) , m_rk_algorithm( a , b , c )
    { }


    template< class System , class StateIn , class DerivIn , class StateOut >
    void do_step_impl( System system , const StateIn &in , const DerivIn &dxdt ,
            const time_type &t , StateOut &out , const time_type &dt )
    {
        typedef typename boost::unwrap_reference< System >::type unwrapped_system_type;
        unwrapped_system_type &sys = system;

        m_resizer.adjust_size( in , boost::bind( &stepper_type::template resize< StateIn > , boost::ref( *this ) , _1 ) );

        // actual calculation done in generic_rk.hpp
        m_rk_algorithm.do_step( stepper_base_type::m_algebra , sys , in , dxdt , t , out , dt , m_x_tmp.m_v , m_F );
    }

    template< class StateIn >
    bool resize( const StateIn &x )
    {
        bool resized( false );
        resized |= adjust_size_by_resizeability( m_x_tmp , x , typename wrapped_state_type::is_resizeable() );
        for( size_t i = 0 ; i < StageCount-1 ; ++i )
        {
            resized |= adjust_size_by_resizeability( m_F[i] , x , typename wrapped_deriv_type::is_resizeable() );
        }
        return resized;
    }

    template< class StateType >
    void adjust_size( const StateType &x )
    {
        resize( x );
        stepper_base_type::adjust_size( x );
    }

    //friend std::ostream& operator << <>( std::ostream &os , const explicit_generic_rk &rk );

private:

    rk_algorithm_type m_rk_algorithm;

    resizer_type m_resizer;

    wrapped_state_type m_x_tmp;
    wrapped_deriv_type m_F[StageCount-1];

};

}
}
}
#endif // BOOST_NUMERIC_ODEINT_STEPPER_EXPLICIT_GENERIC_RK_HPP_INCLUDED
