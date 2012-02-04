/*
 [auto_generated]
 boost/numeric/odeint/stepper/base/explicit_stepper_and_error_stepper_fsal_base.hpp

 [begin_description]
 Base class for all explicit first-same-as-last Runge Kutta steppers.
 [end_description]

 Copyright 2009-2011 Karsten Ahnert
 Copyright 2009-2011 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_STEPPER_BASE_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_STEPPER_BASE_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED


#include <boost/ref.hpp>
#include <boost/bind.hpp>

#include <boost/numeric/odeint/util/state_wrapper.hpp>
#include <boost/numeric/odeint/util/resizer.hpp>


#include <boost/numeric/odeint/stepper/stepper_categories.hpp>

namespace boost {
namespace numeric {
namespace odeint {

/** ToDo: check correct m_first_call implementation */

/*
 * base class for explicit stepper and error steppers with the fsal property
 * models the stepper AND the error stepper fsal concept
 */
template<
class Stepper ,
unsigned short Order ,
unsigned short StepperOrder ,
unsigned short ErrorOrder ,
class State ,
class Value ,
class Deriv ,
class Time ,
class Algebra ,
class Operations ,
class Resizer
>
class explicit_stepper_and_error_stepper_fsal_base
{
public:

    typedef State state_type;
    typedef Value value_type;
    typedef Deriv deriv_type;
    typedef Time time_type;
    typedef Algebra algebra_type;
    typedef Operations operations_type;
    typedef Resizer resizer_type;
    typedef Stepper stepper_type;
    typedef explicit_error_stepper_fsal_tag stepper_category;
    typedef state_wrapper< state_type > wrapped_state_type;
    typedef state_wrapper< deriv_type > wrapped_deriv_type;

    typedef explicit_stepper_and_error_stepper_fsal_base< Stepper , Order , StepperOrder , ErrorOrder ,
            State , Value , Deriv , Time , Algebra , Operations , Resizer > internal_stepper_base_type;

    typedef unsigned short order_type;
    static const order_type order_value = Order;
    static const order_type stepper_order_value = StepperOrder;
    static const order_type error_order_value = ErrorOrder;


    explicit_stepper_and_error_stepper_fsal_base( const algebra_type &algebra = algebra_type() )
    : m_first_call( true ) , m_algebra( algebra )
    { }

    order_type order( void ) const
    {
        return order_value;
    }

    order_type stepper_order( void ) const
    {
        return stepper_order_value;
    }

    order_type error_order( void ) const
    {
        return error_order_value;
    }


    /*
     * version 1 : do_step( sys , x , t , dt )
     *
     * the two overloads are needed in order to solve the forwarding problem
     */
    template< class System , class StateInOut >
    void do_step( System system , StateInOut &x , const time_type &t , const time_type &dt )
    {
        do_step_v1( system , x , t , dt );
    }

    template< class System , class StateInOut >
    void do_step( System system , const StateInOut &x , const time_type &t , const time_type &dt )
    {
        do_step_v1( system , x , t , dt );
    }


    /*
     * version 2 : do_step( sys , x , dxdt , t , dt )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateInOut , class DerivInOut >
    void do_step( System system , StateInOut &x , DerivInOut &dxdt , const time_type &t , const time_type &dt )
    {
        m_first_call = true;
        this->stepper().do_step_impl( system , x , dxdt , t , x , dxdt , dt );
    }


    /*
     * version 3 : do_step( sys , in , t , out , dt )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateIn , class StateOut >
    void do_step( System system , const StateIn &in , const time_type &t , StateOut &out , const time_type &dt )
    {
        if( m_resizer.adjust_size( in , boost::bind( &internal_stepper_base_type::template resize< StateIn > , boost::ref( *this ) , _1 ) ) || m_first_call )
        {
            typename boost::unwrap_reference< System >::type &sys = system;
            sys( in , m_dxdt.m_v ,t );
            m_first_call = false;
        }
        this->stepper().do_step_impl( system , in , m_dxdt.m_v , t , out , m_dxdt.m_v , dt );
    }


    /*
     * version 4 : do_step( sys , in , dxdt_in , t , out , dxdt_out , dt )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut >
    void do_step( System system , const StateIn &in , const DerivIn &dxdt_in , const time_type &t ,
            StateOut &out , DerivOut &dxdt_out , const time_type &dt )
    {
        m_first_call = true;
        this->stepper().do_step_impl( system , in , dxdt_in , t , out , dxdt_out , dt );
    }





    /*
     * version 5 : do_step( sys , x , t , dt , xerr )
     *
     * the two overloads are needed in order to solve the forwarding problem
     */
    template< class System , class StateInOut , class Err >
    void do_step( System system , StateInOut &x , const time_type &t , const time_type &dt , Err &xerr )
    {
        do_step_v5( system , x , t , dt , xerr );
    }

    template< class System , class StateInOut , class Err >
    void do_step( System system , const StateInOut &x , const time_type &t , const time_type &dt , Err &xerr )
    {
        do_step_v5( system , x , t , dt , xerr );
    }


    /*
     * version 6 : do_step( sys , x , dxdt , t , dt , xerr )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateInOut , class DerivInOut , class Err >
    void do_step( System system , StateInOut &x , DerivInOut &dxdt , const time_type &t , const time_type &dt , Err &xerr )
    {
        m_first_call = true;
        this->stepper().do_step_impl( system , x , dxdt , t , x , dxdt , dt , xerr );
    }


    /*
     * version 7 : do_step( sys , in , t , out , dt , xerr )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateIn , class StateOut , class Err >
    void do_step( System system , const StateIn &in , const time_type &t , StateOut &out , const time_type &dt , Err &xerr )
    {
        if( m_resizer.adjust_size( in , boost::bind( &internal_stepper_base_type::template resize< StateIn > , boost::ref( *this ) , _1 ) ) || m_first_call )
        {
            typename boost::unwrap_reference< System >::type &sys = system;
            sys( in , m_dxdt.m_v ,t );
            m_first_call = false;
        }
        this->stepper().do_step_impl( system , in , m_dxdt.m_v , t , out , m_dxdt.m_v , dt , xerr );
    }


    /*
     * version 8 : do_step( sys , in , dxdt_in , t , out , dxdt_out , dt )
     *
     * this version does not solve the forwarding problem, boost.range can not be used
     */
    template< class System , class StateIn , class DerivIn , class StateOut , class DerivOut , class Err >
    void do_step( System system , const StateIn &in , const DerivIn &dxdt_in , const time_type &t ,
            StateOut &out , DerivOut &dxdt_out , const time_type &dt , Err &xerr )
    {
        m_first_call = true;
        this->stepper().do_step_impl( system , in , dxdt_in , t , out , dxdt_out , dt , xerr );
    }


    template< class StateIn >
    bool resize( const StateIn &x )
    {
        return adjust_size_by_resizeability( m_dxdt , x , typename wrapped_deriv_type::is_resizeable() );
    }

    template< class StateIn >
    void adjust_size( const StateIn &x )
    {
        resize( x );
    }


    algebra_type& algebra()
    {   return m_algebra; }

    const algebra_type& algebra() const
    {   return m_algebra; }

private:

    template< class System , class StateInOut >
    void do_step_v1( System system , StateInOut &x , const time_type &t , const time_type &dt )
    {
        if( m_resizer.adjust_size( x , boost::bind( &internal_stepper_base_type::template resize< StateInOut > , boost::ref( *this ) , _1 ) ) || m_first_call )
        {
            typename boost::unwrap_reference< System >::type &sys = system;
            sys( x , m_dxdt.m_v ,t );
            m_first_call = false;
        }
        this->stepper().do_step_impl( system , x , m_dxdt.m_v , t , x , m_dxdt.m_v , dt );
    }

    template< class System , class StateInOut , class Err >
    void do_step_v5( System system , StateInOut &x , const time_type &t , const time_type &dt , Err &xerr )
    {
        if( m_resizer.adjust_size( x , boost::bind( &internal_stepper_base_type::template resize< StateInOut > , boost::ref( *this ) , _1 ) ) || m_first_call )
        {
            typename boost::unwrap_reference< System >::type &sys = system;
            sys( x , m_dxdt.m_v ,t );
            m_first_call = false;
        }
        this->stepper().do_step_impl( system , x , m_dxdt.m_v , t , x , m_dxdt.m_v , dt , xerr );
    }

    stepper_type& stepper( void )
    {
        return *static_cast< stepper_type* >( this );
    }

    const stepper_type& stepper( void ) const
    {
        return *static_cast< const stepper_type* >( this );
    }


    resizer_type m_resizer;
    bool m_first_call;

protected:
    algebra_type m_algebra;
    wrapped_deriv_type m_dxdt;
};


} // odeint
} // numeric
} // boost

#endif // BOOST_NUMERIC_ODEINT_STEPPER_BASE_EXPLICIT_STEPPER_AND_ERROR_STEPPER_FSAL_BASE_HPP_INCLUDED
