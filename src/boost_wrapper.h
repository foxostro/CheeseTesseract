#ifndef BOOST_WRAPPER_H
#define BOOST_WRAPPER_H

#if defined(_MSC_VER)
#	pragma once
#	pragma warning(push)
#	pragma warning(disable: 4512)
#endif

#include <boost/any.hpp>
#include <boost/variant.hpp>
#include <boost/tokenizer.hpp>
#include <boost/function.hpp>
#include <boost/static_assert.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/tuple/tuple.hpp>
using namespace boost;

/* Boost Lambda Library */
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
using namespace boost::lambda;

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
