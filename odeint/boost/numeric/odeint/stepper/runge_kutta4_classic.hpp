/*
 [auto_generated]
 boost/numeric/odeint/stepper/runge_kutta4_classic.hpp

 [begin_description]
 Implementation for the classical Runge Kutta stepper.
 [end_description]

 Copyright 2009-2011 Karsten Ahnert
 Copyright 2009-2011 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_STEPPER_RUNGE_KUTTA4_CLASSIC_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_STEPPER_RUNGE_KUTTA4_CLASSIC_HPP_INCLUDED


#include <boost/ref.hpp>

#include <boost/numeric/odeint/stepper/base/explicit_stepper_base.hpp>
#include <boost/numeric/odeint/algebra/range_algebra.hpp>
#include <boost/numeric/odeint/algebra/default_operations.hpp>
#include <boost/numeric/odeint/stepper/detail/macros.hpp>

#include <boost/numeric/odeint/util/state_wrapper.hpp>
#include <boost/numeric/odeint/util/resizer.hpp>

namespace boost {
namespace numeric {
namespace odeint {


template<
class State ,
class Value = double ,
class Deriv = State ,
class Time = Value ,
class Algebra = range_algebra ,
class Operations = default_operations ,
class Resizer = initially_resizer
>
class runge_kutta4_classic
: public explicit_stepper_base<
  runge_kutta4_classic< State , Value , Deriv , Time , Algebra , Operations , Resizer > ,
  4 , State , Value , Deriv , Time , Algebra , Operations , Resizer >
{

public :


    BOOST_ODEINT_EXPLICIT_STEPPERS_TYPEDEFS( runge_kutta4_classic , 4 );

    typedef runge_kutta4_classic< State , Value , Deriv , Time , Algebra , Operations , Resizer > stepper_type;

    runge_kutta4_classic( const algebra_type &algebra = algebra_type() ) : stepper_base_type( algebra )
    { }


    template< class System , class StateIn , class DerivIn , class StateOut >
    void do_step_impl( System system , const StateIn &in , const DerivIn &dxdt , const time_type &t , StateOut &out , const time_type &dt )
    {
        // ToDo : check if size of in,dxdt,out are equal?

        static const value_type val1 = static_cast< value_type >( 1.0 );

        m_resizer.adjust_size( in , boost::bind( &stepper_type::template resize< StateIn > , boost::ref( *this ) , _1 ) );

        typename boost::unwrap_reference< System >::type &sys = system;

        const time_type dh = static_cast< value_type >( 0.5 ) * dt;
        const time_type th = t + dh;

        // dt * dxdt = k1
        // m_x_tmp = x + dh*dxdt
        stepper_base_type::m_algebra.for_each3( m_x_tmp.m_v , in , dxdt ,
                typename operations_type::template scale_sum2< value_type , time_type >( val1 , dh ) );


        // dt * m_dxt = k2
        sys( m_x_tmp.m_v , m_dxt.m_v , th );

        // m_x_tmp = x + dh*m_dxt
        stepper_base_type::m_algebra.for_each3( m_x_tmp.m_v , in , m_dxt.m_v ,
                typename operations_type::template scale_sum2< value_type , time_type >( val1 , dh ) );


        // dt * m_dxm = k3
        sys( m_x_tmp.m_v , m_dxm.m_v , th );
        //m_x_tmp = x + dt*m_dxm
        stepper_base_type::m_algebra.for_each3( m_x_tmp.m_v , in , m_dxm.m_v ,
                typename operations_type::template scale_sum2< value_type , time_type >( val1 , dt ) );


        // dt * m_dxh = k4
        sys( m_x_tmp.m_v , m_dxh.m_v , t + dt );
        //x += dt/6 * ( m_dxdt + m_dxt + val2*m_dxm )
        time_type dt6 = dt / static_cast< value_type >( 6.0 );
        time_type dt3 = dt / static_cast< value_type >( 3.0 );
        stepper_base_type::m_algebra.for_each6( out , in , dxdt , m_dxt.m_v , m_dxm.m_v , m_dxh.m_v ,
                typename operations_type::template scale_sum5< value_type , time_type , time_type , time_type , time_type >( 1.0 , dt6 , dt3 , dt3 , dt6 ) );
    }

    template< class StateIn >
    bool resize( const StateIn &x )
    {
        bool resized = false;
        resized |= adjust_size_by_resizeability( m_x_tmp , x , typename wrapped_state_type::is_resizeable() );
        resized |= adjust_size_by_resizeability( m_dxm , x , typename wrapped_deriv_type::is_resizeable() );
        resized |= adjust_size_by_resizeability( m_dxt , x , typename wrapped_deriv_type::is_resizeable() );
        resized |= adjust_size_by_resizeability( m_dxh , x , typename wrapped_deriv_type::is_resizeable() );
        return resized;
    }

    template< class StateType >
    void adjust_size( const StateType &x )
    {
        resize( x );
        stepper_base_type::adjust_size( x );
    }


private:

    resizer_type m_resizer;

    wrapped_deriv_type m_dxt;
    wrapped_deriv_type m_dxm;
    wrapped_deriv_type m_dxh;
    wrapped_state_type m_x_tmp;

};




} // odeint
} // numeric
} // boost


#endif // BOOST_NUMERIC_ODEINT_STEPPER_RUNGE_KUTTA4_CLASSIC_HPP_INCLUDED
