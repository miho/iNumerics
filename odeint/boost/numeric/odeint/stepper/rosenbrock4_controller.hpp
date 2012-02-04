/*
 [auto_generated]
 boost/numeric/odeint/stepper/rosenbrock4_controller.hpp

 [begin_description]
 Controller for the Rosenbrock4 method.
 [end_description]

 Copyright 2009-2011 Karsten Ahnert
 Copyright 2009-2011 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_STEPPER_ROSENBROCK4_CONTROLLER_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_STEPPER_ROSENBROCK4_CONTROLLER_HPP_INCLUDED


#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <boost/numeric/odeint/stepper/stepper_categories.hpp>

#include <boost/numeric/odeint/util/copy.hpp>

#include <boost/numeric/odeint/stepper/rosenbrock4.hpp>

#include <iostream>


namespace boost {
namespace numeric {
namespace odeint {

template< class Stepper >
class rosenbrock4_controller
{
private:


public:

    typedef Stepper stepper_type;
    typedef typename stepper_type::value_type value_type;
    typedef typename stepper_type::state_type state_type;
    typedef typename stepper_type::wrapped_state_type wrapped_state_type;
    typedef typename stepper_type::time_type time_type;
    typedef typename stepper_type::deriv_type deriv_type;
    typedef typename stepper_type::wrapped_deriv_type wrapped_deriv_type;
    typedef typename stepper_type::resizer_type resizer_type;
    typedef controlled_stepper_tag stepper_category;

    typedef rosenbrock4_controller< Stepper > controller_type;


    rosenbrock4_controller( value_type atol = 1.0e-6 , value_type rtol = 1.0e-6 , const stepper_type &stepper = stepper_type() )
    : m_atol( atol ) , m_rtol( rtol ) ,
      m_first_step( true ) , m_err_old( 0.0 ) , m_dt_old( 0.0 ) ,
      m_last_rejected( false )
    { }


    value_type error( const state_type &x , const state_type &xold , const state_type &xerr )
    {
        const size_t n = x.size();
        value_type err = 0.0 , sk = 0.0;
        for( size_t i=0 ; i<n ; ++i )
        {
            sk = m_atol + m_rtol * std::max( std::abs( xold[i] ) , std::abs( x[i] ) );
            err += xerr[i] * xerr[i] / sk / sk;
        }
        return sqrt( err / value_type( n ) );
    }

    value_type last_error( void ) const
    {
        return m_err_old;
    }




    /*
     * ToDo : xout in size_adjuster
     * Check
     */
    template< class System >
    boost::numeric::odeint::controlled_step_result
    try_step( System sys , state_type &x , value_type &t , value_type &dt )
    {
        m_xnew_resizer.adjust_size( x , boost::bind( &controller_type::template resize_m_xnew< state_type > , boost::ref( *this ) , _1 ) );
        boost::numeric::odeint::controlled_step_result res = try_step( sys , x , t , m_xnew.m_v , dt );
        if( res == success )
        {
            boost::numeric::odeint::copy( m_xnew.m_v , x );
        }
        return res;
    }


    /*
     * ToDo : xerr in size_adjuster und adjust size
     */
    template< class System >
    boost::numeric::odeint::controlled_step_result
    try_step( System sys , const state_type &x , value_type &t , state_type &xout , value_type &dt )
    {
        static const value_type safe = 0.9 , fac1 = 5.0 , fac2 = 1.0 / 6.0;

        m_xerr_resizer.adjust_size( x , boost::bind( &controller_type::template resize_m_xerr< state_type > , boost::ref( *this ) , _1 ) );

        m_stepper.do_step( sys , x , t , xout , dt , m_xerr.m_v );
        value_type err = error( xout , x , m_xerr.m_v );

        value_type fac = std::max( fac2 ,std::min( fac1 , std::pow( err , 0.25 ) / safe ) );
        value_type dt_new = dt / fac;
        if ( err <= 1.0 )
        {
            if( m_first_step )
            {
                m_first_step = false;
            }
            else
            {
                value_type fac_pred = ( m_dt_old / dt ) * pow( err * err / m_err_old , 0.25 ) / safe;
                fac_pred = std::max( fac2 , std::min( fac1 , fac_pred ) );
                fac = std::max( fac , fac_pred );
                dt_new = dt / fac;
            }

            m_dt_old = dt;
            m_err_old = std::max( 0.01 , err );
            if( m_last_rejected )
                dt_new = ( dt >= 0.0 ? std::min( dt_new , dt ) : std::max( dt_new , dt ) );
            t += dt;
            dt = dt_new;
            m_last_rejected = false;
            return success;
        }
        else
        {
            dt = dt_new;
            m_last_rejected = true;
            return fail;
        }
    }


    template< class StateType >
    void adjust_size( const StateType &x )
    {
        resize_m_xerr( x );
        resize_m_xnew( x );
    }


    template< class StateIn >
    bool resize_m_xerr( const StateIn &x )
    {
        return adjust_size_by_resizeability( m_xerr , x , typename wrapped_state_type::is_resizeable() );
    }

    template< class StateIn >
    bool resize_m_xnew( const StateIn &x )
    {
        return adjust_size_by_resizeability( m_xnew , x , typename wrapped_state_type::is_resizeable() );
    }



    stepper_type& stepper( void )
    {
        return m_stepper;
    }

    const stepper_type& stepper( void ) const
    {
        return m_stepper;
    }




private:

    stepper_type m_stepper;
    resizer_type m_xerr_resizer;
    resizer_type m_xnew_resizer;
    wrapped_state_type m_xerr;
    wrapped_state_type m_xnew;
    value_type m_atol , m_rtol;
    bool m_first_step;
    value_type m_err_old , m_dt_old;
    bool m_last_rejected;
};






} // namespace odeint
} // namespace numeric
} // namespace boost


#endif // BOOST_NUMERIC_ODEINT_STEPPER_ROSENBROCK4_CONTROLLER_HPP_INCLUDED
