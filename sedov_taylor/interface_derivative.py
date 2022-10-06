def main():

    import sympy
    sympy.init_printing()
    from sympy.utilities.codegen import codegen

    r_l, r_m, r_r = sympy.symbols('r_l r_m r_r')
    y_l, y_r = sympy.symbols('y_l y_r')
    A, B = sympy.symbols('A B')
    r = sympy.Symbol('r')
    ansatz = A*(r-r_m)+B
    jac = r**2;
    eqn1 = y_l - sympy.integrate(ansatz*jac,(r,r_l,r_m))/sympy.integrate(jac,(r,r_l,r_m))
    eqn2 = y_r - sympy.integrate(ansatz*jac,(r,r_m,r_r))/sympy.integrate(jac,(r,r_m,r_r))
    sol = sympy.solve([eqn1, eqn2], [A, B])
    slope = sol[A].simplify()

    # Sanity check
    sympy.pprint(slope.subs(y_r,y_l))

    # Write c code
    [(c_name, c_code), (h_name, h_code)] = \
        codegen((('calc_previous',slope.diff(y_l).simplify()),
                 ('calc_next',slope.diff(y_r).simplify())),
                'C99',
                'interface_derivative',
                project='fick')
    with open(c_name,'w') as f:
        f.write(c_code)
    with open(h_name,'w') as f:
        f.write(h_code)                

if __name__ == '__main__':

    main()
