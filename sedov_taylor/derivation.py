def main():

    import sympy
    sympy.init_printing()
    from sympy.utilities.codegen import codegen

    x1, x2, x3 = sympy.symbols('x1 x2 x3')
    y1, y2, y3 = sympy.symbols('y1 y2 y3')
    d1, d2, d3 = sympy.symbols('d1 d2 d3')
    A, B = sympy.symbols('A B')
    x = sympy.Symbol('x')
    y = sympy.Function('y')
    ansatz = y2+A*(x-x2)+B*(x-x2)**2/2
    eqns = [yv-ansatz.subs(x,xv)
            for xv, yv
            in zip([x1,x3],[y1,y3])]
    sol = sympy.solve(eqns, [A, B])

    D = sympy.Function('D')
    dDdx = A.subs(sol).subs({y1:d1,y2:d2,y3:d3})
    Delta1 = sympy.Symbol('Delta_1')
    Delta2 = sympy.Symbol('Delta_2')
    rhs = (x**2*D(x)*y(x).diff(x)).diff(x)/x**2
    temp = rhs
    temp = temp.subs(y(x).diff(x, 2),B)
    temp = temp.subs(y(x).diff(x), A)
    temp = temp.subs(sol)
    temp = temp.subs(D(x).diff(x), dDdx)
    temp = temp.subs(D(x), d2)
    temp = temp.subs(x, x2)
    temp = [temp.diff(yv) for yv in [y1, y2, y3]]
    temp = sympy.Matrix(temp)
    temp = temp.subs(x1,x2-Delta1)
    temp = temp.subs(x3,x2+Delta2)
    temp.simplify()
    temp = temp.subs(Delta1,x2-x1)
    temp = temp.subs(Delta2,x3-x2)
    temp.simplify()
    [(c_name, c_code), (h_name, h_code)] = \
        codegen((('calc_previous',temp[0]),
                 ('calc_current',temp[1]),
                 ('calc_next',temp[2])),
                "C99",
                "derivative_matrix",
                project='fick')
    with open(c_name,'w') as f:
        f.write(c_code)
    with open(h_name,'w') as f:
        f.write(h_code)

if __name__ == '__main__':

    main()
