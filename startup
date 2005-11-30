; lisp reads this file on startup

(define minus (product -1 arg))
(define sqrt (power arg 1/2))
(define exp (power e arg))
(define complex (sum arg1 (product i arg2)))
(define complex-conjugate (eval (subst (product -1 i) i arg)))
(define magnitude2 (product arg (complex-conjugate arg)))
(define magnitude (sqrt (magnitude2 arg)))
(define zerop (equal arg 0))

(define contract12 (contract 1 2 arg))
(define contract13 (contract 1 3 arg))
(define contract14 (contract 1 4 arg))
(define contract15 (contract 1 5 arg))
(define contract16 (contract 1 6 arg))
(define contract23 (contract 2 3 arg))
(define contract24 (contract 2 4 arg))
(define contract25 (contract 2 5 arg))
(define contract26 (contract 2 6 arg))
(define contract34 (contract 3 4 arg))
(define contract35 (contract 3 5 arg))
(define contract36 (contract 3 6 arg))
(define contract45 (contract 4 5 arg))
(define contract46 (contract 4 6 arg))
(define contract56 (contract 5 6 arg))

(define transpose12 (transpose 1 2 arg))
(define transpose13 (transpose 1 3 arg))
(define transpose14 (transpose 1 4 arg))
(define transpose15 (transpose 1 5 arg))
(define transpose16 (transpose 1 6 arg))
(define transpose23 (transpose 2 3 arg))
(define transpose24 (transpose 2 4 arg))
(define transpose25 (transpose 2 5 arg))
(define transpose26 (transpose 2 6 arg))
(define transpose34 (transpose 3 4 arg))
(define transpose35 (transpose 3 5 arg))
(define transpose36 (transpose 3 6 arg))
(define transpose45 (transpose 4 5 arg))
(define transpose46 (transpose 4 6 arg))
(define transpose56 (transpose 5 6 arg))

(define caar (car (car arg)))
(define cadr (car (cdr arg)))
(define cdar (cdr (car arg)))
(define cddr (cdr (cdr arg)))

(define caaar (car (car (car arg))))
(define caadr (car (car (cdr arg))))
(define cadar (car (cdr (car arg))))
(define caddr (car (cdr (cdr arg))))
(define cdaar (cdr (car (car arg))))
(define cdadr (cdr (car (cdr arg))))
(define cddar (cdr (cdr (car arg))))
(define cdddr (cdr (cdr (cdr arg))))

(define caaaar (car (car (car (car arg)))))
(define caaadr (car (car (car (cdr arg)))))
(define caadar (car (car (cdr (car arg)))))
(define caaddr (car (car (cdr (cdr arg)))))
(define cadaar (car (cdr (car (car arg)))))
(define cadadr (car (cdr (car (cdr arg)))))
(define caddar (car (cdr (cdr (car arg)))))
(define cadddr (car (cdr (cdr (cdr arg)))))
(define cdaaar (cdr (car (car (car arg)))))
(define cdaadr (cdr (car (car (cdr arg)))))
(define cdadar (cdr (car (cdr (car arg)))))
(define cdaddr (cdr (car (cdr (cdr arg)))))
(define cddaar (cdr (cdr (car (car arg)))))
(define cddadr (cdr (cdr (car (cdr arg)))))
(define cdddar (cdr (cdr (cdr (car arg)))))
(define cddddr (cdr (cdr (cdr (cdr arg)))))

(define COS (sum
  (product 1/2 (exp (product i arg)))
  (product 1/2 (exp (product -1 i arg)))
))

(define SIN (sum
  (product -1/2 i (exp (product i arg)))
  (product 1/2 i (exp (product -1 i arg)))
))

(define COSH (sum
  (product 1/2 (exp arg))
  (product 1/2 (exp (product -1 arg)))
))

(define SINH (sum
  (product 1/2 (exp arg))
  (product -1/2 (exp (product -1 arg)))
))

(define adjunct (cond
  (arg5 (adjunct4 arg1 arg2 arg3 arg4 arg5))
  (arg4 (adjunct3 arg1 arg2 arg3 arg4))
  (arg3 (adjunct2 arg1 arg2 arg3))
  (t nil)
))

(define adjunct2 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (return (sum
    (product +1 temp11 (tensor arg2 arg2))
    (product -1 temp01 (tensor arg2 arg3))
    (product -1 temp10 (tensor arg3 arg2))
    (product +1 temp00 (tensor arg3 arg3))
  ))
))

(define adjunct3 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp02 (component arg arg2 arg4))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (setq temp12 (component arg arg3 arg4))
  (setq temp20 (component arg arg4 arg2))
  (setq temp21 (component arg arg4 arg3))
  (setq temp22 (component arg arg4 arg4))
  (return (sum
    (product +1 temp11 temp22 (tensor arg2 arg2))
    (product -1 temp21 temp12 (tensor arg2 arg2))
    (product -1 temp10 temp22 (tensor arg3 arg2))
    (product +1 temp20 temp12 (tensor arg3 arg2))
    (product +1 temp10 temp21 (tensor arg4 arg2))
    (product -1 temp20 temp11 (tensor arg4 arg2))
    (product -1 temp01 temp22 (tensor arg2 arg3))
    (product +1 temp21 temp02 (tensor arg2 arg3))
    (product +1 temp00 temp22 (tensor arg3 arg3))
    (product -1 temp20 temp02 (tensor arg3 arg3))
    (product -1 temp00 temp21 (tensor arg4 arg3))
    (product +1 temp20 temp01 (tensor arg4 arg3))
    (product +1 temp01 temp12 (tensor arg2 arg4))
    (product -1 temp11 temp02 (tensor arg2 arg4))
    (product -1 temp00 temp12 (tensor arg3 arg4))
    (product +1 temp10 temp02 (tensor arg3 arg4))
    (product +1 temp00 temp11 (tensor arg4 arg4))
    (product -1 temp10 temp01 (tensor arg4 arg4))
  ))
))

(define adjunct4 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp02 (component arg arg2 arg4))
  (setq temp03 (component arg arg2 arg5))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (setq temp12 (component arg arg3 arg4))
  (setq temp13 (component arg arg3 arg5))
  (setq temp20 (component arg arg4 arg2))
  (setq temp21 (component arg arg4 arg3))
  (setq temp22 (component arg arg4 arg4))
  (setq temp23 (component arg arg4 arg5))
  (setq temp30 (component arg arg5 arg2))
  (setq temp31 (component arg arg5 arg3))
  (setq temp32 (component arg arg5 arg4))
  (setq temp33 (component arg arg5 arg5))
  (return (sum
    (product +1 temp11 temp22 temp33 (tensor arg2 arg2))
    (product -1 temp11 temp32 temp23 (tensor arg2 arg2))
    (product -1 temp21 temp12 temp33 (tensor arg2 arg2))
    (product +1 temp21 temp32 temp13 (tensor arg2 arg2))
    (product +1 temp31 temp12 temp23 (tensor arg2 arg2))
    (product -1 temp31 temp22 temp13 (tensor arg2 arg2))
    (product -1 temp10 temp22 temp33 (tensor arg3 arg2))
    (product +1 temp10 temp32 temp23 (tensor arg3 arg2))
    (product +1 temp20 temp12 temp33 (tensor arg3 arg2))
    (product -1 temp20 temp32 temp13 (tensor arg3 arg2))
    (product -1 temp30 temp12 temp23 (tensor arg3 arg2))
    (product +1 temp30 temp22 temp13 (tensor arg3 arg2))
    (product +1 temp10 temp21 temp33 (tensor arg4 arg2))
    (product -1 temp10 temp31 temp23 (tensor arg4 arg2))
    (product -1 temp20 temp11 temp33 (tensor arg4 arg2))
    (product +1 temp20 temp31 temp13 (tensor arg4 arg2))
    (product +1 temp30 temp11 temp23 (tensor arg4 arg2))
    (product -1 temp30 temp21 temp13 (tensor arg4 arg2))
    (product -1 temp10 temp21 temp32 (tensor arg5 arg2))
    (product +1 temp10 temp31 temp22 (tensor arg5 arg2))
    (product +1 temp20 temp11 temp32 (tensor arg5 arg2))
    (product -1 temp20 temp31 temp12 (tensor arg5 arg2))
    (product -1 temp30 temp11 temp22 (tensor arg5 arg2))
    (product +1 temp30 temp21 temp12 (tensor arg5 arg2))
    (product -1 temp01 temp22 temp33 (tensor arg2 arg3))
    (product +1 temp01 temp32 temp23 (tensor arg2 arg3))
    (product +1 temp21 temp02 temp33 (tensor arg2 arg3))
    (product -1 temp21 temp32 temp03 (tensor arg2 arg3))
    (product -1 temp31 temp02 temp23 (tensor arg2 arg3))
    (product +1 temp31 temp22 temp03 (tensor arg2 arg3))
    (product +1 temp00 temp22 temp33 (tensor arg3 arg3))
    (product -1 temp00 temp32 temp23 (tensor arg3 arg3))
    (product -1 temp20 temp02 temp33 (tensor arg3 arg3))
    (product +1 temp20 temp32 temp03 (tensor arg3 arg3))
    (product +1 temp30 temp02 temp23 (tensor arg3 arg3))
    (product -1 temp30 temp22 temp03 (tensor arg3 arg3))
    (product -1 temp00 temp21 temp33 (tensor arg4 arg3))
    (product +1 temp00 temp31 temp23 (tensor arg4 arg3))
    (product +1 temp20 temp01 temp33 (tensor arg4 arg3))
    (product -1 temp20 temp31 temp03 (tensor arg4 arg3))
    (product -1 temp30 temp01 temp23 (tensor arg4 arg3))
    (product +1 temp30 temp21 temp03 (tensor arg4 arg3))
    (product +1 temp00 temp21 temp32 (tensor arg5 arg3))
    (product -1 temp00 temp31 temp22 (tensor arg5 arg3))
    (product -1 temp20 temp01 temp32 (tensor arg5 arg3))
    (product +1 temp20 temp31 temp02 (tensor arg5 arg3))
    (product +1 temp30 temp01 temp22 (tensor arg5 arg3))
    (product -1 temp30 temp21 temp02 (tensor arg5 arg3))
    (product +1 temp01 temp12 temp33 (tensor arg2 arg4))
    (product -1 temp01 temp32 temp13 (tensor arg2 arg4))
    (product -1 temp11 temp02 temp33 (tensor arg2 arg4))
    (product +1 temp11 temp32 temp03 (tensor arg2 arg4))
    (product +1 temp31 temp02 temp13 (tensor arg2 arg4))
    (product -1 temp31 temp12 temp03 (tensor arg2 arg4))
    (product -1 temp00 temp12 temp33 (tensor arg3 arg4))
    (product +1 temp00 temp32 temp13 (tensor arg3 arg4))
    (product +1 temp10 temp02 temp33 (tensor arg3 arg4))
    (product -1 temp10 temp32 temp03 (tensor arg3 arg4))
    (product -1 temp30 temp02 temp13 (tensor arg3 arg4))
    (product +1 temp30 temp12 temp03 (tensor arg3 arg4))
    (product +1 temp00 temp11 temp33 (tensor arg4 arg4))
    (product -1 temp00 temp31 temp13 (tensor arg4 arg4))
    (product -1 temp10 temp01 temp33 (tensor arg4 arg4))
    (product +1 temp10 temp31 temp03 (tensor arg4 arg4))
    (product +1 temp30 temp01 temp13 (tensor arg4 arg4))
    (product -1 temp30 temp11 temp03 (tensor arg4 arg4))
    (product -1 temp00 temp11 temp32 (tensor arg5 arg4))
    (product +1 temp00 temp31 temp12 (tensor arg5 arg4))
    (product +1 temp10 temp01 temp32 (tensor arg5 arg4))
    (product -1 temp10 temp31 temp02 (tensor arg5 arg4))
    (product -1 temp30 temp01 temp12 (tensor arg5 arg4))
    (product +1 temp30 temp11 temp02 (tensor arg5 arg4))
    (product -1 temp01 temp12 temp23 (tensor arg2 arg5))
    (product +1 temp01 temp22 temp13 (tensor arg2 arg5))
    (product +1 temp11 temp02 temp23 (tensor arg2 arg5))
    (product -1 temp11 temp22 temp03 (tensor arg2 arg5))
    (product -1 temp21 temp02 temp13 (tensor arg2 arg5))
    (product +1 temp21 temp12 temp03 (tensor arg2 arg5))
    (product +1 temp00 temp12 temp23 (tensor arg3 arg5))
    (product -1 temp00 temp22 temp13 (tensor arg3 arg5))
    (product -1 temp10 temp02 temp23 (tensor arg3 arg5))
    (product +1 temp10 temp22 temp03 (tensor arg3 arg5))
    (product +1 temp20 temp02 temp13 (tensor arg3 arg5))
    (product -1 temp20 temp12 temp03 (tensor arg3 arg5))
    (product -1 temp00 temp11 temp23 (tensor arg4 arg5))
    (product +1 temp00 temp21 temp13 (tensor arg4 arg5))
    (product +1 temp10 temp01 temp23 (tensor arg4 arg5))
    (product -1 temp10 temp21 temp03 (tensor arg4 arg5))
    (product -1 temp20 temp01 temp13 (tensor arg4 arg5))
    (product +1 temp20 temp11 temp03 (tensor arg4 arg5))
    (product +1 temp00 temp11 temp22 (tensor arg5 arg5))
    (product -1 temp00 temp21 temp12 (tensor arg5 arg5))
    (product -1 temp10 temp01 temp22 (tensor arg5 arg5))
    (product +1 temp10 temp21 temp02 (tensor arg5 arg5))
    (product +1 temp20 temp01 temp12 (tensor arg5 arg5))
    (product -1 temp20 temp11 temp02 (tensor arg5 arg5))
  ))
))

(define determinant (cond
  (arg5 (determinant4 arg1 arg2 arg3 arg4 arg5))
  (arg4 (determinant3 arg1 arg2 arg3 arg4))
  (arg3 (determinant2 arg1 arg2 arg3))
  (t nil)
))

(define determinant2 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (return (sum
    (product +1 temp00 temp11)
    (product -1 temp01 temp10)
  ))
))

(define determinant3 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp02 (component arg arg2 arg4))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (setq temp12 (component arg arg3 arg4))
  (setq temp20 (component arg arg4 arg2))
  (setq temp21 (component arg arg4 arg3))
  (setq temp22 (component arg arg4 arg4))
  (return (sum
    (product +1 temp00 temp11 temp22)
    (product -1 temp00 temp21 temp12)
    (product -1 temp10 temp01 temp22)
    (product +1 temp10 temp21 temp02)
    (product +1 temp20 temp01 temp12)
    (product -1 temp20 temp11 temp02)
  ))
))

(define determinant4 (prog ()
  (setq temp00 (component arg arg2 arg2))
  (setq temp01 (component arg arg2 arg3))
  (setq temp02 (component arg arg2 arg4))
  (setq temp03 (component arg arg2 arg5))
  (setq temp10 (component arg arg3 arg2))
  (setq temp11 (component arg arg3 arg3))
  (setq temp12 (component arg arg3 arg4))
  (setq temp13 (component arg arg3 arg5))
  (setq temp20 (component arg arg4 arg2))
  (setq temp21 (component arg arg4 arg3))
  (setq temp22 (component arg arg4 arg4))
  (setq temp23 (component arg arg4 arg5))
  (setq temp30 (component arg arg5 arg2))
  (setq temp31 (component arg arg5 arg3))
  (setq temp32 (component arg arg5 arg4))
  (setq temp33 (component arg arg5 arg5))
  (return (sum
    (product +1 temp00 temp11 temp22 temp33)
    (product -1 temp00 temp11 temp32 temp23)
    (product -1 temp00 temp21 temp12 temp33)
    (product +1 temp00 temp21 temp32 temp13)
    (product +1 temp00 temp31 temp12 temp23)
    (product -1 temp00 temp31 temp22 temp13)
    (product -1 temp10 temp01 temp22 temp33)
    (product +1 temp10 temp01 temp32 temp23)
    (product +1 temp10 temp21 temp02 temp33)
    (product -1 temp10 temp21 temp32 temp03)
    (product -1 temp10 temp31 temp02 temp23)
    (product +1 temp10 temp31 temp22 temp03)
    (product +1 temp20 temp01 temp12 temp33)
    (product -1 temp20 temp01 temp32 temp13)
    (product -1 temp20 temp11 temp02 temp33)
    (product +1 temp20 temp11 temp32 temp03)
    (product +1 temp20 temp31 temp02 temp13)
    (product -1 temp20 temp31 temp12 temp03)
    (product -1 temp30 temp01 temp12 temp23)
    (product +1 temp30 temp01 temp22 temp13)
    (product +1 temp30 temp11 temp02 temp23)
    (product -1 temp30 temp11 temp22 temp03)
    (product -1 temp30 temp21 temp02 temp13)
    (product +1 temp30 temp21 temp12 temp03)
  ))
))

; P (x) = 1
;  0
;
; P (x) = x
;  1
;
; n P (x) =  (2n - 1) x P   (x) - (n - 1) P   (x)
;    n                   n-1               n-2
;
; (legendre x n)

(define legendre (cond
  ((equal arg2 0) 1)
  ((equal arg2 1) arg1)
  (t (product (power arg2 -1) (sum
    (product (sum (product 2 arg2) -1) arg1 (legendre arg1 (sum arg2 -1)))
    (product -1 (sum arg2 -1) (legendre arg1 (sum arg2 -2)))
  )))
))

(define nth-derivative (cond
  ((zerop arg3) arg1)
  (t (derivative (nth-derivative arg1 arg2 (sum arg3 -1)) arg2))
))

; L (x, a) = 1
;  0
;
; L (x, a) = 1 + a - x
;  1
;
; n L (x, a) = (2n + a - 1 - x) L   (x, a) + (1 - n - a) L   (x, a)
;    n                           n-1                      n-2
;
; (laguerre x n a)
;
; only works for n <= 6 due to 32-bit integers

(define laguerre (cond
  ((equal arg2 0) 1)
  ((equal arg2 1) (sum 1 arg3 (product -1 arg1)))
  (t (product (power arg2 -1) (sum
    (product (sum (product 2 arg2) arg3 -1 (product -1 arg1)) (laguerre arg1 (sum arg2 -1) arg3))
	(product (sum 1 (product -1 arg2) (product -1 arg3)) (laguerre arg1 (sum arg2 -2) arg3))
  )))
))

;                      -a  x   n   n    n+a  -x
; L(x, n, a) = (1/n!) x   e  (d /dx ) (x    e  )

(define laguerre2 (product
  (power (factorial arg2) -1)
  (power arg1 (product -1 arg3))
  (exp arg)
  (nth-derivative
    (product (power arg1 (sum arg2 arg3)) (exp (product -1 arg)))
    arg1
    arg2
  )
))

(define absval (cond
  ((not (numberp arg)) nil)
  ((lessp arg 0) (product -1 arg))
  (t arg)
))

(define factorial (cond
  ((zerop arg) 1)
  (t (product arg (factorial (sum arg -1))))
))

(define laplacian (sum
  (product
    (power r -2)
    (derivative (product (power r 2) (derivative arg r)) r)
  )
  (product
    (power r -2)
    (power (sin theta) -1)
    (derivative (product (sin theta) (derivative arg theta)) theta)
  )
  (product
    (power r -2)
    (power (sin theta) -2)
    (derivative (derivative arg phi) phi)
  )
))

; arg1=n arg2=x

(define hermite (product
  (power -1 arg1)
  (power e (power arg2 2))
  (nth-derivative (power e (product -1 (power arg2 2))) arg2 arg1)
))

; generic metric

(setq gdd (sum
  (product (g00) (tensor x0 x0))
  (product (g11) (tensor x1 x1))
  (product (g22) (tensor x2 x2))
  (product (g33) (tensor x3 x3))
))

(setq eta (sum
  (product -1 (tensor x0 x0))
  (product +1 (tensor x1 x1))
  (product +1 (tensor x2 x2))
  (product +1 (tensor x3 x3))
))

(define epsilon (sum
  (product +1 (tensor x0 x1 x2 x3)) ;  1
  (product -1 (tensor x0 x1 x3 x2)) ;  2
  (product -1 (tensor x0 x2 x1 x3)) ;  3
  (product +1 (tensor x0 x2 x3 x1)) ;  4
  (product +1 (tensor x0 x3 x1 x2)) ;  5
  (product -1 (tensor x0 x3 x2 x1)) ;  6
  (product -1 (tensor x1 x0 x2 x3)) ;  7
  (product +1 (tensor x1 x0 x3 x2)) ;  8
  (product +1 (tensor x1 x2 x0 x3)) ;  9
  (product -1 (tensor x1 x2 x3 x0)) ; 10
  (product -1 (tensor x1 x3 x0 x2)) ; 11
  (product +1 (tensor x1 x3 x2 x0)) ; 12
  (product +1 (tensor x2 x0 x1 x3)) ; 13
  (product -1 (tensor x2 x0 x3 x1)) ; 14
  (product -1 (tensor x2 x1 x0 x3)) ; 15
  (product +1 (tensor x2 x1 x3 x0)) ; 16
  (product +1 (tensor x2 x3 x0 x1)) ; 17
  (product -1 (tensor x2 x3 x1 x0)) ; 18
  (product -1 (tensor x3 x0 x1 x2)) ; 19
  (product +1 (tensor x3 x0 x2 x1)) ; 20
  (product +1 (tensor x3 x1 x0 x2)) ; 21
  (product -1 (tensor x3 x1 x2 x0)) ; 22
  (product -1 (tensor x3 x2 x0 x1)) ; 23
  (product +1 (tensor x3 x2 x1 x0)) ; 24
))

(define gradient (sum
  (product (derivative arg x0) (tensor x0))
  (product (derivative arg x1) (tensor x1))
  (product (derivative arg x2) (tensor x2))
  (product (derivative arg x3) (tensor x3))
))

; compute g, guu, GAMUDD, RUDDD, RDD, R, GDD, GUD and GUU from gdd

(define gr (prog (tmp tmp1 tmp2)
  (setq g (determinant gdd x0 x1 x2 x3))
  (setq guu (product (power g -1) (adjunct gdd x0 x1 x2 x3)))
  ; connection coefficients
  (setq tmp (gradient gdd))
  (setq GAMUDD (contract23 (product 1/2 guu (sum
    tmp
    (transpose23 tmp)
    (product -1 (transpose12 (transpose23 tmp)))
  ))))
  ; riemann tensor
  (setq tmp1 (gradient GAMUDD))
  (setq tmp2 (contract24 (product GAMUDD GAMUDD)))
  (setq RUDDD (sum
    (transpose34 tmp1)
    (product -1 tmp1)
    (transpose23 tmp2)
    (product -1 (transpose34 (transpose23 tmp2)))
  ))
  (setq RDD (contract13 RUDDD)) ; ricci tensor
  (setq R (contract12 (contract23 (product guu RDD)))) ; ricci scalar
  (setq GDD (sum RDD (product -1/2 gdd R))) ; einstein tensor
  (setq GUD (contract23 (product guu GDD))) ; raise 1st index
  (setq GUU (contract23 (product GUD guu))) ; raise 2nd index
))

; covariant derivative of a vector

(define covariant-derivative (sum
  (gradient arg)
  (contract13 (product arg GAMUDD))
))

(define covariant-derivative-of-up-up (prog (tmp)
  (setq tmp (product arg GAMUDD))
  (return (sum
    (gradient arg)
    (transpose12 (contract14 tmp))
    (contract24 tmp)
  ))
))

(define covariant-derivative-of-up-down (prog (tmp)
  (setq tmp (product arg GAMUDD))
  (return (sum
    (gradient arg)
    (transpose12 (contract14 tmp))
    (product -1 (contract23 tmp))
  ))
))

(define covariant-derivative-of-down-down (prog (tmp)
  (setq tmp (product arg GAMUDD))
  (return (sum
    (gradient arg)
    (product -1 (transpose12 (contract13 tmp)))
    (product -1 (contract23 tmp))
  ))
))

(define add-integral (prog (tmp)
  (setq tmp arg)
  (setq tmp (subst meta-a a tmp))
  (setq tmp (subst meta-n n tmp))
  (setq tmp (subst meta-x x tmp))
  (setq integral-list (append integral-list (cons tmp nil)))
))

(add-integral (list
  '(power x -1)
  '(log x)
))

(add-integral (list
  '(power x n)
  '(product (power (sum n 1) -1) (power x (sum n 1)))
  '(numberp n)
  '(not (equal n -1))
))

(add-integral (list
  '(power e (product a x))
  '(product (power a -1) (power e (product a x)))
))

;  n  ax     -1  n  ax      -1            n-1  ax
; x  e   -> a   x  e   - n a   (integral x    e  )
;
; integer n, n > 0

(add-integral (list
  '(product
    (power x n)
    (power e (product a x))
  )
  '(sum
    (product
      (power a -1)
      (power x n)
      (power e (product a x))
    )
    (product
      -1
      n
      (power a -1)
      (integral
        (product
          (power x (sum n -1))
          (power e (product a x))
        )
        x
      )
    )
  )
  '(integerp n)
  '(greaterp n 0)
))
