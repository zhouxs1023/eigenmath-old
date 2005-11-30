; Page references are for the book "Gravitation."
; generic metric
(setq gdd (sum
  (product (g00) (tensor x0 x0))
  (product (g11) (tensor x1 x1))
  (product (g22) (tensor x2 x2))
  (product (g33) (tensor x3 x3))
))
(gr) ; compute g, guu, GAMUDD, RUDDD, RDD, R, GDD, GUD and GUU
; generic vectors
(setq u (sum
  (product (u0) (tensor x0))
  (product (u1) (tensor x1))
  (product (u2) (tensor x2))
  (product (u3) (tensor x3))
))
(setq v (sum
  (product (v0) (tensor x0))
  (product (v1) (tensor x1))
  (product (v2) (tensor x2))
  (product (v3) (tensor x3))
))
(setq w (sum
  (product (w0) (tensor x0))
  (product (w1) (tensor x1))
  (product (w2) (tensor x2))
  (product (w3) (tensor x3))
))
; how to antisymmetrize three indices (p. 86)
(setq V (sum
  (product 1/6 V)
  (product 1/6 (transpose12 (transpose23 V))) ; nu lambda mu -> mu nu lambda
  (product 1/6 (transpose23 (transpose12 V))) ; lambda mu nu -> mu nu lambda
  (product -1/6 (transpose12 V))              ; nu mu lambda -> mu nu lambda
  (product -1/6 (transpose23 V))              ; mu lambda nu -> mu nu lambda
  (product -1/6 (transpose13 V))              ; lambda nu mu -> mu nu lambda
))
; commutator (p. 206)
(define commutator (sum
  (contract13 (product +1 arg1 (gradient arg2)))
  (contract13 (product -1 arg2 (gradient arg1)))
))
(print "connection coefficients (p. 210)")
(setq temp (gradient gdd))
(setq Gamma (contract23 (product 1/2 guu (sum
  temp
  (transpose23 temp)
  (product -1 (transpose12 (transpose23 temp)))
))))
; check
(print (equal Gamma GAMUDD))
; covariant derivative of a vector (p. 211)
(define covariant-derivative (sum
  (gradient arg)
  (contract13 (product arg GAMUDD))
))
(print "divergence of einstein is zero (p. 222)")
; covariant-derivative-of-up-up is already defined in grlib
(setq temp (covariant-derivative-of-up-up GUU))
(setq temp (contract23 temp)) ; sum over 2nd and 3rd indices
(print (equal temp 0))
(print "computing riemann tensor (p. 219)")
(setq temp1 (gradient GAMUDD))
(setq temp2 (contract24 (product GAMUDD GAMUDD)))
(setq riemann (sum
  (transpose34 temp1)
  (product -1 temp1)
  (transpose23 temp2)
  (product -1 (transpose34 (transpose23 temp2)))
))
; check
(print (equal riemann RUDDD))
; p. 259
(define covariant-derivative-of-down-down (prog (temp)
  (setq temp (product arg GAMUDD))
  (return (sum
    (gradient arg)
    (product -1 (transpose12 (contract13 temp)))
    (product -1 (contract23 temp))
  ))
))
(define covariant-derivative-of-up-down (prog (temp)
  (setq temp (product arg GAMUDD))
  (return (sum
    (gradient arg)
    (transpose12 (contract14 temp))
    (product -1 (contract23 temp))
  ))
))
(define directed-covariant-derivative
  (contract23 (product (covariant-derivative arg1) arg2))
)
(print "symmetry of covariant derivative (p. 252)")
(setq temp1 (sum
  (directed-covariant-derivative v u)
  (product -1 (directed-covariant-derivative u v))
))
(setq temp2 (commutator u v))
(equal temp1 temp2)
(print "covariant derivative chain rule (p. 252)")
(setq temp1 (directed-covariant-derivative (product (f) v) u))
(setq temp2 (sum
  (product (f) (directed-covariant-derivative v u))
  (product v (contract12 (product (gradient (f)) u)))
))
(print (equal temp1 temp2))
(print "additivity of covariant derivative (p. 257)")
(setq temp1 (directed-covariant-derivative u (sum v w)))
(setq temp2 (sum
  (directed-covariant-derivative u v)
  (directed-covariant-derivative u w)
))
(print (equal temp1 temp2))
(print "riemann is antisymmetric on last two indices (p. 286)")
(setq temp (sum
  (product 1/2 RUDDD)
  (product -1/2 (transpose34 RUDDD))
))
(print (equal RUDDD temp))
(print "riemann vanishes when antisymmetrized on last three indices (p. 286)")
(setq temp (sum
  (product 1/6 RUDDD)
  (product 1/6 (transpose34 (transpose24 RUDDD)))
  (product 1/6 (transpose34 (transpose23 RUDDD)))
  (product -1/6 (transpose23 RUDDD))
  (product -1/6 (transpose34 RUDDD))
  (product -1/6 (transpose24 RUDDD))
))
(print (equal temp 0))
(print "double dual of riemann (p. 325)")
(setq temp (contract23 (product gdd RUDDD))) ; lower 1st index
(setq temp (transpose34 (contract35 (product temp guu)))) ; raise 3rd index
(setq RDDUU (contract45 (product temp guu))) ; raise 4th index
(setq temp (product epsilon RDDUU))
(setq temp (contract35 temp)) ; sum over mu
(setq temp (contract34 temp)) ; sum over nu
(setq temp (product temp epsilon))
(setq temp (contract35 temp)) ; sum over rho
(setq temp (contract34 temp)) ; sum over sigma
(setq GUUDD (product -1/4 temp)) ; negative due to levi-civita tensor
; check
(print (equal
  (contract13 GUUDD)
  GUD
))
(print "noncommutation of covariant derivatives (p. 389)")
(setq B (sum
  (product (B0) (tensor x0))
  (product (B1) (tensor x1))
  (product (B2) (tensor x2))
  (product (B3) (tensor x3))
))
(setq temp (covariant-derivative-of-up-down (covariant-derivative B)))
(setq temp1 (sum temp (product -1 (transpose23 temp))))
(setq temp2 (transpose23 (contract25 (product RUDDD B))))
(print (equal temp1 temp2))
(print "bondi metric")
; erase any definitions for U, V, beta and gamma
(setq U (quote U))
(setq V (quote V))
(setq beta (quote beta))
(setq gamma (quote gamma))
; erase any definitions for u, r, theta and phi
(setq u (quote u))
(setq r (quote r))
(setq theta (quote theta))
(setq phi (quote phi))
; new coordinate system
(setq x0 u)
(setq x1 r)
(setq x2 theta)
(setq x3 phi)
; U, V, beta and gamma are functions of u, r and theta
(setq g_uu (sum
  (product
    (V u r theta)
    (power r -1)
    (power e (product 2 (beta u r theta)))
  )
  (product
    -1
    (power (U u r theta) 2)
    (power r 2)
    (power e (product 2 (gamma u r theta)))
  )
))
(setq g_ur (product 2 (power e (product 2 (beta u r theta)))))
(setq g_utheta (product
  2
  (U u r theta)
  (power r 2)
  (power e (product 2 (gamma u r theta)))
))
(setq g_thetatheta (product
  -1
  (power r 2)
  (power e (product 2 (gamma u r theta)))
))
(setq g_phiphi (product
  -1
  (power r 2)
  (power e (product -2 (gamma u r theta)))
  (power (sin theta) 2)
))
; metric tensor
(setq gdd (sum
  (product g_uu (tensor u u))
  (product g_ur (tensor u r))
  (product g_ur (tensor r u))
  (product g_utheta (tensor u theta))
  (product g_utheta (tensor theta u))
  (product g_thetatheta (tensor theta theta))
  (product g_phiphi (tensor phi phi))
))
(gr) ; compute g, guu, GAMUDD, RUDDD, RDD, R, GDD, GUD and GUU
; is covariant derivative of metric zero?
(print (equal (covariant-derivative-of-down-down gdd) 0))
; is divergence of einstein zero?
(setq temp (contract23 (covariant-derivative-of-up-up GUU)))
(print (equal temp 0))
