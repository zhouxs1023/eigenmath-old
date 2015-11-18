; December 7, 2001

; See the paper "Solutions of the Robbins Problem" by William McCune.

; Robbins equation (7)

(setq E7 (eq (n (sum (n (sum (n x) y)) (n (sum x y)))) y))

; From this derive the following:

; n(n(3x)+x)+2x = 2x

; STEP 10 ---------------------------------------------------------------------

; with (7), let x be n(x)+y and y be n(x+y)

(setq E10 (subst Y y E7))

(setq E10 (subst (sum (n x) y) x E10))

(setq E10 (eval (subst (n (sum x y)) Y E10)))

; simplify with (7)

(setq E10 (eval (subst (caddr E7) (cadr E7) E10)))

; n(n(n(x+y)+n(x)+y)+y) = n(x+y) ?

(setq tmp (eq (n (sum (n (sum (n (sum x y)) (n x) y)) y)) (n (sum x y))))

(equal E10 tmp)

; STEP 11 ---------------------------------------------------------------------

; with (7), let y be n(n(x)+y) and x be x+y

(setq E11 (subst X x E7))

(setq E11 (subst (n (sum (n x) y)) y E11))

(setq E11 (eval (subst (sum x y) X E11)))

; simplify with (7)

(setq E11 (eval (subst (caddr E7) (cadr E7) E11)))

; n(n(n(n(x)+y)+x+y)+y) = n(n(x)+y) ?

(setq tmp (eq (n (sum (n (sum (n (sum (n x) y)) x y)) y)) (n (sum (n x) y))))

(equal E11 tmp)

; STEP 29 ---------------------------------------------------------------------

; with (7), let x be n(n(x)+y)+x+y and y be y

(setq E29 (eval (subst (sum (n (sum (n x) y)) x y) x E7)))

; simplify with (11)

(setq E29 (eval (subst (caddr E11) (cadr E11) E29)))

; n(n(n(n(x)+y)+x+2y)+n(n(x)+y)) = y ?

(setq tmp (eq (n (sum (n (sum (n (sum (n x) y)) x y y)) (n (sum (n x) y)))) y))

(equal E29 tmp)

; STEP 54 ---------------------------------------------------------------------

; with (7), let x be n(n(n(x)+y)+x+2y)+n(n(x)+y) and y be z

(setq E54 (subst z y E7))

(setq tmp (sum (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y (n x)))))

(setq E54 (eval (subst tmp x E54)))

; simplify with (29)

(setq E54 (eval (subst (caddr E29) (cadr E29) E54)))

; n(n(n(n(n(x)+y)+x+2y)+n(n(x)+y)+z)+n(y+z)) = z ?

(setq tmp (eq (n (sum (n (sum y z)) (n (sum z (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y (n x))))))) z))

(equal E54 tmp)

; STEP 217 --------------------------------------------------------------------

; with (7), let x be n(n(n(x)+y)+x+2y)+n(n(x)+y)+z and y be n(y+z)

(setq E217 (subst Y y E7))

(setq tmp (sum z (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y (n x)))))

(setq E217 (subst tmp x E217))

(setq E217 (eval (subst (n (sum y z)) Y E217)))

; simplify with E54

(setq E217 (eval (subst (caddr E54) (cadr E54) E217)))

; n(n(n(n(n(x)+y)+x+2y)+n(n(x)+y)+n(y+z)+z)+z) = n(y+z) ?

(setq tmp (eq (n (sum z (n (sum z (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y z)) (n (sum y (n x))))))) (n (sum y z))))

(equal E217 tmp)

; STEP 674 --------------------------------------------------------------------

; with (7), let y be u and x be n(n(n(n(x)+y)+x+2y)+n(n(x)+y)+n(y+z)+z)+z

(setq E674 (subst u y E7))

(setq tmp (sum z (n (sum z (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y z)) (n (sum y (n x)))))))

(setq E674 (eval (subst tmp x E674)))

; simplify with E217

(setq E674 (eval (subst (caddr E217) (cadr E217) E674)))

; n(n(n(n(n(n(x)+y)+x+2y)+n(n(x)+y)+n(y+z)+z)+z+u)+n(n(y+z)+u)) = u ?

(setq tmp (eq (n (sum (n (sum u z (n (sum z (n (sum x (n (sum y (n x))) (product 2 y))) (n (sum y z)) (n (sum y (n x))))))) (n (sum u (n (sum y z)))))) u))

(equal E674 tmp)

; STEP 6736 -------------------------------------------------------------------

; with (10), let x be 3v and y be n(n(3v)+v)+2v

(setq E10p (subst (product 3 v) x E10))

(setq tmp (sum (n (sum v (n (product 3 v)))) (product 2 v)))

(setq E10p (eval (subst tmp y E10p)))

; with (674), let x be 3v, y be v, z be 2v, and u be n(n(3v)+v)

(setq E674p (subst (product 3 v) x E674))

(setq E674p (subst v y E674p))

(setq E674p (subst (product 2 v) z E674p))

(setq tmp (n (sum v (n (product 3 v)))))

(setq E674p (eval (subst tmp u E674p)))

; simplify with (10')

(setq E6736 (eval (subst (caddr E10p) (cadr E10p) E674p)))

; replace v with x

(setq E6736 (eval (subst x v E6736)))

; n(n(n(n(3x)+x)+n(3x))+n(n(n(3x)+x)+5x)) = n(n(3x)+x) ?

(setq tmp (eq (n (sum (n (sum (n (product 3 x)) (n (sum x (n (product 3 x)))))) (n (sum (n (sum x (n (product 3 x)))) (product 5 x))))) (n (sum x (n (product 3 x))))))

(equal E6736 tmp)

; STEP 8855 -------------------------------------------------------------------

; with (7), let x be n(n(3x)+x)+n(3x) and y be n(n(n(3x)+x)+5x)

(setq E8855 (subst Y y E7))

(setq tmp (sum (n (product 3 x)) (n (sum x (n (product 3 x))))))

(setq E8855 (subst tmp x E8855))

(setq tmp (n (sum (n (sum x (n (product 3 x)))) (product 5 x))))

(setq E8855 (eval (subst tmp Y E8855)))

; simplify with (6736)

(setq E8855 (eval (subst (caddr E6736) (cadr E6736) E8855)))

; n(n(n(3x)+x)+n(n(n(3x)+x)+n(3x)+n(n(n(3x)+x)+5x))) = n(n(n(3x)+x)+5x) ?

(setq tmp (eq (n (sum (n (sum x (n (product 3 x)))) (n (sum (n (product 3 x)) (n (sum x (n (product 3 x)))) (n (sum (n (sum x (n (product 3 x)))) (product 5 x))))))) (n (sum (n (sum x (n (product 3 x)))) (product 5 x)))))

(equal E8855 tmp)

; with (54), let x be 3x, z be n(3x), and y be x

(setq tmp (subst (product 3 x) x E54))

(setq tmp (subst (n (product 3 x)) z tmp))

(setq tmp (eval (subst x y tmp)))

; simplify

(setq E8855 (subst (caddr tmp) (cadr tmp) E8855))

; flip

(setq E8855 (eq (caddr E8855) (cadr E8855)))

; n(n(n(3x)+x)+5x) = n(3x) ?

(setq tmp (eq (n (sum (n (sum x (n (product 3 x)))) (product 5 x))) (n (product 3 x))))

(equal E8855 tmp)

; STEP 8865 -------------------------------------------------------------------

; with (7), let y be n(n(3x)+x)+2x and x be 3x

(setq E8865 (subst X x E7))

(setq tmp (sum (n (sum x (n (product 3 x)))) (product 2 x)))

(setq E8865 (subst tmp y E8865))

(setq E8865 (eval (subst (product 3 x) X E8865)))

; simplify with (8855)

(setq E8865 (subst (caddr E8855) (cadr E8855) E8865))

(setq E8865 (eval E8865))

; n(n(n(n(3x)+x)+n(3x)+2x)+n(3x)) = n(n(3x)+x)+2x ?

(setq tmp (eq (n (sum (n (product 3 x)) (n (sum (n (product 3 x)) (n (sum x (n (product 3 x)))) (product 2 x))))) (sum (n (sum x (n (product 3 x)))) (product 2 x))))

(equal E8865 tmp)

; STEP 8866 -------------------------------------------------------------------

; with (7), let x be n(n(3x)+x)+4x and y be x

(setq tmp (sum (n (sum x (n (product 3 x)))) (product 4 x)))

(setq A2 (subst tmp x E7))

(setq A2 (eval (subst x y A2)))

; simplify with (8855)

(setq A2 (eval (subst (caddr E8855) (cadr E8855) A2)))

; with (11), let x be 3x and y be x

(setq A3 (subst (product 3 x) x E11))

(setq A3 (eval (subst x y A3)))

; simplify A2 with A3

(setq E8866 (eval (subst (caddr A3) (cadr A3) A2)))

; n(n(n(3x)+x)+n(3x)) = x ?

(setq tmp (eq (n (sum (n (product 3 x)) (n (sum x (n (product 3 x)))))) x))

(equal E8866 tmp)

; STEP 8870 -------------------------------------------------------------------

; with (7), let x =  n(n(3x)+x)+n(3x)

(setq tmp (sum (n (product 3 x)) (n (sum x (n (product 3 x))))))

(setq E8870 (eval (subst tmp x E7)))

; simplify with (8866)

(setq E8870 (eval (subst (caddr E8866) (cadr E8866) E8870)))

; n(n(n(n(3x)+x)+n(3x)+y)+n(x+y)) = y ?

(setq tmp (eq (n (sum (n (sum x y)) (n (sum y (n (product 3 x)) (n (sum x (n (product 3 x)))))))) y))

(equal E8870 tmp)

; STEP 8871 -------------------------------------------------------------------

; with (8870), let y be 2x

(setq tmp (eval (subst (product 2 x) y E8870)))

; use to simplify (8865)

(setq E8871 (eval (subst (caddr tmp) (cadr tmp) E8865)))

; flip

(setq E8871 (eq (caddr E8871) (cadr E8871)))

; n(n(3x)+x)+2x = 2x ?

(setq tmp (eq (sum (n (sum x (n (product 3 x)))) (product 2 x)) (product 2 x)))

(equal E8871 tmp)
