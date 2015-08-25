#ifndef UTIL_EDGES_H
#define UTIL_EDGES_H

/* EDGES
 * `edges` defines two useful macros. They are used to react only to
 * a positive edge and negative of any Boolean expression.
 * The usage is simple: place posedge( ) around an expression to only return
 * true once when the expression changes from false to true. Likewise, negedge
 * is the opposite.
 *
 * There is one important caveat when using edges: Any variables used must be
 * `static` or have a lifetime of the entire program. For example, function local
 * variables or class variables cannot be tested againt unless they are
 * declared `static`.
 */

namespace edges
{

using boolFunctionPtr = bool(*)(void);
bool posedge_impl(boolFunctionPtr function);
bool negedge_impl(boolFunctionPtr function);

}

#define posedge( expression )\
    edges::posedge_impl( [&](){return (expression);} )

#define negedge( expression ) \
    edges::negedge_impl( [&](){return (expression);} )

#endif
