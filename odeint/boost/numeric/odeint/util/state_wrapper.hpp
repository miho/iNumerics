/*
 [auto_generated]
 boost/numeric/odeint/util/state_wrapper.hpp

 [begin_description]
 State wrapper for the state type in all stepper. The state wrappers are responsible for contruction,
 destruction, copying contruction, assignment and resizing.
 [end_description]

 Copyright 2009-2011 Karsten Ahnert
 Copyright 2009-2011 Mario Mulansky

 Distributed under the Boost Software License, Version 1.0.
 (See accompanying file LICENSE_1_0.txt or
 copy at http://www.boost.org/LICENSE_1_0.txt)
 */


#ifndef BOOST_NUMERIC_ODEINT_UTIL_STATE_WRAPPER_HPP_INCLUDED
#define BOOST_NUMERIC_ODEINT_UTIL_STATE_WRAPPER_HPP_INCLUDED


#include <boost/numeric/odeint/algebra/range_algebra.hpp>
#include <boost/type_traits/integral_constant.hpp>

#include <boost/numeric/odeint/util/is_resizeable.hpp>

namespace boost {
namespace numeric {
namespace odeint {

// same_size function
// standard implementation relies on boost.range
template< class State1 , class State2 >
bool same_size( const State1 &x1 , const State2 &x2 )
{
	return ( boost::size( x1 ) == boost::size( x2 ) );
}

// resize function
// standard implementation relies on boost.range and resize member function
template< class StateIn , class StateOut >
void resize( StateOut &x1 , const StateIn &x2 )
{
	x1.resize( boost::size( x2 ) );
}


template< class V , bool resizeable = is_resizeable< V >::value >
struct state_wrapper;

//two standard implementations, with and without resizing depending on is_resizeable< StateType >

template< class V >
struct state_wrapper< V , true > // with resizing
{
    typedef state_wrapper< V , true > state_wrapper_type;
    //typedef typename V::value_type value_type;
    typedef boost::true_type is_resizeable;

    V m_v;

    template< class StateIn >
    bool same_size( const StateIn &x ) const
    {
        return boost::numeric::odeint::same_size( m_v , x );
    }

    template< class StateIn >
    bool resize( const StateIn &x )
    {
        //standard resizing done like for std::vector
        if( !same_size( x ) )
        {
            boost::numeric::odeint::resize( m_v , x );
            return true;
        } else
            return false;
    }
};


template< class V >
struct state_wrapper< V , false > // without resizing
{
    typedef state_wrapper< V , false > state_wrapper_type;
    //typedef typename V::value_type value_type;
    typedef boost::false_type is_resizeable;

    V m_v;

    //no resize method
};

}
}
}



#endif // BOOST_NUMERIC_ODEINT_UTIL_STATE_WRAPPER_HPP_INCLUDED
